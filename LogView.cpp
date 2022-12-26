/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

#include "LogView.h"

#include "Settings.h"

#include "SpdLogJuce.h"
#include <fmt/core.h>
#include <juce_cryptography/juce_cryptography.h>
#include <spdlog/spdlog.h>


class LogTokenizer : public juce::CodeTokeniser {
public:
    LogTokenizer(juce::CodeDocument& doc) : document_(doc) {}

    virtual int readNextToken(juce::CodeDocument::Iterator& source) override
    {
        // Just read a full line and return the memorized log level for this line
        int lineNo = source.getLine();
        source.skipToEndOfLine();
        return lineNo < levels_.size() ? levels_[lineNo] : spdlog::level::level_enum::off;
    }

    virtual juce::CodeEditorComponent::ColourScheme getDefaultColourScheme() override
    {
        juce::CodeEditorComponent::ColourScheme result;
        result.set(SPDLOG_LEVEL_NAME_TRACE.data(), juce::Colours::darkgrey);
        result.set(SPDLOG_LEVEL_NAME_DEBUG.data(), juce::Colours::lightgrey);
        result.set(SPDLOG_LEVEL_NAME_INFO.data(), juce::Colours::beige);
        result.set(SPDLOG_LEVEL_NAME_WARNING.data(), juce::Colours::lightblue);
        result.set(SPDLOG_LEVEL_NAME_ERROR.data(), juce::Colours::indianred);
        result.set(SPDLOG_LEVEL_NAME_CRITICAL.data(), juce::Colours::orangered);
        result.set(SPDLOG_LEVEL_NAME_OFF.data(), juce::Colours::black);
        return result;
    }

    void addLine(int loglevel) { levels_.push_back(loglevel); }

    void clear() { levels_.clear(); }

private:
    std::vector<int> levels_;
    juce::CodeDocument& document_;
};


LogView::LogView(bool showClear, bool showSave, bool showLineNumbers /* = true */, bool showLevelSelector /* = false */) :
    juce::Component(), document_(new juce::CodeDocument)
{
    tokenizer_ = new LogTokenizer(*document_);
    logBox_ = std::make_unique<juce::CodeEditorComponent>(*document_, tokenizer_);

    addAndMakeVisible(*logBox_);
    logBox_->setReadOnly(true);
    logBox_->setLineNumbersShown(showLineNumbers);
    document_->setNewLineCharacters("\n");
    buttons_ = std::make_unique<LambdaButtonStrip>(101, LambdaButtonStrip::Direction::Horizontal);
    addAndMakeVisible(*buttons_);
    LambdaButtonStrip::TButtonMap lambdas;
    if (showClear) {
        lambdas.push_back({ "clearLog", { "Clear log", [this]() { clearLog(); }, 0x4C /* L on Windows */, juce::ModifierKeys::ctrlModifier } });
    }
    if (showSave) {
        lambdas.push_back({ "saveLog", { "Save log...", [this]() { saveLog(); } } });
    };
    buttons_->setButtonDefinitions(lambdas);
    if (showLevelSelector) {
        juce::StringArray levelItems;
        for (auto level : tokenizer_->getDefaultColourScheme().types) {
            levelItems.add(level.name);
        }
        levelSelector_.addItemList(levelItems, 1);
        levelSelector_.getSelectedIdAsValue().referTo(level_);
        addAndMakeVisible(levelSelector_);
        levelSelector_.onChange = [this]() { refresh(); };
    }
}

LogView::~LogView()
{
    logBox_.reset();
    delete tokenizer_;
}


void LogView::logMessage(spdlog::level::level_enum level, juce::String const& message)
{
    {
        juce::ScopedLock lock(fullLogLock_);
        fullLog_.emplace_back(level, message.toStdString());
    }
    if (level >= ((int) level_.getValue()) - 1) {
        juce::MessageManager::callAsync([this, level, message]() {
            // TODO We need to make sure this is only a single line!
            document_->insertText(document_->getNumCharacters(), message);
            document_->clearUndoHistory();
            logBox_->scrollToKeepCaretOnScreen();
            tokenizer_->addLine(level);
        });
    }
}

void LogView::resized()
{
    juce::Rectangle<int> area(getLocalBounds());
    buttons_->setBounds(area.removeFromTop(30).withTrimmedTop(8).removeFromRight(180).withTrimmedRight(20));
    levelSelector_.setBounds(area.removeFromTop(30).withTrimmedTop(8).removeFromRight(180).withTrimmedRight(20));
    logBox_->setBounds(getLocalBounds());
}

void LogView::addMessageToListWithoutTimestamp(juce::String const& message)
{
    logMessage(spdlog::level::info, message);
}


void LogView::addMessageToList(juce::String const& message)
{
    auto time = juce::Time::getCurrentTime();
    juce::String midiMessageString = fmt::format("{}: {}\n", time.formatted("%H:%M:%S").toStdString(), message.toStdString());
    addMessageToListWithoutTimestamp(midiMessageString);
}

void LogView::clearLog()
{
    clearView();
    {
        juce::ScopedLock lock(fullLogLock_);
        fullLog_.clear();
    }
}

void LogView::clearView()
{
    document_->replaceAllContent("");
    document_->clearUndoHistory();
    document_->setSavePoint();
    tokenizer_->clear();
}

void LogView::refresh()
{
    juce::MessageManager::callAsync([this]() {
        clearView();
        juce::ScopedLock lock(fullLogLock_);
        for (auto const& line : fullLog_) {
            if (line.first >= ((int) level_.getValue()) - 1) {
                // TODO We need to make sure this is only a single line!
                document_->insertText(document_->getNumCharacters(), line.second);
                tokenizer_->addLine(line.first);
            }
        }
        document_->clearUndoHistory();
        logBox_->scrollToKeepCaretOnScreen();
    });
}

void LogView::saveLog()
{
    auto lastpath =
        Settings::instance().get("lastLogfilePath", juce::File::getSpecialLocation(juce::File::userHomeDirectory).getFullPathName().toStdString());
    juce::FileChooser logChooser("Please enter the name under which to save the log file...", juce::File(lastpath), ".log");
    if (logChooser.browseForFileToSave(true)) {
        // Persist path
        Settings::instance().set("lastLogfilePath", logChooser.getResult().getParentDirectory().getFullPathName().toStdString());

        auto logFile = logChooser.getResult();
        juce::FileOutputStream output(logFile);
        if (output.openedOk()) {
            output.writeText(document_->getAllContent(), false, false, "\\n");
            output.flush(); // (called explicitly to force an fsync on posix)
            if (output.getStatus().failed()) {
                spdlog::error("Writing to destination file {}. Disk full?", logFile.getFullPathName());
            }
        }
        else {
            spdlog::error("Couldn't open file for writing: {}", logFile.getFullPathName());
        }
    }
}

void LogView::setLogLevel(spdlog::level::level_enum level)
{
    level_ = level;
}

void LogViewLogger::postMessage(const juce::String& message)
{
    logview_.addMessageToList(message);
}

void LogViewLogger::postMessageOncePerRun(const juce::String& message)
{
    std::string id = juce::MD5(juce::String(message).toUTF8()).toHexString().toStdString();

    if (logsPosted_.find(id) == logsPosted_.end()) {
        // First time exact this text comes up - output, and put it into the suppression log
        postMessage(message);
        logsPosted_.insert(id);
    }
}
