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

#include "xlmgui.h"

#include <iostream>
#include "xleapmouse.h"

using namespace std;

void XLMConfig::on_okcancel_clicked(QAbstractButton *b) {
    switch(okcancel->buttonRole(b)) {
        case QDialogButtonBox::ApplyRole:
            emit applied();

        default:
            break;
    }
}

void XLMConfig::accept() {
    _mouse.conf = _tmpconfig;
    _mouse.conf.update_leap_config = true;
    _mouse.conf.writeDefault();

    done(QDialog::Accepted);
}

void XLMConfig::reject() {
    _tmpconfig = _mouse.conf;
    setTrackingPage();
    setClickingPage();
    setScrollingPage();

    done(QDialog::Rejected);
}

void XLMConfig::apply() {
    _mouse.conf = _tmpconfig;
    _mouse.conf.update_leap_config = true;
    _mouse.conf.writeDefault();
}

void XLMConfig::done(int r __attribute__((unused))) {
    hide();
}

void XLMConfig::closeEvent(QCloseEvent *ev) {
    hide();
    ev->ignore();
}

// ------- Tracking Page
void XLMConfig::on_tracking_finger_combo_currentIndexChanged(int v) {
    _tmpconfig.track_finger = (Finger::Type)v;
}

void XLMConfig::on_cutoff_spinbox_valueChanged(double v) {
    _tmpconfig.distance_cutoff = v;
}

void XLMConfig::on_base_spinbox_valueChanged(double v) {
    _tmpconfig.base_scale = v;
}

void XLMConfig::on_mult_spinbox_valueChanged(double v) {
    _tmpconfig.scale_factor = v;
}

void XLMConfig::on_small_motion_spinbox_valueChanged(double v) {
    _tmpconfig.small_motion_cutoff = v;
}

void XLMConfig::on_st_minforward_valueChanged(double v) {
    _tmpconfig.screentap_minforwardvelocity = v;
}

void XLMConfig::on_st_hist_valueChanged(double v) {
    _tmpconfig.screentap_historyseconds = v;
}

void XLMConfig::on_st_mindist_valueChanged(double v) {
    _tmpconfig.screentap_mindistance = v;
}

// ------- Clicking Page
void XLMConfig::on_thumb_bg_buttonClicked(int b) {
    _tmpconfig.setClickFinger(Finger::TYPE_THUMB, b);
}

void XLMConfig::on_index_bg_buttonClicked(int b) {
    _tmpconfig.setClickFinger(Finger::TYPE_INDEX, b);
}

void XLMConfig::on_middle_bg_buttonClicked(int b) {
    _tmpconfig.setClickFinger(Finger::TYPE_MIDDLE, b);
}

void XLMConfig::on_ring_bg_buttonClicked(int b) {
    _tmpconfig.setClickFinger(Finger::TYPE_RING, b);
}

void XLMConfig::on_pinky_bg_buttonClicked(int b) {
    _tmpconfig.setClickFinger(Finger::TYPE_PINKY, b);
}

void XLMConfig::on_kt_mindist_valueChanged(double v) {
    _tmpconfig.keytap_mindistance = v;
}

void XLMConfig::on_kt_hist_valueChanged(double v) {
    _tmpconfig.keytap_historyseconds = v;
}

void XLMConfig::on_kt_minvel_valueChanged(double v) {
    _tmpconfig.keytap_mindownvelocity = v;
}

// ------- Scrolling Page
void XLMConfig::on_scroll_finger_combo_currentIndexChanged(int v) {
    _tmpconfig.scroll_finger = (Finger::Type)v;
}

void XLMConfig::on_revscroll_checkbox_clicked() {
    _tmpconfig.scroll_reverse = (bool)revscroll_checkbox->checkState();
}

void XLMConfig::on_scrolldelay_spinbox_valueChanged(double v) {
    _tmpconfig.scroll_delay = v;
}

void XLMConfig::on_scrollradius_spinbox_valueChanged(double v) {
    _tmpconfig.scroll_radius = v;
}

// ------- Setup and Config
void XLMConfig::setTrackingPage() {
    MouseConfig &c = _mouse.conf;

    tracking_finger_combo->setCurrentIndex((int)c.track_finger);
    toggle_finger_combo->setCurrentIndex((int)c.tap_finger);
    cutoff_spinbox->setValue(c.distance_cutoff);
    base_spinbox->setValue(c.base_scale);
    mult_spinbox->setValue(c.scale_factor);
    small_motion_spinbox->setValue(c.small_motion_cutoff);

    st_minforward->setValue(c.screentap_minforwardvelocity);
    st_hist->setValue(c.screentap_historyseconds);
    st_mindist->setValue(c.screentap_mindistance);
}

void XLMConfig::setClickingPage() {
    MouseConfig &c = _mouse.conf;

    thumb_bg->button(c.getFingerButton(Finger::TYPE_THUMB))->setChecked(true);
    index_bg->button(c.getFingerButton(Finger::TYPE_INDEX))->setChecked(true);
    middle_bg->button(c.getFingerButton(Finger::TYPE_MIDDLE))->setChecked(true);
    ring_bg->button(c.getFingerButton(Finger::TYPE_RING))->setChecked(true);
    pinky_bg->button(c.getFingerButton(Finger::TYPE_PINKY))->setChecked(true);

    kt_mindist->setValue(c.keytap_mindistance);
    kt_hist->setValue(c.keytap_historyseconds);
    kt_minvel->setValue(c.keytap_mindownvelocity);
}

void XLMConfig::setScrollingPage() {
    MouseConfig &c = _mouse.conf;

    scroll_finger_combo->setCurrentIndex((int)c.scroll_finger);
    revscroll_checkbox->setChecked(c.scroll_reverse);
    scrolldelay_spinbox->setValue(c.scroll_delay);
    scrollradius_spinbox->setValue(c.scroll_radius);
}

// This is so stupid.
void fixOneGroup(QButtonGroup *g) {
    QList<QAbstractButton*> list = g->buttons();

    for(QList<QAbstractButton*>::iterator i = list.begin();
        i != list.end(); ++i) {
        QAbstractButton *b = (*i);
        if(b->text() == "None")  g->setId(b, 0);
        if(b->text() == "Left")  g->setId(b, 1);
        if(b->text() == "Mid")   g->setId(b, 2);
        if(b->text() == "Right") g->setId(b, 3);
    }
}

void XLMConfig::fixClickingButtons() {
    fixOneGroup(thumb_bg);
    fixOneGroup(index_bg);
    fixOneGroup(middle_bg);
    fixOneGroup(ring_bg);
    fixOneGroup(pinky_bg);
}
