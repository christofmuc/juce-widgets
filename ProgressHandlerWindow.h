/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#pragma once

#include "JuceHeader.h"

#include "ProgressHandler.h"

class ProgressHandlerWindow : public ThreadWithProgressWindow, public ProgressHandler
{
public:
	ProgressHandlerWindow(std::string const &title, std::string const &message);

	void run() override;

	bool shouldAbort() const override;
	void setProgressPercentage(double zeroToOne) override;
	void setMessage(std::string const &title) override;
	void onSuccess() override;
	void onCancel() override;
};
