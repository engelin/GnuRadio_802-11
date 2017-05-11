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

#ifndef INCLUDED_DATA_EQUALIZING_LARGE_FFT_IMPL_H
#define INCLUDED_DATA_EQUALIZING_LARGE_FFT_IMPL_H

#include <data_equalizing/large_fft.h>

namespace gr {
  namespace data_equalizing {

    class large_fft_impl : public large_fft
    {
     private:
      // Nothing to declare in this block.

     public:
      large_fft_impl();
      ~large_fft_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

      int d_nsym;
      gr_complex sym[1000 * 64 * 100];
    };

  } // namespace data_equalizing
} // namespace gr

#endif /* INCLUDED_DATA_EQUALIZING_LARGE_FFT_IMPL_H */

