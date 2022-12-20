/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>

class Thumbnail : public juce::Component, public juce::ChangeBroadcaster, private juce::Timer {
public:
    struct CacheInfo {
        int reductionFactor;
        float gainScale;
        juce::MemoryBlock cacheData;
    };

    Thumbnail();

    void loadFromFile(std::string const &fullpath, std::string const &fullpathOfCacheFileToBeCreated);
    void loadFromCache(CacheInfo const &cacheInfo);
    void clearThumbnail();

    void paint(juce::Graphics &g) override;

    void saveCacheInfo(juce::File cacheFile);
    static CacheInfo loadCacheInfo(juce::File cacheFile);

private:
    void initMemoryCache();
    void timerCallback() override;

    static std::weak_ptr<juce::AudioThumbnailCache>
        sCache_; // This is a memory cache only, weak because then it will be released once the last thumbnail is gone and not leak

    juce::AudioFormatManager formatManager_;
    std::shared_ptr<juce::AudioThumbnailCache> memoryCache_;
    std::unique_ptr<juce::AudioThumbnail> audioThumbnail_;
    float gainScale_;
    CacheInfo cacheInfo_;
    juce::File cacheFile_;
};
