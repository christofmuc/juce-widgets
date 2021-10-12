/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiLogView.h"

#include <fmt/core.h>

MidiLogView::MidiLogView(bool showClear, bool showSave) : LogView(showClear, showSave) {
}

void MidiLogView::addMessageToList(const MidiMessage& message, const String& source, bool isOut)
{
	String description(message.getDescription());
	if (message.isSysEx()) description = "Sysex"; // The raw bytes will be printed anyway
	const double time = Time::getMillisecondCounterHiRes() / 1000.0; // message.getTimeStamp(); // -startTime;
	String bytes = String::toHexString(message.getRawData(), std::min(message.getRawDataSize(), 1000));
	if (message.getRawDataSize() > 1000) {
		bytes += "... (" + String(message.getRawDataSize()) + " more bytes)";
	}
	addMessageToList(time, description, bytes, source, isOut);
}

void MidiLogView::addMessageToList(double time, const String& description, const String &bytes, const String& source, bool isOut)
{
	const int hours = ((int)(time / 3600.0)) % 24;
	const int minutes = ((int)(time / 60.0)) % 60;
	const int seconds = ((int)time) % 60;
	const int millis = ((int)(time * 1000.0)) % 1000;

	std::string timecode = fmt::format("{02d}:{02d}:{02d}.{03d}", hours, minutes, seconds, millis);

	const std::string direction = isOut ? "Out" : "In ";
	String midiMessageString = fmt::format("{s}: {s} {s} {s} [{s}]\n", timecode, direction, source.toStdString(), description.toStdString(), bytes.toStdString());
	addMessageToListWithoutTimestamp(midiMessageString);
}


