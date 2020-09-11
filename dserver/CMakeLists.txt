cmake_minimum_required(VERSION 3.7)
project(dserver C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -pthread  -Wall ")
set(CMAKE_L_FLAGS "${CMAKE_C_FLAGS}  -pthread  -Wall ")
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

add_executable(server  main.c
        cpu_usage.c
        cpu_usage.h

        devices.c
        devices.h

        functions.c
        functions.h

        interrupts.c
        interrupts.h

        memory_usage.c
        memory_usage.h

        network_bandwith.c
        network_bandwith.h

        task_manager.c
        task_manager.h

        )

include_directories(${GTK3_INCLUDE_DIRS})

link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

link_directories(${GTK3_LIBRARY_DIRS})
link_libraries(${GTK3_LIBRARIES})
target_link_libraries(server m)
target_link_libraries(server fontconfig)

target_link_libraries(server ${GTK3_LIBRARIES})