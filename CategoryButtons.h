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
	struct Category {
		Category(std::string const &cat, Colour const &color) : category(cat), color(color) {
		}
		std::string category;
		Colour color;
	};

	CategoryButtons(std::vector<Category> const &categories, std::function<void()> updated, bool colouredButtons);

	bool isAtLeastOne() const;
	std::vector<Category> selectedCategories() const;
	void setActive(std::set<Category> const &activeCategories);

	// Override Component method
	void resized() override;
	// Implement button listener
	void buttonClicked(Button*) override;

private:
	std::vector<Category> categories_;
	OwnedArray<TextButton> categoryFilter_;
	std::function<void()> updateHandler_;
};

// To allow storing a set of these categories
bool operator <(CategoryButtons::Category const &left, CategoryButtons::Category const &right);
