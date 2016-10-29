
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

// Called before test
void narrow_bridge_init(unsigned int num_vehicles_left, unsigned int num_vehicles_right,
        unsigned int num_emergency_left, unsigned int num_emergency_right)
{
	sema_init(&sema0, 2);
	sema_init(&sema1, 2);
	sema_init(&sema_ready, 0);
	// Not implemented
}

void arrive_bridge(enum car_priority prio, enum car_direction dir)
{
	if(dir == 0)
	{
		sema_down(&sema0);
		sema1.value=sema0.value;
	}else{
		sema_down(&sema1);
		sema0.value=sema1.value;
	}

	// Not implemented
}

void exit_bridge(enum car_priority prio, enum car_direction dir)
{
/* Need insert this sema_ready, only for one thread */

	static int end_car=0;
	// Not implemented
	if(dir == 0)
	{
		sema_up(&sema1);
		if(list_empty(&sema0.waiters) && end_car==0)
			sema_up(&sema0);	
	}else{
		sema_up(&sema0);
		if(list_empty(&sema0.waiters) && end_car==0)
			sema_up(&sema1);
	}
		
	end_car++;
	if(end_car==2)	end_car=0;
}