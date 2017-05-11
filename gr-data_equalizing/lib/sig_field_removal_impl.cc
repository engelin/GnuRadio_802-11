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
#include "sig_field_removal_impl.h"

#define CRC_FRAME 40

namespace gr {
  namespace data_equalizing {

    sig_field_removal::sptr
    sig_field_removal::make()
    {
      return gnuradio::get_initial_sptr
        (new sig_field_removal_impl());
    }

    //int d_nsym;
    //int frame_num[10] = {62, 64, 111, 243, 245, 249, 255, 258, 272, 557};
    int frame_num[10] = {76, 79, 144, 323, 328, 334, 341, 345, 361, 727};
    int symbol_num[10] = {3, 2, 2, 2, 35, 35, 35, 2, 2, 3};
    //gr::thread::mutex d_mutex;

    /*
     * The private constructor
     */
    sig_field_removal_impl::sig_field_removal_impl()
      : gr::block("sig_field_removal",
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)),
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)))
    {
      this->d_nsym = 0;
      this->nframe = 0;
    }

    /*
     * Our virtual destructor.
     */
    sig_field_removal_impl::~sig_field_removal_impl()
    {
    }

    void
    sig_field_removal_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int cnt = 0;

    int
    sig_field_removal_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
	//gr::thread::scoped_lock lock(d_mutex);
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

	int i = 0;
	int o = 0;

	std::vector<gr::tag_t> tags;
	const uint64_t nread = nitems_read(0);

	while((i < ninput_items[0]) && (o < noutput_items)) {
	  get_tags_in_range(tags, 0, nread + i, nread + i + 1, pmt::string_to_symbol("ofdm_start"));

	  // new Wifi frame
	  if(tags.size()) {
	    std::cout << "cnt : " << cnt << " , nsym : " << this->d_nsym << std::endl;
	    cnt++;
	    this->d_nsym = 0;
	  }

/*	  for(int n = 0; n < 64; n++) {
            std::cout << in[n] << " ";
          } std::cout << cnt << "///" << this->d_nsym << "\n" << std::endl;*/



	  //if(cnt == frame_num[this->nframe]) {
	  if(cnt == CRC_FRAME) {
	    //std::cout << "nsym : " << this->d_nsym << std::endl;
	    //std::cout << "frame num : " << nframe << ", cnt : " << cnt << std::endl;
	    if(this->d_nsym == 2) {
	      add_item_tag(0, nitems_written(0) + o, pmt::string_to_symbol("frame_start"), pmt::string_to_symbol(name()));
	    }
	    //if((this->d_nsym > 2) && (this->d_nsym < symbol_num[this->nframe] + 3)) {
	    if((this->d_nsym > 2) && (this->d_nsym < 38)) {
	      //std::cout << cnt << "th crc out" << std::endl;
                for(int n = 0; n < 64; n++) {
		  out[n] = in[n];
                  //std::cout << out[n] << " ";
                } //std::cout << std::endl;
	      o++;
	      out += 64;
	    }
	    else if(this->d_nsym == symbol_num[this->nframe] + 3) {
	      //std::cout << "nframe : " << nframe << std::endl;
	      this->nframe++;
	    }
	    //this->nframe++;
	  }

	  in += 64;
	  i++;
	  this->d_nsym++;
	}


        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        //consume_each (noutput_items);
	consume(0, i);

        // Tell runtime system how many output items we produced.
        //return noutput_items;
	return o;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

