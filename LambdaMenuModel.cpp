/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "LambdaMenuModel.h"


LambdaMenuModel::LambdaMenuModel(TMenuStructure const &menuStructure, ApplicationCommandManager *commandManager, LambdaButtonStrip *lambdaButtons) :
	menuStructure_(menuStructure), commandManager_(commandManager), lambdaButtons_(lambdaButtons)
{
}

StringArray LambdaMenuModel::getMenuBarNames()
{
	StringArray result;
	for (int i = 0; i < menuStructure_.size(); i++) {
		result.add(menuStructure_[i].first);
	}
	return result;
}

PopupMenu LambdaMenuModel::getMenuForIndex(int topLevelMenuIndex, const String& menuName)
{
	ignoreUnused(menuName);
	PopupMenu menu;
	for (auto &item : menuStructure_[topLevelMenuIndex].second) {
		if (item.hasSubmenu) {
			// Found!
			PopupMenu submenu = item.getMenu();
			menu.addSubMenu(item.name, submenu);
			item.subItemNo = submenu.getNumItems();
		}
		else {
			// Ok, we want a command-based menu bar - for that, we need to search all commands in the lambdaButtons and fine one with the correct name
			Array<int> commands;
			lambdaButtons_->getAllCommands(commands);
			for (auto command : commands) {
				ApplicationCommandInfo info(command);
				lambdaButtons_->getCommandInfo(command, info);
				if (info.description == String(item.name)) {
					// Found!
					menu.addCommandItem(commandManager_, command);
					break;
				}
			}
		}
	}
	return menu;
}

void LambdaMenuModel::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
	// Commands will fire automatically, but submenus need special treatment here
	for (auto const &item : menuStructure_[topLevelMenuIndex].second) {
		if (item.hasSubmenu && menuItemID >= item.baseItemID && item.baseItemID < (menuItemID + item.subItemNo)) {
			item.subitemSelected(menuItemID - item.baseItemID);
		}
	}
}

