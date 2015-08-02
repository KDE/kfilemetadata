remove_definitions(-DQT_NO_CAST_FROM_ASCII)

add_executable(dump dump.cpp)

target_include_directories(dump  PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>")

target_link_libraries(dump
    Qt5::Core
    KF5FileMetaData
)
