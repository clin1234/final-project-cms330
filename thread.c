// fyi: Latin comments are sprinkled everywhere.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iso646.h>

enum { N = 50 }; // Maximum number of processes
                 // Maximus numerus processorum

struct node
{
   short pcb_index;
  struct node* child;
};

/*
* Standard operations on linked lists
* Normales operationes ligatis tabulatis
*/
static struct node* traverse(struct node* head)
{
  struct node* tmp = head;
  while (tmp->child not_eq NULL)
  {
    tmp = tmp->child;
  }
  return tmp;
}

void append(struct node* head, struct node* data)
{
  struct node* tmp = malloc(sizeof(struct node));
  tmp = traverse(head);
  tmp->child = data;
}

struct pcb1_s
{
   short parent;
  struct node* children;
} pcb1_def = {0, NULL};

typedef struct pcb1_s pcb1;

struct pcb2_s
{
   short parent, first_child, younger_sibling, older_sibling;
} pcb2_def = {-1,-1,-1,-1}; // Invalid values

typedef struct pcb2_s pcb2;

pcb1 processes_l[N] = {{0,NULL}};
pcb2 processes_a[N] = {{-1,-1,-1,-1}};


/*

Function suffixed with _l pertain to the linked list implementation, and 
those with _a pertain to the integers implementation.

*/

// Odio hoc frustrum scribere... 
void create_l( short p)
{
  struct node* tmp = malloc(sizeof(struct node));
  for ( short i = 1; i < N; i++)
  {
    if (processes_l[i].children == NULL)
    {
      processes_l[i].parent = p;
      tmp->pcb_index = i;
      //processes_l[i].children = NULL;
      //processes_l[i].children->pcb_index = i;
      //tmp = procceses_l[i];
      append(processes_l[p].children, tmp);
      break;
    }
  }
}

void destroy_l( short p)
{
  struct node* tmp = processes_l[p].children;
  while (tmp not_eq NULL)
  {
    destroy_l(tmp->child->pcb_index);
    //free(processes_l[tmp->pcb_index]);
    //free();
  }
}

// p: parent process
void create_a( short p)
{
    if (processes_a[p].first_child == -1)
    {
      for ( short i = 1; i < N; i++)
      {
        if (processes_a[i].parent == -1)
        {
          processes_a[p].first_child = i;
          processes_a[i].parent = p;
          break;
        }
      }
    }
    else
    {
      for ( short i = 1; i < N; i++)
      {
        if (processes_a[i].parent == -1)
        {
          processes_a[i].parent = p;
           short tmp = processes_a[p].first_child,
              cur = processes_a[tmp].younger_sibling;
          while (cur not_eq -1)
            cur = processes_a[cur].younger_sibling;
          processes_a[cur].younger_sibling = i;
          processes_a[i].older_sibling = processes_a[cur].younger_sibling;
          break;
        }
      }
    }
}

void destroy_a( short p)
{
  for ( short i = 0; i < N; i++)
    processes_a[i] = pcb2_def;
}

int main()
{
  puts("Linked list implementation");
  time_t start = time(NULL);
  for ( short i = 0; i < 5; i++)
  {
    for ( short j = 0; j < 10; j++)
    {
      create_l(i);
    }
  }
  for ( short i = 0; i < 5; i++) destroy_l(i);
  time_t end = time(NULL);
  
  printf("Elapsed time: %f seconds\n", difftime(start, end));
  
  puts("Integers implementation");
  start = time(NULL);
  for ( short i = 0; i < 5; i++)
  {
    for ( short j = 0; j < 10; j++)
    {
      create_a(i);
    }
  }
  for ( short i = 0; i < 5; i++) destroy_a(i);
  end = time(NULL);
  
  printf("Elapsed time: %f seconds\n", difftime(start, end));
}
