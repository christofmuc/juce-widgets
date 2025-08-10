/*
 * MIT License
 *
 * Copyright (c) 2019-2025 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "TreeViewNode.h"

EditOnDoubleClickLabel::EditOnDoubleClickLabel(const juce::String& componentName, const juce::String& labelText) :
    juce::Label(componentName, labelText)
{
    setWantsKeyboardFocus(true);
}

void EditOnDoubleClickLabel::mouseDown(const juce::MouseEvent& e)
{
    if (auto* parent = getParentComponent()) {
        parent->mouseDown(e.getEventRelativeTo(parent)); // let TreeView row handle selection
    }
    juce::Label::mouseDown(e);
}

// Add this override and the accessors (place anywhere in the class body)
void EditOnDoubleClickLabel::editorAboutToBeHidden(juce::TextEditor* te)
{
    if (te != nullptr) {
        savedCaret_ = te->getCaretPosition();
        savedSelection_ = te->getHighlightedRegion();
    }
    lastHiddenAtMs_ = juce::Time::getMillisecondCounter(); // NEW: when the editor hid
    juce::Label::editorAboutToBeHidden(te);
}

juce::uint32 EditOnDoubleClickLabel::getLastHiddenAtMs() const noexcept
{
    return lastHiddenAtMs_;
}


int EditOnDoubleClickLabel::getSavedCaret()
{
    return savedCaret_;
}

juce::Range<int> EditOnDoubleClickLabel::getSavedSelection() const noexcept
{
    return savedSelection_;
}

TreeViewNode::TreeViewNode(juce::String text, juce::String id, bool editable /* = false */) : text_(text), id_(id), editable_(editable)
{
    textValue = text;
}

void TreeViewNode::toggleOpenness()
{
    switch (getOpenness()) {
    case Openness::opennessClosed:
        setOpenness(TreeViewItem::Openness::opennessOpen);
        break;
    case Openness::opennessOpen:
        setOpenness(TreeViewItem::Openness::opennessClosed);
        break;
    case Openness::opennessDefault:
        // This only works as expected when the TreeView has default openness = closed
        setOpenness(TreeViewItem::Openness::opennessOpen);
        break;
    }
}

bool TreeViewNode::mightContainSubItems()
{
    return onGenerateChildren != nullptr;
}

void TreeViewNode::itemOpennessChanged(bool isNowOpen)
{
    if (onGenerateChildren != nullptr && isNowOpen) {
        regenerate();
    }
}

void TreeViewNode::paintItem(juce::Graphics& g, int width, int height)
{
    if (editable_) {
        // Editable items have a custom component and don't paint themselves
        return;
    }
    if (customTextColour_.has_value()) {
        g.setColour(*customTextColour_);
    }
    else {
        auto& lf = juce::LookAndFeel::getDefaultLookAndFeel();
        g.setColour(lf.findColour(juce::Label::textColourId));
    }
    g.drawText(text_, 0, 0, width, height, juce::Justification::centredLeft);
}

void TreeViewNode::paintOpenCloseButton(juce::Graphics& g, const juce::Rectangle<float>& area, juce::Colour backgroundColour, bool isMouseOver)
{
    juce::ignoreUnused(backgroundColour);
    TreeViewItem::paintOpenCloseButton(g, area, juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::TreeView::backgroundColourId),
        isMouseOver);
}

bool TreeViewNode::canBeSelected() const
{
    return onSelected != nullptr;
}

void TreeViewNode::itemSelectionChanged(bool isNowSelected)
{
    if (isNowSelected) {
        onSelected(id_);
    }
}

void TreeViewNode::regenerate()
{
    if (onGenerateChildren) {
        clearSubItems();
        auto children = onGenerateChildren();
        for (auto c : children) {
            addSubItem(c);
        }
        treeHasChanged();
    }
}

void TreeViewNode::setTextColour(juce::Colour color)
{
    customTextColour_ = color;
}

void TreeViewNode::removeTextColour()
{
    customTextColour_.reset();
}

juce::String TreeViewNode::getUniqueName() const
{
    if (id_.isNotEmpty()) {
        return id_;
    }
    else {
        return text_;
    }
}

void TreeViewNode::itemClicked(const juce::MouseEvent&)
{
    if (onSingleClick) {
        onSingleClick(id_);
    }
}

void TreeViewNode::itemDoubleClicked(const juce::MouseEvent&)
{
    if (!editable_) {
        if (onDoubleClick) {
            onDoubleClick(id_);
        }
    }
}

bool TreeViewNode::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& source)
{
    return onItemDropped != nullptr && (acceptsItem == nullptr || acceptsItem(source.description));
}

void TreeViewNode::itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex)
{
    juce::String name = dragSourceDetails.description;
    onItemDropped(dragSourceDetails.description, insertIndex);
}

juce::var TreeViewNode::getDragSourceDescription()
{
    if (onItemDragged) {
        return onItemDragged();
    }
    return {};
}

std::unique_ptr<juce::Component> TreeViewNode::createItemComponent()
{
    if (editable_) {
        auto labelCreated = std::make_unique<EditOnDoubleClickLabel>(id_, text_);
        labelCreated->setEditable(false, true, false);
        labelCreated->getTextValue().referTo(textValue);
        return std::move(labelCreated);
    }
    return nullptr;
}

juce::String TreeViewNode::id() const
{
    return id_;
}

juce::String TreeViewNode::text() const
{
    return text_;
}
