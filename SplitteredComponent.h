/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

struct SplitteredEntry {
	Component* component;
	int desiredPercentage;
	int minimumPercentage;
	int maximumPercentage;
};

class SplitteredComponent : public Component
{
public:
	SplitteredComponent(std::string const& componentName, SplitteredEntry first, SplitteredEntry second, bool isVertical);
	SplitteredComponent(std::string const& componentName, SplitteredEntry first, SplitteredEntry second, SplitteredEntry third, bool isVertical);
	virtual ~SplitteredComponent();

	virtual void resized();

private:
	std::string componentName_;
	Component* first_;
	Component* second_;
	Component* third_;
	bool isVertical_;
	
	StretchableLayoutManager stretchableManager_;
	OwnedArray<StretchableLayoutResizerBar> resizerBars_;
	int nComponents_;
	Component** componentOrder_;
	bool didLoad_;
};

