#include "HorizontalLayoutContainer.h"

HorizontalLayoutContainer::HorizontalLayoutContainer(Component *left, Component *right, double leftSize, double rightSize) :
	left_(left), right_(right)
{
	addAndMakeVisible(left);
	addAndMakeVisible(right);

	layout_.setItemLayout(0, leftSize, leftSize, leftSize);
	layout_.setItemLayout(1, rightSize, rightSize, rightSize);
}

void HorizontalLayoutContainer::resized()
{
	Rectangle<int> area(getLocalBounds());
	Component* components[] = { left_, right_ };
	layout_.layOutComponents(components, 2, area.getX(), area.getY(), area.getWidth(), area.getHeight(), false, true);
}
