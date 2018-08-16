cmake_minimum_required(VERSION 3.12)
Project(Garden)

set(SRCS src/mem.c src/sock-connect.c src/sock-io.c src/view.c
	src/web-header.c src/garden.c)
#target_compile_options(-O2 -Wall)

include_directories("./include")
include_directories("./src")
link_libraries(pthread dl)

#add_library(strings ${SRCS})

add_executable(garden ${SRCS})
