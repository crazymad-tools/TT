cmake_minimum_required(VERSION 2.6)

PROJECT(TT-server)

TARGET_LINK_LIBRARIES(pthread)

ADD_EXECUTABLE(main.cpp MessageAnal.cpp MessageTable.cpp MessagePackage CMutex.h)

