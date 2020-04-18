/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "MidiChannel.h"

class MidiChannelEntry : public Component,
	private Slider::Listener 
{
public:
	typedef std::function<void(MidiChannel newChannel)> TSetMidiChannelCallback;

	MidiChannelEntry(TSetMidiChannelCallback callback);

	// Call this to set the MidiChannel
	void setValue(MidiChannel newChannel);

	// Implementation of behavior
	virtual void sliderValueChanged(Slider* slider) override;
	virtual void resized() override;

private:
	TSetMidiChannelCallback callback_;
	std::unique_ptr<Slider> slider_;
};
