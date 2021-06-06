/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "IconHelper.h"

void IconHelper::setupIcon(Component *parent, ImageComponent& icon, const unsigned char* icondata, size_t iconsize) {
	PNGImageFormat reader;
	MemoryInputStream memStream(icondata, iconsize, false);
	auto im = reader.decodeImage(memStream);
	auto smaller = im.rescaled(16, 16);
	icon.setImage(smaller);
	icon.setInterceptsMouseClicks(false, false);
	parent->addAndMakeVisible(icon);
	icon.setVisible(false);
	icon.toFront(false);
}



