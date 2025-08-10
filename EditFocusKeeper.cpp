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

// Optional: if focus jumps straight to null on deactivate, try to remember the label
void EditSessionKeeper::snapshotOnDeactivation()
{
    if (auto* te = dynamic_cast<juce::TextEditor*>(juce::Component::getCurrentlyFocusedComponent()))
        if (auto* lbl = te->findParentComponentOfClass<EditOnDoubleClickLabel>()) lastLabel_ = lbl;
}

void EditSessionKeeper::restoreOnActivation()
{
    auto lbl = lastLabel_; // SafePointer copy
    lastLabel_ = nullptr;  // one-shot

    if (lbl == nullptr || !lbl->isShowing()) return;

    // Pull the exact snapshot taken by the label at editor close
    int caret = lbl->getSavedCaret();
    auto sel = lbl->getSavedSelection();

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
