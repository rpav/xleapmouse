/* -*- C++ -*-
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

#ifndef XLMGUI_H
#define XLMGUI_H

#include <QtCore/QEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QIcon>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSystemTrayIcon>
#include <QtWidgets/QMenu>

#include "uic_xlmconfig.h"
#include "xleapmouse.h"

class XLMConfig : public QDialog, private Ui_BaseXLMConfig {
    Q_OBJECT;

    QIcon *_icon, *_sleep_icon;
    QSystemTrayIcon *_trayicon;
    QMenu *_menu;

    MouseListener &_mouse;
    MouseConfig _tmpconfig;

    void setTrackingPage();
    void setClickingPage();
    void setScrollingPage();

    void fixClickingButtons();

protected:
    virtual void closeEvent(QCloseEvent*);

protected slots:
    // OkCancel
    void on_okcancel_clicked(QAbstractButton*);

    // Tracking
    void on_tracking_finger_combo_currentIndexChanged(int);
    void on_cutoff_spinbox_valueChanged(double);
    void on_base_spinbox_valueChanged(double);
    void on_mult_spinbox_valueChanged(double);
    void on_small_motion_spinbox_valueChanged(double);
    void on_st_minforward_valueChanged(double);
    void on_st_hist_valueChanged(double);
    void on_st_mindist_valueChanged(double);

    // Clicking
    void on_thumb_bg_buttonClicked(int);
    void on_index_bg_buttonClicked(int);
    void on_middle_bg_buttonClicked(int);
    void on_ring_bg_buttonClicked(int);
    void on_pinky_bg_buttonClicked(int);
    void on_kt_mindist_valueChanged(double);
    void on_kt_hist_valueChanged(double);
    void on_kt_minvel_valueChanged(double);

    // Scrolling
    void on_scroll_finger_combo_currentIndexChanged(int);
    void on_revscroll_checkbox_clicked();
    void on_scrolldelay_spinbox_valueChanged(double);
    void on_scrollradius_spinbox_valueChanged(double);

signals:
    void applied();

public slots:
    // Dialog
    virtual void accept();
    virtual void reject();
    virtual void apply();
    virtual void done(int r);

public:
    XLMConfig(MouseListener &mouse);

    void setTrayIcon(TrackStatus status);
};

class XLMFilter : public QObject {
protected:
    virtual bool eventFilter(QObject *obj, QEvent *ev);
};

extern QEvent::Type StatusEventType;

class StatusEvent : public QEvent {
    TrackStatus _status;

public:
    StatusEvent(TrackStatus status)
        : QEvent(StatusEventType), _status(status) { }

    TrackStatus status() const { return _status; }
};

#endif /* XLMGUI_H */
