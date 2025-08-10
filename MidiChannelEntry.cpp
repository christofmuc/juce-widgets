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

#include "MidiChannelEntry.h"

class MidiChannelEntryWithSnap : public juce::Slider {
public:
    MidiChannelEntryWithSnap()
    {
        setSliderStyle(juce::Slider::IncDecButtons);
        setRange(1.0, 16.0, 1.0);
        setNumDecimalPlacesToDisplay(0);
    }

    virtual double snapValue(double attemptedValue, DragMode dragMode) override
    {
        ignoreUnused(dragMode);
        return juce::roundToIntAccurate(attemptedValue);
    }
};

MidiChannelEntry::MidiChannelEntry(TSetMidiChannelCallback callback) : callback_(callback)
{
    slider_ = std::make_unique<MidiChannelEntryWithSnap>();
    addAndMakeVisible(slider_.get());
    slider_->addListener(this);
}

void MidiChannelEntry::setValue(MidiChannel newChannel)
{
    slider_->setValue(newChannel.toOneBasedInt(), juce::dontSendNotification);
}

void MidiChannelEntry::sliderValueChanged(juce::Slider* slider)
{
    if (slider == slider_.get()) {
        callback_(MidiChannel::fromOneBase(juce::roundToIntAccurate(slider_->getValue())));
    }
}

void MidiChannelEntry::resized()
{
    juce::Rectangle<int> area(getLocalBounds());
    int sliderHeight = 30;
    slider_->setBounds(0, (area.getHeight() - sliderHeight) / 2, area.getWidth(), sliderHeight);
}
