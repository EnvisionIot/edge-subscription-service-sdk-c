AUX_SOURCE_DIRECTORY(. DIR_LIB_SRCS)
STRING(REGEX REPLACE ".*/(.*)" "\\1" CURRENT_DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR})
SET(CURRENT_LIB_NAME "${CURRENT_DIR_NAME}")
ADD_DEFINITIONS("-DWITH_JANSSON")
ADD_LIBRARY(${CURRENT_LIB_NAME} SHARED ${DIR_LIB_SRCS})
TARGET_LINK_LIBRARIES(${CURRENT_LIB_NAME} ev)