/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "Thumbnail.h"
#include <memory>

Thumbnail::Thumbnail()
{
	formatManager_.registerBasicFormats(); //TODO this shouldn't be necessary for all thumbnails, but rather just once?
}

void Thumbnail::loadFromFile(std::string fullpath)
{
	File inputFile(fullpath);

	if (inputFile.existsAsFile()) {
		// We need the total length of the file and its sample rate
		auto reader = formatManager_.createReaderFor(inputFile);
		int64 length = reader->lengthInSamples;
		//double sampleRate = reader->sampleRate;
		delete reader;

		// Ok, we want 500 ms on the thumbnail, so what is the reduction factor?
		/*double lengthInSeconds = length / sampleRate;
		int reductionFactor = (int) (lengthInSeconds / 0.5); // 0.5 being the 500 ms*/
		int reductionFactor = (int)( length / 512);

		audioThumbnail_ = std::make_unique<AudioThumbnail>(reductionFactor, formatManager_, sCache_);
		audioThumbnail_->setSource(new FileInputSource(inputFile));
	}
	else {
		audioThumbnail_.reset();
	}
}

void Thumbnail::paint(Graphics& g)
{
	g.fillAll(Colours::darkblue);

	if (audioThumbnail_) {
		if (audioThumbnail_->isFullyLoaded()) {
			audioThumbnail_->drawChannel(g, getLocalBounds(), 0.0, audioThumbnail_->getTotalLength(), 0, 1.0f);
		}
	}
}

AudioThumbnailCache Thumbnail::sCache_(128);