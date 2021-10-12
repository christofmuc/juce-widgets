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

#pragma once

#include "JuceHeader.h"

class Thumbnail : public Component, public ChangeBroadcaster, private Timer {
public:
	struct CacheInfo {
		int reductionFactor;
		float gainScale;
		MemoryBlock cacheData;
	};

	Thumbnail();

	void loadFromFile(std::string const &fullpath, std::string const &fullpathOfCacheFileToBeCreated);
	void loadFromCache(CacheInfo const &cacheInfo);
	void clearThumbnail();

	void paint(Graphics& g) override;

	void saveCacheInfo(File cacheFile);
	static CacheInfo loadCacheInfo(File cacheFile);

private:
	void initMemoryCache();
	void timerCallback() override;

	static std::weak_ptr<AudioThumbnailCache> sCache_; // This is a memory cache only, weak because then it will be released once the last thumbnail is gone and not leak

	AudioFormatManager formatManager_;
	std::shared_ptr<AudioThumbnailCache> memoryCache_;
	std::unique_ptr<AudioThumbnail> audioThumbnail_;
	float gainScale_;
	CacheInfo cacheInfo_;
	File cacheFile_;
};
