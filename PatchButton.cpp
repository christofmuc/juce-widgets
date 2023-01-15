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

#include "PatchButton.h"

#include "BinaryResources.h"
#include "IconHelper.h"

#include "SpdLogJuce.h"
#include <spdlog/spdlog.h>

class PatchTextButtonFixedFontDraggable : public juce::TextButton {
public:
    using juce::TextButton::TextButton;

    void setDragStartInfo(juce::String dragInfo) { dragInfo_ = dragInfo; }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto& lf = getLookAndFeel();

        lf.drawButtonBackground(g, *this, findColour(getToggleState() ? buttonOnColourId : buttonColourId), shouldDrawButtonAsHighlighted,
            shouldDrawButtonAsDown);

        drawButtonText(lf, g, *this, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
    }

    void drawButtonText(juce::LookAndFeel& lf, juce::Graphics& g, TextButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
    {
        juce::ignoreUnused(shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

        juce::Font font(lf.getTextButtonFont(button, button.getHeight()));
        g.setFont(font);
        g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId : TextButton::textColourOffId)
                        .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));

        const int yIndent = juce::jmin(4, button.proportionOfHeight(0.3f));
        const int cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;

        const int fontHeight = juce::roundToInt(font.getHeight() * 0.6f);
        const int leftIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
        const int rightIndent = juce::jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
        const int textWidth = button.getWidth() - leftIndent - rightIndent;

        if (textWidth > 0) {
            juce::String multilineText = button.getButtonText();
            if (multilineText.containsAnyOf("\r\n")) {
                g.drawFittedText(multilineText, leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2, juce::Justification::centred, 2,
                    1.0f);
            }
            else {
                g.drawText(multilineText, leftIndent, yIndent, textWidth, button.getHeight() - yIndent * 2, juce::Justification::centred, true);
            }
        }
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        juce::ignoreUnused(event);
        if (dragInfo_.isNotEmpty()) {
            // If a drag info has been set and we are located in a drag container, start dragging with the info given!
            auto dragContainer = juce::DragAndDropContainer::findParentDragContainerFor(this);
            if (dragContainer) {
                if (!dragContainer->isDragAndDropActive()) {
                    dragContainer->startDragging(dragInfo_, this);
                }
            }
        }
        TextButton::mouseDrag(event);
    }

private:
    juce::String dragInfo_;
};

PatchButton::PatchButton(int id, bool isToggle, std::function<void(int)> clickHandler) : clicked_(clickHandler), id_(id), active_(false)
{
    button_ = std::make_unique<PatchTextButtonFixedFontDraggable>();
    addAndMakeVisible(button_.get());
    button_->addListener(this);
    button_->setClickingTogglesState(isToggle);
    IconHelper::setupIcon(this, favoriteIcon_, heart_32_png, heart_32_png_size, 16);
    IconHelper::setupIcon(this, hiddenIcon_, blind_symbol_of_an_opened_eye_with_a_slash_png, blind_symbol_of_an_opened_eye_with_a_slash_png_size, 16);
    addAndMakeVisible(thumbnail_);
    thumbnail_.setAlpha(0.3f);
    thumbnail_.setInterceptsMouseClicks(false, false);
    addAndMakeVisible(synthName_);
    synthName_.setInterceptsMouseClicks(false, false);
    synthName_.setJustificationType(juce::Justification::bottomLeft);
}

void PatchButton::updateId(int id)
{
    id_ = id;
}

void PatchButton::setActive(bool active)
{
    active_ = active;
    button_->setToggleState(active, juce::dontSendNotification);
    repaint();
}

void PatchButton::resized()
{
    juce::Rectangle<int> area(getLocalBounds());
    button_->setBounds(area.reduced(2));
    favoriteIcon_.setBounds(area);
    synthName_.setBounds(area.reduced(4));
    favoriteIcon_.setImagePlacement(juce::RectanglePlacement::xRight | juce::RectanglePlacement::yTop | juce::RectanglePlacement::onlyReduceInSize);
    hiddenIcon_.setBounds(area);
    hiddenIcon_.setImagePlacement(juce::RectanglePlacement::xRight | juce::RectanglePlacement::yBottom | juce::RectanglePlacement::onlyReduceInSize);
    thumbnail_.setBounds(area.reduced(4));

    // Calculate font size
    juce::Font font = getLookAndFeel().getTextButtonFont(*button_, button_->getHeight());
    font.setHeight(font.getHeight() * 0.8f);
    synthName_.setFont(font);
}

