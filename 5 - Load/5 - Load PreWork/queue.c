/********************************************************************
Copyright 2010-2017 K.C. Wang, <kwang@eecs.wsu.edu>
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

// queue.c file


extern PROC *freeList;
extern PROC proc[NPROC];

PROC *getproc()
{
  PROC *p = freeList;
  if (p){
    freeList = p->next;
  }
  return p;
}

int putproc(PROC *p)
{
  p->next = freeList;
  freeList = p;
}

int enqueue(PROC **queue, PROC *p)
{
  PROC *q  = *queue;
  if (q==0 || p->priority > q->priority){
    *queue = p;
    p->next = q;
    return;
  }
  while (q->next && p->priority <= q->next->priority){
    q = q->next;
  }
  p->next = q->next;
  q->next = p;
}

PROC *dequeue(PROC **queue)
{
  PROC *p = *queue;
  if (p)
    *queue = p->next;
  return p;
}

int printQ(PROC *p)
{
  kprintf("readyQueue = ");
  while(p){
    kprintf("[%d%d]->", p->pid,p->priority);
    p = p->next;
  }
  kprintf("NULL\n");
}

int printSleepList(PROC *p)
{
  printf("sleepList  = ");
   while(p){
     kprintf("[%d%d]->", p->pid,p->event);
     p = p->next;
  }
  kprintf("NULL\n"); 
}

int printList(PROC *p)
{
   kprintf("freeList   = ");
   while(p){
     kprintf("[%d]->", p->pid);
     p = p->next;
  }
  kprintf("NULL\n"); 
}

int addChild(PROC *kid, int id)
{
  if(kid == 0 || id < 0 || id > 8)
  {
    return -1;
  }
  PROC *currun = &proc[id];
  PROC *hold = currun->child;
  if(hold)
  {
    while (hold->sibling)
    {
      hold = hold->sibling;
    }
    hold->sibling = kid;
    kid->parent = currun;
    return 1;
  }
  currun->child = kid;
  kid->parent = currun;
  return 1;
}

removeChild(int cid, int pid)
{
  PROC *currun = &proc[pid]; 
  PROC *hold = currun->child;
  PROC *prev = hold;
  if(hold)
  {
    if(currun->child->pid == cid)
    {
      hold = currun->child;
      currun->child = hold->sibling;
      hold->sibling = 0;
      hold->parent = 0;
      return 1;
    }
    while (hold->sibling)
    {
      if(hold->pid == cid)
      {
        prev->sibling = hold->sibling;
        hold->sibling = 0;
        hold->parent = 0;
        return 1;
      }
      prev = hold;
      hold = hold->sibling;
    }
    return -1;
  }

}

int printBody()
{
  char *status[ ] = {"FREE", "READY", "SLEEP", "ZOMBIE"};
  printf("  ChildList = ");
  if(running)
  {
    PROC *p = running->child;
    // printf("[%d  %s]->\n", p->pid, status[p->status]);
    // p = p->sibling;
    while (p)
    {
      printf("[%d  %s]->", p->pid, status[p->status]);
      p = p->sibling;
    }
    printf("NULL\n");
    return 1;
  }
  kputc('\n');
  return -1;
}