#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/esl/naver/gr-data_equalizing/lib
export PATH=/home/esl/naver/gr-data_equalizing/build/lib:$PATH
export LD_LIBRARY_PATH=/home/esl/naver/gr-data_equalizing/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-data_equalizing 
