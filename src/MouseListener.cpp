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

#include <math.h>
#include "xleapmouse.h"
#include "trivconfig.h"

using namespace std;
using namespace Leap;

// ---- Public

MouseListener::MouseListener()
    : _debug(cerr), _osd(cout) {

    // Tracking
    conf.track_finger        = Finger::TYPE_MIDDLE;
    conf.distance_cutoff     = 0.10;
    conf.base_scale          = 10.0;
    conf.scale_factor        = 30.0;
    conf.small_motion_cutoff = 0.2;

    _blank_frame       = true;
    _lastx             = 0.0;
    _lasty             = 0.0;
    _xaccum            = 0.0;
    _yaccum            = 0.0;
    _tracking          = true;

    // Clicking
    conf.lclick_fingers.push_back(Finger::TYPE_INDEX);
    conf.lclick_fingers.push_back(Finger::TYPE_RING);
    conf.rclick_fingers.push_back(Finger::TYPE_THUMB);
    conf.keytap_mindistance     = 4.0;
    conf.keytap_historyseconds  = 0.1;
    conf.keytap_mindownvelocity = 1.0;

    // Scrolling
    conf.scroll_finger = Finger::TYPE_INDEX;
    conf.scroll_reverse = false;
    conf.scroll_radius = 1.0;
    conf.scroll_delay = 0.2;
    _scrolling = false;
    _scroll_last = 0.0;

    // Tapping
    conf.tap_finger = Finger::TYPE_PINKY;
    conf.screentap_minforwardvelocity = 30.0;
    conf.screentap_historyseconds     = 0.1;
    conf.screentap_mindistance        = 1.0;

    conf.update_leap_config = false;
}

void MouseListener::updateLeapConfig(Config config) {
    conf.update_leap_config = false;

    config.setFloat("Gesture.KeyTap.MinDistance", conf.keytap_mindistance);
    config.setFloat("Gesture.KeyTap.HistorySeconds", conf.keytap_historyseconds);
    config.setFloat("Gesture.KeyTap.MinDownVelocity", conf.keytap_mindownvelocity);

    config.setFloat("Gesture.ScreenTap.MinForwardVelocity", conf.screentap_minforwardvelocity);
    config.setFloat("Gesture.ScreenTap.HistorySeconds", conf.screentap_historyseconds);
    config.setFloat("Gesture.ScreenTap.MinDistance", conf.screentap_mindistance);

    config.save();
}

void MouseListener::onInit(const Controller &c) {
    _dpy = XOpenDisplay(0);

    updateLeapConfig(c.config());

    c.enableGesture(Gesture::TYPE_KEY_TAP);
    c.enableGesture(Gesture::TYPE_CIRCLE);
    c.enableGesture(Gesture::TYPE_SCREEN_TAP);
}

void MouseListener::onFrame(const Controller &c) {
    Frame frame = c.frame();
    Hand hand = frame.hands().rightmost();

    if(conf.update_leap_config)
        updateLeapConfig(c.config());

    doGestures(frame);
    doMotion(hand.fingers());
}

void MouseListener::doMotion(FingerList fingers) {
    if(!_tracking) return;

    fingers.extended();
    fingers.fingerType(conf.track_finger);

    if(fingers.isEmpty()) return;

    Finger finger = fingers[0];
    float distance = getDistance(finger);

    if(distance < conf.distance_cutoff)
        _blank_frame = true;
    else if(distance >= conf.distance_cutoff && finger.touchZone() > 0) {
        Vector xys = getXYScale(finger, distance);

        if(_blank_frame)
            reacquire(xys);
        else
            move(xys);
    }
}

void MouseListener::doGestures(Frame frame) {
    GestureList gl = frame.gestures();

    for(GestureList::const_iterator i = gl.begin(); i != gl.end(); i++) {
        Gesture g = *i;

        if(!g.isValid()) continue;

        switch(g.type()) {
            case Gesture::TYPE_KEY_TAP:
                doGesture(frame, KeyTapGesture(g));
                break;

            case Gesture::TYPE_CIRCLE:
                doGesture(frame, CircleGesture(g));
                break;

            case Gesture::TYPE_SCREEN_TAP:
                doGesture(frame, ScreenTapGesture(g));
                break;

            case Gesture::TYPE_SWIPE:
            case Gesture::TYPE_INVALID:
                break;
        }
    }
}

