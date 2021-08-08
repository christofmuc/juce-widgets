/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "TreeViewNode.h"



TreeViewNode::TreeViewNode(String text, String id) : text_(text), id_(id)
{

}

void TreeViewNode::toggleOpenness()
{
	auto open = getOpenness();
	switch (open) {
	case Openness::opennessClosed:
		setOpenness(TreeViewItem::opennessOpen);
		break;
	case Openness::opennessOpen:
		setOpenness(TreeViewItem::opennessClosed);
		break;
	case Openness::opennessDefault:
		// This only works as expected when the TreeView has default openness = closed
		setOpenness(TreeViewItem::opennessOpen);
		break;
	}
}

bool TreeViewNode::mightContainSubItems()
{
	return onGenerateChildren != nullptr;
}

void TreeViewNode::itemOpennessChanged(bool isNowOpen)
{
	if (onGenerateChildren != nullptr && isNowOpen) {
		regenerate();
	}
}

void TreeViewNode::paintItem(Graphics& g, int width, int height)
{
	auto& lf = LookAndFeel::getDefaultLookAndFeel();
	g.setColour(lf.findColour(Label::textColourId));
	g.drawText(text_, 0, 0, width, height, Justification::centredLeft);
}

void TreeViewNode::paintOpenCloseButton(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isMouseOver)
{
	ignoreUnused(backgroundColour);
	TreeViewItem::paintOpenCloseButton(g, area, LookAndFeel::getDefaultLookAndFeel().findColour(TreeView::backgroundColourId), isMouseOver);
}

bool TreeViewNode::canBeSelected() const
{
	return onSelected != nullptr;
}

void TreeViewNode::itemSelectionChanged(bool isNowSelected)
{
	if (isNowSelected) {
		onSelected(id_);
	}
}

void TreeViewNode::regenerate()
{
	if (onGenerateChildren) {
		clearSubItems();
		auto children = onGenerateChildren();
		for (auto c : children) {
			addSubItem(c);
		}
		treeHasChanged();
	}
}

juce::String TreeViewNode::getUniqueName() const
{
	if (id_.isNotEmpty()) {
		return id_;
	}
	else {
		return text_;
	}
}

void TreeViewNode::itemClicked(const MouseEvent&)
{
	if (onSingleClick) {
		onSingleClick(id_);
	}
}

void TreeViewNode::itemDoubleClicked(const MouseEvent&)
{
	if (onDoubleClick) {
		onDoubleClick(id_);
	}
}

bool TreeViewNode::isInterestedInDragSource(const DragAndDropTarget::SourceDetails&)
{
	return onItemDropped != nullptr;
}

void TreeViewNode::itemDropped(const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex)
{
	ignoreUnused(insertIndex);
	String name = dragSourceDetails.description;
	//SimpleLogger::instance()->postMessage("Item dropped: " + name + " at " + String(insertIndex));
	onItemDropped(dragSourceDetails.description);
}

juce::var TreeViewNode::getDragSourceDescription()
{
	if (onItemDragged) {
		return onItemDragged();
	}
	return {};
}

juce::String TreeViewNode::id() const
{
	return id_;
}

juce::String TreeViewNode::text() const
{
	return text_;
}
