/*
   Copyright (c) 2019 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

// Implement this function for your data type, effectively providing the descriptive text for the column of the data item
template <typename I>
void visit(I const &dataItem, int column, std::function<void(std::string const &)> visitor);

template <class T>
class SimpleTable : public Component, public TableListBoxModel {
public:

	SimpleTable(std::vector<std::string> const &columnHeader, T const &data, std::function<void(int)> rowSelectedHandler) : items_(data), rowSelectedHandler_(rowSelectedHandler) {
		addAndMakeVisible(table_);

		numColumns_ = 1;
		for (const auto& column : columnHeader) {
			table_.getHeader().addColumn(column,
				numColumns_++,
				200,
				50,
				-1);
		}

		table_.setColour(ListBox::outlineColourId, Colours::grey);
		table_.setOutlineThickness(1);
		table_.setModel(this);
	}

	// Update data
	void updateData(T const &newData) {
		items_ = newData;
		MessageManager::callAsync([this]() {
			table_.updateContent();
			table_.autoSizeAllColumns();
			table_.repaint();
		});
		
	}

	// Implementing the TableListBoxModel
	virtual int getNumRows() override {
		return (int) items_.size();
	}

	virtual void paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected) override {
		auto alternateColour = getLookAndFeel().findColour(ListBox::backgroundColourId).interpolatedWith(getLookAndFeel().findColour(ListBox::textColourId), 0.03f);
		if (rowIsSelected) {
			g.fillAll(Colours::lightblue);
		}
		else if (rowNumber % 2) {
			g.fillAll(alternateColour);
		}
	}

	virtual void paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override {
		g.setColour(rowIsSelected ? Colours::darkblue : getLookAndFeel().findColour(ListBox::textColourId)); 
		//g.setFont(font);

		visit(items_[rowNumber], columnId, [&](std::string const &text) {
			g.drawText(text, 2, 0, width - 4, height, Justification::centredLeft, true);
		});

		// Draw separator line
		g.setColour(getLookAndFeel().findColour(ListBox::backgroundColourId));
		g.fillRect(width - 1, 0, 1, height);
	}

	virtual void selectedRowsChanged(int lastRowSelected) override {
		rowSelectedHandler_(lastRowSelected);
	}

	void resized() override
	{
		table_.setBoundsInset(BorderSize<int>(8));
	}

	int getColumnAutoSizeWidth(int columnId) override
	{
		Font defaultFont;
		int widest = defaultFont.getStringWidth(table_.getHeader().getColumnName(columnId));
		for (auto rowNumber = getNumRows(); --rowNumber >= 0;)
		{
			visit(items_[rowNumber], columnId, [&](std::string const &text) {
				widest = jmax(widest, defaultFont.getStringWidth(text));
			});
		}
		return widest + 8;
	}


private:
	TableListBox table_;
	T items_;
	int numColumns_;
	std::function<void(int)> rowSelectedHandler_;
};

