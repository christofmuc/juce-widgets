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
#include "EditFocusKeeper.h"

#include "TreeViewNode.h"

EditSessionKeeper::EditSessionKeeper()
{
    juce::Desktop::getInstance().addFocusChangeListener(this);
}

EditSessionKeeper::~EditSessionKeeper()
{
    juce::Desktop::getInstance().removeFocusChangeListener(this);
}

// Track when a Label's TextEditor gains focus (user started editing)
void EditSessionKeeper::globalFocusChanged(juce::Component* c)
{
    if (auto* te = dynamic_cast<juce::TextEditor*>(c))
        if (auto* lbl = te->findParentComponentOfClass<EditOnDoubleClickLabel>()) lastLabel_ = lbl;
}

void EditSessionKeeper::snapshotOnDeactivation()
{
    // Just remember *when* we deactivated. Editor is usually already closed now.
    lastDeactivatedAtMs_ = juce::Time::getMillisecondCounter();
}


void EditSessionKeeper::restoreOnActivation()
{
    auto lbl = lastLabel_; // SafePointer copy
    lastLabel_ = nullptr;  // one-shot

    if (lbl == nullptr || !lbl->isShowing()) return;

    // Only reopen if the editor hid just before we deactivated (Alt/Cmd-Tab case).
    constexpr juce::uint32 reopenWindowMs = 250; // tweak if needed
    const juce::uint32 hidAt = lbl->getLastHiddenAtMs();
    const juce::uint32 deactAt = lastDeactivatedAtMs_;

    // If we have no timestamps, or the gap is too large, assume intentional close (Esc/click).
    if (hidAt == 0 || deactAt == 0) return;

    // Unsigned wrap-safe: deactivation should come after hide in this scenario.
    const juce::uint32 gap = deactAt - hidAt;
    if (gap > reopenWindowMs) return;

    const int caret = lbl->getSavedCaret();
    const auto sel = lbl->getSavedSelection();

    juce::MessageManager::callAsync([lbl, caret, sel] {
        if (lbl == nullptr) return;

        lbl->showEditor();
        if (auto* te = lbl->getCurrentTextEditor()) {
            te->setSelectAllWhenFocused(false);
            te->grabKeyboardFocus();

            juce::MessageManager::callAsync([lbl, caret, sel] {
                if (lbl == nullptr) return;
                if (auto* te2 = lbl->getCurrentTextEditor()) {
                    const int textLen = te2->getText().length();
                    const int s = juce::jlimit(0, textLen, sel.getStart());
                    const int e = juce::jlimit(0, textLen, sel.getEnd());
                    const int c = juce::jlimit(0, textLen, caret);

                    te2->setCaretPosition(c);
                    if (e > s) te2->setHighlightedRegion({ s, e });
                }
            });
        }
    });
}
