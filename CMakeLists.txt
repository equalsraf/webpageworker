cmake_minimum_required(VERSION 2.8.11)
project(webpageworker)

add_definitions("-std=c++11")

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Network REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)

set(webpageworker_SOURCES webpageworker.cpp webpage.cpp networkaccessmanager.cpp)
qt5_add_resources(webpageworker_RCC javascript.qrc)
add_executable(webpageworker ${webpageworker_SOURCES} ${webpageworker_RCC})
qt5_use_modules(webpageworker Script WebKitWidgets)

# Manpage
add_custom_command(OUTPUT webpageworker.1
	DEPENDS ${CMAKE_SOURCE_DIR}/MANPAGE.md
	COMMAND md2man-roff2.0 ${CMAKE_SOURCE_DIR}/MANPAGE.md > webpageworker.1)
add_custom_target(man DEPENDS webpageworker.1)
