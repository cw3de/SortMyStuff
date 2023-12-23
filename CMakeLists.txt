cmake_minimum_required(VERSION 3.0.0)
project(SortMyStuff)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME}
    Checksum.h Checksum.cpp
    DirectoryView.h DirectoryView.cpp
    DirectoryModel.h DirectoryModel.cpp
    SmsCounter.h SmsCounter.cpp
    SmsNode.h SmsNode.cpp
    SmsModel.h SmsModel.cpp
    SmsMainWindow.h SmsMainWindow.cpp
    SmsTree.h SmsTree.cpp
    SmsType.h SmsType.cpp
    SortMyStuff.cpp
    DualPaneView.h DualPaneView.cpp
    DirectoryTable.h DirectoryTable.cpp
    InfoCache.h InfoCache.cpp
    FileMover.h FileMover.cpp
)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets)

