INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET                OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED  ON)

###############################################################################
## eventpp

FetchContent_Declare(
    eventpp
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/eventpp"
)

FetchContent_MakeAvailable(eventpp)
