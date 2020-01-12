/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class InsetBox : public Component
{
public:
	InsetBox(Component *content, BorderSize<int> borders);

	void resized() override;

private:
	Component *content_;
	BorderSize<int> borders_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InsetBox)
};
