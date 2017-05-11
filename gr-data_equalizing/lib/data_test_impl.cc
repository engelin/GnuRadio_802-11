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
#include "data_test_impl.h"

namespace gr {
  namespace data_equalizing {

    data_test::sptr
    data_test::make()
    {
      return gnuradio::get_initial_sptr
        (new data_test_impl());
    }

    /*
     * The private constructor
     */
    data_test_impl::data_test_impl()
      : gr::block("data_test",
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)),
              gr::io_signature::make(1, 1, 48 * sizeof(gr_complex)))
    {
      this->d_nsym = 0;
    }

    /*
     * Our virtual destructor.
     */
    data_test_impl::~data_test_impl()
    {
    }
/*
    void
    data_test_impl::equalizer(const gr_complex *in, gr_complex *out, int n) {
      gr_complex polarity[127] = {
                 1, 1, 1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1, 1,-1, 1,
                -1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1,
                 1, 1,-1, 1, 1,-1,-1, 1, 1, 1,-1, 1,-1,-1,-1, 1,
                -1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,
                -1,-1, 1,-1, 1,-1, 1, 1,-1,-1,-1, 1, 1,-1,-1,-1,
                -1, 1,-1,-1, 1,-1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,
                -1,-1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1, 1, 1, 1,-1,
                -1, 1,-1,-1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1 };

      double avg_mag = (abs(in[11]) + abs(in[25]) + abs(in[39]) + abs(in[53])) / 4;

      gr_complex p = polarity[(n - 2) % 127];

      double p1 = arg( p * in[11]);
      double p2 = arg( p * in[25] * conj(p * in[11])) + p1;
      double p3 = arg( p * in[39] * conj(p * in[25])) + p2;
      double p4 = arg(-p * in[53] * conj(p * in[39])) + p3;

      double my = (p1 + p2 + p3 + p4) / 4;
      double mx = (11.0 + 25 + 39 + 53) / 4;

      double var = (((11.0 * 11.0) + (25.0 * 25.0) + (39.0 * 39.0) + (53.0 * 53.0)) / 4) - (mx * mx);
      double cov =  (( (p1 * 11) + (p2 * 25) + (p3 * 39) + (p4 * 53) ) / 4) - (mx * my);
      double beta = cov / var;
      double alpha = my - beta * mx;

      int c = 0;
      for(int n = 0; n < 64; n++) {
        if( (n == 11) || (n == 25) || (n == 32) || (n == 39) || (n == 53) || (n < 6) || ( n > 58)) {
          continue;
        }
        else {
          out[c] = in[n] * exp(gr_complex(0, -n * beta - alpha)) * gr_complex(1 / avg_mag, 0);
          c++;
        }
      }
    }
*/
    void
    data_test_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    data_test_impl::general_work (int noutput_items,
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
          //std::cout << "ninput items : " << ninput_items[0] << "noutput items : " << noutput_items << std::endl;
          get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + symbols_to_read);

          if(tags.size()) {
	    if(this->d_nsym == 0) {
	       int d_len = 102;
	       int d_encoding = 0;
	       add_item_tag(0, nitems_written(0) + o, pmt::string_to_symbol("ofdm_start"), pmt::cons(pmt::from_uint64(d_len), pmt::from_uint64(d_encoding)), pmt::string_to_symbol(name()));
	    }

	    if(this->d_nsym > 3) {
	      //std::cout << o  << " pilot removal" << std::endl;
	      gr_complex temp[48];
	      int c = 0;
	      for(int n = 0; n < 64; n++) {
	        if( (n == 11) || (n == 25) || (n == 32) || (n == 39) || (n == 53) || (n < 6) || ( n > 58)) continue;

	        else {
		  temp[c] = in[n];
		  c++;
		}
	        //std::cout << temp[c] << " ";
	      } //std::cout << std::endl;

	      std::memcpy(out, temp, 48 * sizeof(gr_complex));

	      for(int i = 0; i < 48; i++) {
	        //std::cout << out[i] << " ";
	      } //std::cout << std::endl;
	      o++;
	      out += 48;
	    }
	  }
	  this->d_nsym++;
	  i++;
	  in += 64;
	}


        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return o;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

