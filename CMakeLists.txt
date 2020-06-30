cmake_minimum_required(VERSION 3.1.0)
project("QHash")

if (UNIX)
    set(CMAKE_CXX_FLAGS -Wall)
endif (UNIX)
if (WIN32)
    set(CMAKE_PREFIX_PATH "D:\\Qt\\Qt5.9.8\\5.9.8\\msvc2017_64")
endif(WIN32)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(
    QHashSRC
    droparea.cpp
    hasher.cpp
    main.cpp
    mainwindow.cpp
)

if (UNIX)
    add_executable(QHash ${QHashSRC})
elseif (WIN32)
    add_executable(QHash WIN32 ${QHashSRC})
endif (UNIX)


target_link_libraries(QHash Qt5::Widgets Qt5::Core Qt5::Gui)

if (UNIX)
    install(TARGETS QHash
	RUNTIME DESTINATION /usr/bin)
elseif (WIN32)
    install(TARGETS QHash
	RUNTIME DESTINATION d:/QHash/)
endif (UNIX)