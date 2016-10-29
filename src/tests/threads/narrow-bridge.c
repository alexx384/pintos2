
/* File for 'narrow_bridge' task implementation.  
   Only output required. 
   SPbSTU, IBKS, 2016 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "narrow-bridge.h"

static struct semaphore sema0;
static struct semaphore sema1;
static struct semaphore sema_ready;
static int count_of_cars=0;

static list_less_func compare;

static int thread_normal_left=0;
static int thread_normal_right=1;

int choose_the_side(enum car_direction dir);

struct kernel_thread_frame 
  {
    void *eip;                  /* Return address. */
    thread_func *function;      /* Function to call. */
    void *aux;                  /* Auxiliary data for function. */
  };

// Called before test
void narrow_bridge_init(unsigned int num_vehicles_left, unsigned int num_vehicles_right,
        unsigned int num_emergency_left, unsigned int num_emergency_right)
{
	sema_init(&sema0, 0);			//For first side
	sema_init(&sema1, 0);			//For second side
	sema_init(&sema_ready, 0);		//For ready car
	count_of_cars+=num_vehicles_left + num_vehicles_right +
				  num_emergency_left + num_emergency_right;
	// Not implemented
}

void arrive_bridge(enum car_priority prio, enum car_direction dir)
{
	thread_current()->car=prio*2+dir;

	count_of_cars--;

	if(count_of_cars==0)
	{
		count_of_cars=choose_the_side(dir);
		if(count_of_cars==0)
		{
			sema_up(&sema0);
			sema_up(&sema0);
		}else{
			sema_up(&sema1);
			sema_up(&sema1);
		}
	}

	if(dir == 0)					//sort cars by sides
		sema_down(&sema0);
	else
		sema_down(&sema1);
	


	// Not implemented
}

void exit_bridge(enum car_priority prio, enum car_direction dir)
{
	static int end_car=0;							//show count of cars, how is here on one transit
	end_car++;
	// Not implemented
	if(end_car == 1)	sema_down(&sema_ready);		//if there is one car, then we are waiting second car 
	else				sema_up(&sema_ready);

	if(dir == 0)									//Change direction every time
	{
		sema_up(&sema1);
		if(list_empty(&sema0.waiters) && end_car==1)//if no one car from other side
			sema_up(&sema0);	
	}else{
		sema_up(&sema0);
		if(list_empty(&sema0.waiters) && end_car==1)//if no one car from other side
			sema_up(&sema1);
	}
		
	if(end_car==2)	end_car=0;						//if all cars are there
}

int choose_the_side(enum car_direction dir)
{
	struct thread *cur=thread_current(), *dump;
	struct list_elem *element;

	
	if(cur->car == thread_normal_left ||
	 cur->car == thread_normal_right)
		return dir-1;

	list_sort (&sema0.waiters,	compare, NULL);
	list_sort (&sema1.waiters,	compare, NULL);


	if(dir == 0)
	{
		if(!list_empty(&sema0.waiters))
		{
			element=list_front (&sema0.waiters);
			dump=list_entry (element, struct thread, elem);
			if(cur->car == dump->car)
				return dir;			
		}
	}else{
		if(!list_empty(&sema1.waiters))
		{
			element=list_front (&sema1.waiters);
			dump=list_entry (element, struct thread, elem);
			if(cur->car == dump->car)
				return dir;			
		}
	}
	return dir-1;
}

static bool 
compare (const struct list_elem *a,
      const struct list_elem *b,
      void *aux)
{
  struct thread *t = list_entry (a, struct thread, elem);
  struct thread *g = list_entry (b, struct thread, elem);
//  msg("t(%d) = %d\tg(%d) = %d",t->tid,t->car, t->tid, g->car);
  
  if(t->car > g->car)
  	return false;
  else
  	return true;
}