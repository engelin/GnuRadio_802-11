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
#include "data_equalizing_impl.h"

#include <cfloat>
#include <Eigen/Dense>
#include <itpp/itcomm.h>

/*#define NULLP 0
#define ROK 0

#define K 48
#define L 6
#define POW_2_L 64
#define N 2
#define POW_2_N 4
#define M 1
#define POW_2_M 2
#define STAGE K/M
#define LEN_CODE STAGE*N

unsigned char stream_2_code[POW_2_L*POW_2_M][N] = {0, };
unsigned int next_state[POW_2_L][POW_2_M] = {0, };*/

using namespace Eigen;
using namespace itpp;

namespace gr {
  namespace data_equalizing {

    data_equalizing::sptr
    data_equalizing::make()
    {
      return gnuradio::get_initial_sptr
        (new data_equalizing_impl());
    }

    /*
     * The private constructor
     */
    data_equalizing_impl::data_equalizing_impl()
      : gr::block("data_equalizing",
              gr::io_signature::make2(2, 2, 64 * sizeof(gr_complex), 64 * sizeof(gr_complex)),
              gr::io_signature::make(1, 1, 64 * sizeof(gr_complex)))
    {
      this->d_nsym = 0;
      this->count = 0;
    }

    /*
     * Our virtual destructor.
     */
    data_equalizing_impl::~data_equalizing_impl()
    {
    }

    void
    data_equalizing_impl::data_equalizer(gr_complex *fft_data, gr_complex *re_encoding_data, gr_complex *out) {
      double fft_r[64];
      double fft_i[64];
      double re_r[64];
      double re_i[64];
      double temp_r[64];
      double temp_i[64];
      double out_r[64];
      double out_i[64];
      gr_complex temp[64];

/*      double fft_r[48];
      double fft_i[48];
      double re_r[48];
      double re_i[48];
      double temp_r[48];
      double temp_i[48];
      double out_r[48];
      double out_i[48];
      gr_complex temp[48];*/

      for(int n = 0; n < 64; n++) {
        fft_r[n] = std::real(fft_data[n]);
        fft_i[n] = std::imag(fft_data[n]);
        re_r[n] = std::real(re_encoding_data[n]);
        re_i[n] = std::imag(re_encoding_data[n]);
      }

      std::cout << "channel " << std::endl;
      for(int n = 0; n < 64; n++) {

        temp_r[n] = (fft_r[n] * re_r[n] + fft_i[n] * re_i[n]) / (re_r[n] * re_r[n] + re_i[n] * re_i[n]);
        temp_i[n] = (fft_i[n] * re_r[n] - fft_r[n] * re_i[n]) / (re_r[n] * re_r[n] + re_i[n] * re_i[n]);

        temp[n] = gr_complex(temp_r[n], temp_i[n]);
        std::cout << temp[n] << " ";

        out_r[n] = temp_r[n] * this->first_symbol_r[n] - temp_i[n] * this->first_symbol_i[n];
        out_i[n] = temp_r[n] * this->first_symbol_i[n] + temp_i[n] * this->first_symbol_r[n];


	if((re_r[n] == 0) && (re_i[n] == 0)) {
          out_r[n] = fft_r[n];
          out_i[n] = fft_i[n];
        }

        out[n] = gr_complex(out_r[n], out_i[n]);
      } std::cout << std::endl;
    }

    double
    data_equalizing_impl::gaussianRandom(double average, double stdev) {
      double v1, v2, s, temp;

      do {
	v1 = 2 * ((double)rand() / RAND_MAX) - 1;
	v2 = 2 * ((double)rand() / RAND_MAX) - 1;
	s = v1 * v1 + v2 * v2;
      } while (s >= 1 || s == 0);

      s = sqrt((-2 * log(s)) / s);

      temp = v1 * s;
      temp = (stdev * temp) + average;

      return temp;
    }

