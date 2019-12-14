/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiLogView.h"

#include <boost/format.hpp>

MidiLogView::MidiLogView() : Component(), document_(new CodeDocument), midiMessagesBox(*document_, nullptr) {
	addAndMakeVisible(midiMessagesBox);
	midiMessagesBox.setReadOnly(true);
	document_->setNewLineCharacters("\n");
	buttons_ = std::make_unique<LambdaButtonStrip>();
	addAndMakeVisible(*buttons_);
	LambdaButtonStrip::TButtonMap lambdas = {
		{ "clearLog",{ 0, "Clear log", [this]() {
			clearLog();
		} } },
	};
	buttons_->setButtonDefinitions(lambdas);
//	midiMessagesBox.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
//	midiMessagesBox.setColour(TextEditor::outlineColourId, Colour(0x1c000000));
//	midiMessagesBox.setColour(TextEditor::shadowColourId, Colour(0x16000000));
}

void MidiLogView::resized() {
	Rectangle<int> area(getLocalBounds());
	buttons_->setBounds(area.removeFromBottom(60).reduced(8));
	midiMessagesBox.setBounds(area.reduced(8));
}

String MidiLogView::getMidiMessageDescription(const MidiMessage& m)
{
	if (m.isNoteOn())           return "Note on " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isNoteOff())          return "Note off " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
	if (m.isProgramChange())    return "Program change " + String(m.getProgramChangeNumber());
	if (m.isPitchWheel())       return "Pitch wheel " + String(m.getPitchWheelValue());
	if (m.isAftertouch())       return "After touch " + MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " + String(m.getAfterTouchValue());
	if (m.isChannelPressure())  return "Channel pressure " + String(m.getChannelPressureValue());
	if (m.isAllNotesOff())      return "All notes off";
	if (m.isAllSoundOff())      return "All sound off";
	if (m.isMetaEvent())        return "Meta event";
	if (m.isSysEx())			return "Sysex";

	if (m.isController())
	{
		String name(MidiMessage::getControllerName(m.getControllerNumber()));

		if (name.isEmpty())
			name = "[" + String(m.getControllerNumber()) + "]";

		return "Controller " + name + ": " + String(m.getControllerValue());
	}

	return String::toHexString(m.getRawData(), m.getRawDataSize());
}

void MidiLogView::addMessageToList(const MidiMessage& message, const String& source, bool isOut)
{
	const double time = message.getTimeStamp(); // -startTime;

	const int hours = ((int)(time / 3600.0)) % 24;
	const int minutes = ((int)(time / 60.0)) % 60;
	const int seconds = ((int)time) % 60;
	const int millis = ((int)(time * 1000.0)) % 1000;

	String timecode = (boost::format("%02d:%02d:%02d.%03d") % hours % minutes % seconds %millis).str();

	const String description(getMidiMessageDescription(message));
	const String direction = isOut ? "Out" : "In ";
	const String bytes = String::toHexString(message.getRawData(), message.getRawDataSize());

	String midiMessageString = (boost::format("%s: %s %s %s [%s]\n") % timecode %	direction % source % description %bytes).str();

	MessageManager::callAsync([this, midiMessageString]() {
		document_->insertText(document_->getNumCharacters(), midiMessageString);
		document_->clearUndoHistory();
		midiMessagesBox.scrollToKeepCaretOnScreen();
	});
}

void MidiLogView::clearLog()
{
	document_->replaceAllContent("");
	document_->clearUndoHistory();
	document_->setSavePoint();
}

