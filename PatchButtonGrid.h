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

#include "LayoutConstants.h"

template <class P> class PatchButtonGrid : public juce::Component {
public:
    PatchButtonGrid(int w, int h, std::function<void(int)> buttonClicked) : columns_(w), rows_(h)
    {
        for (int patch = 0; patch < w * h; patch++) {
            auto patchButton = new P(patch, true, [this, buttonClicked](int index) {
                disableAllExcept(index);
                buttonClicked(index);
            });
            addAndMakeVisible(patchButton);
            patchButtons_.add(patchButton);
        }
    }

    virtual void resized() override
    {
        Rectangle<int> area(getLocalBounds());

        juce::Grid grid;
        grid.setGap(juce::Grid::Px(LAYOUT_INSET_SMALL));
        using Track = juce::Grid::TrackInfo;
        for (int i = 0; i < rows_; i++)
            grid.templateRows.add(Track(1_fr));
        for (int i = 0; i < columns_; i++)
            grid.templateColumns.add(Track(1_fr));
        for (auto patchbutton : patchButtons_) {
            grid.items.add(juce::GridItem(*patchbutton));
        }
        grid.performLayout(area);
    }

    P *buttonWithIndex(int index) { return patchButtons_[index]; }

    std::vector<int> getSelected() const
    {
        std::vector<int> result;
        for (int i = 0; i < patchButtons_.size(); i++) {
            if (patchButtons_[i]->getToggleState()) {
                result.push_back(i);
            }
        }
        return result;
    }

    size_t size() const { return patchButtons_.size(); }

private:
    void disableAllExcept(int index)
    {
        for (int i = 0; i < patchButtons_.size(); i++) {
            if (index != i) {
                patchButtons_[i]->setToggleState(false);
            }
        }
    }

    int columns_;
    int rows_;
    juce::OwnedArray<P> patchButtons_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchButtonGrid)
};
