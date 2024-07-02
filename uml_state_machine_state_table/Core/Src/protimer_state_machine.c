/*
 * protimer_state_machine.c
 *
 *  Created on: Jun 19, 2024
 *      Author: dnk121
 */

#include "main.h"
#include <stdio.h>
#include <string.h>

static uint32_t last_tick = 0;
static uint8_t countdown_active = 0; // Flag to control countdown

static void display_time(uint32_t time);

event_status_t protimer_state_machine(protimer_t *const mobj, event_t const *const e);
//static event_status_t protimer_state_handler_IDLE(protimer_t *const mobj, event_t const *const e);
//static event_status_t protimer_state_handler_TIME_SET(protimer_t *const mobj, event_t const *const e);
//static event_status_t protimer_state_handler_COUNTDOWN(protimer_t *const mobj, event_t const *const e);
//static event_status_t protimer_state_handler_PAUSE(protimer_t *const mobj, event_t const *const e);
//static event_status_t protimer_state_handler_STAT(protimer_t *const mobj, event_t const *const e);

void protimer_init(protimer_t *mobj)
{
    printf("\r\nProtimer application...\r\n");
    event_t ee;
    e_handler_t ehandler;
    ee.sig = ENTRY;
    mobj->active_state = IDLE;
    mobj->pro_time = 0;
    ehandler = (e_handler_t) mobj->state_table[IDLE * MAX_SIGNALS + ENTRY];
    (* ehandler)(mobj, &ee);

//    protimer_state_machine(mobj, &ee);
}

//event_status_t protimer_state_machine(protimer_t *const mobj, event_t const *const e) {
//    switch (mobj->active_state) {
//        case IDLE:
//            return protimer_state_handler_IDLE(mobj, e);
//        case TIME_SET:
//            return protimer_state_handler_TIME_SET(mobj, e);
//        case COUNTDOWN:
//            return protimer_state_handler_COUNTDOWN(mobj, e);
//        case PAUSE:
//            return protimer_state_handler_PAUSE(mobj, e);
////        case STAT:
////            return protimer_state_handler_STAT(mobj, e);
//        default:
//            return EVENT_IGNORED;
//    }
//}

event_status_t IDLE_Entry(protimer_t *const mobj, event_t const *const e)
{
	printf("\r\nChoose a number\r\n");
	printf(" 1. Increment \n 2. Decrement \n 3. Countdown/Pause \n 4. Abort \n ");
    mobj->curr_time = 0;
    mobj->elapsed_time = 0;
    printf("\r\nSet Time\r\n");
    return EVENT_HANDLED;
}

event_status_t IDLE_Exit(protimer_t *const mobj, event_t const *const e)
{
//    printf("Exit1\r\n");
    return EVENT_HANDLED;
}

event_status_t IDLE_Inc_time(protimer_t *const mobj, event_t const *const e)
{
    mobj->curr_time += 60;
    mobj->active_state = TIME_SET;
    return EVENT_TRANSITION;
}

event_status_t IDLE_Time_tick(protimer_t *const mobj, event_t const *const e)
{
    if (((protimer_tick_event_t *)(e))->ss == 5) {
        printf("Time-tick event\r\n");
        return EVENT_HANDLED;
    }
    return EVENT_IGNORED;
}

//static event_status_t protimer_state_handler_IDLE(protimer_t *const mobj, event_t const *const e) {
//    switch (e->sig) {
//        case ENTRY:
//        {
//        	printf("1. increment \n 2. decrement \n 3. countdown \n 4. start countdown \n 5. abort \n ");
//            mobj->curr_time = 0;
//            mobj->elapsed_time = 0;
//            printf("\r\nSet Time\r\n");
//            return EVENT_HANDLED;
//        }
//        case EXIT:
//        {
//            printf("Exit1\r\n");
//            return EVENT_HANDLED;
//        }
//        case INC_TIME:
//        {
//            mobj->curr_time += 60;
//            mobj->active_state = TIME_SET;
//            return EVENT_TRANSITION;
//        }
////        case START_PAUSE:
////        {
////            mobj->active_state = STAT;
////            return EVENT_TRANSITION;
////        }
//        case TIME_TICK:
//        {
//            if (((protimer_tick_event_t *)(e))->ss == 5) {
//                printf("Time-tick event\r\n");
//                return EVENT_HANDLED;
//            }
//            return EVENT_IGNORED;
//        }
//        default:
//            return EVENT_IGNORED;
//    }
//}

event_status_t TIME_SET_Entry(protimer_t *const mobj, event_t const *const e)
{
    display_time(mobj->curr_time);
    return EVENT_HANDLED;
}

event_status_t TIME_SET_Exit(protimer_t *const mobj, event_t const *const e)
{
    printf("\r\nExit2..\r\n");
    return EVENT_HANDLED;
}

