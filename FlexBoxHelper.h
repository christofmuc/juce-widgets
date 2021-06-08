/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class FlexBoxHelper {
public:
	static Rectangle<float> computeFlexBoxSize(FlexBox box);

	static Rectangle<float> determineSizeForButtonLayout(Component* component, Component* parent, std::vector<Component *> components, Rectangle<int> const& bounds);
	static Rectangle<float> determineSizeForButtonLayout(Component *component, Component* parent, OwnedArray<Button> &buttons, Rectangle<int> const& bounds);
};

