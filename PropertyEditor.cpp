#include "PropertyEditor.h"

#ifdef GIN_AVAILABLE
#define JUCE_MODULE_AVAILABLE_gin
#include "gin/gin.h"
#endif

PropertyEditor::PropertyEditor()
{
	addAndMakeVisible(propertyPanel_);
}

PropertyEditor::PropertyEditor(TProperties &properties) : PropertyEditor()
{
	setProperties(properties);
}

void PropertyEditor::resized()
{
	auto area = getLocalBounds();
	propertyPanel_.setBounds(area);
}

PropertyComponent *PropertyEditor::createEditor(std::shared_ptr<TypedNamedValue> property) {
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

