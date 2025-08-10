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

#include "CollapsibleContainer.h"

#include "LayoutConstants.h"

CollapseButton::CollapseButton(bool defaultOpen) : Button("open")
{
    setClickingTogglesState(true);
    setToggleState(defaultOpen, juce::dontSendNotification);
}

void CollapseButton::paintButton(juce::Graphics &g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    juce::ignoreUnused(shouldDrawButtonAsDown);
    getLookAndFeel().drawTreeviewPlusMinusBox(g, getLocalBounds().toFloat(), juce::Colours::green, getToggleState(), shouldDrawButtonAsHighlighted);
}

CollapseHeader::CollapseHeader(juce::String const &title, bool defaultOpen) : text_("header", title), collapse_(defaultOpen)
{
    addAndMakeVisible(text_);
    addAndMakeVisible(collapse_);
    collapse_.addListener(this);
}

void CollapseHeader::resized()
{
    auto area = getLocalBounds();
    collapse_.setBounds(area.removeFromLeft(LAYOUT_TINY_ICON_WIDTH).withHeight(LAYOUT_TINY_ICON_HEIGHT));
    text_.setBounds(area.withTrimmedLeft(LAYOUT_INSET_NORMAL)
                        .withHeight(LAYOUT_TINY_ICON_HEIGHT)); // TODO I think this is a bug, and the collapsible header overlaps somewhere
}

bool CollapseHeader::isOpen() const
{
    return collapse_.getToggleState();
}

void CollapseHeader::buttonClicked(juce::Button *)
{
    sendChangeMessage();
}

CollapsibleContainer::CollapsibleContainer(juce::String const &title, Component *collapsible, bool defaultOpen) :
    collapseHeader_(title, defaultOpen), collapsible_(collapsible)
{
    addAndMakeVisible(collapseHeader_);
    addAndMakeVisible(collapsible_);
    collapsible_->setVisible(defaultOpen);
    collapseHeader_.addChangeListener(this);
}

CollapsibleContainer::~CollapsibleContainer()
{
    collapseHeader_.removeChangeListener(this);
}

void CollapsibleContainer::resized()
{
    auto area = getLocalBounds();
    collapseHeader_.setBounds(area.removeFromTop(LAYOUT_TINY_ICON_HEIGHT));
    collapsible_->setBounds(area.withTrimmedTop(LAYOUT_INSET_NORMAL));
}

bool CollapsibleContainer::isOpen() const
{
    return collapseHeader_.isOpen();
}

void CollapsibleContainer::changeListenerCallback(juce::ChangeBroadcaster *)
{
    if (collapseHeader_.isOpen()) {
        collapsible_->setVisible(true);
    }
    else {
        collapsible_->setVisible(false);
    }
    // Trigger parents resized method
    auto parent = findParentComponentOfClass<Component>();
    if (parent) {
        parent->resized();
    }
}
