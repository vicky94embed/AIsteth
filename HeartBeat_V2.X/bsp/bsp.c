/* 
 * File:   bsp.c
 * 
 * Author: Ingeniero Electrónico Tosetti Matías
 * 
 * Created on...
 * 
 * @brief:
 * 
 */

#include <p24FJ256GA702.h>

#include "bsp.h"
#include "../mcc_generated_files/mcc.h"
#include "../SourceFiles/AtCommandBT.h"
#include "../mcc_generated_files/uart1.h"

/************************ Includes hardware Libraries**************************/


uint16_t BATT_LEVEL; // battery voltage level value

/*************************Externs APP functions********************************/

uint8_t GlobalForUART = 0;
BT_Model_t BT_Model;
System_timeouts_t System_timeouts;
////SYSTEM_STATUS General_State;

/*********************General Hardware Initialization***********************/
//<editor-fold defaultstate="collapsed" desc="General hardware resource initialization">
void bspInit(void) {
//    startWatchDog(); // check later
    // Initialize the device
    SYSTEM_Initialize();
//    System_timeouts_Init(&System_timeouts, EACH1s, EACH2s, EACH200ms, NOms, EACH25s);
    System_timeouts_Init(&System_timeouts, EACH1s, NOms, EACH60s); // EACH25s // EACH1s
}
//</editor-fold>

/*************************Interrupt Service Routines***************************/
//<editor-fold defaultstate="collapsed" desc="Servicios TMR1 Interrupt">
void bsp_TMR1_Start(void)
{
    TMR1_Start ();
}
void bsp_TMR1_Stop(void)
{
    TMR1_Stop ();
}
void bsp_TMR1_Initialize(uint8_t Period)
{
    TMR1_Initialize (Period);
}
void bsp_setCommunicationTimeout(void)
{
    setCommunicationTimeout();
}
void bsp_clearCommunicationTimeout(void)
{
    clearCommunicationTimeout();
}
void bsp_startConnectionTimeout(uint8_t Period)
{
    startConnectionTimeout(Period);
}
void bsp_stopConnectionTimeout(void)
{
    stopConnectionTimeout();
}
uint8_t bsp_getCommunicationTimeoutStatus(void)
{
    return getCommunicationTimeoutStatus();
}
//</editor-fold>

/************************* Interrupt Service Routines***************************/
//<editor-fold defaultstate="collapsed" desc="Servicios TMR2 Interrupt">
//void System_timeouts_Init(System_timeouts_t* SysTime, uint16_t STATUS_i, uint16_t BATT_i, uint16_t BUTT_i, uint16_t LED_i, uint16_t POW_i)
void System_timeouts_Init(System_timeouts_t* SysTime, uint16_t STATUS_i, uint16_t LED_i, uint16_t POW_i)
{
    SysTime->STATUS_State_Period = STATUS_i;
//    SysTime->BATT_State_Period = BATT_i;
//    SysTime->BUTTON_Period = BUTT_i;
    SysTime->LED_PeriodCounter = LED_i;
    SysTime->POWER_OFF_Period = POW_i;
}

void bsp_TMR2_Start(void)
{
    TMR2_Start ();
}

void bsp_TMR2_Stop(void)
{
    TMR2_Stop ();
}

void bsp_TMR2_Initialize(void)
{
    TMR2_Initialize ();
}

uint8_t bsp_decrementalCounterFunction(uint16_t * counter)
{
	if (*counter)
    {
		(*counter)--;

		if (0 == *counter)
        {
			return (1);
		}
        else
        {
			return (0);
		}
	}
	return (1);
}
//</editor-fold>

/*************************Interrupt Service Routines***************************/
//<editor-fold defaultstate="collapsed" desc="Servicios EXT Interrupt">
void bsp_EX_INT0_InterruptEnable()
{
    EX_INT0_InterruptEnable();
}

void bsp_EX_INT0_InterruptDisable()
{
    EX_INT0_InterruptDisable();
}
//</editor-fold>

