#pragma once

#include "JuceHeader.h"

#include "TypedNamedValue.h"

class PropertyEditor : public Component {
public:
	typedef std::vector<std::shared_ptr<TypedNamedValue>> TProperties;
	PropertyEditor(TProperties &properties);

	virtual void resized() override;

	void setProperties(TProperties &properties);

private:
	TProperties &properties_;
	PropertyPanel propertyPanel_;
};
