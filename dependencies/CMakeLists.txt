INCLUDE(FetchContent)

SET(FETCHCONTENT_QUIET OFF)
SET(FETCHCONTENT_UPDATES_DISCONNECTED ON)

###############################################################################
## libutil

FetchContent_Declare(
    libutil
    URL https://github.com/dvsku/libutil/archive/edd58aa8cbefcf986d63a5a3ecf0a7f37592afb3.zip
)

FetchContent_MakeAvailable(libutil)
