/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "CustomizedRotary.h"

class CustomRotaryLookAndFeel : public LookAndFeel_V4 {

	virtual void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto outline = slider.findColour(Slider::rotarySliderOutlineColourId);
		auto fill = slider.findColour(Slider::rotarySliderFillColourId);

		auto bounds = Rectangle<int>(x, y, width, height).toFloat().reduced(10);

		auto radius = jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
		auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
		auto lineW = jmin(8.0f, radius * 0.5f);
		auto arcRadius = radius - lineW * 0.5f;

		const float rotaryMidAngle = 0.5f * (rotaryStartAngle + rotaryEndAngle);

		Path backgroundArc;
		backgroundArc.addCentredArc(bounds.getCentreX(),
			bounds.getCentreY(),
			arcRadius,
			arcRadius,
			0.0f,
			rotaryStartAngle,
			rotaryEndAngle,
			true);

		g.setColour(outline);
		g.strokePath(backgroundArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));

		if (slider.isEnabled())
		{
			Path valueArc;
			if (toAngle > rotaryMidAngle) {
				valueArc.addCentredArc(bounds.getCentreX(),
					bounds.getCentreY(),
					arcRadius,
					arcRadius,
					0.0f,
					rotaryMidAngle,
					toAngle,
					true);
			}
			else {
				valueArc.addCentredArc(bounds.getCentreX(),
					bounds.getCentreY(),
					arcRadius,
					arcRadius,
					0.0f,
					toAngle,
					rotaryMidAngle,
					true);
			}

			g.setColour(fill);
			g.strokePath(valueArc, PathStrokeType(lineW, PathStrokeType::curved, PathStrokeType::rounded));
		}

		auto thumbWidth = lineW * 2.0f;
		Point<float> thumbPoint(bounds.getCentreX() + arcRadius * std::cos(toAngle - MathConstants<float>::halfPi),
			bounds.getCentreY() + arcRadius * std::sin(toAngle - MathConstants<float>::halfPi));

		g.setColour(slider.findColour(Slider::thumbColourId));
		g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
	}

};

CustomizedRotary::CustomizedRotary()
{
	auto customLookAndFeel = new CustomRotaryLookAndFeel();
	setLookAndFeel(customLookAndFeel);
}