/****************************** Timeout Services ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicios UserTimeout">

//<editor-fold defaultstate="collapsed" desc="Servicios STATUS_Timeout">
void bsp_set_Status_Timeout(System_timeouts_t* TimeoutModel, uint16_t time)
{
    TimeoutModel->STATUS_State_Period = time;
}

void bsp_clear_Status_Timeout(System_timeouts_t* TimeoutModel)
{
    TimeoutModel->STATUS_State_Period = 0;
}

uint16_t bsp_get_Status_Timeout(System_timeouts_t* TimeoutModel)
{
    return TimeoutModel->STATUS_State_Period;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios BattState_Timeout">
//void bsp_set_BattState_Timeout(System_timeouts_t* TimeoutModel, uint16_t time)
//{
//    TimeoutModel->BATT_State_Period = time;
//}
//
//void bsp_clear_BattState_Timeout(System_timeouts_t* TimeoutModel)
//{
//    TimeoutModel->BATT_State_Period = 0;
//}
//
//uint16_t bsp_get_BattState_Timeout(System_timeouts_t* TimeoutModel)
//{
//    return TimeoutModel->BATT_State_Period;
//}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios Button_Timeout">
void bsp_set_Button_Timeout()
{
    set_Button_Timeout();
}
void bsp_clear_Button_Timeout()
{
    clear_Button_Timeout();
}
uint8_t bsp_get_Button_Timeout()
{
    return get_Button_Timeout();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios PowerOff_Timeout">
void bsp_set_PowerOff_Timeout(System_timeouts_t* TimeoutModel, uint16_t time)
{
    TimeoutModel->POWER_OFF_Period = time;
}

void bsp_clear_PowerOff_Timeout(System_timeouts_t* TimeoutModel)
{
    TimeoutModel->POWER_OFF_Period = 0;
}

uint16_t bsp_get_PowerOff_Timeout(System_timeouts_t* TimeoutModel)
{
    return TimeoutModel->POWER_OFF_Period;
}
//</editor-fold>

//</editor-fold>

/**************************Button Service Routines*****************************/
//<editor-fold defaultstate="collapsed" desc="Button">
//uint8_t bsp_get_button_IN_Status(void)
//{
//    return BUTTON_IN_GetValue();
//}

//void bsp_set_button_OUT_Short_press(void)
//{
//    bsp_set_button_OUT_press(EACH500ms);
//}

//void bsp_set_button_OUT_Long_press(void)
//{
//    bsp_set_button_OUT_press(EACH3s);
//}

//void bsp_set_button_OUT_press(uint16_t Period)
//{
////    bsp_set_Active_Button_Period(System_timeouts, Period); // Seteo período
////    bsp_set_button_OUT_Active(); // Pongo pin activo
////    bsp_clear_Button_Timeout(); // Borro flag de timeout
//}

//void bsp_button_OUT_End_timeout(void)
//{
//    bsp_set_button_OUT_Release();
//    bsp_set_Button_Timeout();
//}

