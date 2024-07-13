cmake_minimum_required(VERSION 3.16)
project(powermode-indicator VERSION 0.1)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -ggdb3")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtkmm-3.0 glibmm-2.4 giomm-2.4)
pkg_check_modules(APP_INDICATOR REQUIRED IMPORTED_TARGET appindicator3-0.1)

add_executable(powermode-indicator main.cpp app_menu.cpp
                                   power_profile_manager.cpp indicator.cpp)

target_link_libraries(powermode-indicator ${GTK_LIBRARIES}
                      ${APP_INDICATOR_LIBRARIES})

target_compile_options(
  powermode-indicator PRIVATE ${GTK_CFLAGS_OTHER} ${APP_INDICATOR_CFLAGS_OTHER})

target_include_directories(
  powermode-indicator PRIVATE ${GTK_INCLUDE_DIRS} ${APP_INDICATOR_INCLUDE_DIRS})

target_link_directories(powermode-indicator PRIVATE ${GTK_LIBRARY_DIRS}
                        ${APP_INDICATOR_LIBRARIES})

install(TARGETS powermode-indicator DESTINATION bin)
