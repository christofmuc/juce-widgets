/*
 * MIT License
 *
 * Copyright (c) 2019-2025 Christof Ruch
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

#include <juce_gui_extra/juce_gui_extra.h>

#include "LambdaButtonStrip.h"
#include "Logger.h"

#include <set>
#include <spdlog/common.h>

class LogTokenizer;

class LogView : public juce::Component {
public:
    explicit LogView(bool showClear = true, bool showSave = true, bool showLineNumbers = true, bool showLevelSelector = false);
    virtual ~LogView() override;

    void logMessage(spdlog::level::level_enum level, juce::String const &message);
    void addMessageToList(juce::String const &message);
    void addMessageToListWithoutTimestamp(juce::String const &message);
    void clearLog();
    void saveLog();

    void setLogLevel(spdlog::level::level_enum level);

    virtual void resized() override;

private:
    void clearView();
    void refresh();

    std::unique_ptr<juce::CodeDocument> document_;
    std::vector<std::pair<spdlog::level::level_enum, std::string>> fullLog_;
    juce::CriticalSection fullLogLock_;
    LogTokenizer *tokenizer_ {};
    std::unique_ptr<LambdaButtonStrip> buttons_;
    std::unique_ptr<juce::CodeEditorComponent> logBox_;
    juce::ComboBox levelSelector_;
    juce::Value level_ { spdlog::level::level_enum::info + 1 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogView)
};

class LogViewLogger : public SimpleLogger {
public:
    LogViewLogger(LogView &logview) : logview_(logview) {}

    virtual void postMessage(const juce::String &message) override;
    virtual void postMessageOncePerRun(const juce::String &message) override;

private:
    LogView &logview_;
    std::set<std::string> logsPosted_;
};
