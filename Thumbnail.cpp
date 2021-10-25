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

#include "Thumbnail.h"
#include <memory>

Thumbnail::Thumbnail()
{
    formatManager_.registerBasicFormats(); // TODO this shouldn't be necessary for all thumbnails, but rather just once?
}

void Thumbnail::initMemoryCache()
{
    if (sCache_.expired()) {
        memoryCache_ = std::make_shared<AudioThumbnailCache>(1024);
        sCache_ = memoryCache_;
    }
    else {
        memoryCache_ = sCache_.lock();
    }
}

void Thumbnail::loadFromFile(std::string const &fullpath, std::string const &fullpathOfCacheFileToBeCreated)
{
    File inputFile(fullpath);

    if (inputFile.existsAsFile()) {
        // We need the total length of the file and its sample rate
        auto reader = formatManager_.createReaderFor(inputFile);
        if (!reader) {
            // That file must be corrupt!
            inputFile.deleteFile();
            return;
        }
        int64 length = reader->lengthInSamples;
        // double sampleRate = reader->sampleRate;
        Range<float> results[1];
        reader->readMaxLevels(0, length, results, 1);
        delete reader;
        gainScale_ = std::max(fabs(results[0].getStart()), fabs(results[0].getEnd()));

        // Ok, we want 500 ms on the thumbnail, so what is the reduction factor?
        /*double lengthInSeconds = length / sampleRate;
        int reductionFactor = (int) (lengthInSeconds / 0.5); // 0.5 being the 500 ms*/
        int reductionFactor = std::max((int) (length / 512), 1);

        initMemoryCache();
        audioThumbnail_ = std::make_unique<AudioThumbnail>(reductionFactor, formatManager_, *memoryCache_);
        audioThumbnail_->setSource(new FileInputSource(inputFile));
        startTimer(100);

        // Record the cache info we already have, when the file is fully loaded, we will also put the cache data in there
        cacheInfo_.reductionFactor = reductionFactor;
        cacheInfo_.gainScale = gainScale_;
        cacheFile_ = File(fullpathOfCacheFileToBeCreated);
    }
    else {
        clearThumbnail();
    }
}

void Thumbnail::loadFromCache(CacheInfo const &cacheInfo)
{
    cacheInfo_ = cacheInfo;
    gainScale_ = cacheInfo.gainScale;
    initMemoryCache();
    audioThumbnail_ = std::make_unique<AudioThumbnail>(cacheInfo.reductionFactor, formatManager_, *memoryCache_);
    MemoryInputStream input(cacheInfo.cacheData, false);
    audioThumbnail_->loadFrom(input);
}

void Thumbnail::clearThumbnail()
{
    audioThumbnail_.reset();
}

void Thumbnail::paint(Graphics &g)
{
    // g.fillAll(Colours::darkblue);
    g.setColour(Colours::white);

    if (audioThumbnail_) {
        if (audioThumbnail_->isFullyLoaded()) {
            audioThumbnail_->drawChannel(g, getLocalBounds(), 0.0, audioThumbnail_->getTotalLength(), 0, 1.0f / gainScale_);
        }
    }
}

void Thumbnail::saveCacheInfo(File cacheFile)
{
    FileOutputStream output(cacheFile);
    output.writeInt(cacheInfo_.reductionFactor);
    output.writeFloat(cacheInfo_.gainScale);
    MemoryInputStream fromBlock(cacheInfo_.cacheData, false);
    output.writeFromInputStream(fromBlock, cacheInfo_.cacheData.getSize());
}

Thumbnail::CacheInfo Thumbnail::loadCacheInfo(File cacheFile)
{
    CacheInfo cacheInfo;
    if (cacheFile.existsAsFile()) {
        FileInputStream inputFile(cacheFile);

        cacheInfo.reductionFactor = inputFile.readInt();
        cacheInfo.gainScale = inputFile.readFloat();
        inputFile.readIntoMemoryBlock(cacheInfo.cacheData, 65536); // Shouldn't be bigger than 64k?
    }
    return cacheInfo;
}

void Thumbnail::timerCallback()
{
    if (audioThumbnail_ && audioThumbnail_->isFullyLoaded()) {
        stopTimer();
        // Record cache info
        MemoryOutputStream output(cacheInfo_.cacheData, false);
        audioThumbnail_->saveTo(output);
        saveCacheInfo(cacheFile_);
        // Notify the UI that the cache is ready and the thumbnail can be rendered
        sendChangeMessage();
    }
}

std::weak_ptr<AudioThumbnailCache> Thumbnail::sCache_;
