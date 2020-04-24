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
		Range<float> results[1];
		reader->readMaxLevels(0, length, results, 1);
		delete reader;
		gainScale_ = std::max(fabs(results[0].getStart()), fabs(results[0].getEnd()));

		// Ok, we want 500 ms on the thumbnail, so what is the reduction factor?
		/*double lengthInSeconds = length / sampleRate;
		int reductionFactor = (int) (lengthInSeconds / 0.5); // 0.5 being the 500 ms*/
		int reductionFactor = std::max((int)( length / 512), 1);

		audioThumbnail_ = std::make_unique<AudioThumbnail>(reductionFactor, formatManager_, sCache_);
		audioThumbnail_->setSource(new FileInputSource(inputFile));
		startTimer(100);
	}
	else {
		clearThumbnail();
	}
}

void Thumbnail::clearThumbnail()
{
	audioThumbnail_.reset();
}

void Thumbnail::paint(Graphics& g)
{
	//g.fillAll(Colours::darkblue);
	g.setColour(Colours::white);

	if (audioThumbnail_) {
		if (audioThumbnail_->isFullyLoaded()) {
			audioThumbnail_->drawChannel(g, getLocalBounds(), 0.0, audioThumbnail_->getTotalLength(), 0, 1.0f / gainScale_);
		}
	}
}

void Thumbnail::timerCallback()
{
	if (audioThumbnail_->isFullyLoaded()) {
		stopTimer();
		sendChangeMessage();
	}
}

AudioThumbnailCache Thumbnail::sCache_(128);