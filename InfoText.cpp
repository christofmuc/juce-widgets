/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "InfoText.h"

#include "ColourHelpers.h"

InfoText::InfoText()
{
	setMultiLine(true);
	setReadOnly(true);
	setColour(TextEditor::ColourIds::outlineColourId, ColourHelpers::getUIColour(this, LookAndFeel_V4::ColourScheme::windowBackground));
	setColour(TextEditor::ColourIds::backgroundColourId, ColourHelpers::getUIColour(this, LookAndFeel_V4::ColourScheme::windowBackground));
}

