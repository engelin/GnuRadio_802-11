/* -*- c++ -*- */

#define DATA_EQUALIZING_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "data_equalizing_swig_doc.i"

%{
#include "data_equalizing/data_equalizing.h"
#include "data_equalizing/sig_field_removal.h"
#include "data_equalizing/sym_to_frame.h"
#include "data_equalizing/preamble_removal.h"
#include "data_equalizing/large_fft.h"
#include "data_equalizing/data_test.h"
#include "data_equalizing/HYR_ofdm.h"
%}


%include "data_equalizing/data_equalizing.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, data_equalizing);
%include "data_equalizing/sig_field_removal.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, sig_field_removal);
%include "data_equalizing/sym_to_frame.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, sym_to_frame);
%include "data_equalizing/preamble_removal.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, preamble_removal);
%include "data_equalizing/large_fft.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, large_fft);
%include "data_equalizing/data_test.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, data_test);
%include "data_equalizing/HYR_ofdm.h"
GR_SWIG_BLOCK_MAGIC2(data_equalizing, HYR_ofdm);
