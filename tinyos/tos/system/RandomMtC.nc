/*
Random number generator based on the Mersenne Twister PRNG. This algorithm is
slower than the Park Miller PRN_MTG used in RandomMlgC, but has better randomness
properties.

Taken from the latest official c version of the MT PRNG.

SEED MUST BE GREATER THAN_MT 0
*/
#define N_MT 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

module RandomMtC {
  provides {
    interface Init;
    interface ParameterInit<uint32_t> as SeedInit;
    interface Random;
  }
  uses {
    interface Leds;
    interface ReadId48 as IdReader;
  }
}
implementation {
  uint8_t m_id[6];
  static uint32_t mt[N_MT]; /* the array for the state vector  */
  static uint32_t mti = N_MT + 1; /* mti==N_MT+1 means mt[N_MT] is not initialized */;
  void gen_init_states();

  command error_t Init.init() {
    int i;
    uint32_t sum;

    for(;;) {
      call IdReader.read(&m_id[0]);
      for(i = 0; i < 6; i++) {
          sum += m_id[i];
      }
      if(sum != 0)
          break;
    }

    for(i = 3; i >= 0; i--) {
      mt[0] <<= 8;
      mt[0] |= (uint32_t) m_id[i];
    }

    gen_init_states();
    return SUCCESS;
  }

/* Initialize with 32-bit seed */
  command error_t SeedInit.init(uint32_t s) {
    atomic  mt[0] = s;
    gen_init_states();
    return SUCCESS;
  }

  async command uint32_t Random.rand32() {
    uint32_t y;
    static uint32_t mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N_MT) { /* generate N_MT words at one time */
        uint32_t kk;

        if (mti == N_MT + 1) {
          mt[0] = (uint32_t)(TOS_NODE_ID);
          gen_init_states();
        }

        for (kk=0;kk<N_MT-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N_MT-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N_MT)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N_MT-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N_MT-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;

  }

  async command uint16_t Random.rand16() {
    return (uint16_t)call Random.rand32();
  }

  inline void gen_init_states() {
    for (mti = 1; mti < N_MT; mti++) {
        mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        /* for >32 bit machines */
    }
  }
}