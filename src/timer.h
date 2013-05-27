#ifndef TIMER_H
#define TIMER_H

typedef void (*ttimercb_t)(void);

typedef struct ttimer
{
	uint32_t	 amt;   /* How long the timer is for (ms) */
	uint32_t	 last;	/* The last time the timer was reset (ms) */
	bool		 elapsed;	/* This is set to true if the timer elapsed this frame */
	ttimercb_t	 cb;    /* This will be called when the timer has elapsed */
	bool		 paused; /* Is the timer paused? */
	uint32_t	 paused_time; /* How long the timer has been paused for, should be 0 after the first elapse after pausing */
	struct ttimer	*next;	/* Next timer in the list */
	struct ttimer	*prev;	/* Previous timer in the list */
	
} ttimer_t;/* Note the second "t" to avoid conflict in naming with the standard library */

extern ttimer_t *g_timers;
extern ttimer_t *g_fall_timer;

ttimer_t *add_timer(uint32_t, ttimercb_t);
void remove_timer(ttimer_t*);
void do_timers(void);
void pause_timer(ttimer_t*);
void resume_timer(ttimer_t*);
void modify_timer(ttimer_t*, uint32_t);

#endif
