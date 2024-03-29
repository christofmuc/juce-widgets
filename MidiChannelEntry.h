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

#include "MidiChannel.h"

class MidiChannelEntry : public juce::Component, private juce::Slider::Listener {
public:
    typedef std::function<void(MidiChannel newChannel)> TSetMidiChannelCallback;

    MidiChannelEntry(TSetMidiChannelCallback callback);

    // Call this to set the MidiChannel
    void setValue(MidiChannel newChannel);

    // Implementation of behavior
    virtual void sliderValueChanged(juce::Slider* slider) override;
    virtual void resized() override;

private:
    TSetMidiChannelCallback callback_;
    std::unique_ptr<juce::Slider> slider_;
};
