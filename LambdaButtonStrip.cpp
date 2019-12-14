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

void LambdaButtonStrip::setButtonDefinitions(TButtonMap const &definitions)
{
	buttonDefinitions_ = definitions;

	// Currently not allowed to be replaced
	jassert(buttons_.size() == 0);

	// Now, make a loop and create these buttons. They will not be visible until 
	// they are explicitly arranged in the resize function, though, don't forget that!
	buttons_.resize(buttonDefinitions_.size());
	for (auto button : buttonDefinitions_) {
		decltype(button.second) buttonDef = button.second;
		int position = std::get<0>(buttonDef);
		auto b = new TextButton(std::get<1>(buttonDef));
		b->addListener(this);
		b->setComponentID(button.first);
		buttons_[position] = std::make_pair(button.first, b);
		addAndMakeVisible(b);
	}
	resized();
}

void LambdaButtonStrip::buttonClicked(Button* button)
{
	// Use our dispatching table we built in the constructor to have less code
	for (auto b : buttons_) {
		if (b.first == button->getComponentID()) {
			// That's our button!
			if (buttonDefinitions_.find(b.first) != buttonDefinitions_.end()) {
				// We have a definition for that!
				auto functor = std::get<2>(buttonDefinitions_[b.first]);
				functor();
				return;
			}
			else {
				// That's weird
				jassert(false);
			}
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
		for (auto b : buttons_) {
			b.second->setBounds(area.removeFromLeft(100));
		}
	}
}

juce::ApplicationCommandTarget* LambdaButtonStrip::getNextCommandTarget()
{
	return nullptr;
}

void LambdaButtonStrip::getAllCommands(Array<CommandID>& commands)
{
	for (int i = 0; i < buttons_.size(); i++) {
		commands.add(commandBaseIndex_ + i);
	}
}

void LambdaButtonStrip::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
	int index = ((int)commandID) - commandBaseIndex_;
	result.setInfo(buttons_[index].first, buttons_[index].second->getButtonText(), "General Category", 0);
}

bool LambdaButtonStrip::perform(const InvocationInfo& info)
{
	int index = ((int)info.commandID) - commandBaseIndex_;
	auto id = buttons_[index].first;
	if (buttonDefinitions_.find(id) != buttonDefinitions_.end()) {
		// We have a definition for that!
		auto functor = std::get<2>(buttonDefinitions_[id]);
		functor();
		return true;
	}
	else {
		// That's weird
		jassert(false);
		return false;
	}
}
