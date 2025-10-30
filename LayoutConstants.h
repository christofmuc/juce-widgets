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

#pragma once

#define LAYOUT_INSET_SMALL 4
#define LAYOUT_INSET_NORMAL 8

#define LAYOUT_LINE_HEIGHT 24
#define LAYOUT_LINE_SPACING (LAYOUT_LINE_HEIGHT + LAYOUT_INSET_NORMAL)

#define LAYOUT_TEXT_LINE_HEIGHT 24
#define LAYOUT_TEXT_LINE_SPACING (LAYOUT_TEXT_LINE_HEIGHT + LAYOUT_INSET_SMALL)

#define LAYOUT_LARGE_LINE_HEIGHT 36
#define LAYOUT_LARGE_LINE_SPACING (LAYOUT_LARGE_LINE_HEIGHT + LAYOUT_INSET_NORMAL)

#define LAYOUT_TINY_ICON_WIDTH 16
#define LAYOUT_TINY_ICON_HEIGHT 16
#define LAYOUT_SMALL_ICON_WIDTH 32
#define LAYOUT_SMALL_ICON_HEIGHT 32

#define LAYOUT_CHECKBOX_WIDTH 128
#define LAYOUT_BUTTON_WIDTH 128
#define LAYOUT_BUTTON_WIDTH_MIN 64
#define LAYOUT_BUTTON_HEIGHT LAYOUT_LINE_HEIGHT

#define LAYOUT_TOUCHBUTTON_HEIGHT (2 * LAYOUT_BUTTON_HEIGHT)

#define LAYOUT_LARGE_FONT_SIZE 24.0f

const juce::Colour kPaletteFill = juce::Colour::fromRGB(0x2a, 0x2d, 0x35);
const juce::Colour kPaletteFillHover = kPaletteFill.brighter(0.08f);
const juce::Colour kPaletteOutline = juce::Colour::fromRGB(0x3a, 0x3f, 0x4a);
const juce::Colour kAccentColour = juce::Colour::fromRGB(0x54, 0xc6, 0xff);
const juce::Colour kAccentColourInactive = kAccentColour.withAlpha(0.45f);
const juce::Colour kKnobFaceColour = juce::Colour::fromRGB(0x36, 0x3a, 0x43);
const juce::Colour kKnobFaceHighlight = kKnobFaceColour.brighter(0.18f);
const juce::Colour kKnobFaceShadow = kKnobFaceColour.darker(0.22f);
const juce::Colour kKnobTrackColour = juce::Colour::fromRGB(0x22, 0x24, 0x2a);
const float kCornerRadius = 10.0f;
const float kPaletteShadowOffset = 4.0f;
const float kPaletteShadowRadius = 14.0f;
