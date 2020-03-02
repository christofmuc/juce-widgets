/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "CategoryButtons.h"

class ColouredCheckbox : public ToggleButton {
public:
	ColouredCheckbox(String text, Colour colour) : ToggleButton(text), colour_(colour) {
	}

protected:
	void paintButton(Graphics &g, bool b, bool c) override
	{
		g.fillAll(colour_);
		ToggleButton::paintButton(g, b, c);
	}

private:
	Colour colour_;
};

CategoryButtons::CategoryButtons(std::vector<Category> const &categories, std::function<void(Category)> updated, bool colouredButtons, bool useCheckboxes) 
	: categories_(categories), updateHandler_(updated), useCheckboxes_(useCheckboxes)
{
	// Build filter buttons for the categories
	for (const auto& c : categories_) {
		Button *button;
		if (useCheckboxes) {
			if (colouredButtons) {
				button = new ColouredCheckbox(c.category, c.color.darker());
			}
			else {
				button = new ToggleButton(c.category);
			}
		}
		else {
			button = new TextButton(c.category);
			button->setColour(TextButton::ColourIds::buttonOnColourId, colouredButtons ? c.color : c.color.darker());
			button->setColour(TextButton::ColourIds::buttonColourId, colouredButtons ? c.color.darker() : Colours::black);
			button->setClickingTogglesState(true);
		}
		button->addListener(this);
		addAndMakeVisible(button);
		categoryFilter_.add(button);
	}
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
	if (!useCheckboxes_) {
		// One row grid for the filter buttons
		juce::Grid grid;
		grid.setGap(4_px);
		using Track = juce::Grid::TrackInfo;
		grid.templateRows.add(Track(1_fr));
		for (int i = 0; i < categoryFilter_.size(); i++) grid.templateColumns.add(Track(1_fr));
		for (auto filterbutton : categoryFilter_) {
			grid.items.add(juce::GridItem(*filterbutton));
		}
		grid.performLayout(getLocalBounds());
	}
	else {
		// Using Flex Box as we need an overflow
		FlexBox fb;
		fb.flexWrap = FlexBox::Wrap::wrap;
		fb.flexDirection = FlexBox::Direction::row;
		fb.justifyContent = FlexBox::JustifyContent::center;
		fb.alignItems = FlexBox::AlignItems::stretch;
		for (auto filterbutton : categoryFilter_) {
			filterbutton->setSize(100, 20);
			((ToggleButton*)filterbutton)->changeWidthToFitText();
			fb.items.add(FlexItem(*filterbutton).withMinWidth((float) filterbutton->getWidth() + 20.0f).withMinHeight(20.0f).withMargin(4));
		}
		fb.performLayout(getLocalBounds().toFloat());
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
