/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiChannelEntry.h"

class MidiChannelEntryWithSnap : public Slider {
public:
	MidiChannelEntryWithSnap() {
		setSliderStyle(Slider::IncDecButtons);
		setRange(1.0, 16.0, 1.0);
		setNumDecimalPlacesToDisplay(0);
	}

	virtual double snapValue(double attemptedValue, DragMode dragMode) override {
		ignoreUnused(dragMode);
		return roundToIntAccurate(attemptedValue);
	}
};

MidiChannelEntry::MidiChannelEntry(TSetMidiChannelCallback callback) : callback_(callback)
{
	slider_ = std::make_unique<MidiChannelEntryWithSnap>();
	addAndMakeVisible(slider_.get());
	slider_->addListener(this);
}

void MidiChannelEntry::setValue(MidiChannel newChannel)
{
	slider_->setValue(newChannel.toOneBasedInt(), dontSendNotification);
}

void MidiChannelEntry::sliderValueChanged(Slider* slider)
{
	if (slider == slider_.get()) {
		callback_(MidiChannel::fromOneBase(roundToIntAccurate(slider_->getValue())));
	}
}

void MidiChannelEntry::resized()
{
	Rectangle<int> area(getLocalBounds());
	int sliderHeight = 30;
	slider_->setBounds(0, (area.getHeight() - sliderHeight) / 2, area.getWidth(), sliderHeight);
}


