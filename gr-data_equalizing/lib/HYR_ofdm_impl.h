/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DATA_EQUALIZING_HYR_OFDM_IMPL_H
#define INCLUDED_DATA_EQUALIZING_HYR_OFDM_IMPL_H

#include <data_equalizing/HYR_ofdm.h>

#define UTILS_H
#include "include/utils.h"

#define EQUALIZER_H
#define DECODESIG_H
#define DECODEMAC_H
#include "include/equalizer.h"
#include "include/decode_sig.h"
#include "include/decode_mac.h"

namespace gr {
  namespace data_equalizing {

    class HYR_ofdm_impl : public HYR_ofdm
    {
     private:
      // Nothing to declare in this block.

     public:
      HYR_ofdm_impl();
      ~HYR_ofdm_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);


        int d_nsym;
        gr::thread::mutex d_mutex;

        int f_mac;
        int d_mac_len;
        int d_mac_encoding;
        int d_mac_symbols;

	int copied;
	gr_complex sym[1000*48*100];
	gr_complex raw_sym[1000*48*100];
/*	void equalizer(gr_complex *in, gr_complex *out, int n);
	void sig_demodulator(gr_complex *in, char *out);
	void sig_deinterleaver(char *in, char *out);
	void Binary2Decimal(unsigned char *binary, unsigned int *decimal, unsigned char bit);
	void Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit);
	unsigned char Modular2Adder(unsigned char *src, unsigned int idx);
	int viterbi(unsigned char *code, unsigned char *stream);
	void sig_viterbi_decoder(char *in, char *out);*/
	bool print_signal(char *in);
    };

  } // namespace data_equalizing
} // namespace gr

#endif /* INCLUDED_DATA_EQUALIZING_HYR_OFDM_IMPL_H */

