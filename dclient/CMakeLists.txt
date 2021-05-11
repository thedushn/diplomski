cmake_minimum_required(VERSION 3.7)
project(dclient C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -pthread  -Wall ")

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)


pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(client main.c drawing.c
        drawing.h buttons.c
        buttons.h common.h
        functions.c functions.h
        window.c window.h
        testing_tree.c testing_tree.h
        main_header.h
        testing.c testing.h application.c)

include_directories(${GTK3_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

link_directories(${GTK3_LIBRARY_DIRS})

link_libraries(${GTK3_LIBRARIES})


target_link_libraries(client m)
target_link_libraries(client fontconfig)


target_link_libraries(client ${GTK3_LIBRARIES})
