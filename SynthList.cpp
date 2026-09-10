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

#include "SynthList.h"

#include "LayoutConstants.h"
#include <algorithm>

SynthButtonWithActiveLight::SynthButtonWithActiveLight(std::string const &name, juce::Colour color, bool active)
{
    button_.setButtonText(name);
    button_.setClickingTogglesState(true);
    button_.setColour(juce::TextButton::buttonOnColourId, color);
    button_.onClick = [this]() {
        if (button_.getToggleState()) {
            onSynthSelected(this->name());
        }
    };
    addAndMakeVisible(button_);

    status_.setTitle("Check connection for " + juce::String(name));
    status_.setName("Check connection for " + juce::String(name));
    status_.onClick = [this]() { if (onCheckConnection) onCheckConnection(this->name()); };
    button_.onContextMenu = status_.onContextMenu = [this]() { showConnectionMenu(); };
    setActiveState(active);
    addAndMakeVisible(status_);
}

void SynthButtonWithActiveLight::resized()
{
    auto activeArea = getLocalBounds();
    auto bottomrow = activeArea.removeFromBottom(20);
    auto toprow = activeArea.removeFromTop(LAYOUT_LARGE_LINE_HEIGHT);
    button_.setBounds(toprow);
    status_.setBounds(bottomrow);
}

std::string SynthButtonWithActiveLight::name() const
{
    return button_.getButtonText().toStdString();
}

void SynthButtonWithActiveLight::setToggleState(bool toggleState)
{
    button_.setToggleState(toggleState, juce::dontSendNotification);
}

void SynthButtonWithActiveLight::setActiveState(bool activeState)
{
    status_.detected = activeState;
    status_.setTooltip(activeState ? "Previously detected - click to check the saved connection"
        : "Not detected - click to check the saved connection");
    if (!status_.isEnabled()) status_.setTooltip("Automatic detection is not available for this synth; use Setup to configure it");
    status_.repaint();
}

void SynthButtonWithActiveLight::setConnectionActionsEnabled(bool enabled)
{
    status_.setEnabled(enabled);
    if (!enabled) status_.setTooltip("Automatic detection is not available for this synth; use Setup to configure it");
}

void SynthButtonWithActiveLight::StatusButton::paintButton(juce::Graphics &g, bool highlighted, bool down)
{
    auto colour = detected ? juce::Colours::darkgreen : juce::Colours::darkgrey;
    g.setColour(highlighted || down ? colour.brighter() : colour);
    g.fillRect(getLocalBounds().reduced(LAYOUT_INSET_NORMAL));
    if (isEnabled() && (highlighted || down || hasKeyboardFocus(false))) {
        g.setColour(juce::Colours::white);
        g.setFont(12.0f);
        g.drawText("Check", getLocalBounds(), juce::Justification::centred);
    }
}

void SynthButtonWithActiveLight::showConnectionMenu()
{
    juce::PopupMenu menu;
    menu.addItem(1, "Check connection", status_.isEnabled() && bool(onCheckConnection));
    menu.addItem(2, "Find this synth...", status_.isEnabled() && bool(onFindSynth));
    juce::Component::SafePointer<SynthButtonWithActiveLight> safeThis(this);
    menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&button_), [safeThis](int result) {
        if (!safeThis) return;
        if (result == 1 && safeThis->onCheckConnection) safeThis->onCheckConnection(safeThis->name());
        if (result == 2 && safeThis->onFindSynth) safeThis->onFindSynth(safeThis->name());
    });
}

void SynthList::setList(std::vector<std::shared_ptr<ActiveListItem>> &synths,
    ItemCallback synthSwitchCallback, ItemCallback checkConnectionCallback, ItemCallback findSynthCallback)
{
    buttons_.clear();
    synths_ = synths;
    std::sort(synths_.begin(), synths_.end(), [](auto const& lhs, auto const& rhs) {
        return lhs->getName() < rhs->getName();
    });
    synthSwitchCallback_ = synthSwitchCallback;
    for (auto synth : synths_) {
        auto button = new SynthButtonWithActiveLight(synth->getName(), synth->getColour(), synth->isActive());
        if (checkConnectionCallback) button->onCheckConnection = [synth, checkConnectionCallback](std::string const &) {
            checkConnectionCallback(synth);
        };
        if (findSynthCallback) button->onFindSynth = [synth, findSynthCallback](std::string const &) {
            findSynthCallback(synth);
        };
        button->setConnectionActionsEnabled(synth->canCheckConnection() && bool(checkConnectionCallback));
        button->onSynthSelected = [this](std::string const &name) {
            for (auto s : synths_) {
                if (name == s->getName()) {
                    setActiveListItem(name);
                    synthSwitchCallback_(s);
                    return;
                }
            }
            jassertfalse;
        };
        button->setToggleState(buttons_.size() == 0);
        buttons_.add(button);
        addAndMakeVisible(button);
    }
    resized();
}

void SynthList::setActiveListItem(std::string const &active)
{
    for (auto button : buttons_) {
        button->setToggleState(button->name() == active);
    }
}

void SynthList::resized()
{
    juce::Rectangle<int> area(getLocalBounds());
    int width = buttons_.size() != 0 ? std::min(area.getWidth() / buttons_.size(), LAYOUT_BUTTON_WIDTH + LAYOUT_INSET_NORMAL) : 0;

    auto activeArea = area.removeFromRight(width * buttons_.size());

    // Horizontal layout
    for (int i = 0; i < buttons_.size(); i++) {
        int rightMargin = 0;
        if (i != buttons_.size() - 1) rightMargin = LAYOUT_INSET_NORMAL;
        buttons_[i]->setBounds(activeArea.removeFromLeft(width).withTrimmedRight(rightMargin));
    }
}

void SynthList::changeListenerCallback(juce::ChangeBroadcaster *)
{
    // Update the availability of the synths (not the list itself)
    int i = 0;
    for (auto synth : synths_) {
        buttons_[i]->setActiveState(synth->isActive());
        i++;
    }
}
