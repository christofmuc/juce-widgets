/*
 * MIT License
 *
 * Copyright (c) 2019-2025 Christof Ruch
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

#include <juce_gui_basics/juce_gui_basics.h>

#include "LayoutConstants.h"

class FlexBoxHelper {
public:
    static juce::Rectangle<float> computeFlexBoxSize(juce::FlexBox box);

    static juce::Rectangle<float> determineSizeForButtonLayout(juce::Component* component, juce::Component* parent,
        std::vector<juce::Component*> components, juce::Rectangle<int> const& bounds, int buttonWidth = LAYOUT_BUTTON_WIDTH,
        int buttonHeight = LAYOUT_LINE_HEIGHT);
    static juce::Rectangle<float> determineSizeForButtonLayout(juce::Component* component, juce::Component* parent,
        juce::OwnedArray<juce::Button>& buttons, juce::Rectangle<int> const& bounds, int buttonWidth = LAYOUT_BUTTON_WIDTH,
        int buttonHeight = LAYOUT_LINE_HEIGHT);
};
