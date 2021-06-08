/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "FlexBoxHelper.h"

#include "LayoutConstants.h"

juce::Rectangle<float> FlexBoxHelper::computeFlexBoxSize(FlexBox box)
{
	RectangleList<float> layout;

	for (auto& item : box.items)
	{
		layout.add(item.currentBounds
			.withTrimmedLeft(-item.margin.left)
			.withTrimmedRight(-item.margin.right)
			.withTrimmedTop(-item.margin.top)
			.withTrimmedBottom(-item.margin.bottom));
	}
	return layout.getBounds();
}

juce::Rectangle<float> FlexBoxHelper::determineSizeForButtonLayout(Component* component, Component* parent, OwnedArray<Button> &buttons, Rectangle<int> const& bounds)
{
	std::vector<Component*> components;
	for (auto button : buttons)
		components.push_back(button);
	return determineSizeForButtonLayout(component, parent, components, bounds);
}

juce::Rectangle<float> FlexBoxHelper::determineSizeForButtonLayout(Component* component, Component* parent, std::vector<Component*> components, Rectangle<int> const& bounds)
{
	// Using Flex Box as we need an overflow
	FlexBox fb;
	fb.flexWrap = FlexBox::Wrap::wrap;
	fb.flexDirection = FlexBox::Direction::row;
	fb.justifyContent = FlexBox::JustifyContent::center;
	//fb.alignItems = FlexBox::AlignItems::flexEnd; // This is horizontal, but only works when align-self is auto
	fb.alignContent = FlexBox::AlignContent::flexStart; // This is cross axis, up
	for (auto filterbutton : components) {
		filterbutton->setSize(LAYOUT_CHECKBOX_WIDTH, LAYOUT_LINE_HEIGHT);
		((ToggleButton*)filterbutton)->changeWidthToFitText();
		fb.items.add(FlexItem(*filterbutton).withMinWidth((float)filterbutton->getWidth() + 20.0f).withMinHeight(LAYOUT_LINE_HEIGHT).withMargin(LAYOUT_INSET_SMALL)); // .withAlignSelf(FlexItem::AlignSelf::autoAlign)
	}
	auto newBounds = component->getLocalArea(parent, bounds); // Need to work in local coordinates
	fb.performLayout(newBounds);
	auto minSize = computeFlexBoxSize(fb);
	return parent->getLocalArea(component, minSize); // Back into parent coordinates
}
