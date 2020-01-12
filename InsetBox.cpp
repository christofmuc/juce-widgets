/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "InsetBox.h"

InsetBox::InsetBox(Component *content, BorderSize<int> borders) : content_(content), borders_(borders)
{
	addAndMakeVisible(content_);
}

void InsetBox::resized()
{
	content_->setBoundsInset(borders_);
}
