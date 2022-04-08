/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
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

PropertyComponent *PropertyEditor::createEditor(std::shared_ptr<TypedNamedValue> property)
{
    switch (property->valueType()) {
    case ValueType::Lookup: {
        StringArray choices;
        Array<var> values;
        for (auto lookup : property->lookup()) {
            values.add(lookup.first);
            choices.add(lookup.second);
        }
        return new ChoicePropertyComponent(Value(property->value()), property->name(), choices, values);
    }
    case ValueType::Integer:
        return new SliderPropertyComponent(Value(property->value()), property->name(), property->minValue(), property->maxValue(), 1.0);
    case ValueType::Bool:
        return new BooleanPropertyComponent(Value(property->value()), property->name(), "On/Off");
    case ValueType::String:
        return new TextPropertyComponent(Value(property->value()), property->name(), property->maxValue(), false, property->enabled());
#ifdef GIN_AVAILABLE
    case ValueType::Color:
        return new gin::ColourPropertyComponent(Value(property->value()), property->name(), false);
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

    Array<PropertyComponent *> editors;
    String activeSectionName = "";
    for (auto property : props) {
        // See if we need to close the previous section
        if (activeSectionName != property->sectionName()) {
            if (!editors.isEmpty()) {
                propertyPanel_.addSection(activeSectionName, editors, true);
                editors.clear();
            }
            activeSectionName = property->sectionName();
        }

        PropertyComponent *editor = createEditor(property);
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

void PropertyEditor::mouseUp(const MouseEvent &)
{
    auto resizeThis = getParentComponent();
    MessageManager::callAsync([resizeThis]() { resizeThis->resized(); });
}