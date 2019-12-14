#pragma once

#include "JuceHeader.h"

#include <map>

class LambdaButtonStrip : public Component,
	public Button::Listener
{
public:
	enum Direction { Horizontal, Vertical };
	typedef std::pair<std::string, std::function<void()>> TButtonDefinition;
	typedef std::map<std::string, TButtonDefinition> TButtonMap;

	LambdaButtonStrip(Direction dir = Vertical);
	virtual ~LambdaButtonStrip();

	void setButtonDefinitions(TButtonMap const &definitions);

	void buttonClicked(Button* button) override;

	virtual void resized() override;

private:
	Direction dir_;
	TButtonMap buttonDefintions_;
	std::map<std::string, TextButton *> buttons_;
};

