/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma  once

#include "JuceHeader.h"

#include "TypedNamedValue.h"
#include "MidiChannel.h"

#include <set>

class MidiChannelPropertyEditor : public TypedNamedValue {
public:
	MidiChannelPropertyEditor(std::string const &title, std::string const &sectionName);
	void setValue(MidiChannel channel);
};

class MidiDevicePropertyEditor: public TypedNamedValue, public ChangeBroadcaster, private Timer {
public:
	MidiDevicePropertyEditor(std::string const &title, std::string const &sectionName, bool inputInsteadOfOutput, bool startTimerToCheckDeviceListUpdates);

private:
	// Frequently check if new devices have become available
	void timerCallback() override;
	void refreshDropdownList(std::vector<std::string> const &deviceList);

	bool inputInsteadOfOutput_;
	std::set<std::string> devices_;
};

