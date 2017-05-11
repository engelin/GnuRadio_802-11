#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/esl/naver/gr-data_equalizing/python
export PATH=/home/esl/naver/gr-data_equalizing/build/python:$PATH
export LD_LIBRARY_PATH=/home/esl/naver/gr-data_equalizing/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/esl/naver/gr-data_equalizing/build/swig:$PYTHONPATH
/usr/bin/python /home/esl/naver/gr-data_equalizing/python/qa_preamble_removal.py 
