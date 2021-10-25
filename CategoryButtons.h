/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "JuceHeader.h"

#include <set>

class CategoryButtons : public Component, private ToggleButton::Listener {
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
    void buttonClicked(Button *) override;

    Rectangle<float> determineSubAreaForButtonLayout(Component *parent, Rectangle<int> const &bounds);

    int numCategories() const;
    int usedHeight() const;

private:
    bool useCheckboxes_;
    bool colouredButtons_;
    std::vector<Category> categories_;
    OwnedArray<Button> categoryFilter_;
    std::function<void(Category)> updateHandler_;
    int usedHeight_; // Cache layout result on how much y space we need on screen

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CategoryButtons)
};

// To allow storing a set of these categories
bool operator<(CategoryButtons::Category const &left, CategoryButtons::Category const &right);
