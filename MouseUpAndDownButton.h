/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class MouseUpAndDownButton : public TextButton {
public:
	MouseUpAndDownButton(std::function<void(TextButton *button)> handler) : handler_(handler) {}

	virtual void buttonStateChanged() override {
		if (isDown()) {
			handler_(this);
		}
	}

private:
	std::function<void(TextButton *button)> handler_;
};

