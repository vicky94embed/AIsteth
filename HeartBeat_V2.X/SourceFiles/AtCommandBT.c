/*******************************************************************************
 * Author: Boggio Pablo Miguel
 * Date: 27/08/2020
 * Current file: AtCommandBT.c
 * Version: 1.1
 ******************************************************************************/

//******************************************************************************
//                              INITIALIZATIONS
//******************************************************************************
//<editor-fold defaultstate="collapsed" desc="Initializations">

//<editor-fold defaultstate="collapsed" desc="Includes">
//******************************************************************************
//                                  INCLUDES
//******************************************************************************
#include "../bsp/bsp.h"
#include "AtCommandBT.h"
#include "../mcc_generated_files/uart1.h"
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Defines">
//******************************************************************************
//                                  DEFINES
//******************************************************************************
#define Timeout100ms    1
#define Timeout200ms    2
#define Timeout300ms    3
#define Timeout400ms    4
#define Timeout500ms    5

//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Structs">
//******************************************************************************
//                                  STRUCTS
//******************************************************************************
AtCommandBTPlatform AT_COMMANDS_BC127 = {
    "$$$$",
    "=",
    "ON",
    "OFF",
    "WRITE",
    "RESET",
    "GET ",
    "SET ",
    "ENTER_DATA_MODE",
    "BATTERY_STATUS",
    "BATT_CONFIG=ON 145 4150 1500 150",
    "DEEP_SLEEP",
    "ENABLE_LED",
    "NAME=AIHESRC2_1B8055",
    "STATUS",
    "STATE ",
    "TX_POWER=4 8",
    "BT_STATE_CONFIG=0 0",
    "BT_STATE_CONFIG=1 2", //1 1 //2 2 //1 2
    "AUDIO=0 0",
    "AUDIO_ANALOG=6 15 2 ON", //  AUDIO_ANALOG=11 15 2 ON"
    "CALL",
    "CLOSE",
    "RESTORE",
    "POWER",
    "ENABLE_BATT_IND",
    "AUTOCONN=1",   // AUTOCONNECT_LENGTH =10 (CHARACTERS)
    "COD=280918", //COD_LENGTH=10 (CHARACTER))
    "CODEC=3 OFF", // 
    "MAX_REC=3",
    "PROFILES=1 0 1 0 0 1 1 0 0 0 0 0", // "PROFILES=1 0 2 0 0 1 1 0 0 0 0 0" = source device. // "PROFILES=2 0 0 2 0 0 2 0 0 0 0 0" = sink device
    "VREG_ROLE=1",
    "HFP_CONFIG=OFF ON OFF OFF ON OFF",
    "AT 13 AT+IPHONEACCEV=1,1,0",
    "AT 13 AT+IPHONEACCEV=1,1,1",
    "AT 13 AT+IPHONEACCEV=1,1,2",
    "AT 13 AT+IPHONEACCEV=1,1,3",
    "AT 13 AT+IPHONEACCEV=1,1,4",
    "AT 13 AT+IPHONEACCEV=1,1,5",
    "AT 13 AT+IPHONEACCEV=1,1,6",
    "AT 13 AT+IPHONEACCEV=1,1,7",
    "AT 13 AT+IPHONEACCEV=1,1,8",
    "AT 13 AT+IPHONEACCEV=1,1,9",
    "VR_10.1"
    "ENABLE_BATT_IND=ON",
    " ",
    "\r\n",
    "?"
};

Battery_Status_Response_Struct Battery_Status_Response = {
    "FAST_CHARGE",
    "NO_POWER",
    "DISABLED",
    "STANDBY",
    "TRICKLE_CHARGE",
    "CHARGE_IN_PROGRESS",
    "CHARGE_COMPLETE"
};

ShortResponseBTStruct ShortResponses = {
    "OK",
    "ERROR" // ERROR 0x0012
};

AutonomusResponseBTStruct AutonomusResponses = {
    "Ready",
    "A2DP_STREAM_START",
    "CLOSE_OK", // CLOSE_OK 13 HFP 60AB674CC06B
                // CLOSE_OK 10 A2DP 60AB674CC06B
                // CLOSE_OK 11 AVRCP 60AB674CC06B
    "ROLE_OK",
    "AVRCP_PLAY", // AVRCP_PLAY 11
    "AVRCP_PAUSE", // AVRCP_PAUSE 11
    "SCO_OPEN", // SCO_OPEN 13
    "SCO_CLOSE", // SCO_CLOSE 13
    "CHARGE_IN_PROGRESS",
    "CHARGE_COMPLETE"
};
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Data init">
//******************************************************************************
//                                 DATA INIT
//******************************************************************************
const void* CommandModelType1 = NULL;
const void* RxFrameModel = NULL;

extern uint8_t ConnectedCounter;
extern uint8_t DisconnectCounter;

//Command_t Command;

//RX_Data_t RX_Data;

//extern Config_Data_t Config;

extern BT_Model_t BT_Model;

uint8_t CommunicationTimeout = 0;
uint8_t ON_OFF_Timeout = 0;
uint8_t Connection_Timeout = 0;
uint8_t Button_Timeout = 0;
//</editor-fold>
//</editor-fold>

//******************************************************************************
//                   SERVICIOS INTERRUPCIÓN BLUETOOTH
//******************************************************************************
//<editor-fold defaultstate="collapsed" desc="Servicio BT_Enable/Disable">

