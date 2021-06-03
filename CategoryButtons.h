/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include <set>

class CategoryButtons : public Component,
	private ToggleButton::Listener 
{
public:
	class Category {
	public:
		Category(std::string const &c, Colour o) : category(c), color(o) {}
		std::string category;
		Colour color;
	};

	CategoryButtons(std::vector<Category> const &categories, std::function<void(Category)> updated, bool colouredButtons, bool useCheckboxes);

	void setCategories(std::vector<Category> const &categories);

	bool isAtLeastOne() const;
	std::vector<Category> selectedCategories() const;
	void setActive(std::set<Category> const &activeCategories);

	// Override Component method
	void resized() override;
	// Implement button listener
	void buttonClicked(Button*) override;

	int numCategories() const;

private:
	bool useCheckboxes_;
	bool colouredButtons_;
	std::vector<Category> categories_;
	OwnedArray<Button> categoryFilter_;
	std::function<void(Category)> updateHandler_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryButtons)
};

// To allow storing a set of these categories
bool operator <(CategoryButtons::Category const &left, CategoryButtons::Category const &right);
