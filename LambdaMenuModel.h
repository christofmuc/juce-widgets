/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "LambdaButtonStrip.h"

class LambdaMenuModel : public MenuBarModel {
public:
	typedef std::map<int, std::pair<std::string, std::vector<std::string>>> TMenuStructure;

	LambdaMenuModel(TMenuStructure const &menuStructure, ApplicationCommandManager *commandManager, LambdaButtonStrip *lambdaButtons);

	StringArray getMenuBarNames() override;
	PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
	void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:
	TMenuStructure menuStructure_;
	ApplicationCommandManager *commandManager_;
	LambdaButtonStrip *lambdaButtons_;
};