/************************* BT_Enable_Read Service *****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio BT_Enable_Read">
void BT_Enable_Read(){
    
    bsp_UART1_Read_Enable();
    
//    U1RXREG         = 0;
//    IFS0bits.U1RXIF = 0;
//    IEC0bits.U1RXIE = 1;
}
//</editor-fold>

/************************* BT_Disable_Read Service ****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio BT_Disable_Read">
void BT_Disable_Read(){
    bsp_UART1_Read_Disable();
//    IEC0bits.U1RXIE = 0;
}
//</editor-fold>
//</editor-fold>

//******************************************************************************
//                        SERVICIOS DEL BLUETOOTH
//******************************************************************************
//<editor-fold defaultstate="collapsed" desc="Servicio de uso BT">

/***************************** setConfig Service ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio setConfig">
CONFIG_STATUS_TYPE setConfig(CONFIG_TO_SET_TYPE SetData, BT_Model_t* BT_Mod)
{

    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Set_Command, Set_Command_Lenght);
        switch(SetData)
        {
            case SET_BT_NAME:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Name_Command, Name_Lenght);
                break;
            case SET_BATT_CONFIG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Batt_Config_Command, Batt_Config_Lenght);
                break;
            case SET_BT_STATE_DISABLE_CONFIG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BT_State_Config_Dis_Command, BT_State_Config_Lenght);
                break;
            case SET_BT_STATE_ENABLE_CONFIG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BT_State_Config_En_Command, BT_State_Config_Lenght);
                break;
            case SET_AUDIO:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Audio_Command, Audio_Lenght);
                break;
            case SET_AUDIO_ANALOG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Audio_Analog_Command, Audio_Analog_Lenght);
                break;
            case SET_DEEP_SLEEP_ON:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Deep_Sleep_Command, Deep_Sleep_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Equal_Command, Equal_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.ON_Command, ON_Command_Lenght);
                break;
            case SET_DEEP_SLEEP_OFF:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Deep_Sleep_Command, Deep_Sleep_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Equal_Command, Equal_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.OFF_Command, OFF_Command_Lenght);
                break;
            case SET_ENABLE_LED_ON:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Enable_Led_Command, Enable_Led_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Equal_Command, Equal_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.ON_Command, ON_Command_Lenght);
                break;
            case SET_ENABLE_LED_OFF:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Enable_Led_Command, Enable_Led_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Equal_Command, Equal_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.OFF_Command, OFF_Command_Lenght);
                break;
            case SET_ENABLE_BAT_INF_ON:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Enable_Batt_Ind_Command, Enable_Bat_Ind_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Equal_Command, Equal_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.ON_Command, ON_Command_Lenght);
                break;
            case SET_ENABLE_BAT_INF_OFF:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Enable_Batt_Ind_Command, Enable_Bat_Ind_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Equal_Command, Equal_Command_Lenght);
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.OFF_Command, OFF_Command_Lenght);
                break;
                
            case SET_AUTOCONN_ON:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Autoconnect_ON_Command, Autoconnect_ON_Lenght);
                break;
            case SET_COD:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.COD_Command, COD_Lenght);
                break;
            case SET_CODEC:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.CODEC_Command, CODEC_Lenght);
                break;
            case SET_MAX_REC:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.MAX_REC_Command, MAX_REC_Lenght);
                break;
            case SET_PROFILES:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.PROFILES_Command, PROFILES_Lenght);
                break;
            case SET_VREG_ROLE:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.VREG_ROLE_Command,VREG_ROLE_Lenght);
                break;
            case SET_HFP_CONFIG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.HFP_CONFIG_Command,HFP_CONFIG_Lenght);
                break;
            case USER_SET_ENABLE_BATT_IND_ON:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.ENABLE_BATT_IND_ON_Command,ENABLE_BATT_IND_ON_Lenght);
                break;
            default:
                break;
        }
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = SHORT_ANSWER_OK;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }
    
    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/***************************** getConfig Service ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio getConfig">

CONFIG_STATUS_TYPE getConfig(CONFIG_TO_GET_TYPE GetData, BT_Model_t* BT_Mod)
{
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Get_Command, Get_Command_Lenght);
        switch(GetData)
        {
            case GET_BT_NAME:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Name_Command, Name_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_NAME;
                break;
            case GET_BATT_CONFIG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Batt_Config_Command, Batt_Config_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_BATT_CONFIG;
                break;
            case GET_BT_STATE_CONFIG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BT_State_Config_Dis_Command, BT_State_Config_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_BT_STATE_CONFIG;
                break;
            case GET_AUDIO:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Audio_Command, Audio_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_AUDIO;
                break;
            case GET_AUDIO_ANALOG:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Audio_Analog_Command, Audio_Analog_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_AUDIO_ANALOG;
                break;
            case GET_DEEP_SLEEP:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Deep_Sleep_Command, Deep_Sleep_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_DEEP_SLEEP;
                break;
            case GET_ENABLE_LED:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Enable_Led_Command, Enable_Led_Command_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_ENABLE_LED;
                break;
            case GET_ENABLE_BAT_INF:
                setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Enable_Batt_Ind_Command, Enable_Bat_Ind_Lenght);
                BT_Mod->Command.ExpectedAnswerType  = GET_ANSWER_ENABLE_BAT_INF;
                break;
            default:
                break;
        }
        endStandarAssemblyString(&BT_Mod->Command);
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }
    
    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/**************************** setTxPower Service ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio setTxPower">

CONFIG_STATUS_TYPE setTxPower(BT_Model_t* BT_Mod)
{
    static uint8_t Initiated = 0;

    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.TX_Power_Command, TX_Power_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = SHORT_ANSWER_OK;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/**************************** Battery Status Service **************************/
