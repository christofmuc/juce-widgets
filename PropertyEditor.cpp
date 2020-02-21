#include "PropertyEditor.h"

PropertyEditor::PropertyEditor(TProperties &properties) : properties_(properties)
{
	addAndMakeVisible(propertyPanel_);
	setProperties(properties);
}

void PropertyEditor::resized()
{
	auto area = getLocalBounds();
	propertyPanel_.setBounds(area);
}

void PropertyEditor::setProperties(TProperties &props)
{
	properties_ = props;
	propertyPanel_.clear();

	Array<PropertyComponent *> editors;
	String activeSectionName = "";
	for (auto property : properties_) {
		// See if we need to close the previous section
		if (activeSectionName != property->sectionName) {
			if (!editors.isEmpty()) {
				propertyPanel_.addSection(activeSectionName, editors, true);
				editors.clear();
			}
			activeSectionName = property->sectionName;
		}

		PropertyComponent *editor = nullptr;
		switch (property->valueType) {
		case ValueType::Lookup: {
			StringArray choices;
			Array<var> values;
			for (auto lookup : property->lookup) {
				values.add(lookup.first);
				choices.add(lookup.second);
			}
			editor = new ChoicePropertyComponent(Value(property->value), property->name, choices, values);
			break;
		}
		case ValueType::Integer:
			editor = new SliderPropertyComponent(Value(property->value), property->name, property->minValue, property->maxValue, 1.0);
			break;
		case ValueType::Bool:
			editor = new BooleanPropertyComponent(Value(property->value), property->name, "On/Off");
			break;
		}
		if (editor) {
			editors.add(editor);
		}
	}
	if (!editors.isEmpty()) {
		propertyPanel_.addSection(activeSectionName, editors, true);
	}
}

