/*
 * MIT License
 *
 * Copyright (c) 2019-2025 Christof Ruch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "LambdaMenuModel.h"


LambdaMenuModel::LambdaMenuModel(TMenuStructure const &menuStructure, juce::ApplicationCommandManager *commandManager,
    LambdaButtonStrip *lambdaButtons) :
    menuStructure_(menuStructure),
    commandManager_(commandManager), lambdaButtons_(lambdaButtons)
{
}

juce::StringArray LambdaMenuModel::getMenuBarNames()
{
    juce::StringArray result;
    for (size_t i = 0; i < menuStructure_.size(); i++) {
        result.add(menuStructure_[i].first);
    }
    return result;
}

juce::PopupMenu LambdaMenuModel::getMenuForIndex(int topLevelMenuIndex, const juce::String &menuName)
{
    ignoreUnused(menuName);
    juce::PopupMenu menu;
    if (topLevelMenuIndex < 0) {
        return menu;
    }

    for (auto &item : menuStructure_[(size_t) topLevelMenuIndex].second) {
        if (item.hasSubmenu) {
            // Found!
            juce::PopupMenu submenu = item.getMenu();
            menu.addSubMenu(item.name, submenu);
            item.subItemNo = submenu.getNumItems();
        }
        else {
            // Ok, we want a command-based menu bar - for that, we need to search all commands in the lambdaButtons and fine one with the correct name
            juce::Array<int> commands;
            lambdaButtons_->getAllCommands(commands);
            for (auto command : commands) {
                juce::ApplicationCommandInfo info(command);
                lambdaButtons_->getCommandInfo(command, info);
                if (info.description == juce::String(item.name)) {
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
    if (topLevelMenuIndex < 0) {
        return;
    }
    // Commands will fire automatically, but submenus need special treatment here
    for (auto const &item : menuStructure_[(size_t) topLevelMenuIndex].second) {
        if (item.hasSubmenu && menuItemID >= item.baseItemID && item.baseItemID < (menuItemID + item.subItemNo)) {
            item.subitemSelected(menuItemID - item.baseItemID);
        }
    }
}
