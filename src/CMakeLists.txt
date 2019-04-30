set(XLEAP_SOURCES
  xleapmouse.cpp
  config.cpp
  MouseListener.cpp
  )

if(Qt5Widgets_LIBRARIES)
  qt5_wrap_cpp(MOC_SOURCES include/xlmgui.h)
  qt5_wrap_ui(UI_SOURCES res/xlm.ui)
  qt5_add_resources(QRC_SOURCES res/xlm.qrc
    OPTIONS -root /res -name xlm)

  set(GUI_SOURCES
    ui_qt.cpp
    ui_qt_slots.cpp
    res/xlm.ui
    res/xlm.qrc

    ${MOC_SOURCES}
    ${UI_SOURCES}
    ${QRC_SOURCES}
    )


else()
  set(GUI_SOURCES
    ui_none.cpp
    )
endif()

add_executable(xleapmouse
  ${XLEAP_SOURCES}
  ${GUI_SOURCES}
  )

target_include_directories(xleapmouse
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(xleapmouse
  PRIVATE -lX11 -lXtst -lLeap
  )

if(Qt5Widgets_LIBRARIES)
  target_link_libraries(xleapmouse
    PRIVATE Qt5::Widgets
    )
endif()

install(TARGETS xleapmouse)
