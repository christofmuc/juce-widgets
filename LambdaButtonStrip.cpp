/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "LambdaButtonStrip.h"

LambdaButtonStrip::LambdaButtonStrip(int commandBaseIndex, Direction dir /*= Vertical*/) : dir_(dir), commandBaseIndex_(commandBaseIndex)
{
}

LambdaButtonStrip::~LambdaButtonStrip()
{
	for (auto button : buttons_) {
		delete button.second;
	}
}

void LambdaButtonStrip::setButtonDefinitions(TButtonMap const& definitions)
{
	buttonDefinitions_ = definitions;

	// Currently not allowed to be replaced
	jassert(buttons_.size() == 0);

	// Now, make a loop and create these buttons. They will not be visible until 
	// they are explicitly arranged in the resize function, though, don't forget that!
	for (auto button : buttonDefinitions_) {
		decltype(button.second) buttonDef = button.second;
		auto b = new TextButton(buttonDef.buttonText);
		b->addListener(this);
		b->setComponentID(button.first);
		buttons_.push_back(std::make_pair(button.first, b));
		addAndMakeVisible(b);
	}
	resized();
}

void LambdaButtonStrip::buttonClicked(Button* button)
{
	// Use our dispatching table we built in the constructor to have less code
	for (auto b : buttonDefinitions_) {
		if (b.first == button->getComponentID()) {
			b.second.functor();
			return;
		}
	}
}

void LambdaButtonStrip::resized()
{
	Rectangle<int> area(getLocalBounds());

	// This is random order I fear
	if (dir_ == Direction::Vertical) {
		for (auto b : buttons_) {
			b.second->setBounds(area.removeFromTop(20));
		}
	}
	else {
		if (buttons_.size() > 0) {
			int buttonWidth = std::min(200, getWidth() / (int)buttons_.size());
			for (auto b : buttons_) {
				//TODO - if this fails, that is because you don't have a continuous count in your buttons for the order field
				if (b == buttons_[0]) {
					b.second->setBounds(area.removeFromLeft(buttonWidth));
				}
				else {
					b.second->setBounds(area.removeFromLeft(buttonWidth).withTrimmedLeft(8));
				}
			}
		}
	}
}

juce::ApplicationCommandTarget* LambdaButtonStrip::getNextCommandTarget()
{
	return nullptr;
}

void LambdaButtonStrip::getAllCommands(Array<CommandID>& commands)
{
	for (int i = 0; i < (int)buttons_.size(); i++) {
		commands.add(commandBaseIndex_ + i);
	}
}

void LambdaButtonStrip::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	int index = ((int)commandID) - commandBaseIndex_;
	auto id = buttons_[index].first;
	result.setInfo(id, buttons_[index].second->getButtonText(), "General Category", 0);
	result.addDefaultKeypress(buttonDefinitions_[index].second.defaultKeycode, buttonDefinitions_[index].second.defaultModifiers);
	bool active = buttonDefinitions_[index].second.canFire ? buttonDefinitions_[index].second.canFire() : true;
	result.setActive(active);
	buttons_[index].second->setEnabled(active);
}

bool LambdaButtonStrip::perform(const InvocationInfo& info)
{
	int index = ((int)info.commandID) - commandBaseIndex_;
	auto id = buttons_[index].first;
	buttonDefinitions_[index].second.functor();
	return true;
}
