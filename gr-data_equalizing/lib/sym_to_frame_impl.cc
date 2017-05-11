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
#include "sym_to_frame_impl.h"

namespace gr {
  namespace data_equalizing {

    sym_to_frame::sptr
    sym_to_frame::make()
    {
      return gnuradio::get_initial_sptr
        (new sym_to_frame_impl());
    }

    int symbol_n[10] = {3, 2, 2, 2, 35, 35, 35, 2, 2, 3};
    /*
     * The private constructor
     */
    sym_to_frame_impl::sym_to_frame_impl()
      : gr::block("sym_to_frame",
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)),
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)))
    {
      this->d_nsym = 0;
      //this->symbol_n[10] = {3, 2, 2, 2, 35, 35, 35, 2, 2, 3};
      this->nFrame = 0;
    }

    /*
     * Our virtual destructor.
     */
    sym_to_frame_impl::~sym_to_frame_impl()
    {
    }

    void
    sym_to_frame_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    sym_to_frame_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];
	int symbols_to_read = 0;

	int i = 0;
	int o = 0;
	this->d_nsym = 0;

	symbols_to_read = std::min(noutput_items, ninput_items[0]);
	noutput_items = symbols_to_read;

	std::vector<gr::tag_t> tags;

	while(i < ninput_items[0]) {
	  //std::cout << "ninput items : " << ninput_items[0] << "noutput items : " << noutput_items << std::endl;
	  get_tags_in_range(tags, 0, nitems_read(0), nitems_read(0) + symbols_to_read);

	  if(tags.size()) {
	    //std::cout << "New frame insert.. " << std::endl;
//	    std::cout << "nsym : " << d_nsym << " nFrame : " << nFrame << std::endl;;


///////////////////////////////////////////////////////////
	    //int d_len = 102;
	    //int d_encoding = 0;
///////////////////////////////////////////////////////////
	    if(d_nsym == 3){
	      add_item_tag(0, nitems_written(0) + o, pmt::string_to_symbol("ofdm_start"), pmt::string_to_symbol(name()));
	      //add_item_tag(0, nitems_written(0) + o, pmt::string_to_symbol("ofdm_start"), pmt::cons(pmt::from_uint64(d_len), pmt::from_uint64(d_encoding)), pmt::string_to_symbol(name()));
	      //out += 64;
	      //o++;
	    }

	    //preamble field remove..
	    if(d_nsym > 3){
	      //std::cout << " out.." << std::endl;
	      //std::memcpy(out + (o * 64), in + (i * 64), 64 * sizeof(gr_complex));
	      /*std::cout << "in......." << std::endl;
	      for (int i = 0; i < 64; i++) {
		std::cout << in[i] << " ";
	      } std::cout << std::endl;
	      gr_complex temp[48];
	      int c = 0;
	      for (int n = 0; n < 64; n++) {
	        if((n == 11) || (n == 25) || (n == 32) || (n == 39) || (n == 53) || (n < 6) || ( n > 58)) {
		  continue;
		}
		else {
		  temp[c] = in[n];
		  c++;
		}
	      }
	      std::cout << "out......." << std::endl;
              for (int i = 0; i < 48; i++) {
                std::cout << temp[i] << " ";
              } std::cout << std::endl;
*/
	      std::memcpy(out, in, 64 * sizeof(gr_complex));
	      //std::memcpy(out, temp, 48*sizeof(gr_complex));
	      /*for(int n = 0; n < 64; n++){
                std::cout << out[n] << " " ;
              }
              std::cout << std::endl;*/

	      out += 64;
	      o++;
	    }
/////////////////////////////////////////////////////////////
	    /*if(d_nsym + 1 > symbol_n[nFrame] + 3) {
	      //std::cout << "same.. same.." << std::endl;
	      d_nsym = 0;
	      nFrame++;
	    }*/
	  }

	  //if(d_nsym > symbol_n[nFrame] + 5) d_nsym = 0;
	  in += 64;
	  i++;
	  this->d_nsym++;
/*
	  if(d_nsym > symbol_n[nFrame] + 4) {
	    d_nsym = 0;
	    nFrame++;
	  }*/
//std::cout << "REENCODING noutput items : " << o << std::endl;
	}
	//nFrame++;
        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume_each (noutput_items);

        // Tell runtime system how many output items we produced.
        return o;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

