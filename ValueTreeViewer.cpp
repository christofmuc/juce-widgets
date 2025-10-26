#include "ValueTreeViewer.h"

#include <algorithm>
#include <vector>

namespace
{
juce::String valueToDisplayString(const juce::var& value)
{
    if (value.isVoid())
        return "<null>";

    if (value.isArray() || value.isObject())
        return juce::JSON::toString(value);

    if (value.isBinaryData())
        return "<binary>";

    return value.toString();
}

juce::var convertTextToVar(const juce::String& text, const juce::var& existing)
{
    const auto trimmed = text.trim();

    if (existing.isBool())
        return trimmed.equalsIgnoreCase("true") || trimmed == "1";

    if (existing.isInt())
        return trimmed.getIntValue();

    if (existing.isInt64())
        return (juce::int64) trimmed.getLargeIntValue();

    if (existing.isDouble())
        return trimmed.getDoubleValue();

    if (existing.isVoid() && trimmed.isEmpty())
        return juce::var();

    return juce::var(trimmed);
}
} // namespace

class ValueTreeViewer::NodeItem;
class ValueTreePropertyItem;

class ValueTreePropertyItem : public juce::TreeViewItem,
                              private juce::ValueTree::Listener
{
public:
    class EditorComponent;

    ValueTreePropertyItem(ValueTreeViewer& ownerIn, juce::ValueTree treeIn, juce::Identifier propertyIdIn)
        : owner(ownerIn), tree(treeIn), propertyId(propertyIdIn)
    {
        tree.addListener(this);
    }

    ~ValueTreePropertyItem() override
    {
        tree.removeListener(this);
    }

    juce::Identifier getPropertyId() const noexcept { return propertyId; }
    juce::String getDisplayName() const { return propertyId.toString(); }
    juce::String getValueAsText() const { return valueToDisplayString(tree.getProperty(propertyId)); }

    void commitValueEdit(const juce::String& newText)
    {
        const auto currentValue = tree.getProperty(propertyId);
        const auto converted = convertTextToVar(newText, currentValue);

        if (currentValue != converted)
            tree.setProperty(propertyId, converted, owner.getUndoManager());
    }

    void refreshEditor()
    {
        if (editor != nullptr)
            editor->refresh();
        else
            treeHasChanged();
    }

    bool mightContainSubItems() override { return false; }
    void paintItem(juce::Graphics&, int, int) override {}
    bool customComponentUsesTreeViewMouseHandler() const override { return true; }

    std::unique_ptr<juce::Component> createItemComponent() override
    {
        auto component = std::make_unique<EditorComponent>(*this);
        editor = component.get();
        return component;
    }

    void editorDestroyed(const EditorComponent* component)
    {
        if (editor == component)
            editor = nullptr;
    }

    class EditorComponent : public juce::Component,
                            private juce::Label::Listener
    {
    public:
        explicit EditorComponent(ValueTreePropertyItem& ownerItem) : item(ownerItem)
        {
            setInterceptsMouseClicks(true, true);
            setOpaque(false);

            nameLabel.setText(item.getDisplayName(), juce::dontSendNotification);
            nameLabel.setJustificationType(juce::Justification::centredLeft);
            nameLabel.setInterceptsMouseClicks(false, false);
            addAndMakeVisible(nameLabel);

            valueLabel.setEditable(true, true, false);
            valueLabel.addListener(this);
            valueLabel.setJustificationType(juce::Justification::centredLeft);
            valueLabel.setInterceptsMouseClicks(true, true);
            addAndMakeVisible(valueLabel);

            refresh();
        }

        ~EditorComponent() override
        {
            item.editorDestroyed(this);
        }

        void resized() override
        {
            auto bounds = getLocalBounds().reduced(2);
            const auto nameWidth = juce::jmin(150, bounds.proportionOfWidth(0.4f));
            nameLabel.setBounds(bounds.removeFromLeft(nameWidth));
            valueLabel.setBounds(bounds);
        }

        void refresh()
        {
            nameLabel.setText(item.getDisplayName(), juce::dontSendNotification);
            valueLabel.setText(item.getValueAsText(), juce::dontSendNotification);
        }

    private:
        void mouseDrag(const juce::MouseEvent& event) override
        {
            if (event.originalComponent == &valueLabel)
            {
                return;
            }

            if (!dragStarted_ && event.mouseWasDraggedSinceMouseDown())
            {
                if (auto* dragContainer = juce::DragAndDropContainer::findParentDragContainerFor(this))
                {
                    if (!dragContainer->isDragAndDropActive())
                    {
                        dragStarted_ = true;
                        dragContainer->startDragging(item.getPropertyId().toString(), this);
                    }
                }
            }

            juce::Component::mouseDrag(event);
        }

        void mouseUp(const juce::MouseEvent& event) override
        {
            dragStarted_ = false;
            juce::Component::mouseUp(event);
        }

        void labelTextChanged(juce::Label* labelThatChanged) override
        {
            if (labelThatChanged == &valueLabel)
                item.commitValueEdit(labelThatChanged->getText());
        }

        ValueTreePropertyItem& item;
        juce::Label nameLabel;
        juce::Label valueLabel;
        bool dragStarted_ = false;
    };

private:
    ValueTreeViewer& owner;
    juce::ValueTree tree;
    juce::Identifier propertyId;
    EditorComponent* editor = nullptr;
};

