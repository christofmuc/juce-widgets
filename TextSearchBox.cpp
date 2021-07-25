/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "TextSearchBox.h"

#include "BinaryResources.h"

#include "Logger.h"
#include "IconHelper.h"
#include "LayoutConstants.h"

class FocusReportingTextEditor : public TextEditor
{
public:
	using TextEditor::TextEditor;

	virtual void focusGained(FocusChangeType cause) override {
		ignoreUnused(cause);
		if (onFocusGained) {
			onFocusGained();
		}
	}

	std::function<void()> onFocusGained;
};

TextSearchBox::TextSearchBox(std::function<void()> updateHandler) : updateHandler_(updateHandler)
{
	nameSearchText_ = std::make_unique<FocusReportingTextEditor>();
	

	// We do not want an outline by the box itself, because we will draw the outline ourselves also around the clear button
	nameSearchText_->setColour(TextEditor::backgroundColourId, Colours::transparentBlack);
	nameSearchText_->setColour(TextEditor::outlineColourId, Colours::transparentBlack);
	nameSearchText_->setColour(TextEditor::focusedOutlineColourId, Colours::transparentBlack);
	nameSearchText_->setOpaque(false);
	nameSearchText_->onTextChange = [this]() { 
		refreshClearButton();
		updateHandler_(); 
	};
	nameSearchText_->onEscapeKey = [this]() {
		nameSearchText_->setText("", true);		
	};
	nameSearchText_->onFocusLost = [this]() {
		refreshClearButton();
		repaint();
	};
	dynamic_cast<FocusReportingTextEditor*>(nameSearchText_.get())->onFocusGained = [this]() { 
		lookingGlass_.setVisible(false);  repaint();  
	};

	// Create an Icon that shows this is a search box
	IconHelper::setupIcon(this, lookingGlass_, iconmonstr_magnifier_6_64_png, iconmonstr_magnifier_6_64_png_size);
	lookingGlass_.setVisible(true);

	// Create a lookAndFeel based clear button
	clearNameSearch_.reset(getLookAndFeel().createDocumentWindowButton(DocumentWindow::closeButton));
	clearNameSearch_->onClick = [this]() {
		nameSearchText_->setText("", true);
	};

	addAndMakeVisible(nameSearchText_.get());
	addAndMakeVisible(clearNameSearch_.get());
	clearNameSearch_->setVisible(false);
}

void TextSearchBox::setFontSize(float fontSize)
{
	Font biggerFont;
	biggerFont.setHeight(fontSize);
	nameSearchText_->setFont(biggerFont);
}

void TextSearchBox::resized()
{
	auto area = getLocalBounds();
	auto left = area;
	left = left.removeFromLeft(LAYOUT_SMALL_ICON_WIDTH + LAYOUT_INSET_NORMAL);
	auto right = area.removeFromRight(LAYOUT_SMALL_ICON_WIDTH);
	clearNameSearch_->setBounds(right.reduced(2)); // We know 2 is the line thickness in the LookAndFeel_V4
	nameSearchText_->setBounds(area);
	lookingGlass_.setBounds(left.withTrimmedLeft(LAYOUT_INSET_NORMAL));
	lookingGlass_.setImagePlacement(RectanglePlacement::xLeft| RectanglePlacement::yMid| RectanglePlacement::onlyReduceInSize);
}

juce::String TextSearchBox::searchText() const
{
	return nameSearchText_->getText();
}

void TextSearchBox::setSearchText(String const& searchText)
{
	nameSearchText_->setText(searchText, false);
	refreshClearButton();
}

void TextSearchBox::refreshClearButton() {
	clearNameSearch_->setVisible(!nameSearchText_->getText().isEmpty());
}

void TextSearchBox::paint(Graphics& g) {
	// Draw an outline box around the text box and the clear button!
	if (nameSearchText_->isEnabled())
	{
		if (nameSearchText_->hasKeyboardFocus(true) && !nameSearchText_->isReadOnly())
		{
			g.setColour(getLookAndFeel().findColour(TextEditor::focusedOutlineColourId));
			g.drawRect(0, 0, getWidth(), getHeight(), 2);
			//SimpleLogger::instance()->postMessage("Painting at " + String(g.getClipBounds().getWidth()) + " but wanted to " + String(getWidth()));
		}
		else
		{
			g.setColour(getLookAndFeel().findColour(TextEditor::outlineColourId));
			g.drawRect(0, 0, getWidth(), getHeight());
		}
	}
}
