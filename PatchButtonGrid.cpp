/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "PatchButtonGrid.h"

PatchButtonGrid::PatchButtonGrid(int w, int h, std::function<void(int)> buttonClicked) : columns_(w), rows_(h)
{
	for (int patch = 0; patch < w * h; patch++) {
		auto patchButton = new PatchButton(patch, true, [this, buttonClicked](int index) {
			disableAllExcept(index);
			buttonClicked(index);
		});
		addAndMakeVisible(patchButton);
		patchButtons_.add(patchButton);
	}
}

void PatchButtonGrid::resized()
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

PatchButton * PatchButtonGrid::buttonWithIndex(int index)
{
	return patchButtons_[index];
}

std::vector<int> PatchButtonGrid::getSelected() const
{
	std::vector<int> result;
	for (int i = 0; i < patchButtons_.size(); i++) {
		if (patchButtons_[i]->getToggleState()) {
			result.push_back(i);
		}
	}
	return result;
}

size_t PatchButtonGrid::size() const
{
	return patchButtons_.size();
}

void PatchButtonGrid::disableAllExcept(int index)
{
	for (int i = 0; i < patchButtons_.size(); i++) { 
		if (index != i) {
			patchButtons_[i]->setToggleState(false);
		}
	}
}
