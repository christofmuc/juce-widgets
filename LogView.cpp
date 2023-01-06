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

#include "LogView.h"

#include "Settings.h"

#include "fmt/core.h"

LogView::LogView(bool showClear, bool showSave, bool showLineNumbers /* = true */) :
    Component(), document_(new CodeDocument), logBox_(*document_, nullptr)
{
    addAndMakeVisible(logBox_);
    logBox_.setReadOnly(true);
    logBox_.setLineNumbersShown(showLineNumbers);
    document_->setNewLineCharacters("\n");
    buttons_ = std::make_unique<LambdaButtonStrip>(101, LambdaButtonStrip::Direction::Horizontal);
    addAndMakeVisible(*buttons_);
    LambdaButtonStrip::TButtonMap lambdas;
    if (showClear) {
        lambdas.push_back({ "clearLog", { "Clear log", [this]() { clearLog(); }, 0x4C /* L on Windows */, ModifierKeys::ctrlModifier } });
    }
    if (showSave) {
        lambdas.push_back({ "saveLog", { "Save log...", [this]() { saveLog(); } } });
    };
    buttons_->setButtonDefinitions(lambdas);
}

void LogView::resized()
{
    Rectangle<int> area(getLocalBounds());
    buttons_->setBounds(area.removeFromTop(30).withTrimmedTop(8).removeFromRight(180).withTrimmedRight(20));
    logBox_.setBounds(getLocalBounds());
}

void LogView::addMessageToListWithoutTimestamp(String const& message)
{
    MessageManager::callAsync([this, message]() {
        document_->insertText(document_->getNumCharacters(), message);
        document_->clearUndoHistory();
        logBox_.scrollToKeepCaretOnScreen();
    });
}


void LogView::addMessageToList(String const& message)
{
    auto time = Time::getCurrentTime();
    String midiMessageString = fmt::format("{}: {}\n", time.formatted("%H:%M:%S").toStdString(), message.toStdString());
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

void LogView::jumpToStart() 
{
    logBox_.scrollToLine(0);
}

void LogViewLogger::postMessage(const String& message)
{
    logview_.addMessageToList(message);
}

void LogViewLogger::postMessageOncePerRun(const String& message)
{
    std::string id = juce::MD5(String(message).toUTF8()).toHexString().toStdString();

    if (logsPosted_.find(id) == logsPosted_.end()) {
        // First time exact this text comes up - output, and put it into the suppression log
        postMessage(message);
        logsPosted_.insert(id);
    }
}
