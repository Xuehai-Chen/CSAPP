cmake_minimum_required(VERSION 3.10)
project(tiny)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(tiny
        cgi-bin/adder.c
        cgi-bin/Makefile
        csapp.c
        csapp.h
        godzilla.gif
        godzilla.jpg
        home.html
        Makefile
        README
        tiny.c)
