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
#include "PropertyEditor.h"

#ifdef GIN_AVAILABLE
#define JUCE_MODULE_AVAILABLE_gin
#include "gin/gin.h"
#endif

#include <spdlog/spdlog.h>

#include "LayoutConstants.h"


class SimpleListBoxModel : public juce::ListBoxModel {
public:
    SimpleListBoxModel(juce::StringArray content, juce::StringArray values) : content_(content), values_(values) {}

    int getNumRows() override { return content_.size(); }

    void paintListBoxItem(int rowNumber, juce::Graphics &g, int width, int height, bool rowIsSelected) override
    {
        auto background = rowIsSelected ? juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::TextEditor::highlightColourId) :
                                          juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::ListBox::backgroundColourId);

        auto textColour = rowIsSelected ? juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::TextEditor::highlightedTextColourId) :
                                          juce::LookAndFeel::getDefaultLookAndFeel().findColour(juce::ListBox::textColourId);

        g.fillAll(background);
        g.setColour(textColour);
        g.setFont((float) juce::jmin(height, 24) * 0.65f);
        if (rowNumber < content_.size()) {
            g.drawText(content_[rowNumber], 5, 0, width, height, juce::Justification::centredLeft);
        }
    }

    void listBoxItemClicked(int row, const juce::MouseEvent &) override
    {
        if (onSelected) {
            onSelected(values_[row]);
        }
    }

    std::function<void(juce::String const &)> onSelected;

private:
    juce::StringArray content_;
    juce::StringArray values_;
};


class ListPropertyEditor : public juce::PropertyComponent {
public:
    ListPropertyEditor(const juce::Value &valueToControl, juce::String const &propertyName, juce::StringArray titles, juce::StringArray values) :
        juce::PropertyComponent(propertyName, 2 * LAYOUT_LINE_SPACING), value_(valueToControl), listModel_(titles, values)
    {
        listBox_.setModel(&listModel_);
        addAndMakeVisible(listBox_);
        listModel_.onSelected = [this](juce::String const &value) { value_ = value; };
    }

    virtual void refresh() {}

    virtual void resized()
    {
        auto area = getLocalBounds();
        auto contentBox = getLookAndFeel().getPropertyComponentContentPosition(*this); // Get the width that JUCE's LookAndFeel uses for labels
        listBox_.setBounds(contentBox.withTrimmedLeft(2));
    }

private:
    SimpleListBoxModel listModel_;
    juce::ListBox listBox_;
    juce::Value value_;
};

PropertyEditor::PropertyEditor(bool dynamicLayout /* = false*/) : dynamicLayout_(dynamicLayout)
{
    addAndMakeVisible(propertyPanel_);
    if (dynamicLayout_) {
        propertyPanel_.addMouseListener(this, true);
    }
}

PropertyEditor::PropertyEditor(TProperties &properties, bool dynamicLayout /* = false */) : PropertyEditor(dynamicLayout)
{
    setProperties(properties);
}

PropertyEditor::~PropertyEditor()
{
    if (dynamicLayout_) {
        propertyPanel_.removeMouseListener(this);
    }
}

void PropertyEditor::resized()
{
    auto area = getLocalBounds();
    propertyPanel_.setBounds(area);
}

int PropertyEditor::getTotalContentHeight()
{
    return propertyPanel_.getTotalContentHeight();
}

juce::PropertyComponent *PropertyEditor::createEditor(std::shared_ptr<TypedNamedValue> property)
{
    switch (property->valueType()) {
    case ValueType::Lookup: {
        juce::StringArray choices;
        juce::Array<juce::var> values;
        for (auto lookup : property->lookup()) {
            values.add(lookup.first);
            choices.add(lookup.second);
        }
        return new juce::ChoicePropertyComponent(juce::Value(property->value()), property->name(), choices, values);
    }
    case ValueType::List: {
        juce::StringArray choices;
        juce::Array<juce::var> values;
        for (auto item : property->list()) {
            choices.add(item.first);
            values.add(juce::String(item.second));
        }
        return new ListPropertyEditor(juce::Value(property->value()), property->name(), choices, values);
    }
    case ValueType::Integer:
        return new juce::SliderPropertyComponent(juce::Value(property->value()), property->name(), property->minValue(), property->maxValue(), 1.0);
    case ValueType::Bool:
        return new juce::BooleanPropertyComponent(juce::Value(property->value()), property->name(), "On/Off");
    case ValueType::String:
        return new juce::TextPropertyComponent(juce::Value(property->value()), property->name(), property->maxValue(), property->multiLine(),
            property->enabled());
#ifdef GIN_AVAILABLE
    case ValueType::Color:
        return new gin::ColourPropertyComponent(juce::Value(property->value()), property->name(), false);
#endif
    default:
        // Type needs to be implemented
        jassert(false);
        return nullptr;
    }
}

void PropertyEditor::setProperties(TProperties const &props)
{
    propertyPanel_.clear();

    juce::Array<juce::PropertyComponent *> editors;
    juce::String activeSectionName = "";
    for (auto property : props) {
        // See if we need to close the previous section
        if (activeSectionName != property->sectionName()) {
            if (!editors.isEmpty()) {
                propertyPanel_.addSection(activeSectionName, editors, true);
                editors.clear();
            }
            activeSectionName = property->sectionName();
        }

        juce::PropertyComponent *editor = createEditor(property);
        if (editor) {
            editors.add(editor);
        }
    }
    if (!editors.isEmpty()) {
        propertyPanel_.addSection(activeSectionName, editors, true);
    }
}

void PropertyEditor::clear()
{
    propertyPanel_.clear();
}

juce::String PropertyEditor::getLayout()
{
    juce::MemoryOutputStream output(512);
    propertyPanel_.getOpennessState()->writeTo(output);
    return output.toUTF8();
}

void PropertyEditor::fromLayout(juce::String layout)
{
    auto element = juce::XmlDocument::parse(layout);
    if (element) {
        propertyPanel_.restoreOpennessState(*element);
    }
}

void PropertyEditor::mouseUp(const juce::MouseEvent &)
{
    auto resizeThis = getParentComponent();
    juce::MessageManager::callAsync([resizeThis]() { resizeThis->resized(); });
}
