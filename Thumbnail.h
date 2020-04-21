/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class Thumbnail : public Component, public ChangeBroadcaster, private Timer {
public:
	Thumbnail();

	void loadFromFile(std::string fullpath);

	void paint(Graphics& g) override;

private:
	void timerCallback() override;

	static AudioThumbnailCache sCache_;

	AudioFormatManager formatManager_;
	std::unique_ptr<AudioThumbnail> audioThumbnail_;
	float gainScale_;
};

