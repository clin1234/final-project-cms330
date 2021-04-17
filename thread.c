#ifdef __STDC_NO_TREADS__
#include <pthread.h>
#else
#include <threads.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const char *const cat = "cat", *const mouse = "mouse";

mtx_t mtx_cr(void) {
  mtx_t m;
  if (mtx_init(&m, mtx_plain) == thrd_error) {
    fprintf(stderr, "Mutex creation failed!\n");
    exit(EXIT_FAILURE);
  }
  return m;
}

void eat(const char const *animal, int animal_num, int bowl_num, int iter) {
	if (strcmp(animal, cat)) _Static_assert(animal_num == 0);
  int seconds = (rand() % 5) + 1; // Low randomness
  thrd_t thr1, thr2;

  printf("%s: %d starts eating for %d seconds: bowl %d", animal, animal_num,
         seconds, bowl_num);
}

int main(int argc, char **argv) {
  char *i = argv[1];
  int iter;

  assert((argc == 1 && (iter = atoi(i)) != 0) &&
         "Only one parameter needed: a number.");

  mtx_t mx = mtx_cr();
  mtx_destroy(&mx);
}
