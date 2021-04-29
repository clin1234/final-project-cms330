// fyi: Latin comments are sprinkled everywhere.

#include <assert.h>
#include <iso646.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

short N; // Maximum number of processes aut maximus numerus processorum
int highest_process_id = 0; // Highest process ID currently allocated
const int invalid_pid = -1;
short i; // Index counter

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
  for (short i = 1; i < N; i++) {
    if (arr[i].parent == invalid_pid) {
      highest_process_id = i;
      arr[i].parent = parent_process_id;
      tmp->pcb_index = i;
      // Appending a node to end of linked list
      if (arr[parent_process_id].children == NULL) {
        arr[parent_process_id].children = tmp;
        arr[parent_process_id].children->child = NULL;
      }
      else{
        struct node *p = arr[parent_process_id].children;
        while (p->child != NULL) p = p->child;
        p->child = tmp;
        tmp->child = NULL;
      }
      break;
    }
  }
}

void destroy_l(pcb1 *arr, short p) {
  struct node *tmp = arr[p].children;
  if (tmp == NULL) return;
  while (tmp not_eq NULL) {
    destroy_l(arr, tmp->pcb_index);
    // free(NULL);
    // free(arr[tmp->pcb_index]);
    // free();
  }
  arr[p].children = NULL;
}

// Sed hoc frustrum magis odio.
// p: parent process
void create_a(pcb2 *arr, short p) {
  if (arr[p].first_child == invalid_pid) {
    for (short i = 1; i < N; i++) {
      // Find first PCB that does not contain a valid parent
      if (arr[i].parent == invalid_pid) {
        arr[p].first_child = i;
        arr[i].parent = p;
        highest_process_id = i;
        break;
      }
    }
  } else {
    for (short i = 1; i < N; i++) {
      if (arr[i].parent == invalid_pid) {
        arr[i].parent = p;
        short tmp = arr[p].first_child, cur = arr[tmp].younger_sibling;
        while (cur not_eq -1)
          cur = arr[cur].younger_sibling;
        arr[cur].younger_sibling = i;
        arr[i].older_sibling = arr[cur].younger_sibling;
        highest_process_id = i;
        break;
      }
    }
  }
}

void destroy_a(pcb2 *arr, short p) {
  if (arr[p].first_child not_eq invalid_pid) {
    arr[p].first_child = invalid_pid;
  }
}

void print_stats_l(pcb1 *arr) {
  for (i = 0; i < N; i++) {
    printf("Process %d: parent %d\n", i, arr[i].parent);
    printf("\tChildren: ");
    for (struct node *t = arr[i].children; t != NULL; t = t->child)
      printf("%d ", t->pcb_index);
    puts("");
  }
}

void print_stats_a(pcb2 *arr) {
  for (i = 0; i < N; i++) {
    printf("Process %d: parent %d\n", i, arr[i].parent);
    if (arr[i].first_child != invalid_pid)
      printf("\tFirst child: %d\n", arr[i].first_child);
    if (arr[i].younger_sibling != invalid_pid)
      printf("\tYounger sibling: %d\n", arr[i].younger_sibling);
    if (arr[i].older_sibling != invalid_pid)
      printf("\tOlder sibling: %d\n", arr[i].older_sibling);
    puts("");
  }
}

int main(int b, char **argv) {

  if (b == 2 && atoi(argv[1]) > 0 && atoi(argv[1]) <= SHRT_MAX) {
    N = atoi(argv[1]);
  } else {
    puts("This program only takes one argument: a positive number to"
         " represent the maximum number of processes.");
    exit(EXIT_FAILURE);
  }

  pcb1 *processes_l = malloc(N * sizeof(pcb1));
  pcb2 *processes_a = malloc(N * sizeof(pcb2));

  unsigned short i;
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
/*
  start = time(NULL);
  srand(time(NULL));
  int random_number = 0;
  for (i = 0; i < N; i++) {
    create_l(processes_l, random_number);
    random_number = rand() % highest_process_id;
  }

  print_stats_l(processes_l);

  puts("Creation complete\n");
  puts("");
  for (i = 0; i < N; i++)
    destroy_l(processes_l, random_number);
  end = time(NULL);

  double linked_elapsed = difftime(end, start) * 1000;
  printf("Elapsed time: %.1f ms\n", linked_elapsed);
  */

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
  print_stats_a(processes_a);
  for (i = 0; i < N; i++)
    destroy_a(processes_a, random_number);
  end = time(NULL);

  double ints_elapsed = difftime(end, start) * 1000;
  printf("Elapsed time: %.1f ms\n", ints_elapsed);
/*
  printf("\nFor %d elements, struct of ints is ", N);
  printf(ints_elapsed > linked_elapsed ? "slower" : "faster");
  printf(" than linked list by %.1f ms.\n",
         fabs(ints_elapsed - linked_elapsed));*/
}
