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

#include "TouchButton.h"

TouchButton::TouchButton() : wasRightClick_(false)
{
}

void TouchButton::clicked(const juce::ModifierKeys& modifiers)
{
    TouchButtonFunction f = TouchButtonFunction::PRIMARY;
    if (modifiers.isAnyModifierKeyDown() || wasRightClick_) {
        f = TouchButtonFunction::SECONDARY;
    }
//    if (getMillisecondsSinceButtonDown() > 500) {
//    f = TouchButtonFunction::LONG;
//    }
    juce::NullCheckedInvocation::invoke(onClickMultifunction, f);
}

void TouchButton::mouseDown(const juce::MouseEvent& e)
{
    // Start a timer to auto release for long click
//    startTimer(500);
    TextButton::mouseDown(e);
}

void TouchButton::mouseUp(const juce::MouseEvent& e)
{
//    stopTimer();
    wasRightClick_ = e.mods.isRightButtonDown();
    TextButton::mouseUp(e);
}

void TouchButton::timerCallback()
{
    clicked(juce::ModifierKeys::currentModifiers);
}

TouchToggleButton::TouchToggleButton() : wasRightClick_(false)
{
}

void TouchToggleButton::clicked(const juce::ModifierKeys& modifiers)
{
    TouchButtonFunction f = TouchButtonFunction::PRIMARY;
    if (modifiers.isAnyModifierKeyDown() || wasRightClick_) {
        f = TouchButtonFunction::SECONDARY;
    }
  //  if (getMillisecondsSinceButtonDown() > 500) {
  //      f = TouchButtonFunction::LONG;
  //  }
    juce::NullCheckedInvocation::invoke(onClickMultifunction, f);
}

void TouchToggleButton::mouseDown(const juce::MouseEvent& e)
{
    // Start a timer to auto release for long click
//    startTimer(500);
    ToggleButton::mouseDown(e);
}

void TouchToggleButton::mouseUp(const juce::MouseEvent& e)
{
//    stopTimer();
    wasRightClick_ = e.mods.isRightButtonDown();
    ToggleButton::mouseUp(e);
}

void TouchToggleButton::timerCallback()
{
    clicked(juce::ModifierKeys::currentModifiers);
}
