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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "large_fft_impl.h"

#include <fftw3.h>
//#include <unsupported/Eigen/FFT>

namespace gr {
  namespace data_equalizing {

    large_fft::sptr
    large_fft::make()
    {
      return gnuradio::get_initial_sptr
        (new large_fft_impl());
    }

    /*
     * The private constructor
     */
    large_fft_impl::large_fft_impl()
      : gr::block("large_fft",
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)),
              gr::io_signature::make(1, 1, 128 * sizeof(gr_complex)))
    {
      this->d_nsym = 0;
    }

    /*
     * Our virtual destructor.
     */
    large_fft_impl::~large_fft_impl()
    {
    }

    void
    large_fft_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    large_fft_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

	int symbols_to_read = 0;
	int i = 0;
	int o = 0;

	symbols_to_read = std::min(noutput_items, ninput_items[0]);
	noutput_items = symbols_to_read;

	std::vector<gr::tag_t> tags;

	while(i < ninput_items[0]) {
	  get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + symbols_to_read);

	  std::cout << "LARGE FFT input : " << ninput_items[0] << " i : " << i << std::endl;

	  int fft_w = ninput_items[0] * 64;

	  //fftw::maxthreads=get_max_threads();
	  //fft1d Forward(fft_w,-1);

	  if(tags.size()) {
	    //this->d_nsym = 0;
	  }

	  if(i < ninput_items[0]) {
	    std::memcpy(sym + (i * 64), in, 64 * sizeof(gr_complex));
	    i++;

	    if(i == ninput_items[0]) {
	      ///large_fft
	      //Forward.fft(sym);
	      //fftw_execute(sym);
	      in += 64;
	    }
	  }

	  in += 64;
	  //i++;
	  this->d_nsym++;
	}

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