uint8_t bsp_get_button_OUT_Status(void) {
    return BUTTON_OUT_GetValue();
}
void bsp_set_button_OUT_Active(void)
{
    BUTTON_OUT_SetLow();
}
void bsp_set_button_OUT_Release(void)
{
    BUTTON_OUT_SetHigh();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="System state services">

ON_OFF_STATUS get_ON_OFF_State(Config_Data_t* Config)
{
    return Config->STATE_ON_C;
}

void Set_ON_OFF_State(Config_Data_t* Config, ON_OFF_STATUS NewStatus)
{
    Config->STATE_ON_C = NewStatus;
}

BATT_CHRG_STATE get_CHARGER_State(Config_Data_t* Config)
{
    return Config->BATTERY_STATUS_C;
}

uint16_t get_BATT_LEVEL_State(Config_Data_t* Config)
{
    return Config->BATTERY_STATUS_LEVEL_C;
}

void bsp_analize_system_State(Config_Data_t* Config)
{
    if(Config->STATE_ON_C == STATUS_ON)
    {
                
        switch(Config->BATTERY_STATUS_C)
        {
            case CH_NO_STATE: default: // case CH_NO_POWER: // case CH_NO_STATE: //default:
                if(Config->BATTERY_STATUS_LEVEL_C < BAT_LOW_LEVEL)
                {
                    if(Config->STATE_CONNECTED_C)
                    {
                        set_system_State(Config, ON_CON_NOCHARGING_BATLOW);
                    }
                    else
                    {
                      //  set_system_State(Config, ON_DISC_NOCHARGING_BATLOW);
                        set_system_State(Config, ON_DISC_NOCHARGING_BATOK);
                    }
                }
                else if(get_BATT_LEVEL_State(&BT_Model.Config) > BAT_FULL_LEVEL)
                {
                    if(Config->STATE_CONNECTED_C)
                    {
                        set_system_State(Config, ON_CON_CHARGED);
                    }
                    else
                    {    
                        
                    //    set_system_State(Config, ON_DISC_CHARGED);
                        set_system_State(Config, ON_DISC_NOCHARGING_BATOK);
                    } 
                }
                else
                {
                    if(Config->STATE_CONNECTED_C)
                    {
                        set_system_State(Config, ON_CON_NOCHARGING_BATOK);
                    }
                    else
                    {   
                        set_system_State(Config, ON_DISC_NOCHARGING_BATOK);
                    }
                }
                break;
            case CH_DISABLED: case CH_TRICKLE:  //ADDED  // case CH_CHARGE_IN_PROGRESS: 
                if(get_BATT_LEVEL_State(&BT_Model.Config) < BAT_FULL_LEVEL)
                {
                    if(Config->STATE_CONNECTED_C)
                    {
                        set_system_State(Config, ON_CON_CHARGING);
                    }
                    else
                    {
                        set_system_State(Config, ON_DISC_CHARGING);
                    }
                }
                else
                {
                    if(Config->STATE_CONNECTED_C)
                    {
                        set_system_State(Config, ON_CON_CHARGED);
                    }
                    else
                    {
                        set_system_State(Config, ON_DISC_CHARGED);
                    } 
                }
                break;
            case CH_FAST_CHARGE:
                if(Config->STATE_CONNECTED_C)
                {
                    set_system_State(Config, ON_CON_CHARGING);
                }
                else
                {
                    set_system_State(Config, ON_DISC_CHARGING);
                }
                break;
            case CH_STANBY: //case CH_CHARGE_COMPLETE:  // ADDED
                if(Config->STATE_CONNECTED_C)
                {
                    set_system_State(Config, ON_CON_CHARGED);
                }
                else
                {
                    set_system_State(Config, ON_DISC_CHARGED);
                }
                break;
        }
    }
    else if(Config->STATE_ON_C == STATUS_SLEEP)
    {
        switch(Config->BATTERY_STATUS_C)
        {
            case CH_NO_STATE: case CH_NO_POWER: default:
              
                /*
               if(get_BATT_LEVEL_State(&BT_Model.Config) > BAT_FULL_LEVEL)  // added
               {
                    set_system_State(Config, SLEEP_CHARGED);  // added
                }
                else
                {
                set_system_State(Config, SLEEP_NOCHARGING);
                }
                 
                 */
                
                set_system_State(Config, SLEEP_NOCHARGING);
                break;
            
            
            case CH_DISABLED: case CH_TRICKLE:   // ADDED //case CH_CHARGE_IN_PROGRESS:
                
                    set_system_State(Config, SLEEP_CHARGED);
    
                break;

            case CH_FAST_CHARGE:
                set_system_State(Config, SLEEP_CHARGING);
                break;

            case CH_STANBY:    //ADDED // case CH_CHARGE_COMPLETE:
                set_system_State(Config, SLEEP_CHARGED);
                break;
        }
    }
    else
    {
        set_system_State(Config, TURN_OFF_NOCHARGING);
    }
}

SYSTEM_STATUS get_system_State(Config_Data_t* Config)
{
    return Config->GENERAL_SYSTEM_STATE_C;
}

void set_system_State(Config_Data_t* Config, SYSTEM_STATUS NewState)
{
    Config->GENERAL_SYSTEM_STATE_C = NewState;
}

DEVICE_STATUS get_device_State(Config_Data_t* Config)
{
    return Config->GENERAL_DEVICE_STATE_C;
}

void set_device_State(Config_Data_t* Config, DEVICE_STATUS NewState)
{
    Config->GENERAL_DEVICE_STATE_C = NewState;
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Led Driver abstraction servicies">
void ledDriverInit(void)
{
    LED_RED_SetHigh();
    LED_BLUE_SetHigh();
    LED_GREEN_SetHigh();
}

void ledRedOn(void)
{
    LED_RED_SetLow();
    LED_BLUE_SetHigh();
    LED_GREEN_SetHigh();
}

void ledBlueOn(void)
{
    LED_RED_SetHigh();
    LED_BLUE_SetLow();
    LED_GREEN_SetHigh();
}

void ledGreenOn(void)
{
    LED_RED_SetHigh();
    LED_BLUE_SetHigh();
    LED_GREEN_SetLow();
}

void ledOff(void)
{
    LED_RED_SetHigh();
    LED_BLUE_SetHigh();
    LED_GREEN_SetHigh();
}

uint8_t getLedRedStatus(void)
{
    return (LED_RED_GetValue());
}

uint8_t getLedBlueStatus(void)
{
    return (LED_BLUE_GetValue());
}

uint8_t getLedGreenStatus(void)
{
    return (LED_GREEN_GetValue());
}

void LedRefresh(LED_COLOR_TYPE led, uint16_t ledTimeOn, uint16_t ledTimeOff, System_timeouts_t* LedModel)
{

    static uint8_t Blink = 0;
    
    if(!Blink)
    {
        if(LedModel->LED_PeriodCounter == 0)
        {
            switch (led)
            {
                case GREEN_LED:
                    ledGreenOn();
                    break;
                case RED_LED:
                    ledRedOn();
                    break;
                case BLUE_LED:
                    ledBlueOn();
                    break;
                default: case NONE_LED:
                    ledOff();
                    break;
            }
            LedModel->LED_PeriodCounter = ledTimeOn;
            if(ledTimeOff) Blink = 1;
        }
    }
    else
    {
        if(LedModel->LED_PeriodCounter == 0)
        {
            ledOff();
            LedModel->LED_PeriodCounter = ledTimeOff;
            Blink = 0;
        }
    }
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Abstraction Functions">
//<editor-fold defaultstate="collapsed" desc="WhatDog ">
void watchDogReset(void) {
    //WWDT_TimerClear();
}

void startWatchDog(void) {
    //WWDT_SoftEnable();
}
//</editor-fold>
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="UART1 Functions">
void bsp_BT_Enable_Read()
{
    BT_Enable_Read();
}

void bsp_BT_Disable_Read()
{
    BT_Disable_Read();
}

bool bsp_UART1_is_tx_ready()
{
    return UART1_IsTxReady();
}

void bsp_UART1_clear_interrupt_flag()
{
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1;
}

void bsp_UART1_Write(uint8_t txData)
{
    UART1_Write(txData);
}

CONFIG_STATUS_TYPE bsp_CheckIfDataToSend(Command_t* CommandModel)
{
    return CheckIfDataToSend(CommandModel);
}

uint8_t bsp_GetDataToSend(Command_t* CommandModel)
{
    return GetDataToSend(CommandModel);
}

uint8_t CheckBTdata = 0;

void bsp_SetCheckForReadyBTInfo()
{
    CheckBTdata = 1;
}

uint8_t bsp_GetCheckForReadyBTInfo()
{
    return CheckBTdata;
}

void bsp_ClearCheckForReadyBTInfo()
{
    CheckBTdata = 0;
}

void bsp_SetGlobalForUART()
{
    GlobalForUART = 0;
}

void bsp_ClearGlobalForUART()
{
    GlobalForUART = 0;
}

uint8_t bsp_GetGlobalForUART()
{
    return GlobalForUART;
}

void bsp_UART1_Read_Enable()
{
    UART1_Read_Enable();  
}

void bsp_UART1_Read_Disable (void)
{
    UART1_Read_Disable();
}

void bsp_UART1_Weak_Disable (void)
{
    UART1_Weak_Disable ();
}

void bsp_UART1_Weak_Enable (void)
{
    UART1_Weak_Enable ();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="COMM Functions">
CONFIG_STATUS_TYPE bsp_SetInternalCommandModelStatus(Command_t* CommandModel, MODEL_STATUS_TYPE ModelStatus)
{
    return (SetInternalCommandModelStatus(CommandModel, ModelStatus));
}

void bsp_ManageNewReceivedData(RX_Data_t* RxModel, uint8_t RxNewData) 
{ 
     if(64 == RxNewData) // 64 is ASCII representation of @
    {
        FLASH_Unlock(FLASH_UNLOCK_KEY);
        FLASH_WriteDoubleWord24(0x1804, 0, 0);
        FLASH_Lock();
        asm("reset");
    }
     
    ManageNewReceivedData(RxModel, RxNewData);
}

void bsp_CheckForReadyBTInfo(RX_Data_t* RxModel)
{
    CheckForReadyBTInfo(RxModel);
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="BT User Functions">

//<editor-fold defaultstate="collapsed" desc="bsp_Function_To_Use">
USER_CONFIG_STATUS_TYPE bsp_Function_To_Use (USER_SET_GET_TYPE SetGetFunction, uint8_t Tries)
{
    uint16_t aux;
    uint8_t CHANCES = 0;
    CONFIG_STATUS_TYPE Response = CONFIG_STATUS_NO_STARTED;
    USER_CONFIG_STATUS_TYPE ForReturn = USER_FAILED;
    
    ResetBTLibrary(&BT_Model);
    
    while(CHANCES < Tries)
    {
        switch(SetGetFunction)
        {
            case USER_SET_BT_NAME:
                Response = setConfig(SET_BT_NAME, &BT_Model);
                break;
            case USER_SET_BATT_CONFIG:
                Response = setConfig(SET_BATT_CONFIG, &BT_Model);
                break;
            case USER_SET_BT_STATE_DISABLE_CONFIG:
                Response = setConfig(SET_BT_STATE_DISABLE_CONFIG, &BT_Model);
                break;
            case USER_SET_BT_STATE_ENABLE_CONFIG:
                Response = setConfig(SET_BT_STATE_ENABLE_CONFIG, &BT_Model);
                break;
            case USER_SET_AUDIO:
                Response = setConfig(SET_AUDIO, &BT_Model);
                break;
            case USER_SET_AUDIO_ANALOG:
                Response = setConfig(SET_AUDIO_ANALOG, &BT_Model);
                break;
            case USER_SET_DEEP_SLEEP_ON:
                Response = setConfig(SET_DEEP_SLEEP_ON, &BT_Model);
                break;
            case USER_SET_DEEP_SLEEP_OFF:
                Response = setConfig(SET_DEEP_SLEEP_OFF, &BT_Model);
                break;
            case USER_SET_ENABLE_LED_ON:
                Response = setConfig(SET_ENABLE_LED_ON, &BT_Model);
                break;
            case USER_SET_ENABLE_LED_OFF:
                Response = setConfig(SET_ENABLE_LED_OFF, &BT_Model);
                break;
            case USER_SET_ENABLE_BAT_INF_ON:
                Response = setConfig(SET_ENABLE_BAT_INF_ON, &BT_Model);
                break;
            case USER_SET_ENABLE_BAT_INF_OFF:
                Response = setConfig(SET_ENABLE_BAT_INF_OFF, &BT_Model);
                break;
            case USER_SET_AUTOCONN_ON:
                Response = setConfig(SET_AUTOCONN_ON, &BT_Model);
                break;
            case USER_SET_COD:
                Response = setConfig(SET_COD, &BT_Model);
                break;
            case USER_SET_CODEC:
                Response = setConfig(SET_CODEC, &BT_Model);
                break;
            case USER_SET_ENABLE_BATT_IND_ON:
                Response = setConfig(SET_ENABLE_BATT_IND_ON, &BT_Model);
                break;
            case USER_SET_MAX_REC:
                Response = setConfig(SET_MAX_REC, &BT_Model);
                break;  
            case USER_SET_PROFILES:
                Response = setConfig(SET_PROFILES, &BT_Model);
                break;
            case USER_SET_VREG_ROLE:
                Response = setConfig(SET_VREG_ROLE, &BT_Model);
                break;
            case USER_SET_HFP_CONFIG:
                Response = setConfig(SET_HFP_CONFIG, &BT_Model);
                break;
            case USER_BATTERY_INDICATOR_0:
                Response = BATTERY_INDICATOR_0(&BT_Model);
                break; 
            case USER_BATTERY_INDICATOR_1:
                Response = BATTERY_INDICATOR_1(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_2:
                Response = BATTERY_INDICATOR_2(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_3:
                Response = BATTERY_INDICATOR_3(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_4:
                Response = BATTERY_INDICATOR_4(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_5:
                Response = BATTERY_INDICATOR_5(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_6:
                Response = BATTERY_INDICATOR_6(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_7:
                Response = BATTERY_INDICATOR_7(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_8:
                Response = BATTERY_INDICATOR_8(&BT_Model);
                break;
            case USER_BATTERY_INDICATOR_9:
                Response = BATTERY_INDICATOR_9(&BT_Model);
                break;
             case USER_VERSION:
                Response = VERSION_NAME(&BT_Model);
                break;    
            case USER_RESTORE:
                Response = restoreSystem(&BT_Model);
                break;
            case USER_GET_BT_NAME:
                Response = getConfig(GET_BT_NAME, &BT_Model);
                break;
            case USER_GET_BATT_CONFIG:
                Response = getConfig(GET_BATT_CONFIG, &BT_Model);
                break;
            case USER_GET_BT_STATE_CONFIG:
                Response = getConfig(GET_BT_STATE_CONFIG, &BT_Model);
                break;
            case USER_GET_AUDIO:
                Response = getConfig(GET_AUDIO, &BT_Model);
                break;
            case USER_GET_AUDIO_ANALOG:
                Response = getConfig(GET_AUDIO_ANALOG, &BT_Model);
                break;
            case USER_GET_DEEP_SLEEP:
                Response = getConfig(GET_DEEP_SLEEP, &BT_Model);
                break;
            case USER_GET_ENABLE_LED:
                Response = getConfig(GET_ENABLE_LED, &BT_Model);
                break;
            case USER_GET_ENABLE_BAT_INF:
                Response = getConfig(GET_ENABLE_BAT_INF, &BT_Model);
                break;
            
            case USER_WRITE:
                Response = saveConfigurations(&BT_Model);
                break;
            case USER_RESET:
                Response = resetSystem(&BT_Model);
                break;
            case USER_BATTERY_STATUS:
                Response = BatteryStatus(&BT_Model);
                break;
            case USER_COMM_STATUS:
                Response = CommStatus(&BT_Model);
                break;
            case USER_POWER_ON:
                Response = PowerOnOff(CONFIG_ON, &BT_Model);
                break;
            case USER_POWER_OFF:
                Response = PowerOnOff(CONFIG_OFF, &BT_Model);
                break;
            default:
                break;
        }
        switch (Response)
        {
            case PROCESSING:
                break;
            case OK:
                CHANCES = Tries;
                ForReturn = USER_OK;
                break;
            case FAILED:
                CHANCES++;
                aux = 65535;
                while(aux) aux--;
                break;
            default:
                break;
        }
    }
    return ForReturn;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="bsp_Status_Refresh">
USER_CONFIG_STATUS_TYPE bsp_Status_Refresh()
{
    USER_CONFIG_STATUS_TYPE Error = USER_OK;
    
    if(bsp_CheckStatus() != USER_OK)  // device status update
    {
        Error = USER_FAILED;
    }
    if(bsp_CheckBattery() != USER_OK) // device battery status and battery level update
    {
        Error = USER_FAILED;
    }
    bsp_analize_system_State(&BT_Model.Config);  // system analise
      
     
    BATT_LEVEL = get_BATT_LEVEL_State(&BT_Model.Config); //battery level 
    
     
    // battery level mapping from 10% to 100% for phone battery indicator AT commands.
     if(BATT_LEVEL<3400)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_0, 1);  // 10% 
     }
    else if(BATT_LEVEL>3400 && BATT_LEVEL<3500)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_1, 1);  // 20%
     }
    else if(BATT_LEVEL>3500 && BATT_LEVEL<3600)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_2, 1);  // 30%
     }
    else if(BATT_LEVEL>3600 && BATT_LEVEL<3700)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_3, 1);  // 40%
     }
    else if(BATT_LEVEL>3700 && BATT_LEVEL<3800)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_4, 1);  // 50%
     }
    else if(BATT_LEVEL>3800 && BATT_LEVEL<3900)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_5, 1);  // 60%
     }
     else if(BATT_LEVEL>3900 && BATT_LEVEL<4000)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_6, 1);  // 70%
     }
    else if(BATT_LEVEL>4000 && BATT_LEVEL<4100)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_7, 1); // 80%
     }  
    else if(BATT_LEVEL>4100 && BATT_LEVEL<4200)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_8, 1); // 90%
     }
    else if(BATT_LEVEL>4200)
     {
     bsp_Function_To_Use(USER_BATTERY_INDICATOR_9, 1); // 100%
     }
    
    bsp_Function_To_Use(USER_VERSION, 2); 
     
    return Error;
}

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="bsp_CheckStatus">
USER_CONFIG_STATUS_TYPE bsp_CheckStatus()
{
    if(USER_OK != bsp_Function_To_Use(USER_COMM_STATUS, ChancesQty))
    {
        Set_ON_OFF_State(&BT_Model.Config, STATUS_OFF);
        return USER_FAILED;
    }
    return USER_OK;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="bsp_CheckBattery">
USER_CONFIG_STATUS_TYPE bsp_CheckBattery()
{
    USER_CONFIG_STATUS_TYPE Error = USER_OK;
    
    if(USER_OK == bsp_Function_To_Use(USER_BATTERY_STATUS, ChancesQty))
    {
        if(get_CHARGER_State(&BT_Model.Config) == CH_DISABLED)
        {
            if(get_BATT_LEVEL_State(&BT_Model.Config) < BAT_FULL_LEVEL)
            {
                if(USER_OK != bsp_Function_To_Use(USER_SET_BATT_CONFIG, ChancesQty))
                {
                    Error = USER_FAILED;
                }
            }
         
        }
        return USER_OK;
    }
    return USER_FAILED;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="bsp_CheckConfig">
USER_CONFIG_STATUS_TYPE bsp_CheckConfig()
{
    USER_CONFIG_STATUS_TYPE Error = USER_OK;
    
    if(get_ON_OFF_State(&BT_Model.Config) != STATUS_OFF)
    {
   
            //SET_BATT_CONFIG
            if(USER_OK != bsp_Function_To_Use(USER_SET_BATT_CONFIG, ChancesQty)) Error = USER_FAILED;
            //SET_BT_STATE_ENABLE_CONFIG
            if(USER_OK != bsp_Function_To_Use(USER_SET_BT_STATE_ENABLE_CONFIG, ChancesQty)) Error = USER_FAILED;
            //SET_AUDIO
            if(USER_OK != bsp_Function_To_Use(USER_SET_AUDIO, ChancesQty)) Error = USER_FAILED;
            //SET_AUDIO_ANALOG
            if(USER_OK != bsp_Function_To_Use(USER_SET_AUDIO_ANALOG, ChancesQty)) Error = USER_FAILED;
            //SET_DEEP_SLEEP_OFF
            if(USER_OK != bsp_Function_To_Use(USER_SET_DEEP_SLEEP_OFF, ChancesQty)) Error = USER_FAILED;
            //SET_ENABLE_LED_OFF
      //      if(USER_OK != bsp_Function_To_Use(USER_SET_ENABLE_LED_OFF, ChancesQty)) Error = USER_FAILED;  // COMMENTED
            //SET_ENABLE_BAT_INF_ON
      //      if(USER_OK != bsp_Function_To_Use(USER_SET_ENABLE_BAT_INF_ON, ChancesQty)) Error = USER_FAILED; // COMMENTED
             
            // NEWLY ADDED COMMANDS
    
      // set profiles      
      if(USER_OK != bsp_Function_To_Use(USER_SET_PROFILES, ChancesQty)) Error = USER_FAILED;
      // set autoconn
      if(USER_OK != bsp_Function_To_Use(USER_SET_AUTOCONN_ON, ChancesQty)) Error = USER_FAILED;
      // set max_rec
      if(USER_OK != bsp_Function_To_Use(USER_SET_MAX_REC, ChancesQty)) Error = USER_FAILED;
      // set cod
      if(USER_OK != bsp_Function_To_Use(USER_SET_COD, ChancesQty)) Error = USER_FAILED;
      // set codec
      if(USER_OK != bsp_Function_To_Use(USER_SET_CODEC, ChancesQty)) Error = USER_FAILED;
      // set enable_batt_indicator
      if(USER_OK != bsp_Function_To_Use(USER_SET_ENABLE_BATT_IND_ON, ChancesQty)) Error = USER_FAILED;
      // set enable led on
      if(USER_OK != bsp_Function_To_Use(USER_SET_ENABLE_LED_ON, ChancesQty)) Error = USER_FAILED;
      // set vreg role
      if(USER_OK != bsp_Function_To_Use(USER_SET_VREG_ROLE, ChancesQty)) Error = USER_FAILED;
      // set hfp profile
      if(USER_OK != bsp_Function_To_Use(USER_SET_HFP_CONFIG, ChancesQty)) Error = USER_FAILED;
            
      
            //WRITE
            if(USER_OK != bsp_Function_To_Use(USER_WRITE, ChancesQty)) Error = USER_FAILED;
            //RESET
            if(USER_OK != bsp_Function_To_Use(USER_RESET, ChancesQty)) Error = USER_FAILED;
      //  }
    }
    else
    {
        Error = USER_FAILED;
    }
    return Error;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios bsp_Configure_StartUp">
void bsp_Configure_StartUp()
{
    if(get_device_State(&BT_Model.Config) == SLEEP_UC)
    {
        // Deshabilita WEAK desde botón
        bsp_EX_INT0_InterruptDisable();
        // Deshabilita WEAK desde UART
        bsp_UART1_Weak_Disable ();
        // Pasa el módulo al estado START UP
        set_device_State(&BT_Model.Config, START_UP);
        // Setea el timeout para esperar que el módulo se prenda antes de consultar estado
        bsp_set_Status_Timeout(&System_timeouts, EACH2200ms);
    }
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios bsp_configureToSleepUC">
//void bsp_configureToSleepUC(Config_Data_t* Config)
//{
//    Config->STATE_ON_C;
//    LedRefresh(NONE_LED, EACH500ms, NOms, &System_timeouts);
//
//}
//</editor-fold>

//</editor-fold>