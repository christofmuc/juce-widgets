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

class ActiveListItem {
public:
    virtual std::string getName() = 0;
    virtual bool isActive() = 0;
    virtual juce::Colour getColour() = 0;
    virtual bool canCheckConnection() { return true; }
};

class SynthButtonWithActiveLight : public juce::Component {
public:
    SynthButtonWithActiveLight(std::string const &name, juce::Colour color, bool active);

    virtual void resized() override;

    std::string name() const;

    void setToggleState(bool toggleState);
    void setActiveState(bool activeState);
    void setConnectionActionsEnabled(bool enabled);

    std::function<void(std::string const &)> onSynthSelected;
    std::function<void(std::string const &)> onCheckConnection;
    std::function<void(std::string const &)> onFindSynth;

private:
    class ContextButton : public juce::TextButton {
    public:
        std::function<void()> onContextMenu;
        void mouseDown(juce::MouseEvent const &event) override {
            if (event.mods.isPopupMenu()) { if (onContextMenu) onContextMenu(); }
            else juce::TextButton::mouseDown(event);
        }
        void mouseUp(juce::MouseEvent const &event) override {
            if (!event.mods.isPopupMenu()) juce::TextButton::mouseUp(event);
        }
    };
    class StatusButton : public ContextButton {
    public:
        bool detected = false;
        void paintButton(juce::Graphics &g, bool highlighted, bool down) override;
    };
    void showConnectionMenu();
    ContextButton button_;
    StatusButton status_;
};

class SynthList : public juce::Component, public juce::ChangeListener {
public:
    using ItemCallback = std::function<void(std::shared_ptr<ActiveListItem>)>;
    void setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, ItemCallback synthSwitchCallback,
        ItemCallback checkConnectionCallback = {}, ItemCallback findSynthCallback = {});
    void setActiveListItem(std::string const &active);

    virtual void resized() override;

    // This is called when the synth list changes
    virtual void changeListenerCallback(juce::ChangeBroadcaster *source) override;

private:
    std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback_;
    std::vector<std::shared_ptr<ActiveListItem>> synths_;
    juce::OwnedArray<SynthButtonWithActiveLight> buttons_;
};
