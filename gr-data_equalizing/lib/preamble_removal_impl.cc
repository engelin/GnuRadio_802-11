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
#include "preamble_removal_impl.h"

namespace gr {
  namespace data_equalizing {

    preamble_removal::sptr
    preamble_removal::make()
    {
      return gnuradio::get_initial_sptr
        (new preamble_removal_impl());
    }


    /*
     * The private constructor
     */
    preamble_removal_impl::preamble_removal_impl()
      : gr::block("preamble_removal",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
      message_port_register_in(pmt::mp("in"));
    }

    /*
     * Our virtual destructor.
     */
    preamble_removal_impl::~preamble_removal_impl()
    {
    }

    void
    preamble_removal_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    void
    preamble_removal_impl::data_to_symbol(gr_complex *data, gr_complex *out, int len){
      int n = 0;
      for(int c = 0; c < (len/48)*64; c++) {
        if((c % 64 == 11) || (c % 64 == 25) || (c % 64 == 32) || (c % 64 == 39) || (c % 64 == 53) || (c % 64 < 6) || (c % 64 > 58)) {
          //std::cout << c << " pilot.." << out[c] << std::endl;
          out[c] = gr_complex(0, 0);
        }
	else {
	  out[c] = data[n];
	  n++;
	}
      }
    }

    int
    preamble_removal_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        gr_complex *out = (gr_complex *) output_items[0];

	int o = 0;	
	pmt::pmt_t msg(delete_head_blocking(pmt::intern("in")));

	if(pmt::is_eof_object(msg)) {
	  //std::cout << "RAW symbol.. : exiting" << std::endl;
	  return -1;
	}
	
	if(pmt::is_pair(msg)) {
	  //std::cout << "Raw symbol received.." << std::endl;
	  gr::thread::scoped_lock lock(d_mutex);

	  pmt::pmt_t raw_data = pmt::dict_ref(pmt::car(msg), pmt::mp("raw_symbol"), pmt::mp("no_raw_data"));
	  const gr_complex *raw_symbols = (std::complex<float>*)(pmt::blob_data(raw_data));
	  int data_len = (pmt::blob_length(raw_data))/8;

	  /*std::cout << "raw symbols" << std::endl;
	  for(int n = 0; n < data_len; n++) {
	    std::cout << raw_symbols[n] << " ";
	    if(n % 48 == 47) std::cout << (int)(n/48) << std::endl;
	  } std::cout << std::endl;*/

          data_to_symbol((gr_complex*)raw_symbols, out, data_len);
          /*std::cout << "raw symbols" << std::endl;
          for(int n = 0; n < (data_len/48)*64; n++) {
            std::cout << out[n] << " ";
            if(n % 64 == 63) std::cout << (int)(n/64) << std::endl;
          } std::cout << std::endl;*/
	  o = (data_len/48)*64;
	}

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        //consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return o;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

