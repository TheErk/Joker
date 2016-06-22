#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	$$PWD/PhMidiObject.h \
	$$PWD/PhMidiInput.h \
	$$PWD/PhMidiOutput.h \
	$$PWD/PhMidiTimeCodeWriter.h \
	$$PWD/PhMidiTimeCodeReader.h

SOURCES += \
	$$PWD/PhMidiObject.cpp \
	$$PWD/PhMidiInput.cpp \
	$$PWD/PhMidiOutput.cpp \
	$$PWD/PhMidiTimeCodeWriter.cpp \
	$$PWD/PhMidiTimeCodeReader.cpp



mac {
INCLUDEPATH += $$TOP_ROOT/vendor/rtmidi
HEADERS += $$TOP_ROOT/vendor/rtmidi/RtMidi.h
SOURCES += $$TOP_ROOT/vendor/rtmidi/RtMidi.cpp
DEFINES += __MACOSX_CORE__
LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
}

linux {
DEFINES += RTMIDI_PRE211_API
LIBS += -lrtmidi
}

win32 {
INCLUDEPATH += $$TOP_ROOT/vendor/rtmidi
HEADERS += $$TOP_ROOT/vendor/rtmidi/RtMidi.h
SOURCES += $$TOP_ROOT/vendor/rtmidi/RtMidi.cpp
DEFINES += __WINDOWS_MM__
LIBS += -lwinmm
}
