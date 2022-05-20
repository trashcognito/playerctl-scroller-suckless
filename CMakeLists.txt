cmake_minimum_required(VERSION 3.0.0)
project(playerctl-scroller-suckless VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(playerctl-scroller-suckless main.cpp cscroll.cpp dbus_interface.cpp thread_helper.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