void MouseListener::doGesture(Frame frame __attribute__((unused)),
                              KeyTapGesture kt) {
    if(_scrolling || !_tracking) return;
    if(!kt.pointable().isFinger()) return;

    Finger f = Finger(kt.pointable());

    if(conf.lclick_fingers.has(f.type()))      click(1);
    else if(conf.rclick_fingers.has(f.type())) click(3);
}

void MouseListener::doGesture(Frame frame, CircleGesture cg) {
    FingerList fl = frame.fingers().extended();

    if(fl.isEmpty()) return;

    if(fl.count() < 3 && !fl.fingerType(conf.scroll_finger).isEmpty()) {
        if(cg.state() == CircleGesture::STATE_STOP) {
            scrollEnd();
            return;
        }

        if(!_tracking) return;

        _scrolling = true;

        if(cg.progress() > _scroll_last + conf.scroll_delay) {
            ScrollDirection d = (cg.normal()[0] > 0.0) ? SCROLL_UP : SCROLL_DOWN;
            
            if(conf.scroll_reverse)
                d = (ScrollDirection)!(int)d;

            scroll(d);
            _scroll_last = cg.progress();
        }
    } else if(_scrolling) {
        scrollEnd();
    }
}

void MouseListener::doGesture(Frame frame, ScreenTapGesture st) {
    if(frame.fingers().extended().count() > 1 || !st.pointable().isFinger())
        return;

    Finger f = Finger(st.pointable());

    if(f.type() == conf.tap_finger)
        toggleTracking();
}

// ---- Private

void MouseListener::move(int x, int y) {
    XLockDisplay(_dpy);
    XTestFakeRelativeMotionEvent(_dpy, x, y, CurrentTime);
    XFlush(_dpy);
    XUnlockDisplay(_dpy);
}

void MouseListener::click(unsigned int button) {
    XLockDisplay(_dpy);
    XTestFakeButtonEvent(_dpy, button, True, CurrentTime);
    XTestFakeButtonEvent(_dpy, button, False, CurrentTime);
    XFlush(_dpy);
    XUnlockDisplay(_dpy);
}

void MouseListener::move(Vector xys) {
    float sx = xys.x;
    float sy = xys.y;
    float scale = xys.z;

    float dx = 0.0, dy = 0.0;

    if(scale < 0.0) {
        scale = -scale;
        dx = _xaccum + (sx - _lastx) / scale;
        dy = _yaccum + (sy - _lasty) / scale;
    } else {
        dx = _xaccum + (sx - _lastx) * scale;
        dy = _yaccum + (sy - _lasty) * scale;
    }

    if(dx < 1.0 && dx > -1.0 && fabs(dx) >= conf.small_motion_cutoff)
        _xaccum = dx;
    else
        _xaccum = 0.0;

    if(dy < 1.0 && dy > -1.0 && fabs(dy) >= conf.small_motion_cutoff)
        _yaccum = dy;
    else
        _yaccum = 0.0;

    move(roundf(dx), roundf(dy));
    setLast(sx, sy);
}

void MouseListener::scroll(ScrollDirection d) {
    switch(d) {
        case SCROLL_UP:     click(4); break;
        case SCROLL_DOWN:   click(5); break;
    }
}

void MouseListener::scrollEnd() {
    _scrolling = false;
    _scroll_last = 0.0;
    _blank_frame = true;
}

void MouseListener::setLast(float x, float y) {
    _lastx = x;
    _lasty = y;
}

void MouseListener::reacquire(Vector xys) {
    setLast(xys.x, xys.y);
    _blank_frame = false;
}

float MouseListener::getDistance(Finger finger) {
    // Normalize (100 .. -100) to (0.0 .. 1.0)
    return (-finger.tipPosition().z + 100.0) / 200.0;
}

Vector MouseListener::getXYScale(Finger finger, float distance) {
    Vector stable = finger.stabilizedTipPosition();
    float scale = conf.base_scale + (distance * conf.scale_factor);

    return Vector(stable.x, -stable.y, scale);
}

void MouseListener::toggleTracking() {
    _tracking = !_tracking;
    setStatus(_tracking ? ON : OFF);
}
