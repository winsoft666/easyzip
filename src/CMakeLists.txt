﻿###############################################################################
# Copyright (C) 2018 - 2020, winsoft666, <winsoft666@outlook.com>.
#
# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
# EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
#
# Expect bugs
#
# Please use and enjoy. Please let me know of any bugs/improvements
# that you have found/implemented and I will fix/incorporate them into this
# file.
###############################################################################

set (CMAKE_CXX_STANDARD 11)

if(BUILD_SHARED_LIBS)
	set(LIB_NAME easyzip)
	set(EASYZIP_STATIC NO)
else()
	set(LIB_NAME easyzip-static)
	set(EASYZIP_STATIC YES)
endif()


if (MSVC AND USE_STATIC_CRT)
    set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
    foreach(CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach()
endif()



file(GLOB HEADER_FILES 		../include/easyzip/*.h ../include/easyzip/*.hpp)
file(GLOB SOURCE_FILES 		./*.cpp ./*.h ./*.hpp ./*.c)
file(GLOB MINIZIP_FILES   ./minizip/*.h ./minizip/*.c)

source_group("Source Files" 			FILES ${SOURCE_FILES})
source_group("Header Files" 			FILES ${HEADER_FILES})
source_group("minizip"            FILES ${MINIZIP_FILES})

if(BUILD_SHARED_LIBS)
	add_definitions(-DEASYZIP_EXPORTS)
	
	add_library(
		${LIB_NAME}
		SHARED
		${HEADER_FILES}
		${SOURCE_FILES}
    ${MINIZIP_FILES}
	)
	
	# set output name prefix/suffix
	set_target_properties(${LIB_NAME} PROPERTIES 
		OUTPUT_NAME ${LIB_NAME}
		DEBUG_OUTPUT_NAME ${LIB_NAME}${DEBUG_LIBNAME_SUFFIX})
else()
	add_definitions(-DEASYZIP_STATIC)
	
	add_library(
		${LIB_NAME}
		STATIC
		${HEADER_FILES}
		${SOURCE_FILES}
    ${MINIZIP_FILES}
	)
	
	# set output name prefix/suffix
    set_target_properties(${LIB_NAME} PROPERTIES 
		OUTPUT_NAME ${LIB_NAME}
		DEBUG_OUTPUT_NAME ${LIB_NAME}${DEBUG_LIBNAME_SUFFIX})
		
	set_target_properties(${LIB_NAME} PROPERTIES 
		INTERFACE_COMPILE_DEFINITIONS EASYZIP_STATIC)
endif()


# zlib
find_package(ZLIB REQUIRED)
target_link_libraries(${LIB_NAME} PUBLIC 
	ZLIB::ZLIB)

# Set output directory
set_target_properties(${LIB_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" )

target_include_directories(${LIB_NAME} INTERFACE
	$<INSTALL_INTERFACE:include>)

install(TARGETS ${LIB_NAME}
	EXPORT ${LIB_NAME}-target
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)


install(FILES ${HEADER_FILES} 			DESTINATION include/easyzip)


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../CMake/easyzip-config.cmake 
	DESTINATION ${EASYZIP_INSTALL_CMAKE_DIR})

export(TARGETS ${LIB_NAME}
       APPEND FILE ${PROJECT_BINARY_DIR}/${LIB_NAME}-target.cmake
)

install(EXPORT ${LIB_NAME}-target
        FILE ${LIB_NAME}-target.cmake
        DESTINATION ${EASYZIP_INSTALL_CMAKE_DIR}
)
