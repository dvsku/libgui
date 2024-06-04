INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## libutil

FetchContent_Declare(
    libutil
    URL https://github.com/dvsku/libutil/archive/refs/heads/master.zip
)

FetchContent_MakeAvailable(libutil)
