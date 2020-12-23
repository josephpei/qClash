# Add desktop file and desktop icon to target machine

if(UNIX AND NOT APPLE)
    install(PROGRAMS com.qclash.qClash.desktop DESTINATION /usr/share/applications)
    install(FILES icons/qClash.svg DESTINATION /usr/share/icons/hicolor/scalable/apps)
endif()