event_status_t TIME_SET_Inc_time(protimer_t *const mobj, event_t const *const e)
{
    mobj->curr_time += 60;
    display_time(mobj->curr_time);
    return EVENT_HANDLED;
}

event_status_t TIME_SET_Dec_time(protimer_t *const mobj, event_t const *const e)
{
    if(mobj->curr_time >= 60) {
        mobj->curr_time -= 60;
        display_time(mobj->curr_time);
        return EVENT_HANDLED;
    }
    return EVENT_IGNORED;
}

event_status_t TIME_SET_Abrt(protimer_t *const mobj, event_t const *const e)
{
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}

event_status_t TIME_SET_Start_pause(protimer_t *const mobj, event_t const *const e)
{
    if(mobj->curr_time >= 60) {
        mobj->active_state = COUNTDOWN;
        return EVENT_TRANSITION;
    }
    return EVENT_IGNORED;
}


//
//static event_status_t protimer_state_handler_TIME_SET(protimer_t *const mobj, event_t const *const e) {
//    switch(e->sig) {
//        case ENTRY:
//        {
//            display_time(mobj->curr_time);
//            return EVENT_HANDLED;
//        }
//        case EXIT:
//        {
//            printf("\r\nExit2..\r\n");
//            return EVENT_HANDLED;
//        }
//        case INC_TIME:
//        {
//            mobj->curr_time += 60;
//            display_time(mobj->curr_time);
//            return EVENT_HANDLED;
//        }
//        case DEC_TIME:
//        {
//            if(mobj->curr_time >= 60) {
//                mobj->curr_time -= 60;
//                display_time(mobj->curr_time);
//                return EVENT_HANDLED;
//            }
//            return EVENT_IGNORED;
//        }
//        case ABRT:
//        {
//            mobj->active_state = IDLE;
//            return EVENT_TRANSITION;
//        }
//        case START_PAUSE:
//        {
//            if(mobj->curr_time >= 60) {
//                mobj->active_state = COUNTDOWN;
//                return EVENT_TRANSITION;
//            }
//            return EVENT_IGNORED;
//        }
//    }
//
//    return EVENT_IGNORED;
//}

event_status_t COUNTDOWN_Entry(protimer_t *const mobj, event_t const *const e)
{
    printf("\r\nCountdown Started\r\n");
    return EVENT_HANDLED;
}

event_status_t COUNTDOWN_Exit(protimer_t *const mobj, event_t const *const e)
{
//	printf("Exit Countdown\r\n");
	return EVENT_HANDLED;
}

event_status_t COUNTDOWN_Start_pause(protimer_t *const mobj, event_t const *const e)
{
    printf("\r\nPause Countdown\r\n");
    mobj->active_state = PAUSE;
    return EVENT_TRANSITION;
}

event_status_t COUNTDOWN_abrt(protimer_t *const mobj, event_t const *const e)
{
    printf("\r\nAbort Countdown\r\n");
    mobj->curr_time = 0;
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}

event_status_t COUNTDOWN_Time_tick(protimer_t *const mobj, event_t const *const e)
{
    printf("Time Tick Event\r\n");

    if (mobj->curr_time > 0) {
        --mobj->curr_time;
        display_time(mobj->curr_time);
    } else {
        printf("Countdown Finished\r\n");
        mobj->active_state = IDLE;
        return EVENT_TRANSITION;
    }
    HAL_Delay(1000);
    return EVENT_HANDLED;
}


//static event_status_t protimer_state_handler_COUNTDOWN(protimer_t *const mobj, event_t const *const e)
//{
//    static uint32_t last_tick = 0;
//    static uint8_t countdown_active = 0; // Flag to control countdown
//
//    switch(e->sig)
//    {
//        case ENTRY:
//        {
//            printf("Ready for Countdown....Press 4 to start countdown \r\n");
//            last_tick = millis(); // Get the current time
//            countdown_active = 1; // Start countdown automatically
//            return EVENT_HANDLED;
//        }
//        case EXIT:
//        {
//            printf("Exit Countdown\r\n");
//            countdown_active = 0; // Reset countdown flag
//            return EVENT_HANDLED;
//        }
//
//        case START_PAUSE:
//        {
//            printf("Pause Countdown\r\n");
//            countdown_active = 0; // Pause countdown
//            return EVENT_HANDLED;
//        }
//        case ABRT:
//        {
//            printf("Abort Countdown\r\n");
//            countdown_active = 0; // Stop countdown
//            mobj->curr_time = 0;
//            mobj->active_state = IDLE;
//            return EVENT_TRANSITION;
//        }
//
//        case TIME_TICK: // Handle time tick event
//        {
//            if (countdown_active && millis() - last_tick >= 1000) // Check if 1 second has elapsed
//            {
//                last_tick = millis(); // Update last tick time
//
//                while(1){
//                if (mobj->curr_time > 0)
//                {
//                    printf("Remaining Time: %d seconds\r\n", mobj->curr_time);
//                    --mobj->curr_time;
//                    HAL_Delay(1000);
//                }
//                else
//                {
//                    printf("Countdown Finished\r\n");
//                    mobj->active_state = IDLE;
//                    return EVENT_TRANSITION;
//                }
//                }
//            }
//            return EVENT_HANDLED;
//        }
//    }
//    return EVENT_IGNORED;
//}

