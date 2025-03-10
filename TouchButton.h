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

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

enum class TouchButtonFunction {
    PRIMARY,  // Without any modifiers
    SECONDARY, // With any modifier or right click
    LONG  // A long press
};


class TouchButton : public juce::TextButton, private juce::Timer {
public:
    TouchButton();

    std::function<void(TouchButtonFunction f)> onClickMultifunction;

protected:
    // Override these to implement special behavior
    virtual void clicked(const juce::ModifierKeys&) override;
    virtual void mouseDown(const juce::MouseEvent& e) override;
    virtual void mouseUp(const juce::MouseEvent& e) override;

private:
    virtual void timerCallback() override;
    bool wasRightClick_ = false;
};


class TouchToggleButton : public juce::ToggleButton, private juce::Timer {
public:
    TouchToggleButton();

    std::function<void(TouchButtonFunction f)> onClickMultifunction;

protected:
    // Override these to implement special behavior
    virtual void clicked(const juce::ModifierKeys&) override;
    virtual void mouseDown(const juce::MouseEvent& e) override;
    virtual void mouseUp(const juce::MouseEvent& e) override;

private:
    virtual void timerCallback() override;
    bool wasRightClick_ = false;
};