    void
    data_equalizing_impl::real_channel(gr_complex *in, gr_complex *out, int n) {
      double in_r[64];
      double in_i[64];
      double out_r[64];
      double out_i[64];
      double ch_r[64];
      double ch_i[64];

      srand(time(NULL));

      std::cout << "channel out" << std::endl;
      for (int i = 0; i < 64; i++) {
        in_r[i] = std::real(in[i]);
        in_i[i] = std::imag(in[i]);

	ch_r[i] = gaussianRandom(0, 1);
	ch_i[i] = gaussianRandom(0, 1);
      }

      for (int i = 0; i < 64; i++) {
        //out_r[i] = (n + 1) * (in_r[i] - in_i[i]);
        //out_i[i] = (n + 1) * (in_r[i] + in_i[i]);
	out_r[i] = in_r[i] * ch_r[i] - in_i[i] * ch_i[i];
	out_i[i] = in_r[i] * ch_i[i] + in_i[i] * ch_r[i];

        out[i] = gr_complex(out_r[i], out_i[i]);
        std::cout << out[i] << " ";
      } std::cout << std::endl;
    }


    void
    data_equalizing_impl::equalizer(gr_complex *in, gr_complex *out, int n) {
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


    void
    data_equalizing_impl::deinterleave(char* bits, char* deinter) {
      double temp[48];
      int inter[48] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,1,4,7,10,13,16,19,22,25,28,31,34,37,40,43,46,2,5,8,11,14,17,20,23,26,29,32,35,38,41,44,47};
      for(int i = 0; i < 48; i++) {
        temp[i] = bits[inter[i]];
      }
      for(int i = 0; i < 48; i++) {
        deinter[i] = temp[i];
      } 

/*      int n_cbps = 48;
      int first[n_cbps];
      int second[n_cbps];
      int s = 1;

      for(int j = 0; j < n_cbps; j++) {
        first[j] = s * (j / s) + ((j + int(floor(16.0 * j / n_cbps))) % s);
      }

      for(int i = 0; i < n_cbps; i++) {
        second[i] = 16 * i - (n_cbps - 1) * int(floor(16.0 * i / n_cbps));
      }

      for(int i = 0; i < 35; i++) {
        for(int k = 0; k < n_cbps; k++) {
          deinter[i * n_cbps + second[first[k]]] = bits[i * n_cbps + k];
        }
      }
*/

/*      for(int i = 0; i < 48; i++) {
        std::cout << (int)deinter[i];
	if(i%48 == 47) std::cout << std::endl;
      } //std::cout << std::endl;*/
    }

/*
    void
    data_equalizing_impl::Binary2Decimal( unsigned char *binary, unsigned int *decimal, unsigned char bit ) {
      unsigned int dec = 0;
      unsigned int t;
      int i;

      for (i = bit - 1, t = 0; i >= 0; i--, t++) {
	dec |= binary[i] << t;
      }
      *decimal = dec;
    }

    void
    data_equalizing_impl::Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit) {
      unsigned int t = 1;
      int i;

      for (i = 0; i < bit - 1; i++) {
	t = t << 1;
      }

      while (t > 0) {
	*(binary++) = (unsigned char)(decimal/t);
	decimal %= t;
	t /= 2;
      }
    }

    unsigned char
    data_equalizing_impl::Modular2Adder(unsigned char *src, unsigned int idx) {
      unsigned char matrix_add[N][M+L] = {{1, 0, 1, 1, 0, 1, 1}, {1, 1, 1, 1, 0, 0, 1}};//generator

      int i;
      unsigned char result = 0;

      for (i = 0; i < L + M; i++) {
	result = result ^ (src[i] & matrix_add[idx][i]);
      }

      return result&0x01;
    }
    

    int
    data_equalizing_impl::viterbi_decode(unsigned char *code, unsigned char *stream) {
      unsigned int trellis[POW_2_L][STAGE+1] = {0, };
      unsigned int trellis_back[POW_2_L][STAGE+1] = {0, };
      unsigned int trellis_path[STAGE] = {0, };
      unsigned int bit_check;
      unsigned char *binary_code;
      unsigned int max_trellis = 0;
      unsigned int idx_max_trellis;
      unsigned int t, tt;
      int i, j, v, idx, t_trellis;

      bit_check = POW_2_L - 1;

      //calculate trellis value
      for (i = 0; i < STAGE; i++, bit_check = bit_check >> M) {
	binary_code = code + (i*N);
	for (j = 0; j < POW_2_L; j++) {

	  if(j & bit_check) continue;

	  for (v = 0; v < POW_2_M; v++) {
	    t = next_state[j][v];
	    tt = j | (v << L);

	    for (idx = 0, t_trellis = 0; idx < N; idx++) {
	      t_trellis += binary_code[idx] == stream_2_code[tt][idx];
	    }

	    if(trellis[t][i+1] < trellis[j][i] + t_trellis) {
	      trellis[t][i+1] = trellis[j][i] + t_trellis;
	      trellis_back[t][i+1] = j;
	    }
	  }
	}
      }

      //fine trellis path
      max_trellis = 0;
      for (i = 0; i < POW_2_L; i++) {
	if(max_trellis < trellis[i][STAGE]) {
	  max_trellis = trellis[i][STAGE];
	  idx_max_trellis = i;
	}
      }

      for (i = STAGE - 1; i >= 0; i -= 1) {
	trellis_path[i] = idx_max_trellis;
	j = idx_max_trellis >> (L - M);
	Decimal2Binary(j, stream+(i*M), M);

	idx_max_trellis = trellis_back[trellis_path[i]][i+1];
      }

      return ROK;
    }

    void
    data_equalizing_impl::convolution_decoder(char *in, char *out) {
      unsigned char code[LEN_CODE] = {0, };
      unsigned char stream[K] = {0, };
      unsigned char bin_i[L+M];
      int ret = ROK;
      int i, j;
      unsigned int t;

      //pre processing
      for (i = 0; i < POW_2_L * POW_2_M; i++) {
	Decimal2Binary(i, bin_i, L+M);
	for (j = 0; j < N; j++) {
	  stream_2_code[i][j] = Modular2Adder(bin_i, j);
	}
      }

      for (i = 0; i < POW_2_L; i++) {
	for (j = 0; j < POW_2_M; j++) {
	  t = i >> M;
	  t |= (j << (L-M));
	  next_state[i][j] = t;
	}
      }

      for (i = 0; i < LEN_CODE; i++) {
	code[i] = in[i];
      }

      ret = viterbi_decode(code, stream);

      if (ret == ROK) {
	for (i = 0; i < K; i++) {
	  out[i] = stream[i];
	}
      }
    }
*/
    void
    data_equalizing_impl::descrambler (char *in, char *out) {
      int state = 0;
      //int state = 93;
      for (int i = 0; i < 7; i++) {
	if(in[i]) state |= 1 << (6 - i);
      }

      int feedback;

      for (int i = 0; i < 24; i++) {
	feedback = ((!!(state & 64)) ^ (!!(state & 8)));
	out[i] = feedback ^ (int)in[i];
	state = ((state << 1) & 0x7e) | feedback;
      }
    }


