#include "SynthList.h"

void SynthList::setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback)
{
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
}

void SynthList::resized() {
	Rectangle<int> area(getLocalBounds());
	// Horizontal layout
	auto bottomrow = area.removeFromBottom(20);
	auto toprow = area;
	int width = std::min(area.getWidth() / buttons_.size(), 150);
	for (int i = 0; i < buttons_.size(); i++) {
		buttons_[i]->setBounds(toprow.removeFromRight(width));
		labels_[i]->setBounds(bottomrow.removeFromRight(width).reduced(8));
	}
}

void SynthList::buttonClicked(Button *button) {
	// Nothing to do
	ignoreUnused(button);
}

void SynthList::buttonStateChanged(Button *button) {
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

void SynthList::changeListenerCallback(ChangeBroadcaster*)
{
	// Update the availability of the synths (not the list itself)
	int i = 0;
	for (auto synth : synths_) {
		labels_[i]->setColour(Label::ColourIds::backgroundColourId, synth->isActive() ? Colours::darkgreen : Colours::darkgrey);
		i++;
	}
}

