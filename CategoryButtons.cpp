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

#include "CategoryButtons.h"

#include "ColourHelpers.h"
#include "FlexBoxHelper.h"
#include "LayoutConstants.h"

class ColouredCheckbox : public ToggleButton {
public:
    ColouredCheckbox(String text, Colour colour) : ToggleButton(text), colour_(colour) {}

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

CategoryButtons::CategoryButtons(std::vector<Category> const &categories, std::function<void(Category)> updated, bool colouredButtons,
    bool useCheckboxes) :
    useCheckboxes_(useCheckboxes)
    , colouredButtons_(colouredButtons)
    , updateHandler_(updated)
    , buttonWidth_(LAYOUT_BUTTON_WIDTH)
    , buttonHeight_(LAYOUT_LINE_HEIGHT)
    , usedHeight_(0)
{
    setCategories(categories);
}

void CategoryButtons::setCategories(std::vector<Category> const &categories)
{
    categories_ = categories;
    categoryFilter_.clear();

    // Build filter buttons for the categories
    for (const auto &c : categories_) {
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
    return std::any_of(categoryFilter_.begin(), categoryFilter_.end(), [](Button *button) { return button->getToggleState(); });
}

std::vector<CategoryButtons::Category> CategoryButtons::selectedCategories() const
{
    // Build list of categories wanted by the user
    std::vector<Category> categories;
    for (auto button : categoryFilter_) {
        if (button->getToggleState()) {
            const auto &category = button->getButtonText();
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
    // fb.alignItems = FlexBox::AlignItems::flexEnd; // This is horizontal, but only works when align-self is auto
    fb.alignContent = FlexBox::AlignContent::flexStart; // This is cross axis, up
    for (auto filterbutton : categoryFilter_) {
        filterbutton->setSize(buttonWidth_, buttonHeight_);
        ((ToggleButton *) filterbutton)->changeWidthToFitText();
        fb.items.add(FlexItem(*filterbutton)
                         .withMinWidth((float) filterbutton->getWidth() + 20.0f)
                         .withMinHeight(static_cast<float>(buttonHeight_))
                         .withMargin(LAYOUT_INSET_SMALL)); // .withAlignSelf(FlexItem::AlignSelf::autoAlign)
    }
    fb.performLayout(getLocalBounds().toFloat());
    if (getNumChildComponents() > 0) {
        usedHeight_ = getChildComponent(getNumChildComponents() - 1)->getBottom();
    }
}

void CategoryButtons::buttonClicked(Button *button)
{
    const auto &category = button->getButtonText();
    for (auto predef : categories_) {
        if (predef.category == category) {
            updateHandler_(predef);
        }
    }
}

juce::Rectangle<float> CategoryButtons::determineSubAreaForButtonLayout(Component *parent, Rectangle<int> const &bounds)
{
    return FlexBoxHelper::determineSizeForButtonLayout(this, parent, categoryFilter_, bounds, buttonWidth_, buttonHeight_);
}

int CategoryButtons::numCategories() const
{
    return categoryFilter_.size();
}

int CategoryButtons::usedHeight() const
{
    return usedHeight_;
}

void CategoryButtons::setButtonSize(int width, int height) 
{
    buttonWidth_ = width;
    buttonHeight_ = height;
    resized();
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
