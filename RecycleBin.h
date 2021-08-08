#pragma once

#include "JuceHeader.h"


class RecycleBin : public Component, public DragAndDropTarget {
public:
	RecycleBin();

	virtual void resized() override;

	bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
	void itemDropped(const SourceDetails& dragSourceDetails) override;

	std::function<void(var)> onItemDropped;

private:
	ImageComponent wasteBin_;
};


