INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_NUMERIC numeric)

FIND_PATH(
    NUMERIC_INCLUDE_DIRS
    NAMES numeric/api.h
    HINTS $ENV{NUMERIC_DIR}/include
        ${PC_NUMERIC_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    NUMERIC_LIBRARIES
    NAMES gnuradio-numeric
    HINTS $ENV{NUMERIC_DIR}/lib
        ${PC_NUMERIC_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NUMERIC DEFAULT_MSG NUMERIC_LIBRARIES NUMERIC_INCLUDE_DIRS)
MARK_AS_ADVANCED(NUMERIC_LIBRARIES NUMERIC_INCLUDE_DIRS)

