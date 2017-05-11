#ifdef UTILS_H
#define UTILS_H

#include <iostream>
#include <cassert>
#include <cstring>
#include <math.h>

const int MAX_SYM = 520;
const int MAX_BITS = 1550*8*2;

enum Encoding {
        BPSK_1_2 = 0,
        BPSK_3_4 = 1,
        QPSK_1_2 = 2,
        QPSK_3_4 = 3,
        QAM16_1_2 = 4,
        QAM16_3_4 = 5,
        QAM64_2_3 = 6,
        QAM64_3_4 = 7,
};

/**
 * OFDM parameters
 */
class ofdm_param {
public:
        ofdm_param(Encoding e); 

        // data rate
        Encoding encoding;
        // rate field of the SIGNAL header
        char     rate_field;
        // number of coded bits per sub carrier
        int      n_bpsc;
        // number of coded bits per OFDM symbol
        int      n_cbps;
        // number of data bits per OFDM symbol
        int      n_dbps;
};

ofdm_param::ofdm_param(Encoding e) {
        encoding = e;

        switch(e) {
                case BPSK_1_2:
                        n_bpsc = 1;
                        n_cbps = 48;
                        n_dbps = 24;
                        rate_field = 0x0D; // 0b00001101
                        break;

                case BPSK_3_4:
                        n_bpsc = 1;
                        n_cbps = 48;
                        n_dbps = 36;
                        rate_field = 0x0F; // 0b00001111
                        break;

                case QPSK_1_2:
                        n_bpsc = 2;
                        n_cbps = 96;
                        n_dbps = 48;
                        rate_field = 0x05; // 0b00000101
                        break;

                case QPSK_3_4:
                        n_bpsc = 2;
                        n_cbps = 96;
                        n_dbps = 72;
                        rate_field = 0x07; // 0b00000111
                        break;

                case QAM16_1_2:
                        n_bpsc = 4;
                        n_cbps = 192;
                        n_dbps = 96;
                        rate_field = 0x09; // 0b00001001
                        break;

                case QAM16_3_4:
                        n_bpsc = 4;
                        n_cbps = 192;
                        n_dbps = 144;
                        rate_field = 0x0B; // 0b00001011
                        break;

                case QAM64_2_3:
                        n_bpsc = 6;
                        n_cbps = 288;
                        n_dbps = 192;
                        rate_field = 0x01; // 0b00000001
                        break;

                case QAM64_3_4:
                        n_bpsc = 6;
                        n_cbps = 288;
                        n_dbps = 216;
                        rate_field = 0x03; // 0b00000011
                        break;
                defaut:
                        assert(false);
                        break;
        }
}

/**
 * packet specific parameters
 */
class tx_param {
public:
        tx_param(ofdm_param &ofdm, int psdu_length);

        // PSDU size in bytes
        int psdu_size;
        // number of OFDM symbols (17-11)
        int n_sym;
        // number of data bits in the DATA field, including service and padding (17-12)
        int n_data;
        // number of padding bits in the DATA field (17-13)
        int n_pad;
        int n_encoded_bits;
};

tx_param::tx_param(ofdm_param &ofdm, int psdu_length) {

        psdu_size = psdu_length;

        // number of symbols (17-11)
        n_sym = (int) ceil((16 + 8 * psdu_size + 6) / (double) ofdm.n_dbps);

        // number of bits of the data field (17-12)
        n_data = n_sym * ofdm.n_dbps;

        // number of padding bits (17-13)
        n_pad = n_data - (16 + 8 * psdu_size + 6);

        n_encoded_bits = n_sym * ofdm.n_cbps;
}


#endif
