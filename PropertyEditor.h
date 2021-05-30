/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

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
	void clear();

	static PropertyComponent *createEditor(std::shared_ptr<TypedNamedValue> property);

private:
	PropertyPanel propertyPanel_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropertyEditor)
};
