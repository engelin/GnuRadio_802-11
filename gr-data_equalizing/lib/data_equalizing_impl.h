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

#ifndef INCLUDED_DATA_EQUALIZING_DATA_EQUALIZING_IMPL_H
#define INCLUDED_DATA_EQUALIZING_DATA_EQUALIZING_IMPL_H

#include <data_equalizing/data_equalizing.h>

namespace gr {
  namespace data_equalizing {

    class data_equalizing_impl : public data_equalizing
    {
     private:
      // Nothing to declare in this block.

     public:
      data_equalizing_impl();
      ~data_equalizing_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

	unsigned int d_nsym;
	unsigned int count;
	std::complex<float> first_symbol[64];
	double first_symbol_r[64];
	double first_symbol_i[64];

	gr_complex sym[1000*48*100];
	gr_complex temp[64*35];
	gr_complex temp_fft[64*35];

	void data_equalizer(gr_complex *in_fft_data, gr_complex *in_re_data, gr_complex *out);

        void equalizer(gr_complex *in, gr_complex *out, int n);
	void demodulate(gr_complex *sym);
	void deinterleave(char *bits, char *deinter);
	void Binary2Decimal(unsigned char *binary, unsigned int *decimal, unsigned char bit);
	void Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit);
	unsigned char Modular2Adder(unsigned char *src, unsigned int idx);
	int viterbi_decode(unsigned char *code, unsigned char *stream);
	void convolution_decoder(char *in, char *out);
	void descrambler(char *in, char *out);

	double gaussianRandom(double average, double stdev);
	void real_channel(gr_complex *in, gr_complex *out, int n);
    };

  } // namespace data_equalizing
} // namespace gr

#endif /* INCLUDED_DATA_EQUALIZING_DATA_EQUALIZING_IMPL_H */

