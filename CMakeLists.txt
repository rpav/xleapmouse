cmake_minimum_required(VERSION 3.6)
project(xleapmouse VERSION 1.0)

set(XLEAP_DISABLE_GUI OFF CACHE BOOL "Disable Qt5 even if available")

if(NOT XLEAP_DISABLE_GUI)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  find_package(Qt5Widgets CONFIG)
endif()

message("** Build options:")

if(Qt5Widgets_LIBRARIES)
  message("**    Qt5: Enabled")
else()
  message("**    Qt5: Disabled (no GUI or status bar)")
endif()

add_subdirectory(src)
