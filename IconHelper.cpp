/*
 * MIT License
 *
 * Copyright (c) 2019-2025 Christof Ruch
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

#include "IconHelper.h"

static juce::Image recolourWithShading(const juce::Image& srcIn, juce::Colour tint)
{
    auto src = srcIn.convertedToFormat(juce::Image::ARGB);
    juce::Image dst(juce::Image::ARGB, src.getWidth(), src.getHeight(), true);

    juce::Image::BitmapData s(src, juce::Image::BitmapData::readOnly);
    juce::Image::BitmapData d(dst, juce::Image::BitmapData::writeOnly);

    const float tr = tint.getFloatRed();
    const float tg = tint.getFloatGreen();
    const float tb = tint.getFloatBlue();

    for (int y = 0; y < src.getHeight(); ++y) {
        auto* sp = reinterpret_cast<const juce::PixelARGB*>(s.getLinePointer(y));
        auto* dp = reinterpret_cast<juce::PixelARGB*>(d.getLinePointer(y));

        for (int x = 0; x < src.getWidth(); ++x, ++sp, ++dp) {
            const juce::uint8 a = sp->getAlpha();
            const float lum = (0.2126f * sp->getRed() + 0.7152f * sp->getGreen() + 0.0722f * sp->getBlue()) / 255.0f;

            dp->setARGB(a, (juce::uint8) std::round(255.0f * tr * lum), (juce::uint8) std::round(255.0f * tg * lum),
                (juce::uint8) std::round(255.0f * tb * lum));
        }
    }
    return dst;
}


void IconHelper::setupIcon(juce::Component* parent, juce::ImageComponent& icon, const unsigned char* icondata, size_t iconsize, int destSize)
{
    juce::PNGImageFormat reader;
    juce::MemoryInputStream memStream(icondata, iconsize, false);
    auto im = reader.decodeImage(memStream);
    auto smaller = im.rescaled(destSize, destSize);
    icon.setImage(smaller);
    icon.setInterceptsMouseClicks(false, false);
    parent->addAndMakeVisible(icon);
    icon.setVisible(false);
    icon.toFront(false);
}

void IconHelper::switchIcon(juce::ImageComponent& icon, const unsigned char* icondata, size_t iconsize, int destSize)
{
    juce::PNGImageFormat reader;
    juce::MemoryInputStream memStream(icondata, iconsize, false);
    auto im = reader.decodeImage(memStream);
    auto smaller = im.rescaled(destSize, destSize);
    icon.setImage(smaller);
}

void IconHelper::switchTintedIcon(juce::ImageComponent& icon, const unsigned char* icondata, size_t iconsize, int destSize,
    juce::Colour tintColour)
{
    juce::PNGImageFormat reader;
    juce::MemoryInputStream memStream(icondata, iconsize, false);
    auto im = reader.decodeImage(memStream).rescaled(destSize, destSize);

    auto tinted = recolourWithShading(im, tintColour);

    icon.setImage(tinted);
}
