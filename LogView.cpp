/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "LogView.h"

#include "Settings.h"

#include <boost/format.hpp>

LogView::LogView() : Component(), document_(new CodeDocument), logBox_(*document_, nullptr) {
	addAndMakeVisible(logBox_);
	logBox_.setReadOnly(true);
	document_->setNewLineCharacters("\n");
	buttons_ = std::make_unique<LambdaButtonStrip>(101, LambdaButtonStrip::Direction::Horizontal);
	addAndMakeVisible(*buttons_);
	LambdaButtonStrip::TButtonMap lambdas = {
		{ "clearLog",{ 0, "Clear log", [this]() {
			clearLog();
		} , 0x4C /* L on Windows */, ModifierKeys::ctrlModifier } },
		{ "saveLog", { 1, "Save log...", [this]() {
			saveLog();
		}}
		}
	};
	buttons_->setButtonDefinitions(lambdas);
}

void LogView::resized() {
	Rectangle<int> area(getLocalBounds());
	buttons_->setBounds(area.removeFromBottom(40).withTrimmedTop(8).withSizeKeepingCentre(180, 30));
	logBox_.setBounds(area);
}

void LogView::addMessageToListWithoutTimestamp(String const &message)
{
	MessageManager::callAsync([this, message]() {
		document_->insertText(document_->getNumCharacters(), message);
		document_->clearUndoHistory();
		logBox_.scrollToKeepCaretOnScreen();
	});
}


void LogView::addMessageToList(String const &message)
{
	auto time = Time::getCurrentTime();
	String midiMessageString = (boost::format("%s: %s\n") % time.formatted("%H:%M:%S")%	message).str();
	addMessageToListWithoutTimestamp(midiMessageString);
}

void LogView::clearLog()
{
	document_->replaceAllContent("");
	document_->clearUndoHistory();
	document_->setSavePoint();
}

void LogView::saveLog()
{
	auto lastpath = Settings::instance().get("lastLogfilePath", File::getSpecialLocation(File::userHomeDirectory).getFullPathName().toStdString());
	FileChooser logChooser("Please enter the name under which to save the log file...", File(lastpath), ".log");
	if (logChooser.browseForFileToSave(true)) {
		// Persist path
		Settings::instance().set("lastLogfilePath", logChooser.getResult().getParentDirectory().getFullPathName().toStdString());

		auto logFile = logChooser.getResult();
		FileOutputStream output(logFile);
		if (output.openedOk()) {
			output.writeText(document_->getAllContent(), false, false, "\\n");
			output.flush(); // (called explicitly to force an fsync on posix)
			if (output.getStatus().failed()) {
				SimpleLogger::instance()->postMessage("ERROR - writing to destination file " + logFile.getFullPathName() + ". Disk full?");
			}
		}
		else {
			SimpleLogger::instance()->postMessage("ERROR - couldn't open file for writing: " + logFile.getFullPathName());
		}
	}
}

void LogViewLogger::postMessage(const String& message)
{
	logview_.addMessageToList(message);
}