//<editor-fold defaultstate="collapsed" desc="Servicio BatteryStatus">
CONFIG_STATUS_TYPE BatteryStatus(BT_Model_t* BT_Mod)
{    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Battery_Status_Command, Battery_Status_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = BATTERY_STATUS_ANSWER;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/****************************** Comm Status Service ***************************/
//<editor-fold defaultstate="collapsed" desc="Servicio CommStatus">
CONFIG_STATUS_TYPE CommStatus(BT_Model_t* BT_Mod)
{    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Status_Command, Status_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = STATUS_ANSWER;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/************************** saveConfigurations Service ************************/
//<editor-fold defaultstate="collapsed" desc="Servicio saveConfigurations">
CONFIG_STATUS_TYPE saveConfigurations(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Write_Command, Write_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = SHORT_ANSWER_OK;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/****************************** PowerOnOff Service ****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio PowerOnOff">
CONFIG_STATUS_TYPE PowerOnOff(CONFIG_ENABLE_TYPE OnOffState, BT_Model_t* BT_Mod)
{    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Power_Command, Power_Command_Lenght);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Space_Command, Space_Command_Lenght);
        if(OnOffState == CONFIG_OFF)
        {
            setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.OFF_Command, OFF_Command_Lenght);
        }
        else
        {
            setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.ON_Command, ON_Command_Lenght);
        }
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = SHORT_ANSWER_OK;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

/*************************** resetSystem Service ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio resetSystem">
CONFIG_STATUS_TYPE resetSystem(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Reset_Command, Reset_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
CONFIG_STATUS_TYPE BATTERY_INDICATOR_0(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_0_Command, BATTERY_INDICATOR_0_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_1(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_1_Command, BATTERY_INDICATOR_1_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_2(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_2_Command, BATTERY_INDICATOR_2_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_3(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_3_Command, BATTERY_INDICATOR_3_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_4(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_4_Command, BATTERY_INDICATOR_4_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_5(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_5_Command, BATTERY_INDICATOR_5_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_6(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_6_Command, BATTERY_INDICATOR_6_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE BATTERY_INDICATOR_7(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_7_Command, BATTERY_INDICATOR_7_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}


CONFIG_STATUS_TYPE BATTERY_INDICATOR_8(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_8_Command, BATTERY_INDICATOR_8_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}



CONFIG_STATUS_TYPE BATTERY_INDICATOR_9(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.BATTERY_INDICATOR_9_Command, BATTERY_INDICATOR_9_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}

CONFIG_STATUS_TYPE VERSION_NAME(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.VERSION_NAME_Command, VERSION_NAME_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}




CONFIG_STATUS_TYPE restoreSystem(BT_Model_t* BT_Mod){
    
    static uint8_t Initiated = 0;
    
    if(!Initiated)
    {
        ClearCommandModel(&BT_Mod->Command);
        startStandarAssemblyString(&BT_Mod->Command);
        setStringToSend(&BT_Mod->Command, AT_COMMANDS_BC127.Restore_Command, Restore_Command_Lenght);
        endStandarAssemblyString(&BT_Mod->Command);

        BT_Mod->Command.ExpectedAnswerType     = AUTO_ANSWER_READY;
        Initiated = 1;
        startConnectionTimeout(Timeout100ms);
    }

    switch(GeneralCommandProcess(BT_Mod))
    {   
        case OK:
            stopConnectionTimeout();
            Initiated = 0;
            return OK;
        case FAILED: default:
            stopConnectionTimeout();
            Initiated = 0;
            return FAILED;
        case PROCESSING:
            return PROCESSING;
    }
}
//</editor-fold>

//</editor-fold>


//******************************************************************************
//                        FUNCIONES COMUNICACION
//******************************************************************************
//<editor-fold defaultstate="collapsed" desc="Servicio de comunicación BT">

/************************ ResetBTLibrary Service ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio ResetBTLibrary">
void ResetBTLibrary(BT_Model_t* BT_Mod)
{
    ClearCommandModel(&BT_Mod->Command);
    ClearRXModel(&BT_Mod->RX_Data);
}
//</editor-fold>

/************************ setStringToSend Service *****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio setStringToSend">
CONFIG_STATUS_TYPE setStringToSend(Command_t* CommandModel, char* StringToAdd, uint8_t StringSize)
{   
    unsigned char i = 0;
    
    if(CommandModel->CommandModelReady == READY)
    {
        CommandModel->CommandModelReady = NOTREADY;
        for(i = 0; i < StringSize; i++)
            CommandModel->TxData.CommandString.TX_VECTOR[i + CommandModel->TxData.CommandString.StringSize] = StringToAdd[i];

        CommandModel->TxData.CommandString.StringSize += StringSize;
        CommandModel->CommandModelReady = READY;
        return  (OK);
    }
    return  (FAILED);
}
//</editor-fold>

/********************** CheckIfDataToSend Service *****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio CheckIfDataToSend">
CONFIG_STATUS_TYPE CheckIfDataToSend(Command_t* CommandModel)
{    
    CONFIG_STATUS_TYPE COnfigSt = OK;
    CommandModel->TxData.QtySent++; // Incremento los datos enviados
    
    if(CommandModel->TxData.QtySent == 0x11){
        COnfigSt = PROCESSING;
    }
    if(CommandModel->TxData.QtySent < CommandModel->TxData.CommandString.StringSize) // Pregunto si envié todos los datos del string
    {
        return (PROCESSING);
    }
    else if(CommandModel->TxData.QtySent == CommandModel->TxData.CommandString.StringSize)
    {
        return (OK);
    }
    else
    {
        return (FAILED);    
    }
}
//</editor-fold>

/************************* GetDataToSend Service ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio GetDataToSend">
uint8_t GetDataToSend(Command_t* CommandModel)
{    
    return ((uint8_t) CommandModel->TxData.CommandString.TX_VECTOR[CommandModel->TxData.QtySent]);
}
//</editor-fold>

/*********************** CommandModelStatus Service ***************************/
//<editor-fold defaultstate="collapsed" desc="Servicios CommandModelStatus">
CONFIG_STATUS_TYPE GetCommandModelStatus(Command_t* CommandModel)
{
    return (CommandModel->Status);
}

CONFIG_STATUS_TYPE SetCommandModelStatus(Command_t* CommandModel, CONFIG_STATUS_TYPE ConfigStatus)
{
    CommandModel->Status = ConfigStatus;
    return (OK);
}
//</editor-fold>

/******************* InternalCommandModelStatus Service ***********************/
//<editor-fold defaultstate="collapsed" desc="Servicios InternalCommandModelStatus">
CONFIG_STATUS_TYPE SetInternalCommandModelStatus(Command_t* CommandModel, MODEL_STATUS_TYPE ModelStatus)
{
    CommandModel->InternalStatus = ModelStatus;
    return (OK);
}
//</editor-fold>

/************************** InitCommandModel Service *******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio InitCommandModel">
const void* InitCommandModel(Command_t* CommandModel)
{
    ClearCommandModel(CommandModel);
    return (CommandModel);
}
//</editor-fold>

/******************** ClearCommandModel Service *******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio ClearCommandModel">
void ClearCommandModel(Command_t* CommandModel)
{
    unsigned char i;

    CommandModel->CommandModelReady     = NOTREADY;
    CommandModel->TxData.CommandString.StringSize      = 0;
    
    for(i = 0; i < TxVectorSize; i++)
    {
        CommandModel->TxData.CommandString.TX_VECTOR[i] = 0;
    }
    CommandModel->TxData.QtySent        = 0;
    CommandModel->TxData.StatusTxData   = OK;
    CommandModel->InternalStatus        = IDLE;
    CommandModel->ExpectedAnswerType    = NO_ANSWER;
    CommandModel->Status                = OK;
    CommandModel->CommandModelReady     = READY;
}
//</editor-fold>

/****************** GeneralCommandProcess Service *****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio GeneralCommandProcess">
CONFIG_STATUS_TYPE GeneralCommandProcess(BT_Model_t* BT_Mod)
{
    static uint8_t Steps;
    
    switch(Steps)
    {
        case 0:
            if(!getCommunicationTimeoutStatus())
            {
                switch(startToSend(BT_Mod))
                {
                    case OK:        
                        Steps++;
                        if(BT_Mod->RX_Data.AnswerType == AUTO_ANSWER_READY)
                        {
                            startConnectionTimeout(Timeout500ms);
                        }
                        else
                        {
                            startConnectionTimeout(Timeout100ms);
                        }
                        return PROCESSING;
                    case FAILED: default:
                        stopConnectionTimeout();
                        Steps = 0;
                        return FAILED;
                    case PROCESSING:
                        return PROCESSING;
                }
            }
            else
            {
                Steps = 0;
                return FAILED;
            }
            break;
        case 1:
            if(!getCommunicationTimeoutStatus())
            {
                switch(GeneralResponseReview(BT_Mod))
                {
                    case OK:
                        Steps = 0;
                        stopConnectionTimeout();
                        return OK;
                    case FAILED: default:
                        Steps = 0;
                        StopAnswerWaiting(BT_Mod);
                        return FAILED;
                    case PROCESSING:
                        return PROCESSING;   
                }
            }
            else
            {
                Steps = 0;
                StopAnswerWaiting(BT_Mod);
                return FAILED;
            }
            break;
        default:
            Steps = 0;
            return FAILED;
            break;
    }
}
//</editor-fold>

/************************** startToSend Service *******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio startToSend">
CONFIG_STATUS_TYPE startToSend(BT_Model_t* BT_Mod)
{
    if(BT_Mod->Command.Status == PROCESSING) // Si el modelo de comando está en proceso
    {
        switch(BT_Mod->Command.InternalStatus) // Si el modelo de TX está libre
        {
            case WRITING_END:
                if(BT_Mod->Command.TxData.QtySent == 0) // Si no hay datos mandando
                {
                    if(bsp_UART1_is_tx_ready()) // Si la UART está libre
                    {
                        //StopAnswerWaiting(CommandModel->ExpectedAnswerType); // Borro el vector de recepción que voy a usar
                        //SetAnswerRxModel(&BT_Mod->RX_Data, &BT_Mod->Command.ExpectedAnswerType);
                        BT_Mod->RX_Data.AnswerType = BT_Mod->Command.ExpectedAnswerType;
                        BT_Mod->Command.InternalStatus        = SENDING;
                        BT_Mod->Command.TxData.StatusTxData   = PROCESSING; // Seteo el modelo de TX en proceso
                        bsp_UART1_clear_interrupt_flag();               // Borro la interrupción
                        //bsp_UART1_Write((uint8_t) &BT_Mod->Command.TxData.CommandString.TX_VECTOR[0]);
                        bsp_UART1_Write((uint8_t) BT_Mod->Command.TxData.CommandString.TX_VECTOR[0]);
                        return (PROCESSING);
                    }
                }
//                ClearCommandModel(&Command);
                return (FAILED);
                break;
            case SENDING:
                return (PROCESSING);
                break;
            case SENDING_END:
//                ClearCommandModel(&Command);
                return (OK);
                break;
            default:
//                ClearCommandModel(&Command);
                return (FAILED);
                break;
        }
    }
    return (FAILED);
}
//</editor-fold>

/****************** GeneralResponseReview Service *****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio GeneralResponseReview">
CONFIG_STATUS_TYPE GeneralResponseReview(BT_Model_t* BT_Mod)
{
    if(OK == CheckResponseType(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector0, BT_Mod))
    {
        ClearRXModel(&BT_Mod->RX_Data);
        //ClearRxIntModel(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector0);
        return OK;
    }
    if(OK == CheckResponseType(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector1, BT_Mod))
    {
        ClearRXModel(&BT_Mod->RX_Data);
        //ClearRxIntModel(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector1);
        return OK;
    }
    if(OK == CheckResponseType(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector2, BT_Mod))
    {
        ClearRXModel(&BT_Mod->RX_Data);
        //ClearRxIntModel(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector2);
        return OK;
    }
    if(OK == CheckResponseType(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector3, BT_Mod))
    {
        ClearRXModel(&BT_Mod->RX_Data);
        //ClearRxIntModel(&BT_Mod->RX_Data.ReceiveVector.ReceiveVector3);
        return OK;
    }
    //return FAILED;
    return PROCESSING;
}
//</editor-fold>

/******************** CheckResponseType Service *******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio CheckResponseType">
CONFIG_STATUS_TYPE CheckResponseType(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod)
{
    switch(BT_Mod->Command.ExpectedAnswerType)
    {
        case SHORT_ANSWER_OK:
            return CheckShortResponse(RxVectModel, ShortResponses.Ok_Response);
            break;
        case AUTO_ANSWER_READY:
            return CheckShortResponse(RxVectModel, AutonomusResponses.Ready_Response);
            break;
        case STATUS_ANSWER:
            return CheckStatusResponse(RxVectModel, BT_Mod);
            break;
        case BATTERY_STATUS_ANSWER:
            return CheckBatteryStatusResponse(RxVectModel, BT_Mod);
            break;
        case GET_ANSWER_BATT_CONFIG:
            return CheckBatteryConfigResponse(RxVectModel, BT_Mod);
            break;
        case GET_ANSWER_NAME:
            return CheckBTNameResponse(RxVectModel, BT_Mod);
            break;
        case GET_ANSWER_BT_STATE_CONFIG:
            return CheckBTStateConfigResponse(RxVectModel, BT_Mod);
            break;
        case NO_ANSWER: default:
            return FAILED;
            break;
    }
}
//</editor-fold>

/******************** StopAnswerWaiting Service *******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio StopAnswerWaiting">
void StopAnswerWaiting(BT_Model_t* BT_Mod)
{
    stopConnectionTimeout();
    BT_Mod->Command.ExpectedAnswerType = NO_ANSWER;
}
//</editor-fold>
//</editor-fold>

/****************************** InitRxModel Service ***************************/
//<editor-fold defaultstate="collapsed" desc="RxModel Services">
//<editor-fold defaultstate="collapsed" desc="Servicios InitRxModel">
const void* InitRxModel(RX_Data_t* RxModel)
{
    ClearRXModel(RxModel);
    return (RxModel);
}
//</editor-fold>

/***************************** ClearRXModel Service ***************************/
//<editor-fold defaultstate="collapsed" desc="Servicios ClearRXModel">
// Revised
void ClearRXModel(RX_Data_t* RxModel)
{

    RxModel->ModelStatus                = NOTREADY;
    RxModel->ReceiveVector.VectorToWrite = 0;
    /*************************************************/
    ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector0);
    /*************************************************/
    ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector1);
    /*************************************************/
    ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector2);
    /*************************************************/
    ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector3);
    /*************************************************/
    RxModel->AnswerType                 = NO_ANSWER;
    RxModel->ModelStatus                = IDLE;
    RxModel->ModelStatus                = READY;
}
//</editor-fold>

