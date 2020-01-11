#pragma once

#include "JuceHeader.h"

class HorizontalLayoutContainer : public Component {
public:
	HorizontalLayoutContainer(Component *left, Component *right, double leftSize, double rightSize);

	void resized() override;

private:
	Component *left_;
	Component *right_;
	StretchableLayoutManager layout_;
};

