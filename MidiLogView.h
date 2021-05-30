/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LogView.h"

class MidiLogView : public LogView {
public:
	MidiLogView(bool showClear = true, bool showSave = true);

	void addMessageToList(const MidiMessage& message, const String& source, bool isOut);
	void addMessageToList(double time, const String& description, const String &bytes, const String& source, bool isOut);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiLogView)
};
