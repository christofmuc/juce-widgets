#include "CollapsibleContainer.h"

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
	collapse_.setBounds(area.removeFromLeft(16).withHeight(16));
	text_.setBounds(area.withTrimmedLeft(16).withHeight(16));
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
	collapseHeader_.addChangeListener(this);
}

CollapsibleContainer::~CollapsibleContainer()
{
	collapseHeader_.removeChangeListener(this);
}

void CollapsibleContainer::resized()
{
	auto area = getLocalBounds();
	collapseHeader_.setBounds(area.removeFromTop(16));
	collapsible_->setBounds(area.withTrimmedTop(8));
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
