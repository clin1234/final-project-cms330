#include <assert.h>
#include <iso646.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// fyi: Latin comments are sprinkled everywhere.

// Global variables

short N; // Maximum number of processes aut maximus numerus processorum
/* Highest process ID currently allocated. Used for choosing random parent
ID to create and destroy*/
int highest_process_id = 0;
const int invalid_pid = -1;
short i; // Index counter aut numerator
_Bool quiet = false;

struct node {
  short pcb_index;
  struct node *child;
};
// Two structures: one containing a linked list and another a bunch of ints

struct pcb1_s {
  short parent;
  struct node *children;
} pcb1_def = {invalid_pid, NULL};

typedef struct pcb1_s pcb1;

struct pcb2_s {
  short parent, first_child, younger_sibling, older_sibling;
} pcb2_def = {invalid_pid, invalid_pid, invalid_pid, invalid_pid};

typedef struct pcb2_s pcb2;

/*

Function suffixed with _l pertain to the linked list implementation, and those
with _a pertain to the integers implementation.

*/

// Odio hoc frustrum scribere...
void create_l(pcb1 *arr, short parent_process_id) {
  struct node *tmp = malloc(sizeof(struct node));
  for (i = 1; i < N; i++) {
    // Find first available PCB with no parent
    if (arr[i].parent == invalid_pid) {
      highest_process_id = i;
      arr[i].parent = parent_process_id;
      tmp->pcb_index = i;
      // Appending a node to end of linked list
      if (arr[parent_process_id].children == NULL) {
        arr[parent_process_id].children = tmp;
        arr[parent_process_id].children->child = NULL;
      } else {
        struct node *p = arr[parent_process_id].children;
        while (p->child != NULL)
          p = p->child;
        p->child = tmp;
        tmp->child = NULL;
      }
      break;
    }
  }
}

void destroy_l(pcb1 *arr, short p) {
  struct node *tmp = arr[p].children;
  if (tmp == NULL || tmp->child == NULL)
    return;
  else {
    int child_index = tmp->pcb_index;
    destroy_l(arr, child_index);
    free(tmp);
  }
}

// Sed hoc frustrum magis odio.
// p: parent process
void create_a(pcb2 *arr, short p) {
  if (arr[p].first_child == invalid_pid) {
    for (i = 1; i < N; i++) {
      // Find first PCB that does not have a child
      if (arr[i].parent == invalid_pid) {
        arr[p].first_child = i;
        arr[i].parent = p;
        highest_process_id = i;
        break;
      }
    }
  } else {
    for (i = 1; i < N; i++) {
      if (arr[i].parent == invalid_pid) {
        arr[i].parent = p;
        highest_process_id = i;
        // Backtrack array, finding first encountered PCB with same parent as p
        for (int j = i - 1; j != 0; j--) {
          if (arr[j].parent == p) {
            arr[i].older_sibling = j;
            arr[j].younger_sibling = i;
            goto success;
          }
        }
      }
    }
  success:;
  }
}

void destroy_a(pcb2 *arr, short p) {
  if (arr[p].first_child == invalid_pid)
    return;

  if (arr[p].first_child != invalid_pid) {
    short child_index = arr[p].first_child;
    arr[child_index].parent = invalid_pid;
    arr[p].first_child = invalid_pid;
    destroy_a(arr, child_index);
  }
  if (arr[p].younger_sibling != invalid_pid) {
    short younger_index = arr[p].younger_sibling;
    arr[younger_index].parent = invalid_pid;
    arr[p].younger_sibling = invalid_pid;
    destroy_a(arr, younger_index);
  }
  if (arr[p].older_sibling != invalid_pid) {
    short older_index = arr[p].older_sibling;
    arr[older_index].parent = invalid_pid;
    arr[p].older_sibling = invalid_pid;
    destroy_a(arr, older_index);
  }

  // child_index = invalid_pid;
}

// Functions to print details

