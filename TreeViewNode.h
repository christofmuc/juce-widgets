/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
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
