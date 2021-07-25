/*
   Copyright (c) 2021 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

class TextSearchBox : public Component {
public:
	TextSearchBox(std::function<void()> updateHandler);

	void setFontSize(float fontSize);

	virtual void resized() override;
	void paint(Graphics& g) override;

	String searchText() const;
	void setSearchText(String const &searchText);
	
private:
	void refreshClearButton();

	std::function<void()> updateHandler_;
	std::unique_ptr<TextEditor> nameSearchText_;
	ImageComponent lookingGlass_;
	std::unique_ptr<Button> clearNameSearch_;
};
