/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LambdaButtonStrip.h"

class MidiLogView : public Component {
public:
	MidiLogView();

	void addMessageToList(const MidiMessage& message, const String& source, bool isOut);
	void addMessageToList(double time, const String& description, const String &bytes, const String& source, bool isOut);

	void clearLog();

	virtual void resized() override;

	static String MidiLogView::getMidiMessageDescription(const MidiMessage& m);

private:
	std::unique_ptr<CodeDocument> document_;
	std::unique_ptr<LambdaButtonStrip> buttons_;
	CodeEditorComponent midiMessagesBox;
};
