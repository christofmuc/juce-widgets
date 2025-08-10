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
};
