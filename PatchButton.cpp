/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "PatchButton.h"

#include "BinaryResources.h"

PatchButton::PatchButton(int id, std::function<void(int)> clickHandler) : clicked_(clickHandler), id_(id), active_(false)
{
	
	addAndMakeVisible(button_);
	button_.addListener(this);
	button_.setClickingTogglesState(true);
	setupIcon(favoriteIcon_, heart_32_png, heart_32_png_size);
	setupIcon(hiddenIcon_, blind_symbol_of_an_opened_eye_with_a_slash_png, blind_symbol_of_an_opened_eye_with_a_slash_png_size);
	addAndMakeVisible(thumbnail_);
	thumbnail_.setAlpha(0.3f);
}

void PatchButton::setupIcon(ImageComponent &icon, const unsigned char *icondata, size_t iconsize) {
	PNGImageFormat reader;
	MemoryInputStream memStream(icondata, iconsize, false);
	auto im = reader.decodeImage(memStream);
	auto smaller = im.rescaled(16, 16);
	icon.setImage(smaller);
	icon.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(icon);
	icon.setVisible(false);
	icon.toFront(false);
}

void PatchButton::setActive(bool active)
{
	active_ = active;
	button_.setToggleState(active, dontSendNotification);
	repaint();
}

void PatchButton::resized()
{
	Rectangle<int> area(getLocalBounds());
	auto upperRight = area;
	button_.setBounds(area.reduced(2));
	favoriteIcon_.setBounds(area);
	favoriteIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yTop | RectanglePlacement::onlyReduceInSize);
	hiddenIcon_.setBounds(area);
	hiddenIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yBottom | RectanglePlacement::onlyReduceInSize);
	thumbnail_.setBounds(area.reduced(4));
}

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

void PatchButton::setButtonText(const String& line1, const String &line2)
{
	button_.setButtonText(line1 + "\n" + line2);
}

void PatchButton::setFavorite(bool isFavorite)
{
	favoriteIcon_.setVisible(isFavorite);
}

void PatchButton::setHidden(bool isHidden) {
	hiddenIcon_.setVisible(isHidden);
}

void PatchButton::setThumbnailFile(const String &filename)
{
	if (filename.isNotEmpty()) {
		thumbnail_.loadFromFile(filename.toStdString());
	}
	else {
		thumbnail_.clearThumbnail();
	}
}

void PatchButton::clearThumbnailFile()
{
	thumbnail_.clearThumbnail();
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
