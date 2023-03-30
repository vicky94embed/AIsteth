/* 
 * File:   bsp.h
 * 
 * Author: Electronic Engineer Pablo Boggio
 * 
 * Created on: July 2020
 * 
 * @brief:
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef BSP_H
#define	BSP_H

#include <stdint.h>
#include <stdbool.h>
#include "../SourceFiles/AtCommandBT.h"
#include "../mcc_generated_files/uart1.h"

extern BT_Model_t BT_Model;
extern System_timeouts_t System_timeouts;
//extern SYSTEM_STATUS General_State;

//<editor-fold defaultstate="collapsed" desc="DEFINES">
// Time base 100ms
//#define NOms        0
//#define EACH100ms   1
//#define EACH200ms   2
//#define EACH300ms   3
//#define EACH400ms   4
//#define EACH500ms   5
//#define EACH600ms   6
//#define EACH700ms   7
//#define EACH800ms   8
//#define EACH900ms   9
//#define EACH1s      10
//#define EACH2s      20
//#define EACH2200ms  22
//#define EACH2500ms  25
//#define EACH3s      30
//#define EACH4s      40
//#define EACH5s      50
//#define EACH25s     250

// Time base 50ms
#define NOms        0
#define EACH100ms   2
#define EACH150ms   3
#define EACH200ms   4
#define EACH250ms   5
#define EACH300ms   6
#define EACH400ms   8
#define EACH500ms   10
#define EACH600ms   12
#define EACH700ms   14
#define EACH800ms   16
#define EACH900ms   18
#define EACH1s      20
#define EACH2s      40
#define EACH2200ms  44
#define EACH2500ms  50
#define EACH3s      60
#define EACH4s      80
#define EACH5s      100
#define EACH25s     500
#define EACH50s     1000
#define EACH60s     1200
#define EACH100s    2000

#define BAT_LOW_LEVEL 3400  //3200
#define BAT_FULL_LEVEL 4200 //4050  // 4300

#define ChancesQty  3 // 4

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="ENUMS">

//<editor-fold defaultstate="collapsed" desc="Enum LED_COLOR_TYPE">
typedef enum LED_COLOR_EN
{
    NONE_LED,
    GREEN_LED,
    RED_LED,
    BLUE_LED
}LED_COLOR_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum CONFIG_TO_SET_TYPE">
typedef enum USER_SET_GET_EN
{
    USER_SET_BT_NAME,
    USER_SET_BATT_CONFIG,
    USER_SET_BT_STATE_DISABLE_CONFIG,
    USER_SET_BT_STATE_ENABLE_CONFIG,
    USER_SET_AUDIO,
    USER_SET_AUDIO_ANALOG,
    USER_SET_DEEP_SLEEP_ON,
    USER_SET_DEEP_SLEEP_OFF,
    USER_SET_ENABLE_LED_ON,
    USER_SET_ENABLE_LED_OFF,
    USER_SET_ENABLE_BAT_INF_ON,
    USER_SET_ENABLE_BAT_INF_OFF,
    USER_SET_AUTOCONN_ON,
    USER_SET_COD,
    USER_SET_CODEC,
    USER_SET_ENABLE_BATT_IND_ON,
    USER_SET_MAX_REC,
    USER_SET_PROFILES,
    USER_SET_VREG_ROLE,
    USER_SET_HFP_CONFIG,
    USER_BATTERY_INDICATOR_0,
    USER_BATTERY_INDICATOR_1,
    USER_BATTERY_INDICATOR_2,
    USER_BATTERY_INDICATOR_3,
    USER_BATTERY_INDICATOR_4,
    USER_BATTERY_INDICATOR_5,
    USER_BATTERY_INDICATOR_6,
    USER_BATTERY_INDICATOR_7,
    USER_BATTERY_INDICATOR_8,
    USER_BATTERY_INDICATOR_9,
    USER_VERSION,
    USER_RESTORE,
    USER_GET_BT_NAME,
    USER_GET_BATT_CONFIG,
    USER_GET_BT_STATE_CONFIG,
    USER_GET_AUDIO,
    USER_GET_AUDIO_ANALOG,
    USER_GET_DEEP_SLEEP,
    USER_GET_ENABLE_LED,
    USER_GET_ENABLE_BAT_INF,
    USER_WRITE,
    USER_RESET,
    USER_BATTERY_STATUS,
    USER_COMM_STATUS,
    USER_POWER_ON,
    USER_POWER_OFF
}USER_SET_GET_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum USER_CONFIG_STATUS_TYPE">
typedef enum USER_CONFIG_STATUS_EN
{
    USER_CONFIG_STATUS_NO_STARTED,
    USER_OK,
    USER_FAILED,
    USER_PROCESSING
}USER_CONFIG_STATUS_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum DEVICE_STATUS_TYPE">
typedef enum DEVICE_STATUS_EN
{
    START_UP,
    ON_UC,
    GO_TO_SLEEP_UC,
    SLEEP_UC
}DEVICE_STATUS;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum SYSTEM_STATUS_TYPE">
typedef enum SYSTEM_STATUS_EN
{
    SLEEP_NOCHARGING,
    SLEEP_CHARGING,
    SLEEP_CHARGED,
    TURN_OFF_NOCHARGING,
    ON_DISC_NOCHARGING_BATOK,
    ON_DISC_NOCHARGING_BATLOW,
    ON_DISC_CHARGING,
    ON_DISC_CHARGED,
    ON_CON_NOCHARGING_BATOK,
    ON_CON_NOCHARGING_BATLOW,
    ON_CON_CHARGING,
    ON_CON_CHARGED
}SYSTEM_STATUS;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum BATTERY_CHARGER_STATE">
typedef enum BATTERY_CHARGER_STATE_en
{
    CH_NO_STATE,
    CH_FAST_CHARGE,
    CH_NO_POWER,
    CH_STANBY,
    CH_TRICKLE,
    CH_DISABLED,
    CH_CHARGE_IN_PROGRESS,   // ADDED
    CH_CHARGE_COMPLETE       // ADDED
}BATT_CHRG_STATE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum CONNECTABLE_STATUS_TYPE">
typedef enum CONNECTABLE_STATUS_en
{
    STATUS_CONNECTABLE,
    STATUS_NO_CONNECTABLE
}CONNECTABLE_STATUS;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum DISCOVERABLE_STATUS_TYPE">
typedef enum DISCOVERABLE_STATUS_en
{
    STATUS_DISCOVERABLE,
    STATUS_NO_DISCOVERABLE
}DISCOVERABLE_STATUS;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum ON_OFF_STATUS_TYPE">
typedef enum ON_OFF_STATUS_en
{
    STATUS_SLEEP,
    STATUS_ON,
    STATUS_OFF
}ON_OFF_STATUS;
//</editor-fold>

//</editor-fold>

/*************************Interrupt Service Routines***************************/
//<editor-fold defaultstate="collapsed" desc="Servicios TMR1 Interrupt prototypes">
void bsp_TMR1_Start(void);
void bsp_TMR1_Stop(void);
void bsp_TMR1_Initialize(uint8_t Period);
void bsp_setCommunicationTimeout(void);
void bsp_clearCommunicationTimeout(void);
void bsp_startConnectionTimeout(uint8_t Period);
void bsp_stopConnectionTimeout(void);
uint8_t bsp_getCommunicationTimeoutStatus(void);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios TMR2 Interrupt prototypes">
//void System_timeouts_Init(System_timeouts_t* SysTime, uint8_t STATUS_i, uint8_t BATT_i, uint8_t BUTT_i, uint8_t LED_i, uint16_t POW_i);
void System_timeouts_Init(System_timeouts_t* SysTime, uint16_t STATUS_i, uint16_t LED_i, uint16_t POW_i);
void bsp_TMR2_Start(void);
void bsp_TMR2_Stop(void);
void bsp_TMR2_Initialize(void);
uint8_t bsp_decrementalCounterFunction(uint16_t * counter);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios EXT Interrupt">
void bsp_EX_INT0_InterruptEnable();
void bsp_EX_INT0_InterruptDisable();
//</editor-fold>

