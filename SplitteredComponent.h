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
	SplitteredComponent(SplitteredEntry first, SplitteredEntry second, bool isVertical);
	SplitteredComponent(SplitteredEntry first, SplitteredEntry second, SplitteredEntry third, bool isVertical);
	virtual ~SplitteredComponent();

	virtual void resized();

private:
	Component* first_;
	Component* second_;
	Component* third_;
	bool isVertical_;
	
	StretchableLayoutManager stretchableManager_;
	OwnedArray<StretchableLayoutResizerBar> resizerBars_;
	int nComponents_;
	Component** componentOrder_;
};

