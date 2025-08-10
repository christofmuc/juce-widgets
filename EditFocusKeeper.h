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
#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

class EditOnDoubleClickLabel;

struct EditSessionKeeper : private juce::FocusChangeListener {
    EditSessionKeeper();
    ~EditSessionKeeper() override;

    void globalFocusChanged(juce::Component* c) override; // remember which label was being edited
    void snapshotOnDeactivation();                        // optional safety: remember the label if focus is null
    void restoreOnActivation();                           // reopen editor, restore caret/selection

private:
    juce::Component::SafePointer<EditOnDoubleClickLabel> lastLabel_;
    bool reopenOnActivate_ = false;
    juce::uint32 lastDeactivatedAtMs_ = 0;
};
