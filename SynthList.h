/*
   Copyright (c) 2020-2021 Christof Ruch. All rights reserved.

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

class SynthButtonWithActiveLight : public Component {
public:
	SynthButtonWithActiveLight(std::string const& name, Colour color, bool active);

	virtual void resized() override;

	std::string name() const;

	void setToggleState(bool toggleState);
	void setActiveState(bool activeState);

	std::function<void(std::string const &)> onSynthSelected;

private:
	TextButton button_;
	Label label_;
};

class SynthList : public Component, public ChangeListener
{
public:
	void setList(std::vector<std::shared_ptr<ActiveListItem>> &synths, std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback);
	void setActiveListItem(std::string const &active);

	virtual void resized() override;

	// This is called when the synth list changes
	virtual void changeListenerCallback(ChangeBroadcaster* source) override;

private:
	std::function<void(std::shared_ptr<ActiveListItem>)> synthSwitchCallback_;
	std::vector<std::shared_ptr<ActiveListItem>> synths_;
	OwnedArray<SynthButtonWithActiveLight> buttons_;
};
