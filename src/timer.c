#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "timer.h"

/* Doubly linked list of timers, points to the beginning */
ttimer_t *g_timers;

/* Has a second elapsed? */
ttimer_t *g_second_timer;

/* Add a timer to the list of timers (g_timers) */
ttimer_t *add_timer(uint32_t ms, ttimercb_t cb)
{
	if(g_timers == NULL){/* If this is the first timer */
		g_timers = malloc(sizeof(ttimer_t));

		g_timers->amt = ms;
		g_timers->last = SDL_GetTicks();
		g_timers->cb = cb;
		g_timers->elapsed = false;
		g_timers->next = NULL;
		g_timers->prev = NULL;
		
		return g_timers;
	} else {/* Otherwise add it to the list */
		ttimer_t *p, *n;
		for(p = g_timers; p != NULL && p->next != NULL; p = p->next)
			continue;
		
		n = malloc(sizeof(ttimer_t));
		
		p->next = n;
		n->amt = ms;
		n->last = SDL_GetTicks();
		n->cb = cb;
		n->elapsed = false;
		n->next = NULL;
		n->prev = g_timers;
		g_timers->next = n;
		
		return n;
	}
}

/* Remove a timer from the list (g_timers) */
void remove_timer(ttimer_t *t)
{
	if(t == NULL)
		return;

	t->prev->next = t->next;
	t->next->prev = t->prev;
	
	free(t);
}

/* Loop through the list and deal with each timer */
void do_timers(void)
{
	ttimer_t *p;
	for(p = g_timers; p != NULL; p = p->next)
	{
		uint32_t ctime = SDL_GetTicks();
		
		if(ctime - p->last >= p->amt){/* If the timer has elapsed */
			p->elapsed = true;/* Make sure the elapsed member is set */

			if(p->cb != NULL)
				p->cb();/* Call the callback if it exists */

			p->last = ctime;/* Set the last time checked to now */
		} else {
			p->elapsed = false;
		}
	}
}
