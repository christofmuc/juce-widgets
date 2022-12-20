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
#include "RecycleBin.h"

#include "BinaryResources.h"
#include "IconHelper.h"

RecycleBin::RecycleBin()
{
    IconHelper::setupIcon(this, wasteBin_, delete_png, delete_png_size, 128);
    addAndMakeVisible(wasteBin_);
}

void RecycleBin::resized()
{
    auto bounds = getLocalBounds();
    wasteBin_.setBounds(bounds);
    wasteBin_.setImagePlacement(juce::RectanglePlacement::xMid | juce::RectanglePlacement::yMid | juce::RectanglePlacement::onlyReduceInSize);
}

bool RecycleBin::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    ignoreUnused(dragSourceDetails);
    return onItemDropped != nullptr;
}

void RecycleBin::itemDropped(const SourceDetails& dragSourceDetails)
{
    if (onItemDropped) onItemDropped(dragSourceDetails.description);
}

void RecycleBin::mouseUp(const juce::MouseEvent& event)
{
    juce::ignoreUnused(event);
    if (onClicked) {
        onClicked();
    }
}
