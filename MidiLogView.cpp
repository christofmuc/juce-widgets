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
	buttons_ = std::make_unique<LambdaButtonStrip>(101);
	addAndMakeVisible(*buttons_);
	LambdaButtonStrip::TButtonMap lambdas = {
		{ "clearLog",{ 0, "Clear log", [this]() {
			clearLog();
		} , 0x4C /* L on Windows */, ModifierKeys::ctrlModifier } },
	};
	buttons_->setButtonDefinitions(lambdas);
//	midiMessagesBox.setColour(TextEditor::backgroundColourId, Colour(0x32ffffff));
//	midiMessagesBox.setColour(TextEditor::outlineColourId, Colour(0x1c000000));
//	midiMessagesBox.setColour(TextEditor::shadowColourId, Colour(0x16000000));
}

void MidiLogView::resized() {
	Rectangle<int> area(getLocalBounds());
	buttons_->setBounds(area.removeFromBottom(40).withTrimmedTop(8).withSizeKeepingCentre(100, 30));
	midiMessagesBox.setBounds(area);
}

void MidiLogView::addMessageToList(const MidiMessage& message, const String& source, bool isOut)
{
	String description(message.getDescription());
	if (message.isSysEx()) description = "Sysex"; // The raw bytes will be printed anyway
	const double time = message.getTimeStamp(); // -startTime;
	const String bytes = String::toHexString(message.getRawData(), message.getRawDataSize());
	addMessageToList(time, description, bytes, source, isOut);
}

void MidiLogView::addMessageToList(double time, const String& description, const String &bytes, const String& source, bool isOut)
{
	const int hours = ((int)(time / 3600.0)) % 24;
	const int minutes = ((int)(time / 60.0)) % 60;
	const int seconds = ((int)time) % 60;
	const int millis = ((int)(time * 1000.0)) % 1000;

	String timecode = (boost::format("%02d:%02d:%02d.%03d") % hours % minutes % seconds %millis).str();

	const String direction = isOut ? "Out" : "In ";
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

