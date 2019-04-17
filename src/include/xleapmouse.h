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

#ifndef XLEAPMOUSE_H
#define XLEAPMOUSE_H

#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include <Leap.h>

using namespace Leap;

class FingerTypeList : public std::list<Finger::Type> {
public:
    bool has(Finger::Type t) {
        return find(this->begin(), this->end(), t) != this->end();
    }
};

enum ScrollDirection { SCROLL_UP, SCROLL_DOWN };
enum TrackStatus { OFF, ON };

struct MouseConfig {
    // Tracking
    Finger::Type track_finger;
    float distance_cutoff;
    float base_scale, scale_factor;
    float small_motion_cutoff;

    // Clicking
    FingerTypeList lclick_fingers;
    FingerTypeList mclick_fingers;
    FingerTypeList rclick_fingers;
    float keytap_mindistance,
        keytap_historyseconds,
        keytap_mindownvelocity;

    // Scrolling
    Finger::Type scroll_finger;
    bool scroll_reverse;
    float scroll_radius, scroll_delay;

    // Tapping
    Finger::Type tap_finger;
    float screentap_minforwardvelocity,
        screentap_historyseconds,
        screentap_mindistance;

    // Update config flag
    bool update_leap_config;

    void setClickFinger(Finger::Type f, int button) {
        lclick_fingers.remove(f);
        mclick_fingers.remove(f);
        rclick_fingers.remove(f);

        switch(button) {
            case 1: lclick_fingers.push_front(f); break;
            case 2: mclick_fingers.push_front(f); break;
            case 3: rclick_fingers.push_front(f); break;
        }
    }

    int getFingerButton(Finger::Type f) {
        if(lclick_fingers.has(f)) return 1;
        if(mclick_fingers.has(f)) return 2;
        if(rclick_fingers.has(f)) return 3;
        return 0;
    }

    void write(const char *filename);
    void read(const char *filename);

    bool getDefaultPath(std::string &s);
    void writeDefault();
    void readDefault();

    void parseOpts(int argc, char **argv);
};

class MouseListener : public Listener {
    Display *_dpy;
    std::ostream &_debug;
    std::ostream &_osd;

    // ---- Members

    // Tracking
    bool _blank_frame;
    float _lastx, _lasty;
    float _xaccum, _yaccum;
    bool _tracking;
    uint64_t _track_start, _track_delay;

    // Scrolling
    float _scroll_last;
    bool _scrolling;

    // ---- Methods
    // Cursor
    void move(int x, int y);
    void move(Vector xys);

    void click(unsigned int button);

    void scroll(ScrollDirection d);
    void scrollEnd(Frame frame);

    // Internal
    void setLast(float x, float y);
    void reacquire(Vector xys);

    float getDistance(Finger finger);
    Vector getXYScale(Finger finger, float distance);

    void doMotion(Frame &frame, FingerList fingers);
    void trackDelayStart(Frame &frame);

    void doGestures(Frame);
    void doGesture(Frame, CircleGesture);
    void doGesture(Frame, KeyTapGesture);
    void doGesture(Frame, ScreenTapGesture);

    void toggleTracking();

    void updateLeapConfig(Config config);

public:
    MouseListener();
    virtual ~MouseListener() { }

    virtual void onInit(const Controller&);
    virtual void onFrame(const Controller&);

    MouseConfig conf;
};

/* UI bridge */
void uiLoop(MouseListener&);
void setStatus(TrackStatus);

#endif /* XLEAPMOUSE_H */
