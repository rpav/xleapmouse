# $Id: Makefile.am,v 1.1.1.1 2002/04/29 03:19:28 rpav Exp $

include $(top_srcdir)/make-extras

SUBDIRS = include test

bin_PROGRAMS = xleapmouse
xleapmouse_SOURCES = xleapmouse.cpp config.cpp MouseListener.cpp
xleapmouse_CXXFLAGS =
xleapmouse_LDADD = -lX11 -lXtst -lLeap

BUILT_SOURCES =
CLEANFILES =

if WITH_QT
xleapmouse_CXXFLAGS += $(QT_CFLAGS) $(QT_INCLUDE)
xleapmouse_SOURCES  += moc_xlmgui.cpp ui_qt.cpp ui_qt_slots.cpp rcc_xlm.cpp
xleapmouse_LDADD    += $(QT_LIBS) -lQt5Widgets

BUILT_SOURCES       += moc_xlmgui.cpp rcc_xlm.cpp
CLEANFILES          += moc_xlmgui.cpp rcc_xlm.cpp
else
xleapmouse_SOURCES  += ui_none.cpp
endif

moc_xlmgui.cpp: include/xlmgui.h
	$(MOC) $(QT_DEFINES) $(QT_INCLUDES) $< -o $@

rcc_xlm.cpp: res/xlm.qrc \
             res/xleapmouse.png \
             res/xsleepmouse.png
	$(RCC) -root /res -name xlm $< -o $@
