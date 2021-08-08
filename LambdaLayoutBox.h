#pragma once

#include "JuceHeader.h"


// This transforms the resized() override required for JUCE components into an assignable lambda
class LambdaLayoutBox : public Component {
public:
	using Component::Component;

	virtual void resized() override {
		onResized(this);
	}

	std::function<void(Component*)> onResized;
};

