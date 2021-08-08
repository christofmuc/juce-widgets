/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "JuceHeader.h"

#include "Logger.h"

class TreeViewNode : public TreeViewItem {
public:
	typedef std::function<std::vector<TreeViewItem*>()> TChildGenerator;
	typedef std::function<void(String)>  TClickedHandler;
	typedef std::function<bool(juce::var)>  TAcceptHandler;
	typedef std::function<void(juce::var)>  TDropHandler;
	typedef std::function<var()> TDragStartHandler;

	TreeViewNode(String text, String id);

	TChildGenerator onGenerateChildren;
	TClickedHandler onSelected;
	TClickedHandler onSingleClick;
	TClickedHandler onDoubleClick;
	TAcceptHandler acceptsItem;
	TDropHandler    onItemDropped;
	TDragStartHandler onItemDragged;

	String id() const;
	String text() const;

	void toggleOpenness();
	void regenerate();

	virtual bool mightContainSubItems() override;
	virtual void itemOpennessChanged(bool isNowOpen) override;
	virtual void paintItem(Graphics& g, int width, int height) override;
	virtual void paintOpenCloseButton(Graphics& g, const Rectangle<float>& area, Colour backgroundColour, bool isMouseOver) override;
	virtual bool canBeSelected() const override;
	virtual void itemSelectionChanged(bool isNowSelected) override;

	virtual String getUniqueName() const override;
	virtual void itemClicked(const MouseEvent&) override;
	virtual void itemDoubleClicked(const MouseEvent&) override;
	virtual bool isInterestedInDragSource(const DragAndDropTarget::SourceDetails&) override;
	virtual void itemDropped(const DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex) override;

	virtual var getDragSourceDescription() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreeViewNode)

private:
	String text_;
	String id_;
};


