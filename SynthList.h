#pragma once

#include "JuceHeader.h"

class ActiveListItem {
public:
	virtual std::string getName() = 0;
	virtual bool isActive() = 0;
};

class SynthList : public Component,
	public ChangeListener,
	private Button::Listener 
{
public:
	void setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback);

	virtual void resized() override;

	// Button listener
	virtual void buttonClicked(Button *button) override;
	virtual void buttonStateChanged(Button *button) override;

	// This is called when the synth list changes
	virtual void changeListenerCallback(ChangeBroadcaster* source) override;

private:
	std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback_;
	std::vector<std::shared_ptr<ActiveListItem>> synths_;
	OwnedArray<Button> buttons_;
	OwnedArray<Label> labels_;
};
