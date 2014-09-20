/*
  Copyright © 2014, Ryan Pavlik
  This file is part of xleapmouse.

  xleapmouse is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  xleapmouse is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with xleapmouse.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>
#include "xlmgui.h"

#include <iostream>
#include "xleapmouse.h"

using namespace std;

QApplication *APP = NULL;
XLMConfig *WIN = NULL;

QEvent::Type StatusEventType = (QEvent::Type)0;

void uiLoop(MouseListener &mouse) {
    Q_INIT_RESOURCE(xlm);

    int qtargc = 0;
    APP = new QApplication(qtargc, NULL);

    StatusEventType = (QEvent::Type)QEvent::registerEventType();

    WIN = new XLMConfig(mouse);
    WIN->installEventFilter(new XLMFilter);

    APP->exec();
}

void setStatus(TrackStatus s) {
    QCoreApplication::postEvent(WIN, new StatusEvent(s));
}

XLMConfig::XLMConfig(MouseListener &mouse)
    : _mouse(mouse)
{
    setupUi(this);

    // -- Tray Icon
    _icon = new QIcon(":/res/xleapmouse.png");
    _sleep_icon = new QIcon(":/res/xsleepmouse.png");

    _trayicon = new QSystemTrayIcon(this);
    _trayicon->setIcon(*_icon);
    _trayicon->show();

    setWindowIcon(*_icon);

    // -- Tray Icon Menu;
    _menu = new QMenu(this);
    QAction *settings = _menu->addAction("Settings...");
    connect(settings, SIGNAL(triggered()), this, SLOT(show()));

    _menu->addSeparator();

    QAction *exit = _menu->addAction("Exit");
    connect(exit, SIGNAL(triggered()), APP, SLOT(quit()));

    _trayicon->setContextMenu(_menu);

    // -- Config
    _tmpconfig = _mouse.conf;

    // -- Forms
    setTrackingPage();
    fixClickingButtons(); // Qt Designer blows
    setClickingPage();
    setScrollingPage();

    QObject::connect(this, &XLMConfig::applied, this, &XLMConfig::apply);
}

void XLMConfig::setTrayIcon(TrackStatus status) {
    switch(status) {
        case OFF:
            _trayicon->setIcon(*_sleep_icon);
            _trayicon->showMessage("Status", "Tracking Off",
                                   QSystemTrayIcon::Information,
                                   3000);
            break;

        case ON:
        default:
            _trayicon->setIcon(*_icon);
            _trayicon->showMessage("Status", "Tracking On",
                                   QSystemTrayIcon::Information,
                                   3000);
            break;
    }
}

bool XLMFilter::eventFilter(QObject *obj, QEvent *qev) {
    if(obj != WIN) goto _default;

    if(qev->type() == StatusEventType) {
        WIN->setTrayIcon(static_cast<StatusEvent*>(qev)->status());

        goto _default;
    }

    return false;

 _default:
    return true;
}
