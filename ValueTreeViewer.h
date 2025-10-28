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

#include <juce_data_structures/juce_data_structures.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include <functional>
#include <optional>

/**
 * ValueTreeViewer renders a juce::ValueTree inside a TreeView and keeps the UI
 * and the underlying ValueTree in sync. Property edits performed via the UI
 * are written back into the ValueTree (optionally via an UndoManager), and
 * external changes to the ValueTree are reflected live.
 */
class ValueTreeViewer : public juce::Component
{
public:
    using PropertyColourFunction = std::function<std::optional<juce::Colour>(const juce::ValueTree&, juce::Identifier, bool isValueLabel)>;

    ValueTreeViewer();
    ~ValueTreeViewer() override;

    /** Assigns the ValueTree to visualise. Pass an invalid tree to clear the view. */
    void setValueTree(juce::ValueTree newTree);

    /** Returns the currently assigned ValueTree. */
    juce::ValueTree getValueTree() const noexcept { return valueTree_; }

    /** Optional undo manager used for property changes triggered from the UI. */
    void setUndoManager(juce::UndoManager* manager) noexcept;
    juce::UndoManager* getUndoManager() const noexcept { return undoManager_; }

    /** Forces the TreeView to rebuild itself from the underlying ValueTree. */
    void refresh();
    void setPropertyColourFunction(PropertyColourFunction fn);

    void resized() override;

private:
    class NodeItem;
    friend class ValueTreePropertyItem;

    void rebuildRoot();
    juce::Colour colourForProperty(const juce::ValueTree& tree, const juce::Identifier& propertyId, juce::Colour defaultColour, bool isValueLabel) const;

    juce::TreeView treeView_;
    juce::ValueTree valueTree_;
    juce::UndoManager* undoManager_ = nullptr;
    std::unique_ptr<NodeItem> rootItem_;
    PropertyColourFunction propertyColourFunction_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValueTreeViewer)
};