    void
    data_equalizing_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
        /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    data_equalizing_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const gr_complex *in_re_data = (const gr_complex *) input_items[0];
	const gr_complex *in_fft_data = (const gr_complex *) input_items[1];
        gr_complex *out = (gr_complex *) output_items[0];

	int symbols_to_read = 0;
	int ninput = std::min(ninput_items[0], ninput_items[1]);
	symbols_to_read = std::min(noutput_items, ninput);
	noutput_items = symbols_to_read;

	int i = 0;
	int o = 0;

	std::vector<gr::tag_t> tags;
	const uint64_t nread = nitems_read(0);

	//while((i < ninput) && (o < noutput_items)) {
	while(i < ninput) {
	  //std::cout << "input re_encoding data num: "  << ninput_items[0] << " fft data num : " << ninput_items[1] << std::endl;
	  get_tags_in_window(tags, 0, nread + i, nread + symbols_to_read);

	  if(tags.size()) {
	    this->d_nsym = 0;
	    //std::cout << "new Frame.." << std::endl;
	  }

	  //if(this->d_nsym  == 0) {
	  if(this->count == 0) {
	    for(int n = 0; n < 64; n++) {
	      this->first_symbol[n] = std::complex<float>(in_re_data[n]);
	      this->first_symbol_r[n] = std::real(in_re_data[n]);
	      this->first_symbol_i[n] = std::imag(in_re_data[n]);
	    }
	    //std::cout << "first_symbol copy.." << std::endl;
	  }

	  /*equalizer((gr_complex *)in_fft_data, this->temp_fft, ninput);
	  std::cout << "\n" << o << "th fft_e out : " << std::endl;
          for(int n = 0; n < 64; n++) {
	    std::cout << this->temp_fft[n] << " ";
	  }std::cout << std::endl;*/

	  //real_channel((gr_complex *)in_fft_data, temp, o);

	  /*std::cout << "fft input" << std::endl;
	  for(int n = 0; n < 64; n++) {
	    std::cout << in_fft_data[n] << " ";
	  } std::cout << std::endl;*/

	  data_equalizer((gr_complex *)in_fft_data, (gr_complex *)in_re_data, out);

	  std::cout << o << "th d_e out : " << std::endl;
          for(int n = 0; n < 64; n++) {
            std::cout << out[n] << " ";
	  }std::cout << std::endl;

	  equalizer(out, this->temp, ninput);
          /*std::cout << "\n" << o << "th eq out : " << std::endl;
          for(int n = 0; n < 64; n++) {
            std::cout << this->temp[n] << " ";
          }std::cout << std::endl;*/

/*	  double bits[48];
	  char m_bits[48];
	  std::cout << o << " bpsk demodulation.." << std::endl;
	  for(int n = 0; n < 48; n++) {
	    bits[n] = std::real(this->temp[n]);
	    //bits[n] = std::real(out[n]);
	    if (bits[n] < 0) {
	      m_bits[n] = 0;
	    }
	    else if (bits[n] >= 0){
	      m_bits[n] = 1;
	    }
	    std::cout << (int)m_bits[n] << " ";
	  } std::cout << std::endl;

	  char i_bits[48];
	  deinterleave(m_bits, i_bits);

	  char conv_bits[24];
	  convolution_decoder(i_bits, conv_bits);
	  //std::cout << "conv : ";
	  for (int n = 0; n < 24; n++) {
	    std::cout << (int)conv_bits[n];
	  } //std::cout << std::endl;

	  char descrambled_bits[24];
	  descrambler(conv_bits, descrambled_bits);
	  //std::cout << "descramble : ";
	  for (int n = 0; n < 24; n++) {
	    std::cout << (int)descrambled_bits[n] << " ";
	  } std::cout << "\n" << std::endl;
*/
/*	  std::cout << o << "th out : " << std::endl;
	  for(int n = 0; n < 64; n++) {
	    std::cout << this->temp[n] << " ";
	  }std::cout << "\n" << std::endl;*/

	  this->count++;
	  if(this->count == ninput_items[0]) {
	    this->count = 0;
	  }

	  out += 64;
	  o++;
	  in_fft_data += 64;
	  in_re_data += 64;
	  i++;
	  this->d_nsym++;
	}

        // Do <+signal processing+>
        // Tell runtime system how many input items we consumed on
        // each input stream.
        //consume_each (noutput_items);
	consume(0, i);
	consume(1, i);

        // Tell runtime system how many output items we produced.
        //return noutput_items;
	return o;
    }

  } /* namespace data_equalizing */
} /* namespace gr */