/****************************** ClearVector Service ***************************/
//<editor-fold defaultstate="collapsed" desc="Servicio ClearVector">
// Revised
void ClearVector(char* Vector, uint8_t size){
    
    unsigned char i;

    for(i = 0; i < size; i++)   Vector[i] = 0;
}
//</editor-fold>

/************************** ClearRxIntModel Services **************************/
//<editor-fold defaultstate="collapsed" desc="Servicio ClearRxIntModel">
// Revised
void ClearRxIntModel(RX_INT_UART_Data_t* RxIntRecModel) // CHECKED
{
    RxIntRecModel->ModelReady                   = NOTREADY;
    RxIntRecModel->ModelStatus                  = IDLE;
    RxIntRecModel->QtyReceived                  = 0;
    ClearVector(&RxIntRecModel->RX_VECTOR[0], RxIntVectorSize);
    RxIntRecModel->ModelReady                   = READY;
}
//</editor-fold>

/*********************** SetAnswerRxModel Services ***********************/
//<editor-fold defaultstate="collapsed" desc="Servicio SetAnswerrRxModel">
// Revised
//void SetAnswerRxModel(RX_Data_t* RxModel, EXPECTED_ANSWER_TYPE* AnswerTypeRec) // CHECKED
//{
//    RxModel->AnswerType = AnswerTypeRec;
//}
//</editor-fold>

/*********************** GetAnswerRxModel Services ***********************/
//<editor-fold defaultstate="collapsed" desc="Servicio GetAnswerrRxModel">
// Revised
EXPECTED_ANSWER_TYPE GetAnswerRxModel(RX_Data_t* RxModel) // CHECKED
{
    return RxModel->AnswerType;
}
//</editor-fold>

