AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)

SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/protobuf)
FOREACH (subdir ${SUBDIRS})
    #        message("------------------------------------>${subdir}")
    IF (NOT subdir MATCHES "CMakeFiles")
        AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/protobuf/${subdir} DIR_LIB_SRCS)
    ENDIF ()
ENDFOREACH ()

STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR})
SET(CURRENT_LIB_NAME "${CURRENT_DIR_NAME}")
ADD_LIBRARY(${CURRENT_LIB_NAME} SHARED ${DIR_LIB_SRCS})
TARGET_LINK_LIBRARIES(${CURRENT_LIB_NAME} ev evbuffsock curl jansson nsq protobuf-c cjson pthread)