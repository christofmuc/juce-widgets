/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LambdaButtonStrip.h"

class LogView : public Component {
public:
	LogView();

	void addMessageToList(String const &message);
	void clearLog();

	virtual void resized() override;

private:
	std::unique_ptr<CodeDocument> document_;
	std::unique_ptr<LambdaButtonStrip> buttons_;
	CodeEditorComponent logBox_;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogView)
};

