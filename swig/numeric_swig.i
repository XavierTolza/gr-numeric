/* -*- c++ -*- */

#define NUMERIC_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "numeric_swig_doc.i"

%{
#include "numeric/pack_byte.h"
#include "numeric/peak_tagger.h"
%}


%include "numeric/pack_byte.h"
GR_SWIG_BLOCK_MAGIC2(numeric, pack_byte);
%include "numeric/peak_tagger.h"
GR_SWIG_BLOCK_MAGIC2(numeric, peak_tagger);
