/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