class ValueTreeViewer::NodeItem : public juce::TreeViewItem,
                                  private juce::ValueTree::Listener
{
public:
    NodeItem(ValueTreeViewer& ownerIn, juce::ValueTree treeIn)
        : owner(ownerIn), tree(treeIn)
    {
        jassert(tree.isValid());
        tree.addListener(this);
        rebuildPropertyItems();
        rebuildChildItems();
    }

    ~NodeItem() override
    {
        tree.removeListener(this);
    }

    juce::ValueTree getTree() const noexcept { return tree; }

    void refreshAll()
    {
        rebuildPropertyItems();
        rebuildChildItems();
        treeHasChanged();
    }

    bool mightContainSubItems() override
    {
        return tree.getNumChildren() > 0 || tree.getNumProperties() > 0;
    }

    void paintItem(juce::Graphics& g, int width, int height) override
    {
        auto textColour = owner.findColour(juce::Label::textColourId);
        if (textColour.isTransparent())
            textColour = owner.getLookAndFeel().findColour(juce::Label::textColourId);
        if (textColour.isTransparent())
            textColour = juce::Colours::white;

        g.setColour(textColour);
        g.setFont(g.getCurrentFont().withHeight(juce::jmin(14.0f, (float) height - 2.0f)));
        g.drawText(buildLabelText(), 4, 0, width - 8, height, juce::Justification::centredLeft, true);
    }

private:
    juce::String buildLabelText() const
    {
        juce::String text = tree.getType().toString();

        if (tree.hasProperty("name"))
        {
            const auto name = tree["name"].toString();
            if (name.isNotEmpty())
                text << " • " << name;
        }

        text << juce::String::formatted(" (%d props, %d children)", tree.getNumProperties(), tree.getNumChildren());
        return text;
    }

    void rebuildPropertyItems()
    {
        const int existingPropertyCount = getPropertyItemCount();
        for (int i = existingPropertyCount - 1; i >= 0; --i)
            removeSubItem(i, true);

        std::vector<juce::Identifier> sortedIds;
        const auto numProperties = tree.getNumProperties();
        sortedIds.reserve((size_t) numProperties);
        for (int i = 0; i < numProperties; ++i)
            sortedIds.emplace_back(tree.getPropertyName(i));

        // Later: add option to sort properties by name
        //if (sortItems_) {
        //    std::sort(sortedIds.begin(), sortedIds.end(),
        //        [](const juce::Identifier& a, const juce::Identifier& b) { return a.toString().compareIgnoreCase(b.toString()) < 0; });
        //}

        for (const auto& id : sortedIds)
            addSubItem(new ValueTreePropertyItem(owner, tree, id));
    }

    void rebuildChildItems()
    {
        const int propertyCount = getPropertyItemCount();
        struct ChildState
        {
            juce::ValueTree tree;
            bool wasOpen = false;
        };

        std::vector<ChildState> previousState;
        const int existingChildCount = getNumSubItems() - propertyCount;
        previousState.reserve((size_t) existingChildCount);

        for (int i = propertyCount; i < getNumSubItems(); ++i)
        {
            if (auto* childItem = dynamic_cast<NodeItem*>(getSubItem(i)))
                previousState.push_back({ childItem->getTree(), childItem->isOpen() });
        }

        while (getNumSubItems() > propertyCount)
            removeSubItem(propertyCount, true);

        for (int i = 0; i < tree.getNumChildren(); ++i)
        {
            auto* child = new NodeItem(owner, tree.getChild(i));
            const auto insertIndex = propertyCount + i;
            addSubItem(child, insertIndex);

            const auto previous = std::find_if(previousState.begin(), previousState.end(),
                                               [childTree = child->getTree()](const ChildState& state) { return state.tree == childTree; });
            if (previous != previousState.end() && previous->wasOpen)
                child->setOpen(true);
        }
    }

    int getPropertyItemCount() const
    {
        int count = 0;

        for (int i = 0; i < getNumSubItems(); ++i)
        {
            if (dynamic_cast<ValueTreePropertyItem*>(getSubItem(i)) != nullptr)
                ++count;
            else
                break;
        }

        return count;
    }

    ValueTreePropertyItem* findPropertyItem(const juce::Identifier& property)
    {
        for (int i = 0; i < getPropertyItemCount(); ++i)
        {
            if (auto* propertyItem = dynamic_cast<ValueTreePropertyItem*>(getSubItem(i)))
                if (propertyItem->getPropertyId() == property)
                    return propertyItem;
        }

        return nullptr;
    }

    void addPropertyItem(const juce::Identifier& property)
    {
        auto* item = new ValueTreePropertyItem(owner, tree, property);
        const auto insertIndex = getOverallPropertyInsertIndex(property);
        addSubItem(item, insertIndex);
    }

    void removePropertyItem(const juce::Identifier& property)
    {
        for (int i = 0; i < getPropertyItemCount(); ++i)
        {
            if (auto* propertyItem = dynamic_cast<ValueTreePropertyItem*>(getSubItem(i)))
            {
                if (propertyItem->getPropertyId() == property)
                {
                    removeSubItem(i, true);
                    break;
                }
            }
        }
    }

    int getOverallPropertyInsertIndex(const juce::Identifier& property) const
    {
        const auto propertyName = property.toString();
        int index = 0;

        for (int i = 0; i < getNumSubItems(); ++i)
        {
            if (auto* propertyItem = dynamic_cast<ValueTreePropertyItem*>(getSubItem(i)))
            {
                if (propertyName.compareIgnoreCase(propertyItem->getPropertyId().toString()) < 0)
                    break;

                ++index;
            }
            else
            {
                break;
            }
        }

        return index;
    }

    void addChildItem(juce::ValueTree childTree, int childIndex)
    {
        const auto insertIndex = getPropertyItemCount() + childIndex;
        addSubItem(new NodeItem(owner, childTree), insertIndex);
    }

    void removeChildItem(juce::ValueTree childTree)
    {
        for (int i = getPropertyItemCount(); i < getNumSubItems(); ++i)
        {
            if (auto* childItem = dynamic_cast<NodeItem*>(getSubItem(i)))
            {
                if (childItem->getTree() == childTree)
                {
                    removeSubItem(i, true);
                    break;
                }
            }
        }
    }

    void valueTreePropertyChanged(juce::ValueTree& changedTree, const juce::Identifier& property) override
    {
        if (changedTree != tree)
            return;

        if (tree.hasProperty(property))
        {
            if (auto* propertyItem = findPropertyItem(property))
                propertyItem->refreshEditor();
            else
                addPropertyItem(property);
        }
        else
        {
            removePropertyItem(property);
        }

        treeHasChanged();
    }

    void valueTreeChildAdded(juce::ValueTree& parent, juce::ValueTree& child) override
    {
        if (parent == tree)
            addChildItem(child, parent.indexOf(child));
    }

    void valueTreeChildRemoved(juce::ValueTree& parent, juce::ValueTree& child, int) override
    {
        if (parent == tree)
            removeChildItem(child);
    }

    void valueTreeChildOrderChanged(juce::ValueTree& parent, int, int) override
    {
        if (parent == tree)
            rebuildChildItems();
    }

    void valueTreeParentChanged(juce::ValueTree& changedTree) override
    {
        if (changedTree == tree)
            treeHasChanged();
    }

    void valueTreeRedirected(juce::ValueTree& changedTree) override
    {
        if (changedTree == tree)
            refreshAll();
    }

    ValueTreeViewer& owner;
    juce::ValueTree tree;
};

