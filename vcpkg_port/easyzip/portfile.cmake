vcpkg_fail_port_install(ON_ARCH "arm" "arm64" ON_TARGET "UWP" "osx")

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO winsoft666/easyzip
    REF ce4529e03c56d5b3cd67d5456a9398bcb9e71a57
    SHA512 a0b13f149f11190a856e8ebedeb5dc2ec1259e80c48028ec07f92f6474298a45b0688563a08486196f6f07ab4316c19765a35f64e1734dba7b4d5c41d7196a5b
    HEAD_REF master
)

string(COMPARE EQUAL "${VCPKG_LIBRARY_LINKAGE}" "static" EASYZIP_STATIC)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    PREFER_NINJA
    OPTIONS
        -DEASYZIP_STATIC:BOOL=${EASYZIP_STATIC}
        -DBUILD_TESTS:BOOL=OFF
)

vcpkg_install_cmake()

if(EXISTS ${CURRENT_PACKAGES_DIR}/lib/cmake/easyzip)
    vcpkg_fixup_cmake_targets(CONFIG_PATH lib/cmake/easyzip)
elseif(EXISTS ${CURRENT_PACKAGES_DIR}/share/easyzip)
    vcpkg_fixup_cmake_targets(CONFIG_PATH share/easyzip)
endif()

file(READ ${CURRENT_PACKAGES_DIR}/include/easyzip/easyzip.h EASYZIP_H)
if(VCPKG_LIBRARY_LINKAGE STREQUAL "static")
    string(REPLACE "#ifdef EASYZIP_STATIC" "#if 1" EASYZIP_H "${EASYZIP_H}")
else()
    string(REPLACE "#ifdef EASYZIP_STATIC" "#if 0" EASYZIP_H "${EASYZIP_H}")
endif()
file(WRITE ${CURRENT_PACKAGES_DIR}/include/easyzip/easyzip.h "${EASYZIP_H}")

file(INSTALL ${SOURCE_PATH}/LICENSE DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/share)

vcpkg_copy_pdbs()