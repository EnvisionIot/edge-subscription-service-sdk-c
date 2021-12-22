#set(LOG "log.txt")
#add_custom_command(OUTPUT  ${LOG}
#  COMMAND echo "Generating log.txt file..."
#  COMMAND echo "welcome"
#  COMMENT  "This is a test"
# )

#add_custom_target(cp_3rd_include ALL DEPENDS ${LOG})
add_custom_target(cp_3rd_include ALL)

IF(OS_DEFINE STREQUAL "linux")
    add_custom_command(TARGET cp_3rd_include
        PRE_BUILD
        COMMAND rm -rf "${PROJECT_SOURCE_DIR}/dst/include/3rd"
        COMMAND cp -r -d "${CMAKE_CURRENT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/dst/include/3rd"
        COMMENT "cp 3rd include and lib to dst(${OS_DEFINE})"
    )
ELSEIF(OS_DEFINE STREQUAL "windows")
    STRING(REGEX REPLACE "/" "\\\\" PROJECT_SOURCE_DIR_NATIVE ${PROJECT_SOURCE_DIR} )
    STRING(REGEX REPLACE "/" "\\\\" CMAKE_CURRENT_SOURCE_DIR_NATIVE ${CMAKE_CURRENT_SOURCE_DIR} )
    IF(SH_TYPE STREQUAL "sh")
        IF(CMAKE_SH STREQUAL "CMAKE_SH-NOTFOUND")
            add_custom_command(TARGET cp_3rd_include
                PRE_BUILD
                COMMAND if exist "${PROJECT_SOURCE_DIR_NATIVE}\\dst\\include\\3rd" rd /s/q "${PROJECT_SOURCE_DIR_NATIVE}\\dst\\include\\3rd"
                COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR_NATIVE}\\include" "${PROJECT_SOURCE_DIR_NATIVE}\\dst\\include\\3rd" /y /i /s /e /h
                COMMENT "cp 3rd include and lib to dst(${OS_DEFINE}) (${CMAKE_CURRENT_SOURCE_DIR_NATIVE})"
            )
        ELSE()
            add_custom_command(TARGET cp_3rd_include
                PRE_BUILD
                COMMAND rm -rf "${PROJECT_SOURCE_DIR}/dst/include/3rd"
                COMMAND cp -r -d "${CMAKE_CURRENT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/dst/include/3rd"
                COMMENT "cp 3rd include and lib to dst(${OS_DEFINE}) (${CMAKE_CURRENT_SOURCE_DIR_NATIVE})"
            )
        ENDIF()
    ELSE()
        add_custom_command(TARGET cp_3rd_include
            PRE_BUILD
            COMMAND if exist "${PROJECT_SOURCE_DIR_NATIVE}\\dst\\include\\3rd" rd /s/q "${PROJECT_SOURCE_DIR_NATIVE}\\dst\\include\\3rd"
            COMMAND xcopy "${CMAKE_CURRENT_SOURCE_DIR_NATIVE}\\include" "${PROJECT_SOURCE_DIR_NATIVE}\\dst\\include\\3rd" /y /i /s /e /h
            COMMENT "cp 3rd include and lib to dst(${OS_DEFINE}) (${CMAKE_CURRENT_SOURCE_DIR_NATIVE})"
        )
    ENDIF()
ELSE()
    add_custom_command(TARGET cp_3rd_include
        PRE_BUILD
        COMMAND rm -rf "${PROJECT_SOURCE_DIR}/dst/include/3rd"
        COMMAND cp -r -d "${CMAKE_CURRENT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/dst/include/3rd"
        COMMENT "cp 3rd include and lib to dst(${OS_DEFINE})"
    )
ENDIF()

SET(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/dst/lib/3rd")
ADD_SUBDIRECTORY(src)
