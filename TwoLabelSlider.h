/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class TwoLabelSlider : public Component {
public:
	TwoLabelSlider(String left, String right, int labelWidth);

	Slider& slider();

	virtual void resized() override;

private:
	int labelWidth_;

	Label leftLabel_;
	Label rightLabel_;
	Slider slider_;
};

