# $Id: Makefile.am,v 1.1.1.1 2002/04/29 03:19:39 rpav Exp $

include $(top_srcdir)/make-extras

nodistdir = .

noinst_HEADERS = xleapmouse.h trivconfig.h
nodist_HEADERS =
BUILT_SOURCES =

if WITH_QT
noinst_HEADERS += xlmgui.h
nodist_HEADERS += uic_xlmconfig.h
BUILT_SOURCES += uic_xlmconfig.h
endif

uic_xlmconfig.h: ../res/xlm.ui
	$(UIC) -o $@ $<
