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

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "xleapmouse.h"

int main(int argc, char **argv) {
    using namespace std;

    XInitThreads();

    Leap::Controller controller;
    MouseListener listener;

    listener.conf.readDefault();
    listener.conf.parseOpts(argc, argv);

    controller.addListener(listener);

    uiLoop(listener);

    listener.conf.writeDefault();

    controller.removeListener(listener);
    usleep(200); // Wait for LeapMotion to catch up?
    exit(0);
}
