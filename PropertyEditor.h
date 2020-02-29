#pragma once

#include "JuceHeader.h"

#include "TypedNamedValue.h"

class PropertyEditor : public Component {
public:
	typedef std::vector<std::shared_ptr<TypedNamedValue>> TProperties;
	PropertyEditor();
	PropertyEditor(TProperties &properties);

	virtual void resized() override;

	void setProperties(TProperties const &props);

private:
	PropertyPanel propertyPanel_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyEditor)
};
