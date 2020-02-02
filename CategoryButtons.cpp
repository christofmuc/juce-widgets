/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "CategoryButtons.h"

CategoryButtons::CategoryButtons(std::vector<Category> const &categories, std::function<void()> updated, bool colouredButtons) : categories_(categories), updateHandler_(updated)
{
	// Build filter buttons for the categories
	for (const auto& c : categories_) {
		auto button = new TextButton(c.category);
		button->setColour(TextButton::ColourIds::buttonOnColourId, colouredButtons ? c.color : c.color.darker());
		button->setColour(TextButton::ColourIds::buttonColourId, colouredButtons ? c.color.darker() : Colours::black);
		button->setClickingTogglesState(true);
		button->addListener(this);
		addAndMakeVisible(button);
		categoryFilter_.add(button);
	}
}

bool CategoryButtons::isAtLeastOne() const
{
	return std::any_of(categoryFilter_.begin(), categoryFilter_.end(), [](TextButton *button) { return button->getToggleState();  });
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

void CategoryButtons::buttonClicked(Button*)
{
	updateHandler_();
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
