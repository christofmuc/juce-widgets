/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class HorizontalLayoutContainer : public Component {
public:
	HorizontalLayoutContainer(Component *left, Component *right, double leftSize, double rightSize);

	void resized() override;

	void setComponents(Component *left, Component *right);

private:
	Component *left_;
	Component *right_;
	StretchableLayoutManager layout_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HorizontalLayoutContainer)
};