/*********************** ClearAnswerRxModel Services ***********************/
//<editor-fold defaultstate="collapsed" desc="Servicio CleaAnswerrRxModel">
// Revised
void ClearAnswerRxModel(RX_Data_t* RxModel) // CHECKED
{
    RxModel->AnswerType = NO_ANSWER;
}
//</editor-fold>

/********************** GetRecVectorModelStatus Services **********************/
//<editor-fold defaultstate="collapsed" desc="Servicio GetRecVectorModelStatus">
// Revised
MODEL_STATUS_TYPE GetRecVectorModelStatus(RX_Data_t* RxModel, uint8_t VectNmbr) // CHECKED
{
    switch(VectNmbr)
    {
        case 0: return RxModel->ReceiveVector.ReceiveVector0.ModelStatus;
            break;
        case 1: return RxModel->ReceiveVector.ReceiveVector1.ModelStatus;
            break;
        case 2: return RxModel->ReceiveVector.ReceiveVector2.ModelStatus;
            break;
        case 3: return RxModel->ReceiveVector.ReceiveVector3.ModelStatus;
            break;
        default: return MODEL_STATUS_NO_STARTED;
    }   
}
//</editor-fold>

/********************** GetRecVectorModelReady Services ***********************/
//<editor-fold defaultstate="collapsed" desc="Servicio GetRecVectorModelReady">
// Revised
MODEL_READY_STATUS_TYPE GetRecVectorModelReady(RX_Data_t* RxModel, uint8_t VectNmbr) // CHECKED
{
    switch(VectNmbr)
    {
        case 0: return RxModel->ReceiveVector.ReceiveVector0.ModelReady;
            break;
        case 1: return RxModel->ReceiveVector.ReceiveVector1.ModelReady;
            break;
        case 2: return RxModel->ReceiveVector.ReceiveVector2.ModelReady;
            break;
        case 3: return RxModel->ReceiveVector.ReceiveVector3.ModelReady;
            break;
        default: return MODEL_READY_NO_STARTED;
    }   
}
//</editor-fold>
//</editor-fold>

/******************************* RX Data Services *****************************/
//<editor-fold defaultstate="collapsed" desc="Rx DATA Services">
/*********************** ManageNewReceivedData Service ************************/
//<editor-fold defaultstate="collapsed" desc="Servicio ManageNewReceivedData">
void ManageNewReceivedData(RX_Data_t* RxModel, unsigned char RxNewData)
{
    MODEL_STATUS_TYPE Status = MODEL_STATUS_NO_STARTED;
    
    switch(RxModel->ReceiveVector.VectorToWrite)
    {
        case 0: default:
            Status = SetNewReceivedData(&RxModel->ReceiveVector.ReceiveVector0, RxNewData);
            break;
        case 1:
            Status = SetNewReceivedData(&RxModel->ReceiveVector.ReceiveVector1, RxNewData);
            break;
        case 2:
            Status = SetNewReceivedData(&RxModel->ReceiveVector.ReceiveVector2, RxNewData);
            break;
        case 3:
            Status = SetNewReceivedData(&RxModel->ReceiveVector.ReceiveVector3, RxNewData);
            break;
    }
    if(Status == RECEIVING_END)
    {
        RxModel->ReceiveVector.VectorToWrite++;
        if(RxModel->ReceiveVector.VectorToWrite >= RxVectQty)
        {
            RxModel->ReceiveVector.VectorToWrite = 0;
        }
        switch(RxModel->ReceiveVector.VectorToWrite)
        {
            case 0: ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector0);
                break;
            case 1: ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector1);
                break;
            case 2: ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector2);
                break;
            case 3: ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector3);
                break;   
        }
    }
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio SetNewReceivedData">
// Revisado
MODEL_STATUS_TYPE SetNewReceivedData(RX_INT_UART_Data_t* RxModel, uint8_t RxNewData)
{
    if((RxModel->ModelStatus == RECEIVING) || (RxModel->ModelStatus == IDLE))
    {
        if(RxNewData < 123){
            //if((RxNewData == 0x0A) && (RxModel->RX_VECTOR[RxModel->QtyReceived - 1] == 0x0D)) // Pregunto si es = \r\n
            if((RxNewData == 0x0D)) // Pregunto si es = \r
            {
                if(RxModel->QtyReceived != 0)
                {
                    //RxModel->QtyReceived--; // le resto el \r a la trama
                    RxModel->ModelStatus    = RECEIVING_END;
                    RxModel->ModelReady     = READY;
                }
            }
            else
            {   
                RxModel->RX_VECTOR[RxModel->QtyReceived] = (char) RxNewData;

                RxModel->ModelStatus = RECEIVING;
                RxModel->QtyReceived++;
                if(RxModel->QtyReceived >= RxIntVectorSize)
                {
//                    bsp_BT_Disable_Read();
                    ClearRxIntModel(RxModel);
//                    bsp_BT_Enable_Read();
                }
            }
        }
    }
    return (RxModel->ModelStatus);
}
//</editor-fold>

/************************ CheckForReadyBTInfo Service *************************/
//<editor-fold defaultstate="collapsed" desc="Servicio CheckForReadyBTInfo">
void CheckForReadyBTInfo(RX_Data_t* RxModel)
{
    if(RxModel->ReceiveVector.ReceiveVector0.ModelStatus == RECEIVING_END)
    {
        if(RxModel->ReceiveVector.ReceiveVector0.ModelReady == READY)
        {
            RxModel->ReceiveVector.ReceiveVector0.ModelReady = NOTREADY;
            SetReceivedDataType(&RxModel->ReceiveVector.ReceiveVector0, RxModel);
            ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector0);
        }
    }
    if(RxModel->ReceiveVector.ReceiveVector1.ModelStatus == RECEIVING_END)
    {
        if(RxModel->ReceiveVector.ReceiveVector1.ModelReady == READY)
        {
            RxModel->ReceiveVector.ReceiveVector1.ModelReady = NOTREADY;
            SetReceivedDataType(&RxModel->ReceiveVector.ReceiveVector1, RxModel);
            ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector1);
        }
    }
    if(RxModel->ReceiveVector.ReceiveVector2.ModelStatus == RECEIVING_END)
    {
        if(RxModel->ReceiveVector.ReceiveVector2.ModelReady == READY)
        {
            RxModel->ReceiveVector.ReceiveVector2.ModelReady = NOTREADY;
            SetReceivedDataType(&RxModel->ReceiveVector.ReceiveVector2, RxModel);
            ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector2);
        }
    }
    if(RxModel->ReceiveVector.ReceiveVector3.ModelStatus == RECEIVING_END)
    {
        if(RxModel->ReceiveVector.ReceiveVector3.ModelReady == READY)
        {
            RxModel->ReceiveVector.ReceiveVector3.ModelReady = NOTREADY;
            SetReceivedDataType(&RxModel->ReceiveVector.ReceiveVector3, RxModel);
            ClearRxIntModel(&RxModel->ReceiveVector.ReceiveVector3);
        }
    }
}
//</editor-fold>

