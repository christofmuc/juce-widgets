/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "Logger.h"
#include "LambdaButtonStrip.h"

#include <set>

class LogView : public Component {
public:
	LogView(bool showClear = true, bool showSave = true, bool showLineNumbers = true);

	void addMessageToList(String const &message);
	void addMessageToListWithoutTimestamp(String const &message);
	void clearLog();
	void saveLog();

	virtual void resized() override;

private:
	std::unique_ptr<CodeDocument> document_;
	std::unique_ptr<LambdaButtonStrip> buttons_;
	CodeEditorComponent logBox_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogView)
};

class LogViewLogger : public SimpleLogger {
public:
	LogViewLogger(LogView &logview) : logview_(logview) {}

	virtual void postMessage(const String& message) override;
	virtual void postMessageOncePerRun(const String& message) override;

private:
	LogView &logview_;
	std::set<std::string> logsPosted_;
};

