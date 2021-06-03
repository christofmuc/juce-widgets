/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "TextSearchBox.h"

#include "Logger.h"

TextSearchBox::TextSearchBox(std::function<void()> updateHandler) : updateHandler_(updateHandler)
{
	// We do not want an outline by the box itself, because we will draw the outline ourselves also around the clear button
	nameSearchText_.setOpaque(false);
	nameSearchText_.setColour(TextEditor::backgroundColourId, Colours::transparentBlack);
	nameSearchText_.setColour(TextEditor::outlineColourId, Colours::transparentBlack);
	nameSearchText_.setColour(TextEditor::focusedOutlineColourId, Colours::transparentBlack);
	nameSearchText_.onTextChange = [this]() { 
		clearNameSearch_->setVisible(!nameSearchText_.getText().isEmpty());
		updateHandler_(); 
	};
	nameSearchText_.onEscapeKey = [this]() {
		nameSearchText_.setText("", true);
		
	};

	// Create a lookAndFeel based clear button
	clearNameSearch_ = getLookAndFeel().createDocumentWindowButton(DocumentWindow::closeButton);
	clearNameSearch_->onClick = [this]() {
		nameSearchText_.setText("", true);
	};

	addAndMakeVisible(nameSearchText_);
	addAndMakeVisible(clearNameSearch_);
	clearNameSearch_->setVisible(false);
}

void TextSearchBox::resized()
{
	auto area = getLocalBounds();
	auto right = area.removeFromRight(32);
	clearNameSearch_->setBounds(right.reduced(2)); // We know 2 is the line thickness in the LookAndFeel_V4
	nameSearchText_.setBounds(area);
}

juce::String TextSearchBox::searchText() const
{
	return nameSearchText_.getText();
}

void TextSearchBox::paint(Graphics& g) {
	/*g.setColour(Colours::red);
	g.fillRect(getLocalBounds());*/

	// Draw an outline box around the text box and the clear button!
	if (nameSearchText_.isEnabled())
	{
		if (nameSearchText_.hasKeyboardFocus(true) && !nameSearchText_.isReadOnly())
		{
			g.setColour(getLookAndFeel().findColour(TextEditor::focusedOutlineColourId));
			g.drawRect(0, 0, getWidth(), getHeight(), 2);
			SimpleLogger::instance()->postMessage("Painting at " + String(g.getClipBounds().getWidth()) + " but wanted to " + String(getWidth()));
		}
		else
		{
			g.setColour(getLookAndFeel().findColour(TextEditor::outlineColourId));
			g.drawRect(0, 0, getWidth(), getHeight());
		}
	}
}