/************************** SetReceivedDataType Services **********************/
//<editor-fold defaultstate="collapsed" desc="Servicio SetReceivedDataType">
CONFIG_STATUS_TYPE SetReceivedDataType(RX_INT_UART_Data_t* RxIntModel, RX_Data_t* RxModel)
{
    CONFIG_STATUS_TYPE Status = OK;

    return Status;
}
//</editor-fold>
//</editor-fold>

/************************** CheckResponses Services ***************************/
//<editor-fold defaultstate="collapsed" desc="Servicio CheckResponses">
//<editor-fold defaultstate="collapsed" desc="Servicio CheckShortResponse">
CONFIG_STATUS_TYPE CheckShortResponse(RX_INT_UART_Data_t* RxVectModel, char* ToCompare)
{
    CONFIG_STATUS_TYPE InternalStatus = PROCESSING;
    
    if(RxVectModel->ModelStatus == RECEIVING_END){
        if(RxVectModel->ModelReady != NOTREADY)
        {
            RxVectModel->ModelReady = NOTREADY;
            InternalStatus = StringCompare(RxVectModel, ToCompare, 0, RxVectModel->QtyReceived);
            RxVectModel->ModelReady = READY;
        }
    }
    return InternalStatus;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio CheckStatusResponse">
CONFIG_STATUS_TYPE CheckStatusResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod)
{
    CONFIG_STATUS_TYPE InternalStatus = PROCESSING;
    uint8_t index = 16;
    
    if(RxVectModel->ModelStatus == RECEIVING_END)
    {
        if(RxVectModel->ModelReady != NOTREADY)
        {
            RxVectModel->ModelReady = NOTREADY;
            
            InternalStatus = StringCompare(RxVectModel, AT_COMMANDS_BC127.State_Command, 0, State_Command_Lenght);
            
            if(InternalStatus == OK)
            {
                // Primero veo si está prendido
                if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.OFF_Command, State_Command_Lenght, OFF_Command_Lenght))
                {
                    BT_Mod->Config.STATE_ON_C           = STATUS_SLEEP;
                    BT_Mod->Config.STATE_CONNECTED_C    = 0;
                    BT_Mod->Config.STATE_CONNECTABLE_C  = 0;
                    BT_Mod->Config.STATE_DISCOVERABLE_C = 0;
                }
                else
                {
                    BT_Mod->Config.STATE_ON_C           = STATUS_ON;
                    BT_Mod->Config.STATE_CONNECTED_C = RxVectModel->RX_VECTOR[index] - '0';
                    index += 15;
                    if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.ON_Command, index, ON_Command_Lenght))
                    {
                        BT_Mod->Config.STATE_CONNECTABLE_C = STATUS_CONNECTABLE;
                        index += 17;
                    }
                    else
                    {
                        BT_Mod->Config.STATE_CONNECTABLE_C = STATUS_NO_CONNECTABLE;
                        index = 18;
                    }
                    if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.ON_Command, index, ON_Command_Lenght))
                    {
                        BT_Mod->Config.STATE_DISCOVERABLE_C = STATUS_DISCOVERABLE;
                    }
                    else
                    {
                        BT_Mod->Config.STATE_DISCOVERABLE_C = STATUS_NO_DISCOVERABLE;
                    }                    
                }
            }
            RxVectModel->ModelReady = READY;
        }
    }
    return InternalStatus;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio CheckBatteryStatusResponse">
CONFIG_STATUS_TYPE CheckBatteryStatusResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod)
{
    //RX_BATT_STAT_Data_t RX_BATT_STAT_Data;
    CONFIG_STATUS_TYPE InternalStatus = PROCESSING;
//    uint8_t index = 16;
    uint8_t ChargerStateSize = 0;
    
    if(RxVectModel->ModelStatus == RECEIVING_END)
    {
        if(RxVectModel->ModelReady != NOTREADY)
        {
            RxVectModel->ModelReady = NOTREADY;
            InternalStatus = OK;
            
                 
                 
            if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.Battery_Status_Command, 0, Battery_Status_Command_Lenght))
            {
                //if(OK == ConvertAsciiDecimalToNumber(&RxVectModel->RX_VECTOR[RxVectModel->QtyReceived-4], &BT_Mod->Config.BATTERY_STATUS_LEVEL_C, BATT_LEVEL_SIZE)) // Convierte y guarda nivel de batería
                if(OK == ConvertAsciiDecimalToNumber(RxVectModel, RxVectModel->QtyReceived-BATT_LEVEL_SIZE, BATT_LEVEL_SIZE, &BT_Mod->Config.BATTERY_STATUS_LEVEL_C)) // Convierte y guarda nivel de batería
                {
                    ChargerStateSize = RxVectModel->QtyReceived - (BATT_LEVEL_SIZE+1) - (BATT_STATUS_RESPONSE_SIZE+1); 
                    
                    if(ChargerStateSize == Fast_Charge_Response_Lenght)  // Chequea estado de carga
                    {
                        if(OK != StringCompare(RxVectModel, Battery_Status_Response.Fast_Charge_Response, BATT_STATUS_RESPONSE_SIZE+1, Fast_Charge_Response_Lenght))
                        {
                            InternalStatus = FAILED;
                            BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;
                        }
                        else
                        {
                            BT_Mod->Config.BATTERY_STATUS_C = CH_FAST_CHARGE;
                        }
                    }
                    else if(ChargerStateSize == No_Power_Response_Lenght)  // Chequea estado de carga
                    {
                        if(OK != StringCompare(RxVectModel, Battery_Status_Response.No_Power_Response, BATT_STATUS_RESPONSE_SIZE+1, No_Power_Response_Lenght))
                        {
                            if(OK != StringCompare(RxVectModel, Battery_Status_Response.Disabled_Response, BATT_STATUS_RESPONSE_SIZE+1, Disabled_Response_Lenght))
                            {
                                InternalStatus = FAILED;
                                BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;
                            }
                            else
                            {
                                BT_Mod->Config.BATTERY_STATUS_C = CH_DISABLED;
                            }
                        }
                        else
                        {
                            BT_Mod->Config.BATTERY_STATUS_C = CH_NO_POWER;
                        }
                    }
                    else if(ChargerStateSize == Stanby_Response_Lenght)  // Chequea estado de carga
                    {
                        if(OK != StringCompare(RxVectModel, Battery_Status_Response.Stanby_Response, BATT_STATUS_RESPONSE_SIZE+1, Stanby_Response_Lenght))
                        {
                            InternalStatus = FAILED;
                            BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;
                        }
                        else
                        {
                            BT_Mod->Config.BATTERY_STATUS_C = CH_STANBY;
                        }
                    }
                    else if(ChargerStateSize == Trickle_Response_Lenght)  // Chequea estado de carga
                    {
                        if(OK != StringCompare(RxVectModel, Battery_Status_Response.Trickle_Response, BATT_STATUS_RESPONSE_SIZE+1, Trickle_Response_Lenght))
                        {
                            InternalStatus = FAILED;
                            BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;
                        }
                        else
                        {
                            BT_Mod->Config.BATTERY_STATUS_C = CH_TRICKLE;
                        }
                    }
                    else if(ChargerStateSize == Chrg_in_Progess_Response_Lenght)  // ADDED
                    {
                        if(OK != StringCompare(RxVectModel, Battery_Status_Response.Charge_in_Progress_Response, BATT_STATUS_RESPONSE_SIZE+1, Chrg_in_Progess_Response_Lenght))
                        {
                            InternalStatus = FAILED;
                            BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;
                        }
                        else
                        {
                            BT_Mod->Config.BATTERY_STATUS_C = CH_CHARGE_IN_PROGRESS; //ADDED
                        }
                    }
                    else if(ChargerStateSize == Chrg_Complete_Response_Lenght)  // ADDED
                    {   
                        
                        if(OK != StringCompare(RxVectModel, Battery_Status_Response.Charge_Complete_Response, BATT_STATUS_RESPONSE_SIZE+1, Chrg_Complete_Response_Lenght))
                        {
                            InternalStatus = FAILED;
                            BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;    // CH_NO_STATE
                        }
                       else
                        {
                            BT_Mod->Config.BATTERY_STATUS_C = CH_CHARGE_COMPLETE; //ADDED  // CH_CHARGE_COMPLETE
                        }
                       
                       
                    }
                    else
                    {
                        InternalStatus = FAILED;
                        BT_Mod->Config.BATTERY_STATUS_C = CH_NO_STATE;
                    }
                }
                else
                {
                    InternalStatus = FAILED;
                }
            }
            else
            {
                InternalStatus = FAILED;
            }
            RxVectModel->ModelReady = READY;
        }
    }
    return InternalStatus;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio CheckBatteryConfigResponse">
