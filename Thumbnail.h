/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
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

