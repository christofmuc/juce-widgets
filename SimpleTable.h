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

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// Implement this function for your data type, effectively providing the descriptive text for the column of the data item
template <typename I> void visit(I const &dataItem, int column, std::function<void(std::string const &)> visitor);

template <class T> class SimpleTable : public juce::Component, public juce::TableListBoxModel {
public:
    SimpleTable(std::vector<std::string> const &columnHeader, T const &data, std::function<void(int)> rowSelectedHandlerParam) :
        rowSelectedHandler(rowSelectedHandlerParam), items_(data)
    {
        addAndMakeVisible(table_);

        numColumns_ = 1;
        for (const auto &column : columnHeader) {
            table_.getHeader().addColumn(column, numColumns_++, 200, 50, -1);
        }

        table_.setColour(ListBox::outlineColourId, Colours::grey);
        table_.setOutlineThickness(1);
        table_.setModel(this);
    }

    // Update data
    void updateData(T const &newData)
    {
        items_ = newData;
        MessageManager::callAsync([this]() {
            table_.updateContent();
            table_.autoSizeAllColumns();
            table_.repaint();
        });
    }

    void selectRow(int rowNum) { table_.selectRow(rowNum); }

    void clearSelection() { table_.deselectAllRows(); }

    // Implementing the TableListBoxModel
    virtual int getNumRows() override { return (int) items_.size(); }

    virtual void paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) override
    {
        ignoreUnused(width, height);
        auto alternateColour = getLookAndFeel()
                                   .findColour(juce::ListBox::backgroundColourId)
                                   .interpolatedWith(getLookAndFeel().findColour(juce::ListBox::textColourId), 0.03f);
        if (rowIsSelected) {
            g.fillAll(juce::Colours::lightblue);
        }
        else if (rowNumber % 2) {
            g.fillAll(alternateColour);
        }
    }

    virtual void paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override
    {
        g.setColour(rowIsSelected ? juce::Colours::darkblue : getLookAndFeel().findColour(juce::ListBox::textColourId));
        // g.setFont(font);

        visit(items_[rowNumber], columnId,
            [&](std::string const &text) { g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true); });

        // Draw separator line
        g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
        g.fillRect(width - 1, 0, 1, height);
    }

    virtual void selectedRowsChanged(int lastRowSelected) override
    {
        if (rowSelectedHandler) {
            rowSelectedHandler(lastRowSelected);
        }
    }

    void resized() override { table_.setBounds(getLocalBounds()); }

    int getColumnAutoSizeWidth(int columnId) override
    {
        juce::Font defaultFont;
        int widest = defaultFont.getStringWidth(table_.getHeader().getColumnName(columnId));
        for (auto rowNumber = getNumRows(); --rowNumber >= 0;) {
            visit(items_[rowNumber], columnId, [&](std::string const &text) { widest = jmax(widest, defaultFont.getStringWidth(text)); });
        }
        return widest + 8;
    }

    std::function<void(int)> rowSelectedHandler;


private:
    juce::TableListBox table_;
    T items_;
    int numColumns_;
};
