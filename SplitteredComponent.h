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

struct SplitteredEntry {
    juce::Component* component;
    int desiredPercentage;
    int minimumPercentage;
    int maximumPercentage;
};

class SplitteredComponent : public juce::Component {
public:
    SplitteredComponent(std::string const& componentName, SplitteredEntry first, SplitteredEntry second, bool isVertical);
    SplitteredComponent(std::string const& componentName, SplitteredEntry first, SplitteredEntry second, SplitteredEntry third, bool isVertical);
    virtual ~SplitteredComponent();

    virtual void resized();

private:
    std::string componentName_;
    juce::Component* first_;
    juce::Component* second_;
    juce::Component* third_;
    bool isVertical_;

    juce::StretchableLayoutManager stretchableManager_;
    juce::OwnedArray<juce::StretchableLayoutResizerBar> resizerBars_;
    int nComponents_;
    Component** componentOrder_;
    bool didLoad_;
};
