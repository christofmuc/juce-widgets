#include "RecycleBin.h"

#include "IconHelper.h"
#include "BinaryResources.h"

RecycleBin::RecycleBin()
{
	IconHelper::setupIcon(this, wasteBin_, delete_png, delete_png_size, 128);
	addAndMakeVisible(wasteBin_);
}

void RecycleBin::resized()
{
	auto bounds = getLocalBounds();
	wasteBin_.setBounds(bounds);
	wasteBin_.setImagePlacement(RectanglePlacement::xMid | RectanglePlacement::yMid| RectanglePlacement::onlyReduceInSize);
}

bool RecycleBin::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
	ignoreUnused(dragSourceDetails);
	return onItemDropped != nullptr;
}

void RecycleBin::itemDropped(const SourceDetails& dragSourceDetails)
{
	if (onItemDropped)
		onItemDropped(dragSourceDetails.description);
}
