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

class MidiDevicePropertyEditor: public TypedNamedValue {
public:
	MidiDevicePropertyEditor(std::string const &title, std::string const &sectionName, bool inputInsteadOfOutput, bool keepOldEntries = false);

	void refreshDeviceList();

protected:
	void refreshDropdownList(std::vector<std::string> const &deviceList);

private:
	bool inputInsteadOfOutput_;
	bool keepOldEntries_;
};

