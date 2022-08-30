cmake_minimum_required(VERSION 3.14)

project(LibQSql LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql)

add_executable(LibQSql
  main.cpp
  libqsql.cpp
  libqsql.h
  l_sqlte.h l_sqlte.cpp
)
target_link_libraries(LibQSql Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)

install(TARGETS LibQSql
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
