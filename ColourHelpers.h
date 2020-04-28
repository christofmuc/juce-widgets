/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class ColourHelpers {
public:
	static Colour getUIColour(Component *component, LookAndFeel_V4::ColourScheme::UIColour colourToGet);
};

