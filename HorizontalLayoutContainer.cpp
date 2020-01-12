/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "HorizontalLayoutContainer.h"

#include "InsetBox.h"

HorizontalLayoutContainer::HorizontalLayoutContainer(Component *left, Component *right, double leftSize, double rightSize) :
	left_(new InsetBox(left, BorderSize<int>(0, 0, 0, 4))), right_(new InsetBox(right, BorderSize<int>(0, 4, 0, 0)))
{
	addAndMakeVisible(left_);
	addAndMakeVisible(right_);

	layout_.setItemLayout(0, leftSize, leftSize, leftSize);
	layout_.setItemLayout(1, rightSize, rightSize, rightSize);
}

void HorizontalLayoutContainer::resized()
{
	Rectangle<int> area(getLocalBounds());
	Component* components[] = { left_, right_ };
	layout_.layOutComponents(components, 2, area.getX(), area.getY(), area.getWidth(), area.getHeight(), false, true);
}
