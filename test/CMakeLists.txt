add_executable(dump dump.cpp ../autotests/simpleresult.cpp)
target_link_libraries(dump
    Qt5::Core
    KF5::FileMetaData
    KF5::Service
)
