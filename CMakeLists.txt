cmake_minimum_required(VERSION 3.0.0)
project(playerctl-scroller-suckless VERSION 0.1.0)

include(CTest)
enable_testing()
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(playerctl-scroller-suckless main.cpp cscroll.cpp dbus_interface.cpp thread_helper.cpp)

target_link_libraries(playerctl-scroller-suckless
    ${GTKMM_LIBRARIES}  )

install(TARGETS playerctl-scroller-suckless DESTINATION bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
