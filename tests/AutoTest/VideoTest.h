/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOTEST_H
#define VIDEOTEST_H

#include <QObject>

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"
#include "VideoTestSettings.h"

class VideoTest : public PhGraphicView
{
	Q_OBJECT
public:
	explicit VideoTest(QWidget *parent = 0);

private slots:

	void openMovieTest();
	void goToTest();
	void playTest();
	void deinterlaceTest();
private:
	void paint();
	PhVideoEngine _videoEngine;
	VideoTestSettings _settings;

#warning /// @todo move to a generic graphicTest implementation
	void saveBuffer();
};

#endif // VIDEOTEST_H
