// *Really* minimal PCG32 code / (c) 2014 M.E. O'Neill / pcg-random.org
// Licensed under Apache License 2.0 (NO WARRANTY, etc. see website)

#include "rng.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <bcrypt.h>
uint64_t rng_get_entropy(void) {
    uint64_t value;
    if (BCryptGenRandom(NULL, (PUCHAR)&value, sizeof(value), BCRYPT_USE_SYSTEM_PREFERRED_RNG) == 0) {
        return value;
    }
    return (uint64_t)time(NULL); // Fallback
}
#else
#include <fcntl.h>
#include <unistd.h>
uint64_t rng_get_entropy(void) {
    uint64_t value;
    const int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0 && read(fd, &value, sizeof(value)) == sizeof(value)) {
        close(fd);
        return value;
    }
    return (uint64_t)time(NULL); // Fallback
}
#endif

static pcg32_random_t rng;

// Seed complet (state + stream)
void pcg32_srandom_r(const uint64_t initstate, const uint64_t initseq) {
    rng.state = 0U;
    rng.inc = (initseq << 1u) | 1u;
    pcg32_random_r();
    rng.state += initstate;
    pcg32_random_r();
}

// Générateur PCG32
uint32_t pcg32_random_r() {
    const uint64_t oldstate = rng.state;
    rng.state = oldstate * 6364136223846793005ULL + (rng.inc | 1);
    const uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    const uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

// Seed manuel
void rng_seed(const uint64_t seed) {
    pcg32_srandom_r(seed, 0xDEADBEEFCAFEBABEULL);
}

// Seed automatique via entropie système
void rng_seed_auto() {
    const uint64_t seed = rng_get_entropy();
    const uint64_t stream = (uintptr_t)&rng;
    pcg32_srandom_r(seed, stream);
}

// Tirage brut d’un entier 32 bits
uint32_t rng_u32() {
    return pcg32_random_r();
}

// Tirage d'un entier
int rng_int() {
    return (int)(rng_u32() & 0x7FFFFFFF);  // force un positif
}

// Tirage d'un flottant entre [0.0, 1.0]
float rng_float() {
    return (float)rng_u32() / (float)UINT32_MAX;
}

// Tirage d’un double entre [0.0, 1.0]
double rng_double() {
    const uint64_t hi = rng_u32();
    const uint64_t lo = rng_u32();
    const uint64_t combined = ((uint64_t)hi << 32) | lo;
    return (double)combined / (double)UINT64_MAX;
}

// Entier dans [min, max] inclus
int randint(int min, int max) {
    if (min > max) {
        const int tmp = min;
        min = max;
        max = tmp;
    }
    return min + rng_int() % (max - min + 1);
}

// Flottant dans [min, max]
float randfloat(float min, float max) {
    if (min > max) {
        const float tmp = min;
        min = max;
        max = tmp;
    }
    return min + rng_float() * (max - min);
}

// Double dans [min, max]
double randdouble(double min, double max) {
    if (min > max) {
        const double tmp = min; min = max; max = tmp;
    }
    return min + rng_double() * (max - min);
}

_Bool randbool() {
    return rng_u32() & 1;
}