//
//static event_status_t protimer_state_handler_STAT(protimer_t *const mobj, event_t const *const e) {
//    static uint8_t tick_count;
//
//    switch(e->sig) {
//        case ENTRY:
//        {
//            display_time(mobj->pro_time);
//            printf("Productive time");
//            return EVENT_HANDLED;
//        }
//        case EXIT:
//        {
//            printf("Exit3..\r\n");
//            return EVENT_HANDLED;
//        }
//        case TIME_TICK:
//        {
//            if(++tick_count == 30) {
//                tick_count = 0;
//                mobj->active_state = IDLE;
//                return EVENT_TRANSITION;
//            }
//            return EVENT_IGNORED;
//        }
//    }
//    return EVENT_IGNORED;
//}

event_status_t PAUSE_Entry(protimer_t *const mobj, event_t const *const e)
{
//            printf("Paused\r\n");
	   return EVENT_HANDLED;
}

event_status_t PAUSE_Exit(protimer_t *const mobj, event_t const *const e)
{
//    printf("Exit4..\r\n");
    return EVENT_HANDLED;
}

event_status_t PAUSE_Start_pause(protimer_t *const mobj, event_t const *const e)
{
    mobj->active_state = COUNTDOWN;
    return EVENT_TRANSITION;
}

event_status_t PAUSE_Abrt(protimer_t *const mobj, event_t const *const e)
{
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}

event_status_t PAUSE_Inc_time(protimer_t *const mobj, event_t const *const e)
{
    mobj->curr_time += 60;
    mobj->active_state = TIME_SET;
    return EVENT_TRANSITION;
}

event_status_t PAUSE_Dec_time(protimer_t *const mobj, event_t const *const e)
{
    if(mobj->curr_time >= 60) {
        mobj->curr_time -= 60;
        mobj->active_state = TIME_SET;
        return EVENT_TRANSITION;
    }
    return EVENT_IGNORED;
}

event_status_t PAUSE_Time_tick(protimer_t *const mobj, event_t const *const e)
{
    if (mobj->curr_time > 0) {
        mobj->active_state = COUNTDOWN;
        return EVENT_TRANSITION;
    }
    return EVENT_IGNORED;
}
//static event_status_t protimer_state_handler_PAUSE(protimer_t *const mobj, event_t const *const e) {
//    switch(e->sig) {
//        case ENTRY:
//        {
////            printf("Paused\r\n");
//            return EVENT_HANDLED;
//        }
//        case EXIT:
//        {
//            printf("Exit4..\r\n");
//            return EVENT_HANDLED;
//        }
//        case INC_TIME:
//        {
//            mobj->curr_time += 60;
//            mobj->active_state = TIME_SET;
//            return EVENT_TRANSITION;
//        }
//        case DEC_TIME:
//        {
//            if(mobj->curr_time >= 60) {
//                mobj->curr_time -= 60;
//                mobj->active_state = TIME_SET;
//                return EVENT_TRANSITION;
//            }
//            return EVENT_IGNORED;
//        }
//        case START_PAUSE:
//        {
//            mobj->active_state = COUNTDOWN;
//            return EVENT_TRANSITION;
//        }
//        case TIME_TICK:
//        {
//            if (mobj->curr_time > 0) {
//                mobj->active_state = COUNTDOWN;
//                return EVENT_TRANSITION;
//            }
//            return EVENT_IGNORED;
//        }
//        case ABRT:
//        {
//            mobj->active_state = IDLE;
//            return EVENT_TRANSITION;
//        }
//    }
//    return EVENT_IGNORED;
//}
//
//************** HELPER FUNCTIONS ***************//
static void display_time(uint32_t time) {
    char buf[6];

    uint16_t m = time / 60;
    uint8_t s = time % 60;

    // Format time string using sprintf
    sprintf(buf, "%02d:%02d\r\n", m, s);  // Add newline and carriage return

    // Send formatted time string over UART
    // HAL_UART_Transmit(&hlpuart1, (uint8_t*)buf, strlen(buf), 500);  // Replace huart1 with your UART instance
    printf("%s\r\n", buf);
}