CONFIG_STATUS_TYPE CheckBatteryConfigResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod)
{
    //RX_BATT_STAT_Data_t RX_BATT_STAT_Data;

    uint8_t index = Battery_Status_Command_Lenght + 1;
    uint8_t NextDataSize = 0;
    
    if(RxVectModel->ModelStatus == RECEIVING_END)
    {
        if(RxVectModel->ModelReady != NOTREADY)
        {
            RxVectModel->ModelReady = NOTREADY;
            
            //////////////////////////////////////

            if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.Batt_Config_Command, 0, Battery_Status_Command_Lenght))
            {
                if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.ON_Command, index, ON_Command_Lenght))
                {
                    BT_Mod->Config.BATT_CONFIG_ENABLE_C = 1;
                    index += 3;
                }
                else
                {
                    BT_Mod->Config.BATT_CONFIG_ENABLE_C = 0;
                    index += 4;
                }
                
                NextDataSize = SearchSpace(RxVectModel, index, RxVectModel->QtyReceived);
                if(OK == ConvertAsciiDecimalToNumber(RxVectModel, index, NextDataSize, &BT_Mod->Config.BATT_CONFIG_CRITICAL_LEVEL_C))
                //if(OK == ConvertAsciiDecimalToNumber(&RxVectModel->RX_VECTOR[index], &BT_Mod->Config.BATT_CONFIG_CRITICAL_LEVEL_C, NextDataSize))
                {
                    index += NextDataSize + 1;
                    NextDataSize = SearchSpace(RxVectModel, index, RxVectModel->QtyReceived);
                    if(OK == ConvertAsciiDecimalToNumber(RxVectModel, index, NextDataSize, &BT_Mod->Config.BATT_CONFIG_CHARGE_LEVEL_C))
                    //if(OK == ConvertAsciiDecimalToNumber(&RxVectModel->RX_VECTOR[index], &BT_Mod->Config.BATT_CONFIG_CHARGE_LEVEL_C, NextDataSize))
                    {
                        index += NextDataSize + 1;
                        NextDataSize = SearchSpace(RxVectModel, index, RxVectModel->QtyReceived);
                        if(OK == ConvertAsciiDecimalToNumber(RxVectModel, index, NextDataSize, &BT_Mod->Config.BATT_CONFIG_THERMISTOR_LEVEL_C))
                        //if(OK == ConvertAsciiDecimalToNumber(&RxVectModel->RX_VECTOR[index], &BT_Mod->Config.BATT_CONFIG_THERMISTOR_LEVEL_C, NextDataSize))
                        {
                            index = RxVectModel->QtyReceived - (index + NextDataSize + 1);
                            if(OK == ConvertAsciiDecimalToNumber(RxVectModel, index, RxVectModel->QtyReceived - index, &BT_Mod->Config.BATT_CONFIG_CHARGING_CURRENT_C))
                            //if(OK == ConvertAsciiDecimalToNumber(&RxVectModel->RX_VECTOR[index], &BT_Mod->Config.BATT_CONFIG_CHARGING_CURRENT_C, RxVectModel->QtyReceived - index))
                            {
                                RxVectModel->ModelReady = READY;
                                return OK;
                            }
                        }
                    }
                }
            }
            RxVectModel->ModelReady = READY;
        }
    }
    return FAILED;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio CheckBTStateConfigResponse">
CONFIG_STATUS_TYPE CheckBTStateConfigResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod)
{
    if(RxVectModel->ModelStatus == RECEIVING_END)
    {
        if(RxVectModel->ModelReady != NOTREADY)
        {
            RxVectModel->ModelReady = NOTREADY;
            if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.BT_State_Config_Dis_Command, 0, BT_State_Config_Command_Lenght))
            {
                BT_Mod->Config.BT_STATE_CONFIG_CONNECTABLE_C = RxVectModel->RX_VECTOR[BT_State_Config_Connec_Pos] - '0';
                BT_Mod->Config.BT_STATE_CONFIG_DISCOVERABLE_C = RxVectModel->RX_VECTOR[BT_State_Config_Discov_Pos] - '0';
                RxVectModel->ModelReady = READY;
                return OK;
            }
            RxVectModel->ModelReady = READY;
        }
    }
    return FAILED;
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio CheckBTNameResponse">
CONFIG_STATUS_TYPE CheckBTNameResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod)
{
    if(RxVectModel->ModelStatus == RECEIVING_END)
    {
        if(RxVectModel->ModelReady != NOTREADY)
        {
            RxVectModel->ModelReady = NOTREADY;
            if(OK == StringCompare(RxVectModel, AT_COMMANDS_BC127.Name_Command, 0, Name_Lenght))
            {
                RxVectModel->ModelReady = READY;
                return OK;
            }
            RxVectModel->ModelReady = READY;
        }
    }
    return FAILED;
}
//</editor-fold>
//</editor-fold>