void bspInit(void);

//<editor-fold defaultstate="collapsed" desc="Led Driver abstraction prototypes">
void ledDriverInit(void);
void ledRedOn(void);
void ledBlueOn(void);
void ledGreenOn(void);

uint8_t getLedRedStatus(void);
uint8_t getLedBlueStatus(void);
uint8_t getLedGreenStatus(void);

void LedRefresh(LED_COLOR_TYPE led, uint16_t ledTimeOn, uint16_t ledTimeOff, System_timeouts_t* LedModel);
void ledOff(void);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Abstraction Functions">
//<editor-fold defaultstate="collapsed" desc="WhatDog ">
void watchDogReset(void);

void startWatchDog(void);
//</editor-fold>
//</editor-fold>

/**************************Button Service Routines*****************************/
//<editor-fold defaultstate="collapsed" desc="Button Services">
//uint8_t bsp_get_button_IN_Status(void);
//void bsp_set_button_OUT_Short_press(void);
//void bsp_set_button_OUT_Long_press(void);
//void bsp_set_button_OUT_press(uint16_t Period);
//void bsp_button_OUT_End_timeout(void);
uint8_t bsp_get_button_OUT_Status(void);
void bsp_set_button_OUT_Active(void);
void bsp_set_button_OUT_Release(void);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios STATUS_Timeout">
void bsp_set_Status_Timeout(System_timeouts_t* TimeoutModel, uint16_t time);
void bsp_clear_Status_Timeout(System_timeouts_t* TimeoutModel);
uint16_t bsp_get_Status_Timeout(System_timeouts_t* TimeoutModel);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios BattState_Timeout">
//void bsp_set_BattState_Timeout(System_timeouts_t* TimeoutModel, uint16_t time);
//void bsp_clear_BattState_Timeout(System_timeouts_t* TimeoutModel);
//uint16_t bsp_get_BattState_Timeout(System_timeouts_t* TimeoutModel);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios TurnOff_Timeout">
void bsp_set_PowerOff_Timeout(System_timeouts_t* TimeoutModel, uint16_t time);
void bsp_clear_PowerOff_Timeout(System_timeouts_t* TimeoutModel);
uint16_t bsp_get_PowerOff_Timeout(System_timeouts_t* TimeoutModel);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="System state services">
ON_OFF_STATUS get_ON_OFF_State(Config_Data_t* Config);
void Set_ON_OFF_State(Config_Data_t* Config, ON_OFF_STATUS NewStatus);
BATT_CHRG_STATE get_CHARGER_State(Config_Data_t* Config);
uint16_t get_BATT_LEVEL_State(Config_Data_t* Config);
void bsp_analize_system_State(Config_Data_t* Config);
SYSTEM_STATUS get_system_State(Config_Data_t* Config);
void set_system_State(Config_Data_t* Config, SYSTEM_STATUS NewState);
DEVICE_STATUS get_device_State(Config_Data_t* Config);
void set_device_State(Config_Data_t* Config, DEVICE_STATUS NewState);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="UART1 Functions">
void bsp_BT_Enable_Read(void);
void bsp_BT_Disable_Read(void);
bool bsp_UART1_is_tx_ready(void);
void bsp_UART1_clear_interrupt_flag(void);
void bsp_UART1_Write(uint8_t txData);
CONFIG_STATUS_TYPE bsp_CheckIfDataToSend(Command_t* CommandModel);
uint8_t bsp_GetDataToSend(Command_t* CommandModel);
void bsp_SetCheckForReadyBTInfo(void);
uint8_t bsp_GetCheckForReadyBTInfo(void);
void bsp_ClearCheckForReadyBTInfo(void);
void bsp_SetGlobalForUART();
void bsp_ClearGlobalForUART();
uint8_t bsp_GetGlobalForUART();
void bsp_UART1_Read_Enable(void);
void bsp_UART1_Read_Disable (void);
void bsp_UART1_Weak_Disable (void);
void bsp_UART1_Weak_Enable (void);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="COMM Functions">
CONFIG_STATUS_TYPE bsp_SetInternalCommandModelStatus(Command_t* CommandModel, MODEL_STATUS_TYPE ModelStatus);
void bsp_ManageNewReceivedData(RX_Data_t* RxModel, uint8_t RxNewData);
void bsp_CheckForReadyBTInfo(RX_Data_t* RxModel);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="BT User Functions">
USER_CONFIG_STATUS_TYPE bsp_Function_To_Use (USER_SET_GET_TYPE SetGetFunction, uint8_t Tries);
USER_CONFIG_STATUS_TYPE bsp_Status_Refresh(void);
USER_CONFIG_STATUS_TYPE bsp_CheckStatus(void);
USER_CONFIG_STATUS_TYPE bsp_CheckBattery(void);
USER_CONFIG_STATUS_TYPE bsp_CheckConfig(void);
void bsp_Configure_StartUp(void);
//</editor-fold>
#endif	/* BSP_H */

