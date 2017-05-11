# Install script for directory: /home/esl/naver/gr-ieee802-11/include/ieee802-11

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ieee802_11_devel")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ieee802-11/gnuradio/ieee802_11" TYPE FILE FILES
    "/home/esl/naver/gr-ieee802-11/lib/include/ieee802-11/moving_average_ff.h"
    "/home/esl/naver/gr-ieee802-11/lib/include/ieee802-11/moving_average_cc.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/api.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/chunks_to_symbols.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ether_encap.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_decode_mac.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_decode_signal.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_equalize_symbols.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_mac.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_mapper.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_parse_mac.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_sync_long.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/ofdm_sync_short.h"
    "/home/esl/naver/gr-ieee802-11/include/ieee802-11/wifi_signal_field.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "ieee802_11_devel")

