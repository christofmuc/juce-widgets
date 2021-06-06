/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "CollapsibleContainer.h"

#include "LayoutConstants.h"

CollapseButton::CollapseButton(bool defaultOpen) : Button("open")
{
	setClickingTogglesState(true);
	setToggleState(defaultOpen, dontSendNotification);
}

void CollapseButton::paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	ignoreUnused(shouldDrawButtonAsDown);
	getLookAndFeel().drawTreeviewPlusMinusBox(g, getLocalBounds().toFloat(), Colours::green, getToggleState(), shouldDrawButtonAsHighlighted);
}

CollapseHeader::CollapseHeader(String const &title, bool defaultOpen) : text_("header", title), collapse_(defaultOpen)
{
	addAndMakeVisible(text_);
	addAndMakeVisible(collapse_);
	collapse_.addListener(this);
}

void CollapseHeader::resized()
{
	auto area = getLocalBounds();
	collapse_.setBounds(area.removeFromLeft(LAYOUT_TINY_ICON_WIDTH).withHeight(LAYOUT_TINY_ICON_HEIGHT));
	text_.setBounds(area.withTrimmedLeft(LAYOUT_INSET_NORMAL).withHeight(LAYOUT_TINY_ICON_HEIGHT)); //TODO I think this is a bug, and the collapsible header overlaps somewhere
}

bool CollapseHeader::isOpen() const
{
	return collapse_.getToggleState();
}

void CollapseHeader::buttonClicked(Button*)
{
	sendChangeMessage();
}

CollapsibleContainer::CollapsibleContainer(String const &title, Component *collapsible, bool defaultOpen) 
	: collapseHeader_(title, defaultOpen), collapsible_(collapsible)
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

void CollapsibleContainer::changeListenerCallback(ChangeBroadcaster* )
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
