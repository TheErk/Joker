#ifndef FORMTESTSETTINGS_H
#define FORMTESTSETTINGS_H

#include <QDir>
#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class FormTestSettings : protected PhGenericSettings, public PhWindowSettings
{
public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)
};

#endif // FORMTESTSETTINGS_H
