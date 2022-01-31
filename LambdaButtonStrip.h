/*
 * MIT License
 *
 * Copyright (c) 2019-2021 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "JuceHeader.h"

#include <map>

struct ButtonDefinition {
	ButtonDefinition(std::string const &text, std::function<void()> ftor) : buttonText(text), functor(ftor), defaultKeycode(0), defaultModifiers(0)  {}
	ButtonDefinition(std::string const &text, std::function<void()> ftor, int defKeycode) : buttonText(text), functor(ftor) , defaultKeycode(defKeycode), defaultModifiers(0) {}
	ButtonDefinition(std::string const &text, std::function<void()> ftor, int defKeycode, ModifierKeys modifiers) : buttonText(text), functor(ftor), defaultKeycode(defKeycode), defaultModifiers(modifiers) {}
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
	virtual ~LambdaButtonStrip() override;

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
