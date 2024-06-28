INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## libutil

FetchContent_Declare(
    libutil
    URL https://github.com/dvsku/libutil/archive/0dda2d1ba7baf1f6dfa065573db32fe9305afb4a.zip
)

FetchContent_MakeAvailable(libutil)
