#ifdef EQUALIZER_H
   void equalizer(gr_complex *in, gr_complex *out, int n) {
      gr_complex polarity[127] = {
         1, 1, 1, 1,-1,-1,-1, 1,-1,-1,-1,-1, 1, 1,-1, 1,
        -1,-1, 1, 1,-1, 1, 1,-1, 1, 1, 1, 1, 1, 1,-1, 1,
         1, 1,-1, 1, 1,-1,-1, 1, 1, 1,-1, 1,-1,-1,-1, 1,
        -1, 1,-1,-1, 1,-1,-1, 1, 1, 1, 1, 1,-1,-1, 1, 1,
        -1,-1, 1,-1, 1,-1, 1, 1,-1,-1,-1, 1, 1,-1,-1,-1,
        -1, 1,-1,-1, 1,-1, 1, 1, 1, 1,-1, 1,-1, 1,-1, 1,
        -1,-1,-1,-1,-1, 1,-1, 1, 1,-1, 1,-1, 1, 1, 1,-1,
        -1, 1,-1,-1,-1, 1, 1, 1,-1,-1,-1,-1,-1,-1,-1
      };

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

#else
   void equalizer(gr_complex *in, gr_complex *out, int n);

#endif
