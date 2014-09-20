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

#include <iostream>
#include "xleapmouse.h"

using namespace std;

void uiLoop(MouseListener &listener __attribute__((unused))) {
    char dummy[2];
    cout << "Press enter to quit..." << endl;
    cin.getline(dummy, 2);
}

void setStatus(TrackStatus status) {
    switch(status) {
        case OFF: cout << "Tracking is OFF" << endl; break;
        case ON: cout << "Tracking is ON" << endl; break;
    }
}