void PatchButton::setPatchColour(int colourId, juce::Colour newColour)
{
    button_->setColour(colourId, newColour);
}

void PatchButton::bindColour(int colourId, juce::Value colourValue) 
{
    colourListener_ = std::make_unique<LambdaValueListener>(colourValue,
        [colourId, this](juce::Value const& newValue) { setPatchColour(colourId, juce::Colour::fromString(newValue.getValue().operator juce::String())); 
        });
    colourListener_->trigger();
}

juce::String PatchButton::getButtonText() const
{
    return button_->getButtonText();
}

void PatchButton::setButtonData(const juce::String& text)
{
    button_->setButtonText(text.trim());
}

void PatchButton::setButtonDragInfo(const juce::String& dragInfo) 
{
    // TODO - ugly, but kind of impl pattern
    dynamic_cast<PatchTextButtonFixedFontDraggable*>(button_.get())->setDragStartInfo(dragInfo);
}

void PatchButton::bindButtonData(juce::Value textValue)
{
    titleListener_ = std::make_unique<LambdaValueListener>(textValue,
        [this](juce::Value const& newValue) { setButtonData(newValue.getValue());
        });
    titleListener_->trigger();
}

void PatchButton::setSubtitle(const juce::String& text)
{
    synthName_.setText(text.trim(), juce::dontSendNotification);
}

void PatchButton::bindSubtitle(juce::Value textValue) 
{
    subtitleListener_ =
        std::make_unique<LambdaValueListener>(textValue, [this](juce::Value const& newValue) { setSubtitle(newValue.getValue()); });
    subtitleListener_->trigger();
}

void PatchButton::setFavorite(bool isFavorite)
{
    favoriteIcon_.setVisible(isFavorite);
}

void PatchButton::bindFavorite(juce::Value isFavorite)
{
    favoriteListener_ =
        std::make_unique<LambdaValueListener>(isFavorite, [this](juce::Value const& newValue) { setFavorite((bool) newValue.getValue()); });
    favoriteListener_->trigger();
}

void PatchButton::setHidden(bool isHidden)
{
    hiddenIcon_.setVisible(isHidden);
}

void PatchButton::bindHidden(juce::Value isHidden)
{
    hiddenListener_ =
        std::make_unique<LambdaValueListener>(isHidden, [this](juce::Value const& newValue) { setHidden((bool) newValue.getValue()); });
    hiddenListener_->trigger();
}

void PatchButton::setThumbnailFile(const juce::String& filename, const juce::String& cacheFileName)
{
    if (filename.isNotEmpty()) {
        thumbnail_.loadFromFile(filename.toStdString(), cacheFileName.toStdString());
    }
    else {
        thumbnail_.clearThumbnail();
    }
}

void PatchButton::setThumbnailFromCache(const Thumbnail::CacheInfo& cacheInfo)
{
    thumbnail_.loadFromCache(cacheInfo);
}

void PatchButton::clearThumbnailFile()
{
    thumbnail_.clearThumbnail();
}

void PatchButton::setToggleState(bool state)
{
    button_->setToggleState(state, juce::dontSendNotification);
}

void PatchButton::buttonClicked(juce::Button*)
{
    clicked_(id_);
}

bool PatchButton::getToggleState() const
{
    return button_->getToggleState();
}

bool PatchButtonWithDropTarget::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    return onItemDropped != nullptr && (acceptsItem == nullptr || acceptsItem(dragSourceDetails.description));
}

void PatchButtonWithDropTarget::itemDropped(const SourceDetails& dragSourceDetails)
{
    juce::String name = dragSourceDetails.description;
    spdlog::trace("Item dropped: {}", name);
    onItemDropped(dragSourceDetails.description);
}
