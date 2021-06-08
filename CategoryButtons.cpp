/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "CategoryButtons.h"

#include "ColourHelpers.h"
#include "LayoutConstants.h"
#include "FlexBoxHelper.h"

class ColouredCheckbox : public ToggleButton {
public:
	ColouredCheckbox(String text, Colour colour) : ToggleButton(text), colour_(colour) {
	}

protected:
	void paintButton(Graphics &g, bool b, bool c) override
	{
		g.setFillType(FillType(colour_));
		g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
		ToggleButton::paintButton(g, b, c);
	}

private:
	Colour colour_;
};

CategoryButtons::CategoryButtons(std::vector<Category> const &categories, std::function<void(Category)> updated, bool colouredButtons, bool useCheckboxes) 
	: updateHandler_(updated), useCheckboxes_(useCheckboxes), colouredButtons_(colouredButtons), usedHeight_(0)
{
	setCategories(categories);
}

void CategoryButtons::setCategories(std::vector<Category> const &categories)
{
	categories_ = categories;
	categoryFilter_.clear();

	// Build filter buttons for the categories
	for (const auto& c : categories_) {
		Button *button;
		if (useCheckboxes_) {
			if (colouredButtons_) {
				button = new ColouredCheckbox(c.category, c.color);
			}
			else {
				button = new ToggleButton(c.category);
			}
		}
		else {
			button = new TextButton(c.category);
			button->setColour(TextButton::ColourIds::buttonOnColourId, c.color);
			button->setClickingTogglesState(true);
		}
		button->addListener(this);
		addAndMakeVisible(button);
		categoryFilter_.add(button);
	}
	resized();
}

bool CategoryButtons::isAtLeastOne() const
{
	return std::any_of(categoryFilter_.begin(), categoryFilter_.end(), [](Button *button) { return button->getToggleState();  });
}

std::vector<CategoryButtons::Category> CategoryButtons::selectedCategories() const
{
	// Build list of categories wanted by the user
	std::vector<Category> categories;
	for (auto button : categoryFilter_) {
		if (button->getToggleState()) {
			const auto& category = button->getButtonText();
			for (auto predef : categories_) {
				if (predef.category == category) {
					categories.push_back(predef);
				}
			}
		}
	}
	return categories;
}

void CategoryButtons::resized()
{
	// Using Flex Box as we need an overflow
	FlexBox fb;
	fb.flexWrap = FlexBox::Wrap::wrap;
	fb.flexDirection = FlexBox::Direction::row;
	fb.justifyContent = FlexBox::JustifyContent::center;
	//fb.alignItems = FlexBox::AlignItems::flexEnd; // This is horizontal, but only works when align-self is auto
	fb.alignContent = FlexBox::AlignContent::flexStart; // This is cross axis, up
	for (auto filterbutton : categoryFilter_) {
		filterbutton->setSize(LAYOUT_CHECKBOX_WIDTH, LAYOUT_LINE_HEIGHT);
		((ToggleButton*)filterbutton)->changeWidthToFitText();
		fb.items.add(FlexItem(*filterbutton).withMinWidth((float) filterbutton->getWidth() + 20.0f).withMinHeight(LAYOUT_LINE_HEIGHT).withMargin(LAYOUT_INSET_SMALL)); // .withAlignSelf(FlexItem::AlignSelf::autoAlign)
	}
	fb.performLayout(getLocalBounds().toFloat());
	if (getNumChildComponents() > 0) {
		usedHeight_ = getChildComponent(getNumChildComponents() - 1)->getBottom();
	}
}

void CategoryButtons::buttonClicked(Button* button)
{
	const auto& category = button->getButtonText();
	for (auto predef : categories_) {
		if (predef.category == category) {
			updateHandler_(predef);
		}
	}
}

juce::Rectangle<float> CategoryButtons::determineSubAreaForButtonLayout(Component* parent, Rectangle<int> const& bounds)
{
	return FlexBoxHelper::determineSizeForButtonLayout(this, parent, categoryFilter_, bounds);
}

int CategoryButtons::numCategories() const
{
	return categoryFilter_.size();
}

int CategoryButtons::usedHeight() const
{
	return usedHeight_;
}

void CategoryButtons::setActive(std::set<Category> const &activeCategories)
{
	for (auto button : categoryFilter_) {
		bool found = false;
		for (auto cat : activeCategories) {
			if (cat.category == button->getButtonText()) {
				found = true;
			}
		}
		button->setToggleState(found, dontSendNotification);
	}
}

bool operator<(CategoryButtons::Category const &left, CategoryButtons::Category const &right)
{
	return left.category < right.category;
}
