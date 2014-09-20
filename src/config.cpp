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

#include <string.h>
#include <getopt.h>
#include "xleapmouse.h"
#include "trivconfig.h"

using namespace std;
using namespace Leap;

void tc_write(ostream &os, FingerTypeList *t) {
    for(FingerTypeList::iterator i = t->begin(); i != t->end(); ++i) {
        if(i != t->begin()) os << " ";
        tc_write(os, (int*)&(*i));
    }
}
void tc_read(istream &is, FingerTypeList *t) {
    t->clear();

    while(is.good()) {
        int i;
        is >> i;

        if(!is.fail())
            t->push_back((Finger::Type)i);
    }
}

class TrivialMouseConfig : public TrivialConfig {
public:
    /* Keep these identical to 'options' below, because this
       is how we parse option values too */
    TrivialMouseConfig(MouseConfig &c) {
        add("track-finger", (int*)&c.track_finger);
        add("distance-cutoff", &c.distance_cutoff);
        add("base-scale", &c.base_scale);
        add("scale-factor", &c.scale_factor);
        add("small-motion-bias", &c.small_motion_bias);

        add("lclick-fingers", &c.lclick_fingers);
        add("mclick-fingers", &c.mclick_fingers);
        add("rclick-fingers", &c.rclick_fingers);
        add("kt-mindist", &c.keytap_mindistance);
        add("kt-hist", &c.keytap_historyseconds);
        add("kt-mindown", &c.keytap_mindownvelocity);

        add("scroll-finger", (int*)&c.scroll_finger);
        add("scroll-reverse", &c.scroll_reverse);
        add("scroll-radius", &c.scroll_radius);
        add("scroll-delay", &c.scroll_delay);

        add("toggle-finger", (int*)&c.tap_finger);
        add("st-minforward", &c.screentap_minforwardvelocity);
        add("st-hist", &c.screentap_historyseconds);
        add("st-mindist", &c.screentap_mindistance);
    }
};

void MouseConfig::write(const char *filename) {
    TrivialMouseConfig c(*this);
    c.write(filename);
}

void MouseConfig::read(const char *filename) {
    TrivialMouseConfig c(*this);
    c.read(filename);
}

bool MouseConfig::getDefaultPath(string &s) {
    char *home = getenv("HOME");

    if(!home || strlen(home) == 0)
        return false;

    s = string(home) + "/.xleapmouserc";
    return true;
}

void MouseConfig::writeDefault() {
    string path;

    if(getDefaultPath(path))
        write(path.c_str());
}

void MouseConfig::readDefault() {
    string path;

    if(getDefaultPath(path))
        read(path.c_str());
}

void usage() {
    cout <<
"Usage: xleapmouse [options...]\n"
"\n"
"  -t, --track-finger N       Finger off which to track motion\n"
"  -T, --toggle-finger N      Finger which toggles tracking (screen tap)\n"
"  -s, --scroll-finger N      Finger which scrolls (circle motion)\n"
"\n"
"  --lclick-fingers LIST      Finger(s) which left-click (default: 1 3)\n"
"  --mclick-fingers LIST      Finger(s) which middle-click (default: none)\n"
"  --rclick-fingers LIST      Finger(s) which right-click (default: 0)\n"
"\n"
"Tracking:\n"
"  --distance-cutoff F        Minimum hand distance from sensor (default: 0.1)\n"
"  --base-scale F             Base speed (default: 10.0)\n"
"  --scale-factor F           Speed multiplier as hand moves toward sensor\n"
"                               (default: 30.0)\n"
"  --small-motion-bias F      Value 0.0-0.3 added to smooth very small motions\n"
"                               (default: 0.2)\n"
"\n"
"Scrolling:\n"
"  --scroll-reverse (0|1)     Reverse scrolling directions\n"
"  --scroll-radius F          Minimum circle radius to register (default: 1.0)\n"
"  --scroll-delay F           Delay between scroll clicks (default: 0.2)\n"        
"\n"
"Clicking tweaks (see SDK for details):\n"
"  --kt-mindist F             Gesture.KeyTap.MinDistance        Default: 4.0\n"
"  --kt-hist F                Gesture.KeyTap.HistorySeconds     Default: 0.1\n"
"  --kt-mindown F             Gesture.KeyTap.MinDownVelocity    Default: 1.0\n"
"\n"
"Tapping tweaks for toggle (see SDK for details):\n"
"  --st-minforward F          Gesture.ScreenTap.MinForwardVelocity\n"
"                               Default: 30.0\n"
"  --st-hist F                Gesture.ScreenTap.HistorySeconds  Default: 0.1\n"
"  --st-mindist F             Gesture.ScreenTap.MinDistance     Default: 1.0\n"
"\n"
"Note: Fingers should be specified as a number, 0-4, or a space-\n"
"      separated list of numbers, e.g. --lclick-fingers \"1 2\"\n"
         << endl;
}

static char short_opt[] = "t:h";

static struct option options[] = {
    { "track-finger",              required_argument, 0, 't' },
    { "toggle-finger",             required_argument, 0, 'T' },
    { "scroll-finger",             required_argument, 0, 's' },
    { "lclick-fingers",            required_argument, 0,  0  },
    { "mclick-fingers",            required_argument, 0,  0  },
    { "rclick-fingers",            required_argument, 0,  0  },
    { "distance-cutoff",           required_argument, 0,  0, },
    { "base-scale",                required_argument, 0,  0, },
    { "small-motion-bias",         required_argument, 0,  0, },
    { "scroll-reverse",            required_argument, 0,  0, },
    { "scroll-radius",             required_argument, 0,  0, },
    { "scroll-delay",              required_argument, 0,  0, },
    { "kt-mindist",                required_argument, 0,  0, },
    { "kt-hist",                   required_argument, 0,  0, },
    { "kt-mindown",                required_argument, 0,  0, },
    { "st-minforward",             required_argument, 0,  0, },
    { "st-hist",                   required_argument, 0,  0, },
    { "st-mindist",                required_argument, 0,  0, },
    { "help",                      no_argument,       0, 'h' },
    { 0, 0, 0, 0 }
};

void MouseConfig::parseOpts(int argc, char **argv) {
    TrivialMouseConfig config(*this);

    int o, index;
    string s;

    for(;;) {
        o = getopt_long(argc, argv, short_opt, options, &index);
        if(optarg) s = optarg;
        else       s = "";

        stringstream ss(s);

        if(o == -1) break;

        switch(o) {
            case 0: {
                string name = options[index].name;
                config.readVal(name, ss);
                break;
            }

            // Sucks index is invalid for short opts
            case 't':
                config.readVal("track-finger", ss);
                break;

            case 'T':
                config.readVal("toggle-finger", ss);
                break;

            case 's':
                config.readVal("scroll-finger", ss);

            case 'h':
                usage();
                exit(0);

            case '?':
            default:
                usage();
                exit(1);
        }
    }

    writeDefault();
}
