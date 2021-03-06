/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class ActiveListItem {
public:
	virtual std::string getName() = 0;
	virtual bool isActive() = 0;
	virtual Colour getColour() = 0;
};

class SynthList : public Component,
	public ChangeListener,
	private Button::Listener 
{
public:
	void setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback);
	void setActiveListItem(std::string const &active);

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
