# easyzip-config.cmake - package configuration file

get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(EXISTS ${SELF_DIR}/easyzip-target.cmake)
	include(${SELF_DIR}/easyzip-target.cmake)
endif()

if(EXISTS ${SELF_DIR}/easyzip-static-target.cmake)
	include(${SELF_DIR}/easyzip-static-target.cmake)
endif()