/***************************** Servicios varios *******************************/
//<editor-fold defaultstate="collapsed" desc="Servicio Varios">
//<editor-fold defaultstate="collapsed" desc="Servicio StringCompare">
CONFIG_STATUS_TYPE StringCompare(RX_INT_UART_Data_t* RxVectModel, char* ToCompare, uint8_t Start, uint8_t size)
{
    uint8_t i;
                
    for(i = 0; i < size; i++){ // Comparo el inicio de la respuesta
        if(RxVectModel->RX_VECTOR[i+Start] != ToCompare[i])
        {
            return (FAILED);
        }
    }
    return(OK);
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio SearchSpace">
uint8_t SearchSpace(RX_INT_UART_Data_t* RxVectModel, uint8_t Start, uint8_t VectSize)
{
    uint8_t i, count = 0;
                
    for(i = Start; i < (VectSize - Start); i++){ // Comparo el inicio de la respuesta
        if(RxVectModel->RX_VECTOR[i] != " ")
        {
            count++;
        }
        else
        {
            return count;
        }
    }
    return 0;
}
//</editor-fold>

/********************* ConvertAsciiDecimalToNumber Service ********************/
//<editor-fold defaultstate="collapsed" desc="Servicio ConvertAsciiDecimalToNumber">

//CONFIG_STATUS_TYPE ConvertAsciiDecimalToNumber(char* CharStringToConvert, uint16_t* FinalValue, uint8_t StringCharSize)
CONFIG_STATUS_TYPE ConvertAsciiDecimalToNumber(RX_INT_UART_Data_t* RxVectModel, uint8_t StringCharSTART, uint8_t StringCharSize, uint16_t* FinalValue)
{
    uint8_t i = 0;
    uint16_t Value = 0;
    uint16_t InternalValue = 0;
    uint16_t Multiplier = 1;

    for(i = 0; i < StringCharSize; i++){ // Los 3 ultimos no se usan

        //Value = (uint16_t) *CharStringToConvert + StringCharSize - i;
        Value = (uint16_t) RxVectModel->RX_VECTOR[(StringCharSTART - 1) + (StringCharSize - i)];
        if((Value > 47) && (Value < 58))
        {
            Value -= 48;
            Value *= Multiplier;
            InternalValue += Value;
        }
        else
        {
            return (FAILED);
        }
        Multiplier *= 10;
    }
    *FinalValue = InternalValue;
    return OK;
}
//</editor-fold>

/************************ NumberToAsciiNumber Service *************************/
//<editor-fold defaultstate="collapsed" desc="Servicios NumberToAsciiNumber">
uint8_t NumberToAsciiNumber(uint32_t Number, char* Vector)
{
    uint8_t i;
    uint8_t DigitQty = 0;
    uint32_t NumberAux = Number;
    
    while (NumberAux) 
    { 
        NumberAux = NumberAux/10;
        DigitQty++;
    }
    
    if(!DigitQty) DigitQty++;

    for (i = DigitQty; i > 0; i--) 
    { 
        Vector[i - 1] = (Number%10) + '0'; 
        Number = Number/10;
    }
    return DigitQty;
}
//</editor-fold>

/********************* startStandarAssemblyString Service *********************/
//<editor-fold defaultstate="collapsed" desc="Servicio startStandarAssemblyString">
CONFIG_STATUS_TYPE startStandarAssemblyString(Command_t* CommandModel)
{
    if(OK != GetCommandModelStatus(CommandModel)) return FAILED;
    if(OK != SetCommandModelStatus(CommandModel, PROCESSING)) return FAILED;
    if(OK != SetInternalCommandModelStatus(CommandModel, WRITING)) return FAILED;
    return OK;
}
//</editor-fold>

/*********************** endStandarAssemblyString Service *********************/
//<editor-fold defaultstate="collapsed" desc="Servicio endStandarAssemblyString">
CONFIG_STATUS_TYPE endStandarAssemblyString(Command_t* CommandModel)
{
//    if(OK != setStringToSend(CommandModel, AT_COMMANDS_BC127.END_Command, END_Command_Lenght)) return FAILED;
//    if(OK != setStringToSend(CommandModel, AT_COMMANDS_BC127.RETURN_Command, RETURN_Command_Lenght)) return FAILED;
    if(OK != SetInternalCommandModelStatus(CommandModel, WRITING_END)) return FAILED;
    return OK;
}
//</editor-fold>

/***************************** hexToAscii Service *****************************/
//<editor-fold defaultstate="collapsed" desc="Servicio hexToAscii">
//char hexToAscii(uint8_t Hex)
//{
//    if(Hex < 10) return Hex + '0';
//    if(Hex < 16) return Hex - 10 + 'a';
//}
//</editor-fold>

/******************* ConvertAsciiCharIntoUint8Vector Service ******************/
//<editor-fold defaultstate="collapsed" desc="Servicio ConvertAsciiCharIntoUint8Vector">
CONFIG_STATUS_TYPE ConvertAsciiCharIntoUint8Vector(char* CharStringToConvert, uint8_t* FinalVector, uint8_t StringCharSize)
{
    uint8_t i, j = 0, k = 0;
    uint8_t less;

    for(i = 0; i < StringCharSize; i++){ // Los 3 ultimos no se usan

        less = 0;
        if(CharStringToConvert[i] > 47)
        {
            if(CharStringToConvert[i] < 58)
                less = 48;
            else if(CharStringToConvert[i] > 64)
            {
                if(CharStringToConvert[i] < 91)
                    less = 55;
                else if(CharStringToConvert[i] > 96)
                {
                    if(CharStringToConvert[i] < 123)
                        less = 87;
                }
            }
        }

        if(less)
        {
            if(!j) // Parte alta
                FinalVector[k] = (CharStringToConvert[i] - less) << 4;
            else // Parte baja
                FinalVector[k] += (CharStringToConvert[i] - less);

            if(j){
                j = 0;
                k++;
            }            
            else
            {
                j = 1;
            }
        }
    }
    return OK;
}
//</editor-fold>

/************************* ConnectionTimeout Services *************************/
//<editor-fold defaultstate="collapsed" desc="Servicios ConnectionTimeout">
void startConnectionTimeout(uint8_t Period)
{
    bsp_TMR1_Initialize (Period);
    bsp_clearCommunicationTimeout ();
    bsp_TMR1_Start();
}

void stopConnectionTimeout()
{
    bsp_TMR1_Stop();
    bsp_clearCommunicationTimeout();
}

void setCommunicationTimeout()
{
    CommunicationTimeout = TIMEOUT_ON;
}

void clearCommunicationTimeout()
{
    CommunicationTimeout = TIMEOUT_OFF;
}

uint8_t getCommunicationTimeoutStatus()
{
    return CommunicationTimeout;
}
//</editor-fold>

/****************************** Timeout Services ******************************/
//<editor-fold defaultstate="collapsed" desc="Servicios UserTimeout">

//<editor-fold defaultstate="collapsed" desc="Servicios Button_Timeout">
void set_Button_Timeout()
{
    Button_Timeout = TIMEOUT_ON;
}
void clear_Button_Timeout()
{
    Button_Timeout = TIMEOUT_OFF;
}
uint8_t get_Button_Timeout()
{
    return Button_Timeout;
}
//</editor-fold>

//</editor-fold>
//</editor-fold>
//</editor-fold>