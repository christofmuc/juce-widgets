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

class ActiveListItem {
public:
    virtual std::string getName() = 0;
    virtual bool isActive() = 0;
    virtual juce::Colour getColour() = 0;
};

class SynthButtonWithActiveLight : public juce::Component {
public:
    SynthButtonWithActiveLight(std::string const &name, juce::Colour color, bool active);

    virtual void resized() override;

    std::string name() const;

    void setToggleState(bool toggleState);
    void setActiveState(bool activeState);

    std::function<void(std::string const &)> onSynthSelected;

private:
    juce::TextButton button_;
    juce::Label label_;
};

class SynthList : public juce::Component, public juce::ChangeListener {
public:
    void setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback);
    void setActiveListItem(std::string const &active);

    virtual void resized() override;

    // This is called when the synth list changes
    virtual void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback_;
    std::vector<std::shared_ptr<ActiveListItem>> synths_;
    juce::OwnedArray<SynthButtonWithActiveLight> buttons_;
};
