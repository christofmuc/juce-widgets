/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include <map>

struct ButtonDefinition {
	ButtonDefinition(std::string const &text, std::function<void()> functor) : buttonText(text), functor(functor), defaultKeycode(0), defaultModifiers(0)  {}
	ButtonDefinition(std::string const &text, std::function<void()> functor, int defaultKeycode) : buttonText(text), functor(functor) , defaultKeycode(defaultKeycode), defaultModifiers(0) {}
	ButtonDefinition(std::string const &text, std::function<void()> functor, int defaultKeycode, ModifierKeys modifiers) : buttonText(text), functor(functor), defaultKeycode(defaultKeycode), defaultModifiers(modifiers) {}
	ButtonDefinition(std::string const &text, std::function<void()> function, std::function<bool()> isEnabled) : buttonText(text), functor(function), defaultKeycode(0), defaultModifiers(0), canFire(isEnabled) {}

	std::string buttonText;
	std::function<void()> functor;
	int defaultKeycode;
	ModifierKeys defaultModifiers;
	std::function<bool()> canFire;
};

class LambdaButtonStrip : public Component,
	public Button::Listener,
	public ApplicationCommandTarget
{
public:
	enum class Direction { Horizontal, Vertical };
	typedef std::vector<std::pair<std::string, ButtonDefinition>> TButtonMap;

	LambdaButtonStrip(int commandBaseIndex, Direction dir = Direction::Vertical);
	virtual ~LambdaButtonStrip();

	void setButtonDefinitions(TButtonMap const &definitions);

	void buttonClicked(Button* button) override;

	virtual void resized() override;

	// All lambda buttons registered will be available as command targets within the JUCE framework
	virtual ApplicationCommandTarget* getNextCommandTarget() override;
	virtual void getAllCommands(Array<CommandID>& commands) override;
	virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	virtual bool perform(const InvocationInfo& info) override;

private:
	int commandBaseIndex_;
	Direction dir_;
	TButtonMap buttonDefinitions_;
	std::vector<std::pair<std::string, TextButton *>> buttons_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LambdaButtonStrip)
};

