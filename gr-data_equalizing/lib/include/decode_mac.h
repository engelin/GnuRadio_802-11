#ifdef DECODEMAC_H

#define ROK 0

#define D_K 48*35
#define D_L 6
#define D_POW_2_L 64
#define D_N 2
#define D_POW_2_N 4
#define D_M 1
#define D_POW_2_M 2
#define D_STAGE K/M
#define D_LEN_CODE STAGE*N
#define D_TRUNC_LEN 6

unsigned char m_stream_2_code[D_POW_2_L*D_POW_2_M][D_N] = {0, };
unsigned int m_next_state[D_POW_2_L][D_POW_2_M] = {0, };

void mac_demodulator(gr_complex *in, char *out) {
   double bits[48];
   for(int n = 0; n < 48; n++) {
      bits[n] = std::real(in[n]);
      if(bits[n] < 0) {
         out[n] = 0;
      }
      else if (bits[n] >= 0) {
         out[n] =1;
      }
   }
}

void mac_deinterleaver(char *in, char *out, int n_data_sym, int n_cbps, int n_bpsc) {
   int first[n_cbps];
   int second[n_cbps];
   int s = std::max(n_bpsc/2, 1);

   for(int j = 0; j < n_cbps; j++) {
      first[j] = s * (j / s) + ((j + int(floor(16.0 * j / n_cbps))) % s);
   }
   for(int i = 0; i < n_cbps; i++) {
      second[i] = 16 * i - (n_cbps - 1) * int(floor(16.0 * i / n_cbps));
   }
   for(int i = 0; i < n_data_sym; i++) {
      for(int k = 0; k < n_cbps; k++) {
         out[i * n_cbps + second[first[k]]] = in[i * n_cbps + k];
      }
   }
}

void mac_Binary2Decimal( unsigned char *binary, unsigned int *decimal, unsigned char bit ) {
   unsigned int dec = 0;
   unsigned int t;
   int i;

   for (i = bit - 1, t = 0; i >= 0; i--, t++) {
     dec |= binary[i] << t;
   }
   *decimal = dec;
}

void mac_Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit) {
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

unsigned char mac_Modular2Adder(unsigned char *src, unsigned int idx) {
   unsigned char matrix_add[D_N][D_M+D_L] = {{1, 0, 1, 1, 0, 1, 1}, {1, 1, 1, 1, 0, 0, 1}};//generator

   int i;
   unsigned char result = 0;

   for (i = 0; i < D_L + D_M; i++) {
     result = result ^ (src[i] & matrix_add[idx][i]);
   }

   return result&0x01;
}

int mac_viterbi(unsigned char *code, unsigned char *stream) {
   unsigned int trellis[D_POW_2_L][D_STAGE+1] = {0, };
   unsigned int trellis_back[D_POW_2_L][D_STAGE+1] = {0, };
   unsigned int trellis_path[D_STAGE] = {0, };
   unsigned int bit_check;
   unsigned char *binary_code;
   unsigned int max_trellis = 0;
   unsigned int idx_max_trellis;
   unsigned int t, tt;
   int i, j, v, idx, t_trellis;

   bit_check = D_POW_2_L - 1;

   //calculate trellis value
   for (i = 0; i < STAGE; i++, bit_check = bit_check >> M) {
     binary_code = code + (i*N);
     for (j = 0; j < D_POW_2_L; j++) {

       if(j & bit_check) continue;

       for (v = 0; v < D_POW_2_M; v++) {
         t = m_next_state[j][v];
         tt = j | (v << D_L);

         for (idx = 0, t_trellis = 0; idx < D_N; idx++) {
           t_trellis += binary_code[idx] == m_stream_2_code[tt][idx];
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
     if(max_trellis < trellis[i][D_STAGE]) {
       max_trellis = trellis[i][D_STAGE];
       idx_max_trellis = i;
     }
   }

   for (i = STAGE - 1; i >= 0; i -= 1) {
     trellis_path[i] = idx_max_trellis;
     j = idx_max_trellis >> (D_L - D_M);
     sig_Decimal2Binary(j, stream+(i*D_M), D_M);

     idx_max_trellis = trellis_back[trellis_path[i]][i+1];
   }

   return ROK;
}

int mac_viterbi_decoder(char *in, char *out) {
   unsigned char code[D_LEN_CODE] = {0, };
   unsigned char stream[D_K] = {0, };
   unsigned char bin_i[D_L+D_M];
   int ret = ROK;
   int i, j;
   unsigned int t;

   /*********pre processing*********/
   for (i = 0; i < D_POW_2_L * D_POW_2_M; i++) {
     sig_Decimal2Binary(i, bin_i, D_L+D_M);
     for (j = 0; j < D_N; j++) {
       m_stream_2_code[i][j] = sig_Modular2Adder(bin_i, j);
     }
   }

   for (i = 0; i < D_POW_2_L; i++) {
     for (j = 0; j < D_POW_2_M; j++) {
       t = i >> D_M;
       t |= (j << (D_L-D_M));
       m_next_state[i][j] = t;
     }
   }

   for (i = 0; i < D_LEN_CODE; i++) {
     code[i] = in[i];
   }

   ret = sig_viterbi(code, stream);

   if (ret == ROK) {
     for (i = 0; i < D_K/2 - TRUNC_LEN; i++) {
       out[i] = stream[i];
     }
   }

   return D_K/2 - TRUNC_LEN;
}

void mac_descrambler(char *in, char *out, int n_data) {
   int state = 0;
   for(int i = 0; i < 7; i++) {
      if(in[i]) {
         state |= 1 << (6 - i);
      }
   }

   int feedback;

   for(int i = 7; i < n_data; i++) {
      feedback = ((!!(state & 64))) ^ (!!(state & 8));
      out_bits[i] = feedback ^ (int)in[i];
      state = ((state << 1) & 0x7e) | feedback;
   }

   for(int i = 0; i < n_data; i++) {
      int bit = i % 8;
      int byte = i / 8;
      if(bit == 0) {
        out_bytes[byte] = 0;
      }

      if(out_bits[i]) {
         out_bytes[byte] |= (1 << bit);
      }
   }
}

#else
void mac_demodulator(gr_complex *in, char *out);
void mac_deinterleaver(char *in, char *out);
void mac_Binary2Decimal(unsigned char *binary, unsigned int *decimal, unsigned char bit);
void mac_Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit);
unsigned char mac_Modular2Adder(unsigned char *src, unsigned int idx);
int mac_viterbi(unsigned char *code, unsigned char *stream);
int mac_viterbi_decoder(char *in, char *out);
void mac_descrambler(char *in, char *out);

#endif
