/*
   Copyright (c) 2020-2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "SynthList.h"

#include "LayoutConstants.h"

SynthButtonWithActiveLight::SynthButtonWithActiveLight(std::string const& name, Colour color, bool active)
{
	button_.setButtonText(name);
	button_.setClickingTogglesState(true);
	button_.setColour(TextButton::buttonOnColourId, color);
	button_.onClick = [this]() {
		if (button_.getToggleState()) {
			onSynthSelected(this->name());
		}
	};
	addAndMakeVisible(button_);

	setActiveState(active);
	addAndMakeVisible(label_);
}

void SynthButtonWithActiveLight::resized()
{
	auto activeArea = getLocalBounds();
	auto bottomrow = activeArea.removeFromBottom(20);
	auto toprow = activeArea.removeFromTop(LAYOUT_LARGE_LINE_HEIGHT);
	button_.setBounds(toprow);
	label_.setBounds(bottomrow.reduced(LAYOUT_INSET_NORMAL));
}

std::string SynthButtonWithActiveLight::name() const
{
	return button_.getButtonText().toStdString();
}

void SynthButtonWithActiveLight::setToggleState(bool toggleState)
{
	if (toggleState)
		button_.setToggleState(toggleState, dontSendNotification);
	else
		button_.setToggleState(toggleState, dontSendNotification);
}

void SynthButtonWithActiveLight::setActiveState(bool activeState)
{
	label_.setColour(Label::ColourIds::backgroundColourId, activeState ? Colours::darkgreen : Colours::darkgrey);
}

void SynthList::setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback)
{
	buttons_.clear();
	synths_ = synths;
	synthSwitchCallback_ = synthSwitchCallback;
	for (auto synth : synths_) {
		auto button = new SynthButtonWithActiveLight(synth->getName(), synth->getColour(), synth->isActive());
		button->onSynthSelected = [this](std::string const &name) {
			for (auto synth : synths_) {
				if (name == synth->getName()) {
					setActiveListItem(name);
					synthSwitchCallback_(synth);
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
		if (button->name() == active) {
			button->setToggleState(true);
		}
		else {
			button->setToggleState(false);
		}
	}
}

void SynthList::resized() {
	Rectangle<int> area(getLocalBounds());
	int width = buttons_.size() != 0 ? std::min(area.getWidth() / buttons_.size(), LAYOUT_BUTTON_WIDTH + LAYOUT_INSET_NORMAL) : 0;

	auto activeArea = area.removeFromRight(width * buttons_.size());
	
	// Horizontal layout
	for (int i = 0; i < buttons_.size(); i++) {
		int rightMargin = 0;
		if (i != buttons_.size() - 1) rightMargin = LAYOUT_INSET_NORMAL;
		buttons_[i]->setBounds(activeArea.removeFromLeft(width).withTrimmedRight(rightMargin));
	}
}

void SynthList::changeListenerCallback(ChangeBroadcaster*)
{
	// Update the availability of the synths (not the list itself)
	int i = 0;
	for (auto synth : synths_) {
		buttons_[i]->setActiveState(synth->isActive());
		i++;
	}
}

