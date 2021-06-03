/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class TextSearchBox : public Component {
public:
	TextSearchBox(std::function<void()> updateHandler);

	virtual void resized() override;
	//virtual void paintOverChildren(Graphics& g) override;

	String searchText() const;

	void paint(Graphics& g) override;
private:
	std::function<void()> updateHandler_;
	TextEditor nameSearchText_;
	Button* clearNameSearch_;
};