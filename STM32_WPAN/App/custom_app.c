/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    App/custom_app.c
 * @author  MCD Application Team
 * @brief   Custom Example Application (Server)
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  /* demo_service */
  uint8_t Char_data_Notification_Status;
  uint8_t Char_btn_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
  bool connected;
  bool char_btn_notif_enabled;
  bool char_data_notif_enabled;

  uint32_t last_btn1_press;
  uint32_t last_btn2_press;
  uint32_t last_data_send;

  uint16_t data_idx_cntr;
  int16_t data_cntr;

  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* demo_service */
static void Custom_Char_data_Update_Char(void);
static void Custom_Char_data_Send_Notification(void);
static void Custom_Char_btn_Update_Char(void);
static void Custom_Char_btn_Send_Notification(void);

/* USER CODE BEGIN PFP */
static void TASK_USR_DATA_TIMER_CALLBACK(void);
static void TASK_USR_BTN1_PRESS_CALLBACK(void);
static void TASK_USR_BTN2_PRESS_CALLBACK(void);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification) {
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode) {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* demo_service */
    case CUSTOM_STM_CHAR_DATA_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_DATA_NOTIFY_ENABLED_EVT */
      APP_DBG_MSG("CHAR_DATA_NOTIFY_ENABLE\n\r");
      Custom_App_Context.char_data_notif_enabled = true;
      /* USER CODE END CUSTOM_STM_CHAR_DATA_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_CHAR_DATA_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_DATA_NOTIFY_DISABLED_EVT */
      Custom_App_Context.char_data_notif_enabled = false;
      APP_DBG_MSG("CHAR_DATA_NOTIFY_DISABLE\n\r");
      /* USER CODE END CUSTOM_STM_CHAR_DATA_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_CHAR_BTN_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_BTN_NOTIFY_ENABLED_EVT */
      Custom_App_Context.char_btn_notif_enabled = true;
      APP_DBG_MSG("CHAR_BTN_NOTIFY_ENABLE\n\r");
      /* USER CODE END CUSTOM_STM_CHAR_BTN_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_CHAR_BTN_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_CHAR_BTN_NOTIFY_DISABLED_EVT */
      Custom_App_Context.char_btn_notif_enabled = false;
      APP_DBG_MSG("CHAR_BTN_NOTIFY_DISABLE\n\r");
      /* USER CODE END CUSTOM_STM_CHAR_BTN_NOTIFY_DISABLED_EVT */
      break;

    case CUSTOM_STM_NOTIFICATION_COMPLETE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */
      APP_DBG_MSG("NOTIF_COMPLETE\n\r");
      /* USER CODE END CUSTOM_STM_NOTIFICATION_COMPLETE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */
      APP_DBG_MSG("OTHER\n\r");
      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification) {
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */
  APP_DBG_MSG("[EVT] Custom_App_Notification(..):");
  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode) {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT:
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */
      APP_DBG_MSG("CONN\n\r");
      Custom_App_Context.connected = true;

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT:
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */
      APP_DBG_MSG("OTHER\n\r");
      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void) {
  /* USER CODE BEGIN CUSTOM_APP_Init */

  APP_DBG_MSG("Custom_APP_INIT()");
  UTIL_SEQ_RegTask(1 << CFG_TASK_USR_BTN1_PRESS_ID, UTIL_SEQ_RFU, TASK_USR_BTN1_PRESS_CALLBACK);
  UTIL_SEQ_RegTask(1 << CFG_TASK_USR_BTN2_PRESS_ID, UTIL_SEQ_RFU, TASK_USR_BTN2_PRESS_CALLBACK);
  UTIL_SEQ_RegTask(1 << CFG_TASK_USR_DATA_TIMER_ID, UTIL_SEQ_RFU, TASK_USR_DATA_TIMER_CALLBACK);

  Custom_App_Context.connected = false;
  Custom_App_Context.char_btn_notif_enabled = false;
  Custom_App_Context.char_btn_notif_enabled = false;

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* demo_service */
void Custom_Char_data_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Char_data_UC_1*/

  updateflag = Custom_App_Context.connected && Custom_App_Context.char_data_notif_enabled;

  /* USER CODE END Char_data_UC_1*/

  if (updateflag != 0) {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHAR_DATA, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Char_data_UC_Last*/

  /* USER CODE END Char_data_UC_Last*/
  return;
}

void Custom_Char_data_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Char_data_NS_1*/

  updateflag = Custom_App_Context.connected && Custom_App_Context.char_data_notif_enabled;

  /* USER CODE END Char_data_NS_1*/

  if (updateflag != 0) {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHAR_DATA, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Char_data_NS_Last*/

  /* USER CODE END Char_data_NS_Last*/

  return;
}

void Custom_Char_btn_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Char_btn_UC_1*/

  updateflag = Custom_App_Context.connected && Custom_App_Context.char_btn_notif_enabled;

  /* USER CODE END Char_btn_UC_1*/

  if (updateflag != 0) {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHAR_BTN, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Char_btn_UC_Last*/

  /* USER CODE END Char_btn_UC_Last*/
  return;
}

void Custom_Char_btn_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Char_btn_NS_1*/

  updateflag = Custom_App_Context.connected && Custom_App_Context.char_btn_notif_enabled;

  /* USER CODE END Char_btn_NS_1*/

  if (updateflag != 0) {
    Custom_STM_App_Update_Char(CUSTOM_STM_CHAR_BTN, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Char_btn_NS_Last*/

  /* USER CODE END Char_btn_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/

// ==== USER TASKS =================================================================================

static void TASK_USR_DATA_TIMER_CALLBACK(void) {
  for (size_t i = 0; i < 40; i++) {
    uint16_t idx = Custom_App_Context.data_idx_cntr;
    int16_t data = Custom_App_Context.data_cntr;

    if (!HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)) {
      data += 256;
    }

    if (!HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin)) {
      data += 256;
    }

    // Index: (Little endian)
    NotifyCharData[i * 4 + 0] = (idx >> 0) & 0xFF;
    NotifyCharData[i * 4 + 1] = (idx >> 8) & 0xFF;

    // Data: (Little endian)
    NotifyCharData[i * 4 + 2] = (data >> 0) & 0xFF;
    NotifyCharData[i * 4 + 3] = (data >> 8) & 0xFF;

    Custom_App_Context.data_idx_cntr += 1;
    if (Custom_App_Context.data_cntr >= 127) {
      Custom_App_Context.data_cntr = -128;
    } else {
      Custom_App_Context.data_cntr += 1;
    }
  }

  Custom_Char_data_Send_Notification();
}

static void TASK_USR_BTN1_PRESS_CALLBACK(void) {
  NotifyCharData[0] = 1;
  Custom_Char_btn_Send_Notification();
}

static void TASK_USR_BTN2_PRESS_CALLBACK(void) {
  NotifyCharData[0] = 2;
  Custom_Char_btn_Send_Notification();
}

// ==== INTERRUPTS =================================================================================

#define DATA_PERIOD_MS 50
#define DEBOUNCE_MS    10

void app_custom_tick() {
  if ((uwTick - Custom_App_Context.last_data_send) > DATA_PERIOD_MS) {
    Custom_App_Context.last_data_send = uwTick;
    UTIL_SEQ_SetTask(1 << CFG_TASK_USR_DATA_TIMER_ID, CFG_SCH_PRIO_0);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

  switch (GPIO_Pin) {
    case BTN1_Pin:
      if ((uwTick - Custom_App_Context.last_btn1_press) > DEBOUNCE_MS) {
        Custom_App_Context.last_btn1_press = uwTick;
        UTIL_SEQ_SetTask(1 << CFG_TASK_USR_BTN1_PRESS_ID, CFG_SCH_PRIO_0);
      }
      break;
    case BTN2_Pin:
      if ((uwTick - Custom_App_Context.last_btn2_press) > DEBOUNCE_MS) {
        Custom_App_Context.last_btn2_press = uwTick;
        UTIL_SEQ_SetTask(1 << CFG_TASK_USR_BTN2_PRESS_ID, CFG_SCH_PRIO_0);
      }
      break;
    default:
      APP_DBG_MSG("Unknown GPIO interrupt: %d\n\r", GPIO_Pin);
      break;
  }
}

/* USER CODE END FD_LOCAL_FUNCTIONS*/
