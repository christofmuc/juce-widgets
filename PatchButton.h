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

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Thumbnail.h"
#include "LambdaValueListener.h"
#include "TouchButton.h"

class PatchButton : public juce::Component {
public:
    PatchButton(int id, bool isToggle, std::function<void(int)> clickHandler);

    void setActive(bool active);

    virtual void resized() override;
    // virtual void paint(Graphics& g) override;

    void setPatchColour(int colourId, juce::Colour newColour);
    void bindColour(int colourId, juce::Value colourValue);
    juce::String getButtonText() const;
    void setButtonData(const juce::String &text);
    void setButtonDragInfo(const juce::String &dragInfo);
    void bindButtonData(juce::Value textValue);
    void setSubtitle(const juce::String &text);
    void bindSubtitle(juce::Value textValue);

    void setFavorite(bool isFavorite);
    void bindFavorite(juce::Value isFavorite);
    void setHidden(bool isHidden);
    void bindHidden(juce::Value isHidden);
    void setThumbnailFile(const juce::String &filename, const juce::String &cacheFileName);
    void setThumbnailFromCache(const Thumbnail::CacheInfo &cacheInfo);
    void clearThumbnailFile();

    void setToggleState(bool state);
    bool getToggleState() const;

    // For external control
    void trigger(TouchButtonFunction f = TouchButtonFunction::PRIMARY);

    // This is required when using the PatchButton with a scrollable list model, which reuses the buttons
    void updateId(int id);

private:
    std::function<void(int)> clicked_;
    std::unique_ptr<TouchButton> button_;
    Thumbnail thumbnail_;
    juce::ImageComponent favoriteIcon_;
    juce::ImageComponent hiddenIcon_;
    juce::Label synthName_;
    int id_;
    bool active_;

    std::unique_ptr<LambdaValueListener> titleListener_;
    std::unique_ptr<LambdaValueListener> subtitleListener_;
    std::unique_ptr<LambdaValueListener> colourListener_;
    std::unique_ptr<LambdaValueListener> favoriteListener_;
    std::unique_ptr<LambdaValueListener> hiddenListener_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchButton)
};

class PatchButtonWithDropTarget : public PatchButton, public juce::DragAndDropTarget {
public:
    using PatchButton::PatchButton;

    typedef std::function<bool(juce::var)> TAcceptHandler;
    typedef std::function<void(juce::var)> TDropHandler;

    TAcceptHandler acceptsItem;
    TDropHandler onItemDropped;

    // DragAndDropTarget implementation
    bool isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
    void itemDropped(const SourceDetails &dragSourceDetails) override;
};
