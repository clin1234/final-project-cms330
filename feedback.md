You did a good job on what you implemented, but you have a logic bug in the non-linked list version which causes you to get the incorrect results.

Also, good job recognizing the effects of output and using the -q option to avoid timing output rather than computation. 

In the non-linked list version, you have nested for-loops which are unnecessary and costly (they end up be O(N^2)) for larger N.  The point of having the oldest & youngest integers is to construct a "list" of children which can be traversed just by following the indexes through the process array and setting the newly created process to be the youngest process in the chain.  But you just use a for loop (from 1 to N) searching for the parent and then backtracking.  All of that searching is uncessessary and I'm pretty sure that's where your skewed results are coming from.

Instead, you could do something like this helper function which finds the end of the chain and inserts the newest (youngest) pcb:

```
//p is parent, desiredIndex is index of newest PCB
void helper_method(int p, int desiredIndex) {
    
    //links siblings to each other
    //subsequent children need to point to their older siblings and younger siblings
    //last created child (youngest sibling) points to newest created pcb
    //and newest created pcb sets YS to -1 and establish OS

    struct pcb_no_LL * traverser;
    
    traverser = array_no_LL[array_no_LL[p]->first_child];
    int current_index = array_no_LL[p]->first_child;
    
    while(traverser->younger_sibling != -1) {
        traverser = array_no_LL[traverser->younger_sibling];
    }
    
        traverser->younger_sibling = desiredIndex;
        array_no_LL[traverser->younger_sibling]->older_sibling = current_index;
        return;
}
```

It's really just like a linked list traversal with a bit more bookkeeping required because you're not allocating the new PCB.

Otherwise, nice job.  Your code is very clean and well-structured.  I enjoyed the Latin comments.

85/100