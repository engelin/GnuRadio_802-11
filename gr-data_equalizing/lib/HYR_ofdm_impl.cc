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
#include "HYR_ofdm_impl.h"

namespace gr {
  namespace data_equalizing {

    HYR_ofdm::sptr
    HYR_ofdm::make()
    {
      return gnuradio::get_initial_sptr
        (new HYR_ofdm_impl());
    }

    /*
     * The private constructor
     */
    HYR_ofdm_impl::HYR_ofdm_impl()
      : gr::block("HYR_ofdm",
              gr::io_signature::make(1, 1, 64*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, 64*sizeof(gr_complex)))
    {
       this->d_nsym = 0;
       this->f_mac = 0;
       this->d_mac_len = 0;
       this->d_mac_encoding = 0;
       this->d_mac_symbols = 0;
    }

    /*
     * Our virtual destructor.
     */
    HYR_ofdm_impl::~HYR_ofdm_impl()
    {
    }

    bool
    HYR_ofdm_impl::print_signal(char *in) {
      this->d_mac_len = 0;
      this->d_mac_encoding = 0;
      this->d_mac_symbols = 0;

      int r = 0;
      //m_len = 0;
      bool parity = false;
      for (int i = 0; i < 17; i++) {
          parity ^= (bool)in[i];

         if ((i < 4) && in[i]) {
            r = r | (1 << i);
         }

         if (in[i] && (i > 4) && (i < 17)) {
            this->d_mac_len = this->d_mac_len | (1 << (i-5));
         }
      }

      if (parity != (bool)in[17]) {
         std::cout << "SIGNAL: wrong parity" << std::endl;
         return false;
      }

      if (r == 11) {
         this->d_mac_encoding = 0;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 24);
         std::cout << "Encoding: 3 Mbit/s, d_encoding : " << this->d_mac_encoding << " , symbols : " << this->d_mac_symbols << std::endl;;
      }
      else if (r == 15) {
         this->d_mac_encoding = 1;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 36);
         std::cout << "Encoding: 4.5 Mbit/s   ";
      }
      else if (r == 10) {
         this->d_mac_encoding = 2;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 48);
         std::cout << "Encoding: 6 Mbit/s   ";
      }
      else if (r == 14) {
         this->d_mac_encoding = 3;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 72);
         std::cout << "Encoding: 9 Mbit/s   ";
      }
      else if (r ==  9) {
         this->d_mac_encoding = 4;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 96);
         std::cout << "Encoding: 12 Mbit/s   ";
      }
      else if (r == 13) {
         this->d_mac_encoding = 5;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 144);
         std::cout << "Encoding: 18 Mbit/s   ";
      }
      else if (r ==  8) {
         this->d_mac_encoding = 6;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 192);
         std::cout << "Encoding: 24 Mbit/s   ";
      }
      else if (r ==  12) {
         this->d_mac_encoding = 7;
         this->d_mac_symbols = (int) ceil((16 + 8 * this->d_mac_len + 6) / (double) 216);
         std::cout << "Encoding: 27 Mbit/s   ";
      }
      else {
         std::cout << "unknown encoding" << std::endl;
         return false;
      }
      std::cout << "len : " << this->d_mac_len << ", encoding : " << this->d_mac_encoding << ", symbol : " << this->d_mac_symbols << std::endl;
      return true;
   }

    void
    HYR_ofdm_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    HYR_ofdm_impl::general_work (int noutput_items,
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

	//std::cout << "SYMBOLS: input " << ninput_items[0] << " output " << noutput_items << std::endl;

        while((i < ninput_items[0])/* && o < noutput_items*/) {
           get_tags_in_window(tags, 0, i, i + 1, pmt::string_to_symbol("ofdm_start"));

           if(tags.size()) {
	      //std::cout << "nsym : " << this->d_nsym << std::endl;
              this->d_nsym = 0;
           }

           gr_complex eq_out[48];
           if(d_nsym > 1) {
              //channel equalizing
              equalizer((gr_complex*)in, eq_out, d_nsym);
           }

	   //signal field decoding
	   if(d_nsym == 2) {
	      char sig_dem_out[48];
	      char sig_deinter_out[48];
	      char sig_deconv_out[48];
	      int sig_decoded_len;

	      sig_demodulator(eq_out, sig_dem_out);
              sig_deinterleaver(sig_dem_out, sig_deinter_out);

              sig_decoded_len = sig_viterbi_decoder(sig_deinter_out, sig_deconv_out);
	      /*std::cout << "viviviviterbi" << std::endl;
              for(int c = 0; c < sig_decoded_len; c++) {
                std::cout << (int)sig_deconv_out[c] << " ";
                //std::cout << eq_out_r[c] << " ";
              } std::cout << std::endl;*/

              if(print_signal(sig_deconv_out)) {
	         std::cout << "d_mac_len : " << this->d_mac_len << ", d_mac_encoding : " << this->d_mac_encoding << ", d_mac_symbols : " << this->d_mac_symbols << std::endl;

		 ofdm_param ofdm = ofdm_param((Encoding)this->d_mac_encoding);
		 tx_param tx = tx_param(ofdm, this->d_mac_len);
		 std::cout << "tx : " << tx.n_sym << " " << tx.n_encoded_bits << std::endl;

		 if((tx.n_sym <= MAX_SYM) && (tx.n_encoded_bits <= MAX_BITS)) {
		    //d_ofdm = ofdm;
		    //d_tx = tx;
		    copied = 0;
                    //data decoding on
		    f_mac = 1;
		 }
		 else {
		    std::cout << "Dropping frame which is too large (symbols or bits)" << std::endl;
		 }
              }
	   }

	   if((d_nsym > 2) && (d_nsym < 2 + d_mac_symbols + 1) && (f_mac == 1)) {
	   //if((copied < tx.n_sym) && (f_mac == 1)) {
	      //data symbols copy
	      std::memcpy(this->sym + (copied * 48), eq_out, 48 * sizeof(gr_complex));
	      std::memcpy(this->raw_sym + (copied * 64), in, 64 * sizeof(gr_complex));
	      copied++;
	      //std::cout << copied << " ";

	      //frame copy complete..
	      if(copied == d_mac_symbols) {
	         /*for(int c = 0; c < 48*35; c++) {
		    std::cout << sym[c] << " ";
		    if(c%48 == 47) std::cout << std::endl;
		 } std::cout << std::endl;*/

                 char mac_dem_out[48 * copied];
                 char mac_deinter_out[48 * copied];
                 char mac_deconv_out[24 * copied];
		 int mac_deconv_len;

		 //decoding...
		 mac_demodulator(sym, mac_dem_out);
		 mac_deinterleaver(mac_dem_out, mac_deinter_out);
		 mac_deconv_len = mac_viterbi_decoder(mac_deinter_out, mac_deconv_out);
//////////////////////////////////////////////
		 char mac_descrambler_out[mac_deconv_len];
		 mac_descrambler();

		 //data decoding off
		 f_mac = 0;

		 //crc checking (skip service field)
	      }
	   }

	   in += 64;
	   i++;
	   this->d_nsym++;
        }


        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        consume(0, i);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

