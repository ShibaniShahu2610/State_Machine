
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern UART_HandleTypeDef hlpuart1;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
/* signals of the application */

typedef enum{
    INC_TIME,
    DEC_TIME,
    TIME_TICK,
    START_PAUSE,
    ABRT,

    /* internal activity signals */
    ENTRY,
    EXIT,
	MAX_SIGNALS
}protimer_signal_t;

/* various states of the application */

typedef enum{
    IDLE,
    TIME_SET,
    COUNTDOWN,
    PAUSE,
	MAX_STATES
//    STAT
}protimer_state_t;

/* main application structure */

typedef struct{
    uint32_t curr_time;
    uint32_t elapsed_time;
    uint32_t pro_time;
    protimer_state_t active_state;
    uintptr_t *state_table;
}protimer_t;

/* generic (super) event structure */
typedef struct{
    uint8_t sig;
}event_t;

/* for user generated events */
typedef struct
{
    event_t super;
}protimer_user_event_t;

/* for tick event */

typedef struct
{
    event_t super;
    uint8_t ss;
}protimer_tick_event_t;

typedef enum{
    EVENT_HANDLED,
    EVENT_IGNORED,
    EVENT_TRANSITION
}event_status_t;


// function pointer for event handlers
typedef event_status_t (*e_handler_t)(protimer_t *const mobj, event_t const *const e);

void protimer_init(protimer_t *mobj);
event_status_t protimer_state_machine(protimer_t *const mobj, event_t const *const e);


event_status_t IDLE_Entry(protimer_t *const mobj, event_t const *const e);
event_status_t IDLE_Exit(protimer_t *const mobj, event_t const *const e);
event_status_t IDLE_Inc_time(protimer_t *const mobj, event_t const *const e);
event_status_t IDLE_Time_tick(protimer_t *const mobj, event_t const *const e);

event_status_t TIME_SET_Entry(protimer_t *const mobj, event_t const *const e);
event_status_t TIME_SET_Exit(protimer_t *const mobj, event_t const *const e);
event_status_t TIME_SET_Inc_time(protimer_t *const mobj, event_t const *const e);
event_status_t TIME_SET_Dec_time(protimer_t *const mobj, event_t const *const e);
event_status_t TIME_SET_Abrt(protimer_t *const mobj, event_t const *const e);
event_status_t TIME_SET_Start_pause(protimer_t *const mobj, event_t const *const e);

event_status_t COUNTDOWN_Entry(protimer_t *const mobj, event_t const *const e);
event_status_t COUNTDOWN_Exit(protimer_t *const mobj, event_t const *const e);
event_status_t COUNTDOWN_Start_pause(protimer_t *const mobj, event_t const *const e);
event_status_t COUNTDOWN_abrt(protimer_t *const mobj, event_t const *const e);
event_status_t COUNTDOWN_Time_tick(protimer_t *const mobj, event_t const *const e);

event_status_t PAUSE_Entry(protimer_t *const mobj, event_t const *const e);
event_status_t PAUSE_Exit(protimer_t *const mobj, event_t const *const e);
event_status_t PAUSE_Start_pause(protimer_t *const mobj, event_t const *const e);
event_status_t PAUSE_Abrt(protimer_t *const mobj, event_t const *const e);
event_status_t PAUSE_Inc_time(protimer_t *const mobj, event_t const *const e);
event_status_t PAUSE_Dec_time(protimer_t *const mobj, event_t const *const e);
event_status_t PAUSE_Time_tick(protimer_t *const mobj, event_t const *const e);



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */