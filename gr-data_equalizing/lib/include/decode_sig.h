#ifdef DECODESIG_H

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
#define TRUNC_LEN 6

unsigned char stream_2_code[POW_2_L*POW_2_M][N] = {0, };
unsigned int next_state[POW_2_L][POW_2_M] = {0, };

void sig_demodulator(gr_complex *in, char *out) {
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

void sig_deinterleaver(char *in, char *out) {
   double temp[48];
   int inter[48] = {0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,45,1,4,7,10,13,16,19,22,25,28,31,34,37,40,43,46,2,5,8,11,14,17,20,23,26,29,32,35,38,41,44,47};

   for(int i = 0; i < 48; i++) {
     temp[i] = in[inter[i]];
   }
   for(int i = 0; i < 48; i++) {
     out[i] = temp[i];
   }
}

void sig_Binary2Decimal( unsigned char *binary, unsigned int *decimal, unsigned char bit ) {
   unsigned int dec = 0;
   unsigned int t;
   int i;

   for (i = bit - 1, t = 0; i >= 0; i--, t++) {
     dec |= binary[i] << t;
   }
   *decimal = dec;
}

void sig_Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit) {
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

unsigned char sig_Modular2Adder(unsigned char *src, unsigned int idx) {
   unsigned char matrix_add[N][M+L] = {{1, 0, 1, 1, 0, 1, 1}, {1, 1, 1, 1, 0, 0, 1}};//generator

   int i;
   unsigned char result = 0;

   for (i = 0; i < L + M; i++) {
     result = result ^ (src[i] & matrix_add[idx][i]);
   }

   return result&0x01;
}

int sig_viterbi(unsigned char *code, unsigned char *stream) {
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
     sig_Decimal2Binary(j, stream+(i*M), M);

     idx_max_trellis = trellis_back[trellis_path[i]][i+1];
   }

   return ROK;
}

int sig_viterbi_decoder(char *in, char *out) {
   unsigned char code[LEN_CODE] = {0, };
   unsigned char stream[K] = {0, };
   unsigned char bin_i[L+M];
   int ret = ROK;
   int i, j;
   unsigned int t;

   /*********pre processing*********/
   for (i = 0; i < POW_2_L * POW_2_M; i++) {
     sig_Decimal2Binary(i, bin_i, L+M);
     for (j = 0; j < N; j++) {
       stream_2_code[i][j] = sig_Modular2Adder(bin_i, j);
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

   ret = sig_viterbi(code, stream);

   if (ret == ROK) {
     for (i = 0; i < K/2 - TRUNC_LEN; i++) {
       out[i] = stream[i];
     }
   }

   return K/2 - TRUNC_LEN;
}

/*bool print_signal(char *in, int d_len, int d_encoding, int d_copy_symbols) {
   int r = 0;
   d_len = 0;
   bool parity = false;
   for(int i = 0; i < 17; i++) {
      parity ^= (bool)in[i];

      if((i < 4) && in[i]) {
         r = r | (1 << i);
      }

      if(in[i] && (i > 4) && (i < 17)) {
         d_len = d_len | (1 << (i-5));
      }
   }

   if(parity != (bool)in[17]) {
      std::cout << "SIGNAL: wrong parity" << std::endl;
      return false;
   }

   if(r == 11) {
      d_encoding = 0;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 24);
      std::cout << "Encoding: 3 Mbit/s, d_encoding : " << d_encoding << " , symbols : " << d_copy_symbols << std::endl;;
   }
   else if(r == 15) {
      d_encoding = 1;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 36);
      std::cout << "Encoding: 4.5 Mbit/s   ";
   }
   else if(r == 10) {
      d_encoding = 2;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 48);
      std::cout << "Encoding: 6 Mbit/s   ";
   }
   else if(r == 14) {
      d_encoding = 3;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 72);
      std::cout << "Encoding: 9 Mbit/s   ";
   }
   else if(r ==  9) {
      d_encoding = 4;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 96);
      std::cout << "Encoding: 12 Mbit/s   ";
   }
   else if(r == 13) {
      d_encoding = 5;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 144);
      std::cout << "Encoding: 18 Mbit/s   ";
   }
   else if(r ==  8) {
      d_encoding = 6;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 192);
      std::cout << "Encoding: 24 Mbit/s   ";
   }
   else if(r ==  12) {
      d_encoding = 7;
      d_copy_symbols = (int) ceil((16 + 8 * d_len + 6) / (double) 216);
      std::cout << "Encoding: 27 Mbit/s   ";
   }
   else {
      std::cout << "unknown encoding" << std::endl;
      return false;
   }
   return true;
}*/

#else
void sig_demodulator(gr_complex *in, char *out);
void sig_deinterleaver(char *in, char *out);
void sig_Binary2Decimal(unsigned char *binary, unsigned int *decimal, unsigned char bit);
void sig_Decimal2Binary(unsigned int decimal, unsigned char *binary, unsigned char bit);
unsigned char sig_Modular2Adder(unsigned char *src, unsigned int idx);
int sig_viterbi(unsigned char *code, unsigned char *stream);
void sig_viterbi_decoder(char *in, char *out);
bool print_signal(char *in, int d_len, int d_encoding, int d_copy_symbols);

#endif
