#pragma once

#include "JuceHeader.h"


class CollapseButton : public Button {
public:
	CollapseButton(bool defaultOpen);

protected:
	void paintButton(Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

class CollapseHeader : public Component, public ChangeBroadcaster, private Button::Listener {
public:
	CollapseHeader(String const &title, bool defaultOpen);

	virtual void resized() override;

	bool isOpen() const;

private:
	void buttonClicked(Button*) override;

	Label text_;
	CollapseButton collapse_;
};

class CollapsibleContainer : public Component, private ChangeListener {
public:
	CollapsibleContainer(String const &title, Component *collapsible, bool defaultOpen);
	virtual ~CollapsibleContainer();

	virtual void resized() override;

	bool isOpen() const;

private:
	void changeListenerCallback(ChangeBroadcaster* source) override;

	CollapseHeader collapseHeader_;
	Component *collapsible_;
};

