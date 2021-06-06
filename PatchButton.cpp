/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "PatchButton.h"

#include "BinaryResources.h"
#include "IconHelper.h"

PatchButton::PatchButton(int id, bool isToggle, std::function<void(int)> clickHandler) : clicked_(clickHandler), id_(id), active_(false)
{
	addAndMakeVisible(button_);
	button_.addListener(this);
	button_.setClickingTogglesState(isToggle);
	IconHelper::setupIcon(this, favoriteIcon_, heart_32_png, heart_32_png_size);
	IconHelper::setupIcon(this ,hiddenIcon_, blind_symbol_of_an_opened_eye_with_a_slash_png, blind_symbol_of_an_opened_eye_with_a_slash_png_size);
	addAndMakeVisible(thumbnail_);
	thumbnail_.setAlpha(0.3f);
	thumbnail_.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(synthName_);
	synthName_.setInterceptsMouseClicks(false, false);
	synthName_.setJustificationType(Justification::bottomLeft);
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
	synthName_.setBounds(area.reduced(4));
	favoriteIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yTop | RectanglePlacement::onlyReduceInSize);
	hiddenIcon_.setBounds(area);
	hiddenIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yBottom | RectanglePlacement::onlyReduceInSize);
	thumbnail_.setBounds(area.reduced(4));

	// Calculate font size
	Font font = synthName_.getFont();
	font.setHeight(button_.getHeight() / 4.0f);
	synthName_.setFont(font);
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

void PatchButton::setSubtitle(const String &text)
{
	synthName_.setText(text, dontSendNotification);
}

void PatchButton::setFavorite(bool isFavorite)
{
	favoriteIcon_.setVisible(isFavorite);
}

void PatchButton::setHidden(bool isHidden) {
	hiddenIcon_.setVisible(isHidden);
}

void PatchButton::setThumbnailFile(const String &filename, const String &cacheFileName)
{
	if (filename.isNotEmpty()) {
		thumbnail_.loadFromFile(filename.toStdString(), cacheFileName.toStdString());
	}
	else {
		thumbnail_.clearThumbnail();
	}
}

void PatchButton::setThumbnailFromCache(const Thumbnail::CacheInfo &cacheInfo)
{
	thumbnail_.loadFromCache(cacheInfo);
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
