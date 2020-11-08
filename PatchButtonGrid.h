/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

template<class P>
class PatchButtonGrid : public Component {
public:

	PatchButtonGrid(int w, int h, std::function<void(int)> buttonClicked) : columns_(w), rows_(h)
	{
		for (int patch = 0; patch < w * h; patch++) {
			auto patchButton = new P(patch, true, [this, buttonClicked](int index) {
				disableAllExcept(index);
				buttonClicked(index);
			});
			addAndMakeVisible(patchButton);
			patchButtons_.add(patchButton);
		}
	}

	virtual void resized() override
	{
		Rectangle<int> area(getLocalBounds());

		juce::Grid grid;
		grid.setGap(8_px);
		using Track = juce::Grid::TrackInfo;
		for (int i = 0; i < rows_; i++) grid.templateRows.add(Track(1_fr));
		for (int i = 0; i < columns_; i++) grid.templateColumns.add(Track(1_fr));
		for (auto patchbutton : patchButtons_) {
			grid.items.add(juce::GridItem(*patchbutton));
		}
		grid.performLayout(area);
	}

	P *buttonWithIndex(int index)
	{
		return patchButtons_[index];
	}

	std::vector<int> getSelected() const
	{
		std::vector<int> result;
		for (int i = 0; i < patchButtons_.size(); i++) {
			if (patchButtons_[i]->getToggleState()) {
				result.push_back(i);
			}
		}
		return result;
	}

	size_t size() const
	{
		return patchButtons_.size();
	}

private:
	void disableAllExcept(int index)
	{
		for (int i = 0; i < patchButtons_.size(); i++) {
			if (index != i) {
				patchButtons_[i]->setToggleState(false);
			}
		}
	}

	int columns_;
	int rows_;
	OwnedArray<P> patchButtons_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchButtonGrid)
};
