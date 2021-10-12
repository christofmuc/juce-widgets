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
