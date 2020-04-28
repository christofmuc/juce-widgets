/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "ColourHelpers.h"

juce::Colour ColourHelpers::getUIColour(Component *component, LookAndFeel_V4::ColourScheme::UIColour colourToGet)
{
	auto lAF = &component->getLookAndFeel();
	auto v4 = dynamic_cast<LookAndFeel_V4 *>(lAF);
	if (v4) {
		auto colorScheme = v4->getCurrentColourScheme();
		return colorScheme.getUIColour(colourToGet);
	}
	jassertfalse;
	return Colours::black;
}
