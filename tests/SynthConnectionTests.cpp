#include "PropertyEditor.h"
#include "SynthList.h"

struct SynthConnectionTestAccess {
    static juce::PopupMenu connectionMenu(SynthButtonWithActiveLight const &button) {
        return button.createConnectionMenu();
    }
};

namespace {
struct Item : public ActiveListItem {
    explicit Item(std::string itemName) : name(std::move(itemName)) {}
    std::string getName() override { return name; }
    bool isActive() override { return false; }
    juce::Colour getColour() override { return juce::Colours::blue; }
    std::string name;
};

void click(juce::Button *button) {
    button->triggerClick();
    juce::MessageManager::getInstance()->runDispatchLoopUntil(50);
}

void collectActions(juce::Component &component, std::vector<juce::ButtonPropertyComponent *> &actions) {
    if (auto *action = dynamic_cast<juce::ButtonPropertyComponent *>(&component))
        actions.push_back(action);
    for (auto *child : component.getChildren())
        collectActions(*child, actions);
}

class ConnectionTests : public juce::UnitTest {
  public:
    ConnectionTests() : UnitTest("Synth connection controls", "Connections") {}
    void runTest() override {
        beginTest("Checking a synth's status does not change the selection");
        SynthList list;
        std::vector<std::shared_ptr<ActiveListItem>> items{std::make_shared<Item>("B"), std::make_shared<Item>("A")};
        std::string selected, checked;
        list.setList(items, [&](auto item) { selected = item->getName(); }, [&](auto item) { checked = item->getName(); });
        list.setActiveListItem("A");
        auto *first = dynamic_cast<SynthButtonWithActiveLight *>(list.getChildComponent(0));
        auto *second = dynamic_cast<SynthButtonWithActiveLight *>(list.getChildComponent(1));
        expect(first && second);
        if (!first || !second)
            return;
        expectEquals(first->name(), std::string("A"));
        auto *nameA = dynamic_cast<juce::Button *>(first->getChildComponent(0));
        auto *nameB = dynamic_cast<juce::Button *>(second->getChildComponent(0));
        auto *statusB = dynamic_cast<juce::Button *>(second->getChildComponent(1));
        expect(nameA && nameB && statusB);
        if (!nameA || !nameB || !statusB)
            return;
        click(statusB);
        expectEquals(checked, std::string("B"));
        expect(selected.empty());
        expect(nameA->getToggleState() && !nameB->getToggleState());
        click(nameB);
        expectEquals(selected, std::string("B"));
        expect(!nameA->getToggleState() && nameB->getToggleState());

        beginTest("Status actions remain available after a successful detection");
        checked.clear();
        second->setActiveState(true);
        click(statusB);
        expectEquals(checked, std::string("B"));

        beginTest("Unsupported detection disables the status action");
        checked.clear();
        second->setConnectionActionsEnabled(false);
        second->setActiveState(false);
        click(statusB);
        expect(checked.empty());
        expect(statusB->getTooltip().contains("not available"));

        beginTest("Re-enabling checks restores the tooltip for the current detection state");
        for (bool detected : {false, true}) {
            second->setConnectionActionsEnabled(false);
            second->setActiveState(detected);
            expect(statusB->getTooltip().contains("not available"));
            second->setConnectionActionsEnabled(true);
            expectEquals(statusB->getTooltip(), juce::String(detected
                ? "Previously detected - click to check the saved connection"
                : "Not detected - click to check the saved connection"));
        }

        beginTest("A find-only callback remains available in the connection menu");
        SynthList findOnlyList;
        findOnlyList.setList(items, [](auto) {}, {}, [](auto) {});
        auto *findOnlyButton = dynamic_cast<SynthButtonWithActiveLight *>(findOnlyList.getChildComponent(0));
        expect(findOnlyButton != nullptr);
        if (findOnlyButton) {
            auto menu = SynthConnectionTestAccess::connectionMenu(*findOnlyButton);
            bool checkFound = false, findFound = false;
            for (juce::PopupMenu::MenuItemIterator it(menu); it.next();) {
                const auto &item = it.getItem();
                if (item.itemID == 1) {
                    checkFound = true;
                    expect(!item.isEnabled);
                }
                if (item.itemID == 2) {
                    findFound = true;
                    expect(item.isEnabled);
                }
            }
            expect(checkFound && findFound);
        }

        beginTest("Section actions are rendered without adding fake values to the property list");
        PropertyEditor editor;
        PropertyEditor::TProperties values{std::make_shared<TypedNamedValue>("Channel", "A", 1, 1, 16)};
        int actions = 0;
        editor.setProperties(values, {{"A", {{"Find this synth...", "Search ports", [&]() { ++actions; }, true}}}});
        std::vector<juce::ButtonPropertyComponent *> actionButtons;
        collectActions(editor, actionButtons);
        expectEquals(int(actionButtons.size()), 1);
        if (!actionButtons.empty())
            actionButtons.front()->buttonClicked();
        expectEquals(actions, 1);
        expectEquals(int(values.size()), 1);
    }
} tests;
} // namespace

int main() {
    juce::ScopedJuceInitialiser_GUI juce;
    juce::UnitTestRunner runner;
    runner.setAssertOnFailure(false);
    runner.runTestsInCategory("Connections");
    int failures = 0;
    for (int i = 0; i < runner.getNumResults(); ++i)
        failures += runner.getResult(i)->failures;
    return failures == 0 ? 0 : 1;
}
