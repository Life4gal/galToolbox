set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(
    Boost COMPONENTS 

    unit_test_framework 
    system 
    thread 
    REQUIRED
)

if (NOT Boost_FOUND)
    MESSAGE(FATAL "Not found boost library!")
endif()

message(STATUS "found boost include: " ${Boost_INCLUDE_DIRS})
message(STATUS "found boost library: " ${Boost_LIBRARIES})
