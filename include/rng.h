// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

#ifndef RNG_H
#define RNG_H

#include <stdint.h>

// Structure du générateur PCG32
typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg32_random_t;

void pcg32_srandom_r(uint64_t initstate, uint64_t initseq);
uint32_t pcg32_random_r();

// Initialisation (manual seed or auto entropy)
void rng_seed(uint64_t seed);
void rng_seed_auto();

// Tirages de base
int rng_int();
float rng_float();
double rng_double();

// Tirages dans un intervalle (style Python)
int randint(int min, int max);
float randfloat(float min, float max);
double randdouble(double min, double max);
_Bool randbool();

#endif // RNG_H
