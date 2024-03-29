/*
 * MIT License
 *
 * Copyright (c) 2019-2023 Christof Ruch
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

#include "MidiChannelPropertyEditor.h"

#include <juce_audio_devices/juce_audio_devices.h>

std::map<int, std::string> sInputLookup;
std::map<int, std::string> sOutputLookup;

std::map<int, std::string> sMidiChannelLookup = {
    { 1, "1" },
    { 2, "2" },
    { 3, "3" },
    { 4, "4" },
    { 5, "5" },
    { 6, "6" },
    { 7, "7" },
    { 8, "8" },
    { 9, "9" },
    { 10, "10" },
    { 11, "11" },
    { 12, "12" },
    { 13, "13" },
    { 14, "14" },
    { 15, "15" },
    { 16, "16" },
    { 17, "Omni" },
    { 18, "Invalid" },
};

MidiChannelPropertyEditor::MidiChannelPropertyEditor(std::string const &title, std::string const &sn) :
    TypedNamedValue(title, sn, 18, sMidiChannelLookup)
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

MidiDevicePropertyEditor::MidiDevicePropertyEditor(std::string const &title, std::string const &sn, bool inputInsteadOfOutput) :
    TypedNamedValue(title, sn, 0, std::map<int, std::string>()), inputInsteadOfOutput_(inputInsteadOfOutput)
{
    value_ = juce::Value(1);
    refreshDeviceList();
}

void MidiDevicePropertyEditor::refreshDeviceList()
{
    if (inputInsteadOfOutput_) {
        refreshDropdownList(juce::MidiInput::getAvailableDevices());
    }
    else {
        refreshDropdownList(juce::MidiOutput::getAvailableDevices());
    }
    // Need to call this to calculate minimum and maximum value. Smell!
    setLookup(lookup_);
}

void MidiDevicePropertyEditor::refreshDropdownList(juce::Array<juce::MidiDeviceInfo> deviceList)
{
    int i = 0;
    lookup_.clear();
    identifierPerRow_.clear();
    for (const auto &device : deviceList) {
        lookup_[++i] = device.name.toStdString();
        identifierPerRow_[i] = device.identifier;
        devices_[device.identifier] = device;
    }
    // Need to call this to calculate minimum and maximum value. Smell!
    setLookup(lookup_);
}

juce::MidiDeviceInfo MidiDevicePropertyEditor::selectedDevice() const
{
    int rowSelected = int(value_.getValue());
    auto id = identifierPerRow_.find(rowSelected);
    juce::String selected = id->second;
    return devices_.find(selected)->second;
}

void MidiDevicePropertyEditor::setSelectedDevice(juce::MidiDeviceInfo const &device)
{
    for (auto row : identifierPerRow_) {
        if (row.second == device.identifier) {
            value() = row.first;
            return;
        }
    }
    jassertfalse;
}
