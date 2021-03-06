/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "SynthList.h"

#include "LayoutConstants.h"

void SynthList::setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback)
{
	buttons_.clear();
	labels_.clear();
	synths_ = synths;
	synthSwitchCallback_ = synthSwitchCallback;
	for (auto synth : synths_) {
		auto button = new TextButton(synth->getName());
		button->addListener(this);
		button->setClickingTogglesState(true);
		button->setRadioGroupId(111);
		button->setToggleState(buttons_.size() == 0, dontSendNotification);
		button->setColour(TextButton::buttonOnColourId, synth->getColour());
		buttons_.add(button);
		addAndMakeVisible(button);

		auto label = new Label();
		labels_.add(label);
		label->setColour(Label::ColourIds::backgroundColourId, synth->isActive() ? Colours::darkgreen : Colours::darkgrey);
		addAndMakeVisible(label);
	}
	resized();
}

void SynthList::setActiveListItem(std::string const &active)
{
	for (auto button : buttons_) {
		if (button->getButtonText().toStdString() == active) {
			button->setToggleState(true, dontSendNotification);
		}
	}
}

void SynthList::resized() {
	Rectangle<int> area(getLocalBounds());
	int width = buttons_.size() != 0 ? std::min(area.getWidth() / buttons_.size(), LAYOUT_BUTTON_WIDTH + LAYOUT_INSET_NORMAL) : 0;

	auto activeArea = area.removeFromRight(width * buttons_.size());
	
	// Horizontal layout
	auto bottomrow = activeArea.removeFromBottom(20);
	auto toprow = activeArea.removeFromTop(LAYOUT_LARGE_LINE_HEIGHT);
	for (int i = 0; i < buttons_.size(); i++) {
		int rightMargin = 0;
		if (i != buttons_.size() - 1) rightMargin = LAYOUT_INSET_NORMAL;
		buttons_[i]->setBounds(toprow.removeFromLeft(width).withTrimmedRight(rightMargin));
		labels_[i]->setBounds(bottomrow.removeFromLeft(width).withTrimmedRight(rightMargin).reduced(LAYOUT_INSET_NORMAL));
	}
}

void SynthList::buttonClicked(Button *button) {
	if (button->getToggleState()) {
		for (auto synth : synths_) {
			if (button->getButtonText() == juce::String(synth->getName())) {
				synthSwitchCallback_(synth);
				return;
			}
		}
		jassert(false);
	}
}

void SynthList::buttonStateChanged(Button *button) {
	// Nothing to do
	ignoreUnused(button);
}

void SynthList::changeListenerCallback(ChangeBroadcaster*)
{
	// Update the availability of the synths (not the list itself)
	int i = 0;
	for (auto synth : synths_) {
		labels_[i]->setColour(Label::ColourIds::backgroundColourId, synth->isActive() ? Colours::darkgreen : Colours::darkgrey);
		i++;
	}
}

