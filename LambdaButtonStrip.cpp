#include "LambdaButtonStrip.h"

LambdaButtonStrip::LambdaButtonStrip(Direction dir /*= Vertical*/) : dir_(dir)
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
	buttonDefintions_ = definitions;

	// Currently not allowed to be replaced
	jassert(buttons_.size() == 0);

	// Now, make a loop and create these buttons. They will not be visible until 
	// they are explicitly arranged in the resize function, though, don't forget that!
	for (auto button : buttonDefintions_) {
		decltype(button.second) buttonDef = button.second;
		buttons_[button.first] = new TextButton(std::get<0>(buttonDef));
		buttons_[button.first]->addListener(this);
		buttons_[button.first]->setComponentID(button.first);
		addAndMakeVisible(buttons_[button.first]);
	}
	resized();
}

void LambdaButtonStrip::buttonClicked(Button* button)
{
	// Use our dispatching table we built in the constructor to have less code
	for (auto b : buttons_) {
		if (b.first == button->getComponentID()) {
			// That's our button!
			if (buttonDefintions_.find(b.first) != buttonDefintions_.end()) {
				// We have a definition for that!
				auto functor = std::get<1>(buttonDefintions_[b.first]);
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
	if (dir_ == Vertical) {
		for (auto b : buttons_) {
			b.second->setBounds(area.removeFromTop(20));
		}
	}
	else {
		for (auto b : buttons_) {
			b.second->setBounds(area.removeFromLeft(150));
		}
	}
}
