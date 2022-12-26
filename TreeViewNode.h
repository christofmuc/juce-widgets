/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

#include <juce_gui_basics/juce_gui_basics.h>

#include "Logger.h"

class TreeViewNode : public juce::TreeViewItem {
public:
    typedef std::function<std::vector<juce::TreeViewItem*>()> TChildGenerator;
    typedef std::function<void(juce::String)> TClickedHandler;
    typedef std::function<bool(juce::var)> TAcceptHandler;
    typedef std::function<void(juce::var, int)> TDropHandler;
    typedef std::function<juce::var()> TDragStartHandler;

    TreeViewNode(juce::String text, juce::String id, bool editable = false);

    juce::Value textValue;

    TChildGenerator onGenerateChildren;
    TClickedHandler onSelected;
    TClickedHandler onSingleClick;
    TClickedHandler onDoubleClick;
    TAcceptHandler acceptsItem;
    TDropHandler onItemDropped;
    TDragStartHandler onItemDragged;

    juce::String id() const;
    juce::String text() const;

    void toggleOpenness();
    void regenerate();

    virtual bool mightContainSubItems() override;
    virtual void itemOpennessChanged(bool isNowOpen) override;
    virtual void paintItem(juce::Graphics& g, int width, int height) override;
    virtual void paintOpenCloseButton(juce::Graphics& g, const juce::Rectangle<float>& area, juce::Colour backgroundColour,
        bool isMouseOver) override;
    virtual bool canBeSelected() const override;
    virtual void itemSelectionChanged(bool isNowSelected) override;

    virtual juce::String getUniqueName() const override;
    virtual void itemClicked(const juce::MouseEvent&) override;
    virtual void itemDoubleClicked(const juce::MouseEvent&) override;
    virtual bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails&) override;
    virtual void itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails, int insertIndex) override;

    virtual juce::var getDragSourceDescription() override;

    virtual std::unique_ptr<juce::Component> createItemComponent() override;

    // Requred since JUCE 6.1.2, see 
    // https://github.com/juce-framework/JUCE/commit/b2f318a775ec2b9fbcaf2f7a90b75c9912dc2dce
    // https://forum.juce.com/t/breaking-change-in-treeview-custom-component/47916/6
    virtual bool customComponentUsesTreeViewMouseHandler() const override { return true; };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreeViewNode)

private:
    juce::String text_;
    juce::String id_;
    bool editable_;
};
