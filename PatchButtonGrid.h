/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "PatchButton.h"

class PatchButtonGrid : public Component {
public:
	PatchButtonGrid(int w, int h, std::function<void(int)> buttonClicked);

	virtual void resized() override;

	PatchButton *buttonWithIndex(int index);
	std::vector<int> getSelected() const;
	size_t size() const;

private:
	void disableAllExcept(int index);

	int columns_;
	int rows_;
	OwnedArray<PatchButton> patchButtons_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchButtonGrid)
};