void print_stats_l(pcb1 *arr) {
  for (i = 0; i < N; i++) {
    printf("Process %d: parent %d\n", i, arr[i].parent);
    if (arr[i].children != NULL) {
      printf("\tChildren: ");
      for (struct node *t = arr[i].children; t != NULL; t = t->child)
        if ((t->pcb_index != invalid_pid) && (t->pcb_index <= N))
          printf("%d ", t->pcb_index);
      puts("");
    }
  }
}

void print_stats_a(pcb2 *arr) {
  for (i = 0; i < N; i++) {
    printf("Process %d: parent %d\n", i, arr[i].parent);
    if (arr[i].first_child != invalid_pid) {
      printf("\tImmediate children: ");
      int j = arr[i].first_child;
      while (j != invalid_pid) {
        printf("%d ", j);
        j = arr[j].younger_sibling;
      }
      puts("");
      printf("\tFirst child: %d\n", arr[i].first_child);
    }
    if (arr[i].younger_sibling != invalid_pid)
      printf("\tYounger sibling: %d\n", arr[i].younger_sibling);
    if (arr[i].older_sibling != invalid_pid)
      printf("\tOlder sibling: %d\n", arr[i].older_sibling);
  }
}

// Haec problema simplex me confundit longiter.
void init(int argc, char **argv) {
  quiet = false;
  if (argc == 3 || argc == 2) {
    if (argc == 3) {
      if (strcmp(argv[2], "-q") == 0) {
        quiet = true;
      }
    }
    long short_max = SHRT_MAX;
    char* end;
    switch(strtol(argv[1], &end, 10))
    {
        case LONG_MIN:
        case LONG_MAX:
        case 0: goto fail;
        default: N = strtol(argv[1], &end, 10);
    }
    if (N < 5 || N > short_max) goto fail;
    return;
  }
  fail:
    // Otherwise
    printf("Usage: %s n [-q]\n "
           "\t-q: (Optional) Don't print out details about each PCB "
           "implementation.\n"
           "\t n: a number between 5 and %d to represent the maximum number"
           " of processes.\n",
           argv[0], SHRT_MAX);
    exit(EXIT_FAILURE);
}

int main(int b, char **argv) {
  init(b, argv);

  pcb1 *processes_l = malloc(N * sizeof(pcb1));
  pcb2 *processes_a = malloc(N * sizeof(pcb2));

  unsigned short i;
  // Initialize both arrays to sensible defaults
  for (i = 0; i < N; i++) {
    processes_l[i] = pcb1_def;
    processes_a[i] = pcb2_def;
  }
  // Handle pcb[0] at start
  processes_l[0].parent = processes_a[0].parent = 0;

  time_t start, end;
  int random_number = 0;

  puts("Linked list implementation\n");

  // Randomness of rand() leaves much to be desired.
  start = time(NULL);
  srand(time(NULL));
  for (i = 0; i < N; i++) {
    create_l(processes_l, random_number);
    random_number = rand() % highest_process_id;
  }

  if (!quiet)
    print_stats_l(processes_l);
  puts("Creation complete");

  destroy_l(processes_l, 0);
  puts("Destuction complete");
  end = time(NULL);

  // Elaped time of linked list mechanism
  double linked_elapsed = difftime(end, start) * 1000;
  printf("Elapsed time: %.1f ms\n\n", linked_elapsed);

  // Reset for other implementation.
  highest_process_id = 0;

  puts("Integers implementation\n");
  start = time(NULL);
  srand(time(NULL));
  random_number = 0;
  for (i = 0; i < N; i++) {
    create_a(processes_a, random_number);
    random_number = rand() % highest_process_id;
  }

  if (!quiet)
    print_stats_a(processes_a);
  puts("Creation complete\n");

  destroy_a(processes_a, 0);
  // if (!quiet) print_stats_a(processes_a);

  puts("Destuction complete");
  end = time(NULL);

  // Elaped time of struct of ints mechanism
  double ints_elapsed = difftime(end, start) * 1000;
  printf("Elapsed time: %.1f ms\n", ints_elapsed);

  printf("\nFor %d processes, struct of ints is ", N);
  printf(ints_elapsed > linked_elapsed ? "slower" : "faster");
  printf(" than linked list by %.1f ms.\n",
         fabs(ints_elapsed - linked_elapsed));
}
