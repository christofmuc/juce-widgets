#include "PropertyEditor.h"

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

		PropertyComponent *editor = nullptr;
		switch (property->valueType()) {
		case ValueType::Lookup: {
			StringArray choices;
			Array<var> values;
			for (auto lookup : property->lookup()) {
				values.add(lookup.first);
				choices.add(lookup.second);
			}
			editor = new ChoicePropertyComponent(Value(property->value()), property->name(), choices, values);
			//editor->setEnabled(property->enabled);
			break;
		}
		case ValueType::Integer:
			editor = new SliderPropertyComponent(Value(property->value()), property->name(), property->minValue(), property->maxValue(), 1.0);
			//editor->setEnabled(property->enabled);
			break;
		case ValueType::Bool:
			editor = new BooleanPropertyComponent(Value(property->value()), property->name(), "On/Off");
			//editor->setEnabled(property->enabled);
			break;
		case ValueType::String:
			editor = new TextPropertyComponent(Value(property->value()), property->name(), property->maxValue(), false, property->enabled());
			break;
		default:
			// Type needs to be implemented
			jassert(false);
		}
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

