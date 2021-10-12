/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "TwoLabelSlider.h"

TwoLabelSlider::TwoLabelSlider(String left, String right, int labelWidth) : labelWidth_(labelWidth)
{
	leftLabel_.setText(left, dontSendNotification);
	rightLabel_.setText(right, dontSendNotification);

	addAndMakeVisible(leftLabel_);
	addAndMakeVisible(rightLabel_);
	addAndMakeVisible(slider_);
}

juce::Slider& TwoLabelSlider::slider()
{
	return slider_;
}

void TwoLabelSlider::resized()
{
	auto area = getLocalBounds();
	leftLabel_.setBounds(area.removeFromLeft(labelWidth_));
	rightLabel_.setBounds(area.removeFromRight(labelWidth_));
	slider_.setBounds(area.withTrimmedLeft(8).withTrimmedRight(8));
}
