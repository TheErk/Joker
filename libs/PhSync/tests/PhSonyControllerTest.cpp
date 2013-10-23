#include <QtMath>

#include "PhSonyControllerTest.h"

#include "PhSonyController.h"

#include "PhTools/PhDebug.h"

void PhSonyControllerTest::testComputeRate()
{
	QVERIFY(qAbs(PhSonyController::computeRate(0)) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(32) - 0.1) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(64) - 1.0) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(79) - 2.94) < 0.01);
	QVERIFY(qAbs(PhSonyController::computeRate(118) - 48.69) < 0.01);
}
