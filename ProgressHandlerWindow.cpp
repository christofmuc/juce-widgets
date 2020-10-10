/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "ProgressHandlerWindow.h"

ProgressHandlerWindow::ProgressHandlerWindow(std::string const &title, std::string const &message)
	: ThreadWithProgressWindow(title, true, true)
{
	setMessage(message);
}

void ProgressHandlerWindow::run()
{
	while (!threadShouldExit()) {
		Thread::sleep(100);
	}
}

bool ProgressHandlerWindow::shouldAbort() const
{
	return threadShouldExit();
}

void ProgressHandlerWindow::setProgressPercentage(double zeroToOne)
{
	setProgress(zeroToOne);
}

void ProgressHandlerWindow::setMessage(std::string const &title)
{
	setStatusMessage(title);
}

void ProgressHandlerWindow::onSuccess()
{
}

void ProgressHandlerWindow::onCancel()
{
	signalThreadShouldExit();
}

