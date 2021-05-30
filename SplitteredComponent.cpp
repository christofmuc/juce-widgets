#include "SplitteredComponent.h"

#include "Settings.h"

SplitteredComponent::SplitteredComponent(std::string const &componentName, SplitteredEntry first, SplitteredEntry second, bool isVertical) 
	: componentName_(componentName), first_(first.component), second_(second.component), isVertical_(isVertical), didLoad_(false)
{
	auto resizer = new StretchableLayoutResizerBar(&stretchableManager_, 1, isVertical);
	resizerBars_.add(resizer);

	addAndMakeVisible(first_);
	addAndMakeVisible(second_);
	addAndMakeVisible(resizer);

	stretchableManager_.setItemLayout(0, -first.minimumPercentage/100.0f, -first.maximumPercentage/100.0f, -first.desiredPercentage/100.0f); 
	stretchableManager_.setItemLayout(1, 5, 5, 5);  // Left resizer, hard-coded to 5 pixels
	stretchableManager_.setItemLayout(2, -second.minimumPercentage / 100.0f, -second.maximumPercentage / 100.0f, -second.desiredPercentage / 100.0f);
	nComponents_ = 3;

	componentOrder_ = new Component * [nComponents_];
	componentOrder_[0] = first_;
	componentOrder_[1] = resizer;
	componentOrder_[2] = second_;
}

SplitteredComponent::SplitteredComponent(std::string const& componentName, SplitteredEntry first, SplitteredEntry second, SplitteredEntry third, bool isVertical)
	: componentName_(componentName), first_(first.component), second_(second.component), third_(third.component), isVertical_(isVertical), didLoad_(false)
{
	auto resizer1 = new StretchableLayoutResizerBar(&stretchableManager_, 1, isVertical);
	auto resizer2 = new StretchableLayoutResizerBar(&stretchableManager_, 3, isVertical);
	resizerBars_.add(resizer1);
	resizerBars_.add(resizer2);

	addAndMakeVisible(first_);
	addAndMakeVisible(second_);
	addAndMakeVisible(third_);
	addAndMakeVisible(resizer1);
	addAndMakeVisible(resizer2);

	stretchableManager_.setItemLayout(0, -first.minimumPercentage / 100.0f, -first.maximumPercentage / 100.0f, -first.desiredPercentage / 100.0f);
	stretchableManager_.setItemLayout(1, 5, 5, 5);  // Left resizer, hard-coded to 5 pixels
	stretchableManager_.setItemLayout(2, -second.minimumPercentage / 100.0f, -second.maximumPercentage / 100.0f, -second.desiredPercentage / 100.0f);
	stretchableManager_.setItemLayout(3, 5, 5, 5);  // Right resizer, hard-coded to 5 pixels
	stretchableManager_.setItemLayout(4, -third.minimumPercentage / 100.0f, -third.maximumPercentage / 100.0f, -third.desiredPercentage / 100.0f);
	nComponents_ = 5;

	componentOrder_ = new Component * [nComponents_];
	componentOrder_[0] = first_;
	componentOrder_[1] = resizer1;
	componentOrder_[2] = second_;
	componentOrder_[3] = resizer2;
	componentOrder_[4] = third_;
}

SplitteredComponent::~SplitteredComponent()
{
	// Store the position(s)
	Settings::instance().set(componentName_ + "-pos1", String(stretchableManager_.getItemCurrentPosition(1)).toStdString());
	if (nComponents_ > 3) {
		Settings::instance().set(componentName_ + "-pos2", String(stretchableManager_.getItemCurrentPosition(3)).toStdString());
	}
	delete componentOrder_;
}

void SplitteredComponent::resized()
{
	Rectangle<int> area(getLocalBounds());
	stretchableManager_.layOutComponents(componentOrder_, 5,
		area.getX(), area.getY(), area.getWidth(), area.getHeight(),
		!isVertical_, true);

	if (!didLoad_) {
		// Lazy loading of position, can do this only after one successful layout run
		if (Settings::instance().keyIsSet(componentName_ + "-pos1")) {
			auto storedPos = Settings::instance().get(componentName_ + "-pos1", "0");
			stretchableManager_.setItemPosition(1, atoi(storedPos.c_str()));
		}
		if (Settings::instance().keyIsSet(componentName_ + "-pos2")) {
			auto storedPos = Settings::instance().get(componentName_ + "-pos2", "0");
			stretchableManager_.setItemPosition(3, atoi(storedPos.c_str()));
		}
		didLoad_ = true;
		resized();
	}

}
