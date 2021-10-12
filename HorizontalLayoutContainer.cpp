/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
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

#include "HorizontalLayoutContainer.h"

#include "InsetBox.h"

HorizontalLayoutContainer::HorizontalLayoutContainer(Component *left, Component *right, double leftSize, double rightSize) :
	left_(new InsetBox(left, BorderSize<int>(0, 0, 0, 4))), right_(new InsetBox(right, BorderSize<int>(0, 4, 0, 0)))
{
	addAndMakeVisible(left_);
	addAndMakeVisible(right_);

	layout_.setItemLayout(0, leftSize, leftSize, leftSize);
	layout_.setItemLayout(1, rightSize, rightSize, rightSize);
}

void HorizontalLayoutContainer::resized()
{
	Rectangle<int> area(getLocalBounds());
	Component* components[] = { left_, right_ };
	layout_.layOutComponents(components, 2, area.getX(), area.getY(), area.getWidth(), area.getHeight(), false, true);
}

void HorizontalLayoutContainer::setComponents(Component *left, Component *right)
{
	if (left != left_) {
		delete left_;
		left_ = left;
		addAndMakeVisible(left);
	}
	if (right != right_) {
		delete right_;
		right_ = right;
		addAndMakeVisible(right);
	}
	resized();
}
