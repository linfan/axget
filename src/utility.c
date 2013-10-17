#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

/* global verbose level */
int verbose = 0;

// Print work information
void echo(int type, const char* format, ...)
{
    va_list args;
    if (type >= verbose)
    {
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

/* time() with more precision                       */
double gettime()
{
    struct timeval time[1];
    gettimeofday(time, 0);
    return((double) time->tv_sec + (double) time->tv_usec / 1000000);
}

/* Whether the rnd system (either rand or [dl]rand48) has been
   seeded.  */
static int rnd_seeded = 0;

/* Return a random number between 0 and MAX-1, inclusive.

   If the system does not support lrand48 and MAX is greater than the
   value of RAND_MAX+1 on the system, the returned value will be in
   the range [0, RAND_MAX].  This may be fixed in a future release.
   The random number generator is seeded automatically the first time
   it is called.

   This uses lrand48 where available, rand elsewhere.  DO NOT use it
   for cryptography.  It is only meant to be used in situations where
   quality of the random numbers returned doesn't really matter.  */

int random_number (int max)
{
#ifdef HAVE_DRAND48
  if (!rnd_seeded)
    {
      srand48 ((long) time (NULL) ^ (long) getpid ());
      rnd_seeded = 1;
    }
  return lrand48 () % max;
#else  /* not HAVE_DRAND48 */

  double bounded;
  int rnd;
  if (!rnd_seeded)
    {
      srand ((unsigned) time (NULL) ^ (unsigned) getpid ());
      rnd_seeded = 1;
    }
  rnd = rand ();

  /* Like rand() % max, but uses the high-order bits for better
     randomness on architectures where rand() is implemented using a
     simple congruential generator.  */

  bounded = (double) max * rnd / (RAND_MAX + 1.0);
  return (int) bounded;

#endif /* not HAVE_DRAND48 */
}
