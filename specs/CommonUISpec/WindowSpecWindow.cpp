#include <QPlainTextEdit>

#include "PhTools/PhDebug.h"

#include "WindowSpecWindow.h"
#include "ui_WindowSpecWindow.h"

WindowSpecWindow::WindowSpecWindow(PhWindowSettings *settings) :
	PhWindow(settings),
	ui(new Ui::WindowSpecWindow)
{
	ui->setupUi(this);

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));
}

WindowSpecWindow::~WindowSpecWindow()
{
	delete ui;
}

QAction *WindowSpecWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}
