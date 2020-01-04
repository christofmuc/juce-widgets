/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "LogView.h"

#include <boost/format.hpp>

LogView::LogView() : Component(), document_(new CodeDocument), logBox_(*document_, nullptr) {
	addAndMakeVisible(logBox_);
	logBox_.setReadOnly(true);
	document_->setNewLineCharacters("\n");
	buttons_ = std::make_unique<LambdaButtonStrip>(101);
	addAndMakeVisible(*buttons_);
	LambdaButtonStrip::TButtonMap lambdas = {
		{ "clearLog",{ 0, "Clear log", [this]() {
			clearLog();
		} , 0x4C /* L on Windows */, ModifierKeys::ctrlModifier } },
	};
	buttons_->setButtonDefinitions(lambdas);
}

void LogView::resized() {
	Rectangle<int> area(getLocalBounds());
	buttons_->setBounds(area.removeFromBottom(60).reduced(8));
	logBox_.setBounds(area.reduced(8));
}

void LogView::addMessageToList(String const &message)
{
	auto time = Time::getCurrentTime();

	String midiMessageString = (boost::format("%s: %s\n") % time.formatted("%H:%M:%S")%	message).str();

	MessageManager::callAsync([this, midiMessageString]() {
		document_->insertText(document_->getNumCharacters(), midiMessageString);
		document_->clearUndoHistory();
		logBox_.scrollToKeepCaretOnScreen();
	});
}

void LogView::clearLog()
{
	document_->replaceAllContent("");
	document_->clearUndoHistory();
	document_->setSavePoint();
}

