INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_DATA_EQUALIZING data_equalizing)

FIND_PATH(
    DATA_EQUALIZING_INCLUDE_DIRS
    NAMES data_equalizing/api.h
    HINTS $ENV{DATA_EQUALIZING_DIR}/include
        ${PC_DATA_EQUALIZING_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    DATA_EQUALIZING_LIBRARIES
    NAMES gnuradio-data_equalizing
    HINTS $ENV{DATA_EQUALIZING_DIR}/lib
        ${PC_DATA_EQUALIZING_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DATA_EQUALIZING DEFAULT_MSG DATA_EQUALIZING_LIBRARIES DATA_EQUALIZING_INCLUDE_DIRS)
MARK_AS_ADVANCED(DATA_EQUALIZING_LIBRARIES DATA_EQUALIZING_INCLUDE_DIRS)
