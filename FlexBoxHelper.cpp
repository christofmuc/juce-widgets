/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

#include "FlexBoxHelper.h"

#include "LayoutConstants.h"

juce::Rectangle<float> FlexBoxHelper::computeFlexBoxSize(juce::FlexBox box)
{
    juce::RectangleList<float> layout;

    for (auto& item : box.items) {
        layout.add(item.currentBounds.withTrimmedLeft(-item.margin.left)
                       .withTrimmedRight(-item.margin.right)
                       .withTrimmedTop(-item.margin.top)
                       .withTrimmedBottom(-item.margin.bottom));
    }
    return layout.getBounds();
}

juce::Rectangle<float> FlexBoxHelper::determineSizeForButtonLayout(juce::Component* component, juce::Component* parent,
    juce::OwnedArray<juce::Button>& buttons, juce::Rectangle<int> const& bounds, int buttonWidth, int buttonHeight)
{
    std::vector<juce::Component*> components;
    for (auto button : buttons)
        components.push_back(button);
    return determineSizeForButtonLayout(component, parent, components, bounds, buttonWidth, buttonHeight);
}

juce::Rectangle<float> FlexBoxHelper::determineSizeForButtonLayout(juce::Component* component, juce::Component* parent,
    std::vector<juce::Component*> components, juce::Rectangle<int> const& bounds, int buttonWidth, int buttonHeight)
{
    // Using Flex Box as we need an overflow
    juce::FlexBox fb;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    // fb.alignItems = juce::FlexBox::AlignItems::flexEnd; // This is horizontal, but only works when align-self is auto
    fb.alignContent = juce::FlexBox::AlignContent::flexStart; // This is cross axis, up
    for (auto filterbutton : components) {
        filterbutton->setSize(buttonWidth, buttonHeight);
        ((juce::ToggleButton*) filterbutton)->changeWidthToFitText();
        fb.items.add(juce::FlexItem(*filterbutton)
                         .withMinWidth((float) filterbutton->getWidth() + 20.0f)
                         .withMinHeight(static_cast<float>(buttonHeight))
                         .withMargin(LAYOUT_INSET_SMALL)); // .withAlignSelf(FlexItem::AlignSelf::autoAlign)
    }
    auto newBounds = component->getLocalArea(parent, bounds); // Need to work in local coordinates
    fb.performLayout(newBounds);
    auto minSize = computeFlexBoxSize(fb);
    return parent->getLocalArea(component, minSize); // Back into parent coordinates
}
