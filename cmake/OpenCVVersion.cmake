SET(OPENCV_VERSION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/modules/core/include/opencv2/core/version.hpp")
file(STRINGS "${OPENCV_VERSION_FILE}" OPENCV_VERSION_PARTS REGEX "#define CV_VERSION_[A-Z]+[ ]+" )

string(REGEX REPLACE ".+CV_VERSION_EPOCH[ ]+([0-9]+).*" "\\1" OPENCV_VERSION_MAJOR "${OPENCV_VERSION_PARTS}")
string(REGEX REPLACE ".+CV_VERSION_MAJOR[ ]+([0-9]+).*" "\\1" OPENCV_VERSION_MINOR "${OPENCV_VERSION_PARTS}")
string(REGEX REPLACE ".+CV_VERSION_MINOR[ ]+([0-9]+).*" "\\1" OPENCV_VERSION_PATCH "${OPENCV_VERSION_PARTS}")
string(REGEX REPLACE ".+CV_VERSION_STATUS[ ]+\"([^\"]*)\".*" "\\1" OPENCV_VERSION_STATUS "${OPENCV_VERSION_PARTS}")

set(OPENCV_VERSION_PLAIN "${OPENCV_VERSION_MAJOR}.${OPENCV_VERSION_MINOR}.${OPENCV_VERSION_PATCH}")

set(OPENCV_VERSION "${OPENCV_VERSION_PLAIN}${OPENCV_VERSION_STATUS}")

set(OPENCV_SOVERSION "${OPENCV_VERSION_MAJOR}.${OPENCV_VERSION_MINOR}")
set(OPENCV_LIBVERSION "${OPENCV_VERSION_MAJOR}.${OPENCV_VERSION_MINOR}.${OPENCV_VERSION_PATCH}")

# create a dependency on version file
# we never use output of the following command but cmake will rerun automatically if the version file changes
configure_file("${OPENCV_VERSION_FILE}" "${CMAKE_BINARY_DIR}/junk/version.junk" COPYONLY)