ValueTreeViewer::ValueTreeViewer()
{
    treeView_.setMultiSelectEnabled(false);
    treeView_.setDefaultOpenness(true);
    treeView_.setRootItemVisible(true);
    treeView_.setIndentSize(16);
    addAndMakeVisible(treeView_);
}

ValueTreeViewer::~ValueTreeViewer()
{
    treeView_.setRootItem(nullptr);
    rootItem_.reset();
}

void ValueTreeViewer::setValueTree(juce::ValueTree newTree)
{
    if (valueTree_ == newTree)
        return;

    valueTree_ = newTree;
    rebuildRoot();
}

void ValueTreeViewer::setUndoManager(juce::UndoManager* manager) noexcept
{
    undoManager_ = manager;
}

void ValueTreeViewer::refresh()
{
    if (rootItem_ != nullptr)
        rootItem_->refreshAll();
    else
        rebuildRoot();
}

void ValueTreeViewer::resized()
{
    treeView_.setBounds(getLocalBounds());
}

void ValueTreeViewer::rebuildRoot()
{
    treeView_.setRootItem(nullptr);
    rootItem_.reset();

    if (valueTree_.isValid())
    {
        rootItem_ = std::make_unique<NodeItem>(*this, valueTree_);
        treeView_.setRootItem(rootItem_.get());
        rootItem_->setOpen(true);
        treeView_.setRootItemVisible(true);
    }
    else
    {
        treeView_.setRootItemVisible(false);
    }
}
