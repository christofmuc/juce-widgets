/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "PatchButton.h"

PatchButton::PatchButton(int id, std::function<void(int)> clickHandler) : clicked_(clickHandler), id_(id), active_(false)
{
	addAndMakeVisible(button_);
	button_.addListener(this);
	button_.setClickingTogglesState(true);
	PNGImageFormat reader;
//	auto im = reader.decodeImage(MemoryInputStream(BinaryData::hearts32_png, BinaryData::hearts32_pngSize, false));
//	auto smaller = im.rescaled(16, 16);
//	favoriteIcon_.setImage(smaller);
	//favoriteIcon_.setInterceptsMouseClicks(false, false);
	//addAndMakeVisible(favoriteIcon_);
	//favoriteIcon_.setVisible(false);
	//favoriteIcon_.toFront(false);
}

void PatchButton::setActive(bool active)
{
	active_ = active;
	repaint();
}

void PatchButton::resized()
{
	Rectangle<int> area(getLocalBounds());
	auto upperRight = area;
	button_.setBounds(area.reduced(2));
	favoriteIcon_.setBounds(area);
	favoriteIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yTop | RectanglePlacement::onlyReduceInSize);
}

/*void PatchButton::paint(Graphics& g) 
{
	if (active_) {
		g.fillAll(Colours::darkgrey);
		g.setColour(Colours::lightgrey);
		g.fillRect(getLocalBounds());
	}
	else {
		Component::paint(g);
	}
}*/

void PatchButton::setColour(int colourId, Colour newColour)
{
	button_.setColour(colourId, newColour);
}

String PatchButton::getButtonText() const
{
	return button_.getButtonText();
}

void PatchButton::setButtonText(const String& text)
{
	button_.setButtonText(text);
}

void PatchButton::setFavorite(bool isFavorite)
{
	favoriteIcon_.setVisible(isFavorite);
}

void PatchButton::setToggleState(bool state)
{
	button_.setToggleState(state, dontSendNotification);
}

void PatchButton::buttonClicked(Button*)
{
	clicked_(id_);
}

bool PatchButton::getToggleState() const
{
	return button_.getToggleState();
}
