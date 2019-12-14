/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include <map>

class LambdaButtonStrip : public Component,
	public Button::Listener
{
public:
	enum Direction { Horizontal, Vertical };
	typedef std::tuple<int, std::string, std::function<void()>> TButtonDefinition;
	typedef std::map<std::string, TButtonDefinition> TButtonMap;

	LambdaButtonStrip(Direction dir = Vertical);
	virtual ~LambdaButtonStrip();

	void setButtonDefinitions(TButtonMap const &definitions);

	void buttonClicked(Button* button) override;

	virtual void resized() override;

private:
	Direction dir_;
	TButtonMap buttonDefinitions_;
	std::vector<std::pair<std::string, TextButton *>> buttons_;
};

