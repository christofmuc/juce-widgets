#include "SynthList.h"

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
		buttons_.add(button);
		addAndMakeVisible(button);

		auto label = new Label();
		labels_.add(label);
		label->setColour(Label::ColourIds::backgroundColourId, synth->isActive() ? Colours::darkgreen : Colours::darkgrey);
		addAndMakeVisible(label);
	}
	resized();
}

void SynthList::resized() {
	Rectangle<int> area(getLocalBounds());
	int width = buttons_.size() != 0 ? std::min(area.getWidth() / buttons_.size(), 150) : 0;

	auto activeArea = area.removeFromRight(width * buttons_.size());
	
	// Horizontal layout
	auto bottomrow = activeArea.removeFromBottom(20);
	auto toprow = activeArea;
	for (int i = 0; i < buttons_.size(); i++) {
		buttons_[i]->setBounds(toprow.removeFromLeft(width).withTrimmedRight(8));
		labels_[i]->setBounds(bottomrow.removeFromLeft(width).withTrimmedRight(8).reduced(8));
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

