/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "PatchButton.h"

#include "BinaryResources.h"
#include "IconHelper.h"

class TextButtonFixedFont : public TextButton {
public:
	using TextButton::TextButton;

	void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
	{
		auto& lf = getLookAndFeel();

		lf.drawButtonBackground(g, *this,
			findColour(getToggleState() ? buttonOnColourId : buttonColourId),
			shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

		drawButtonText(lf, g, *this, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
	}

	void drawButtonText(LookAndFeel &lf, Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
	{
		ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

		Font font(lf.getTextButtonFont(button, button.getHeight()));
		g.setFont(font);
		g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
			: TextButton::textColourOffId)
			.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

		const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
		const int cornerSize = jmin(button.getHeight(), button.getWidth()) / 2;

		const int fontHeight = roundToInt(font.getHeight() * 0.6f);
		const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		const int textWidth = button.getWidth() - leftIndent - rightIndent;

		if (textWidth > 0) {
			String multilineText= button.getButtonText();
			if (multilineText.containsAnyOf("\r\n")) {
				g.drawFittedText(multilineText, leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
					Justification::centred, 2, 1.0f);
			} else {
				g.drawText(multilineText,
					leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2,
					Justification::centred, true);
			}
		}
	}

};

PatchButton::PatchButton(int id, bool isToggle, std::function<void(int)> clickHandler) : clicked_(clickHandler), id_(id), active_(false)
{
	button_ = std::make_unique<TextButtonFixedFont>();
	addAndMakeVisible(button_.get());
	button_->addListener(this);
	button_->setClickingTogglesState(isToggle);
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
	button_->setToggleState(active, dontSendNotification);
	repaint();
}

void PatchButton::resized()
{
	Rectangle<int> area(getLocalBounds());
	auto upperRight = area;
	button_->setBounds(area.reduced(2));
	favoriteIcon_.setBounds(area);
	synthName_.setBounds(area.reduced(4));
	favoriteIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yTop | RectanglePlacement::onlyReduceInSize);
	hiddenIcon_.setBounds(area);
	hiddenIcon_.setImagePlacement(RectanglePlacement::xRight | RectanglePlacement::yBottom | RectanglePlacement::onlyReduceInSize);
	thumbnail_.setBounds(area.reduced(4));

	// Calculate font size
	Font font = getLookAndFeel().getTextButtonFont(*button_, button_->getHeight());
	font.setHeight(font.getHeight() * 0.8f);
	synthName_.setFont(font);
}

void PatchButton::setColour(int colourId, Colour newColour)
{
	button_->setColour(colourId, newColour);
}

String PatchButton::getButtonText() const
{
	return button_->getButtonText();
}

void PatchButton::setButtonText(const String& text)
{
	button_->setButtonText(text.trim());
}

void PatchButton::setButtonText(const String& line1, const String &line2)
{
	button_->setButtonText(line1.trim() + "\n" + line2.trim());
}

void PatchButton::setSubtitle(const String &text)
{
	synthName_.setText(text.trim(), dontSendNotification);
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
	button_->setToggleState(state, dontSendNotification);
}

void PatchButton::buttonClicked(Button*)
{
	clicked_(id_);
}

bool PatchButton::getToggleState() const
{
	return button_->getToggleState();
}
