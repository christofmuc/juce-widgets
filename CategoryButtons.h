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
		Category(std::string const &c, Colour o, int i) : category(c), color(o), bitIndex(i) {}
		std::string category;
		Colour color;
		int bitIndex;
	};

	CategoryButtons(std::vector<Category> const &categories, std::function<void(Category)> updated, bool colouredButtons);

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
	std::function<void(Category)> updateHandler_;
};

// To allow storing a set of these categories
bool operator <(CategoryButtons::Category const &left, CategoryButtons::Category const &right);
