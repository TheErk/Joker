/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"
#include "PhGraphic/PhGraphicView.h"

#include "PhVideo/PhVideoEngine.h"

#include "VideoSpecSettings.h"

#define FRAME_WAIT_TIME 40

#include "PhSpec.h"

using namespace bandit;

go_bandit([](){
	describe("video_test", [](){
		PhGraphicView *view;
		PhVideoEngine *engine;
		VideoSpecSettings settings;

		before_each([&](){
			PhDebug::disable();

			view = new PhGraphicView(64, 64);
			engine = new PhVideoEngine(&settings);

			view->show();

			engine->setBilinearFiltering(false);

			QObject::connect(view, &PhGraphicView::paint, [&](int w, int h) {
				engine->drawVideo(0, 0, w, h);
			});
		});

		after_each([&](){
			engine->close();

			delete engine;
			delete view;
		});

		it("open_video", [&](){
			AssertThat(engine->open("video/interlace_%03d.bmp"), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);
		});

		it("default_framerate", [&](){
			AssertThat(engine->open("video/interlace_%03d.bmp"), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(engine->framePerSecond(), Equals(25.00f));
		});

		it("go_to_01", [&](){
			AssertThat(engine->open("video/interlace_%03d.bmp"), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);
			QImage result = view->renderPixmap(64, 64).toImage();
			result.save("result.bmp");
			AssertThat(result == QImage("video/interlace_000.bmp"), IsTrue());

			engine->clock()->setFrame(20, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_020.bmp"), IsTrue());

			engine->clock()->setFrame(100, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_100.bmp"), IsTrue());

			engine->clock()->setFrame(75, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_075.bmp"), IsTrue());
		});

		it("go_to_02", [&](){
			AssertThat(engine->open("video/interlace_%03d.bmp"), IsTrue());

			engine->clock()->setFrame(100, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_100.bmp"), IsTrue());

			engine->clock()->setFrame(99, PhTimeCodeType25);

			PHDEBUG << "second paint";

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_099.bmp"), IsTrue());

			for(int i = 75; i >= 50; i--) {
				engine->clock()->setFrame(i, PhTimeCodeType25);

				qDebug() << "Set frame :" << i;

				QThread::msleep(FRAME_WAIT_TIME);
				QString name = QString("video/interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
				AssertThat(view->renderPixmap(64, 64).toImage() == QImage(name), IsTrue());
			}
		});

		// This "stress test" cue the video engine at different random location
		it("go_to_03", [&](){
			AssertThat(engine->open("video/interlace_%03d.bmp"), IsTrue());

			QList<PhFrame> list = QList<PhFrame>() << 183 << 25 << 71 << 59 << 158 << 8 << 137
												   << 32 << 37 << 53 << 133 << 108 << 166 << 134
												   << 110 << 42 << 54 << 107 << 133 << 100 << 169
												   << 70 << 22 << 12 << 176 << 159 << 180 << 55
												   << 113 << 89 << 48 << 187 << 16 << 124 << 153
												   << 90 << 100 << 68 << 96 << 51 << 146 << 154
												   << 115 << 165 << 85 << 83 << 181 << 57 << 86 << 166;


			for(int i = 0; i < list.count(); i++) {
				PhFrame frame = list[i];
				engine->clock()->setFrame(frame, PhTimeCodeType25);

				QThread::msleep(FRAME_WAIT_TIME);
				QString name = QString("video/interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
				AssertThat(view->renderPixmap(64, 64).toImage() == QImage(name), IsTrue());
			}
		});

		it("play", [&](){
			AssertThat(engine->open("video/interlace_%03d.bmp"), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_000.bmp"), IsTrue());

			engine->clock()->setRate(1);
			engine->clock()->elapse(960); // 1 frame at 25 fps

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_001.bmp"), IsTrue());


			// Play 1 second
			for(int i = 0; i < 25; i++) {
				engine->clock()->elapse(960); // 1 frame at 25 fps
				QThread::msleep(FRAME_WAIT_TIME);
			}

			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_026.bmp"), IsTrue());

			engine->clock()->setRate(-1);
			engine->clock()->elapse(960); // 1 frame at 25 fps
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_025.bmp"), IsTrue());

			// Play 1 second
			for(int i = 24; i >= 0; i--) {
				engine->clock()->elapse(960); // 1 frame at 25 fps
				QThread::msleep(FRAME_WAIT_TIME);
			}
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_000.bmp"), IsTrue());
		});

		it("deinterlace", [&](){
			//Open the video file in interlaced mode
			engine->open("video/interlace_%03d.bmp");
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_000.bmp"), IsTrue());

			//Change mode to deinterlaced
			engine->setDeinterlace(true);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/deinterlace_000.bmp"), IsTrue());

			//Move one picture forward
			engine->clock()->setFrame(1, PhTimeCodeType25);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/deinterlace_001.bmp"), IsTrue());

			//Go back to interlaced mode
			engine->setDeinterlace(false);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("video/interlace_001.bmp"), IsTrue());
		});

		//		it("saveBuffer(QString fileName) {
		//		   QImage test = view->renderPixmap(64, 64).toImage();
		//		test.save(fileName);
		//		system(PHNQ(QString("open %0").arg(fileName)));
		//	}

		//	it("findMatch(QImage source", [&](){
		//	   for(int i = 0; i < 200; i++) {
		//		if(QImage(QString("video/interlace_%1.bmp").arg(i, 3, 10, QChar('0'))) == source) {
		//			PHDEBUG << "The matching frame is" << QString("video/interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
		//			break;
		//		}
		//	}
		//	for(int i = 0; i < 200; i++) {
		//		if(QImage(QString("deinterlace_%1.bmp").arg(i, 3, 10, QChar('0'))) == source) {
		//			PHDEBUG << "The matching frame is" << QString("deinterlace_%1.bmp").arg(i, 3, 10, QChar('0'));
		//			break;
		//		}
		//	}
	});
});
