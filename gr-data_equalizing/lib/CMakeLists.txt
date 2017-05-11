# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND data_equalizing_sources
    data_equalizing_impl.cc
    sig_field_removal_impl.cc
    sym_to_frame_impl.cc
    preamble_removal_impl.cc
    large_fft_impl.cc
    data_test_impl.cc
    HYR_ofdm_impl.cc
)

set(data_equalizing_sources "${data_equalizing_sources}" PARENT_SCOPE)
if(NOT data_equalizing_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT data_equalizing_sources)

list(APPEND data_equalzing_libs
    ${Boost_LIBRARIES}
    ${GNURADIO_ALL_LIBRARIES}
)

add_library(gnuradio-data_equalizing SHARED ${data_equalizing_sources})
target_link_libraries(gnuradio-data_equalizing ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${EIGEN3_LIBRARIES} ${FFTW3_LIBRARIES} ${ITPP_LIBRARIES})
set_target_properties(gnuradio-data_equalizing PROPERTIES DEFINE_SYMBOL "gnuradio_data_equalizing_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-data_equalizing PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-data_equalizing
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_data_equalizing_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_data_equalizing.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_data_equalizing.cc
)

add_executable(test-data_equalizing ${test_data_equalizing_sources})

target_link_libraries(
  test-data_equalizing
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${EIGEN3_LIBRARIES}
  ${FFTW3_LIBRARIES}
  ${ITPP_LIBRARIES}
  gnuradio-data_equalizing
)

GR_ADD_TEST(test_data_equalizing test-data_equalizing)
