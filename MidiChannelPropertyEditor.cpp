/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "MidiChannelPropertyEditor.h"

std::map<int, std::string> sInputLookup;
std::map<int, std::string> sOutputLookup;

std::map<int, std::string> sMidiChannelLookup = {
	{ 1, "1" }, { 2, "2" }, { 3, "3" }, { 4, "4" }, { 5, "5" }, { 6, "6" }, { 7, "7" }, { 8, "8" }, { 9, "9" },
	{ 10, "10" }, { 11, "11" }, { 12, "12" }, { 13, "13" }, { 14, "14" }, { 15, "15" }, { 16, "16" }, { 17, "Omni" }, { 18, "Invalid" },
};

std::vector<std::string> currentOutputDevices() {
	std::vector<std::string> outputs;
	auto devices = MidiOutput::getAvailableDevices();
	for (const auto& device : devices) {
		outputs.push_back(device.name.toStdString());
	}
	return outputs;
}

//TODO this should go into MidiController
std::vector<std::string> currentInputDevices() {
	std::vector<std::string> inputs;
	auto devices = MidiInput::getAvailableDevices();
	for (const auto& device : devices) {
		inputs.push_back(device.name.toStdString());
	}
	return inputs;
}

MidiChannelPropertyEditor::MidiChannelPropertyEditor(std::string const &title, std::string const &sn) : TypedNamedValue(title, sn, 18, sMidiChannelLookup)
{
}


void MidiChannelPropertyEditor::setValue(MidiChannel channel)
{
	if (!channel.isValid()) {
		value_ = 18;
	}
	else if (channel.isOmni()) {
		value_ = 17;
	}
	else {
		value_ = channel.toOneBasedInt();
	}
}

MidiDevicePropertyEditor::MidiDevicePropertyEditor(std::string const &title, std::string const &sn, bool inputInsteadOfOutput, bool startTimerToCheckDeviceListUpdates) :
	TypedNamedValue(title, sn, 0, std::map<int, std::string>()), inputInsteadOfOutput_(inputInsteadOfOutput)
{
	value_ = Value(1);
	if (inputInsteadOfOutput) {
		refreshDropdownList(currentInputDevices());
	}
	else {
		refreshDropdownList(currentOutputDevices());
	}
	// Need to call this to calculate minimum and maximum value. Smell!
	setLookup(lookup_);

	if (startTimerToCheckDeviceListUpdates) {
		// Start a timer checking for plugged in devices
		startTimer(500);
	}
}

void MidiDevicePropertyEditor::timerCallback()
{
	auto newDeviceVector = currentOutputDevices();
	if (inputInsteadOfOutput_) {
		newDeviceVector = currentInputDevices();
	}
	std::set<std::string> newDeviceSet(newDeviceVector.begin(), newDeviceVector.end());
	if (devices_ != newDeviceSet) {
		// List changed, refresh dropdown box
		refreshDropdownList(newDeviceVector);
		sendChangeMessage();
	}
}

void MidiDevicePropertyEditor::refreshDropdownList(std::vector<std::string> const &deviceList) {
	int i = 0;
	lookup_.clear();
	for (const auto &device : deviceList) {
		lookup_[++i] = device;
	}
	// Need to call this to calculate minimum and maximum value. Smell!
	setLookup(lookup_);
	devices_ = std::set<std::string>(deviceList.begin(), deviceList.end());
}
