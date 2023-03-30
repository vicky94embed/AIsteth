/*******************************************************************************
* Author: Pablo Boggio
* Date: 04/08/2020
* Current file: AtCommandBT.h
* Version: 2.0
*******************************************************************************/

#ifndef _ATCOMMANDBT_H_
#define _ATCOMMANDBT_H_

#include "../bsp/bsp.h"
 
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

extern const void* CommandModelType1;
extern const void* RxFrameModel;

extern uint8_t CommunicationTimeout;

/********************************* DEFINES ************************************/
//<editor-fold defaultstate="collapsed" desc="Lenght commands defines">
#define AT_Command_Lenght               4
#define Equal_Command_Lenght            1
#define ON_Command_Lenght               2
#define OFF_Command_Lenght              3
#define Write_Command_Lenght            5
#define Reset_Command_Lenght            5
#define Restore_Command_Lenght          7
#define Get_Command_Lenght              4
#define Set_Command_Lenght              4
#define Enter_Data_Mode_Command_Lenght  15
#define Battery_Status_Command_Lenght   14

#define Batt_Config_Lenght              32
#define Batt_Config_Command_Lenght      12

#define Deep_Sleep_Command_Lenght       10
#define Enable_Led_Command_Lenght       10

#define Name_Lenght                     20
#define Name_Command_Lenght             4

#define Status_Command_Lenght           6
#define State_Command_Lenght            6

#define TX_Power_Lenght                 12
#define TX_Power_Command_Lenght         9

#define BT_State_Config_Lenght          19
#define BT_State_Config_Command_Lenght  16
#define BT_State_Config_Connec_Pos      17
#define BT_State_Config_Discov_Pos      19

#define Audio_Lenght                    9
#define Audio_Command_Lenght            6

#define Audio_Analog_Lenght             22  //23
#define Audio_Analog_Command_Lenght     13

#define Call_Command_Lenght             4
#define Close_Command_Lenght            5
#define Restore_Command_Lenght          7
#define Power_Command_Lenght            5
#define Enable_Bat_Ind_Lenght           15
#define Autoconnect_ON_Lenght           10
#define COD_Lenght                      10
#define CODEC_Lenght                    11
#define MAX_REC_Lenght                  9
#define PROFILES_Lenght                 32  //21 //32
#define VREG_ROLE_Lenght                11
#define HFP_CONFIG_Lenght               32 //OFF=  33  //ON=32 
#define BATTERY_INDICATOR_0_Command_Lenght 26
#define BATTERY_INDICATOR_1_Command_Lenght 26
#define BATTERY_INDICATOR_2_Command_Lenght 26
#define BATTERY_INDICATOR_3_Command_Lenght 26
#define BATTERY_INDICATOR_4_Command_Lenght 26
#define BATTERY_INDICATOR_5_Command_Lenght 26
#define BATTERY_INDICATOR_6_Command_Lenght 26
#define BATTERY_INDICATOR_7_Command_Lenght 26
#define BATTERY_INDICATOR_8_Command_Lenght 26
#define BATTERY_INDICATOR_9_Command_Lenght 26
#define VERSION_NAME_Command_Lenght     7
#define ENABLE_BATT_IND_ON_Lenght       18
#define Space_Command_Lenght            1
#define END_Command_Lenght              2
#define RETURN_Command_Lenght           1


#define Fast_Charge_Response_Lenght     11
#define No_Power_Response_Lenght        8
#define Disabled_Response_Lenght        8
#define Stanby_Response_Lenght          7
#define Trickle_Response_Lenght         14
#define Chrg_in_Progess_Response_Lenght 18
#define Chrg_Complete_Response_Lenght   15

// ******************************************
#define TIMEOUT_OFF    0
#define TIMEOUT_ON     1
// ******************************************
//</editor-fold>

/********************************** ENUMS *************************************/
//<editor-fold defaultstate="collapsed" desc="Declaraciones de Enums">
//<editor-fold defaultstate="collapsed" desc="Enum CONFIG_STATUS_TYPE">
typedef enum CONFIG_STATUS_EN
{
    CONFIG_STATUS_NO_STARTED,
    OK,
    FAILED,
    PROCESSING
}CONFIG_STATUS_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum CONFIG_ENABLE_TYPE">
typedef enum CONFIG_ENABLE_EN
{
    MODEL_ENABLE_NO_STARTED,
    CONFIG_ON,
    CONFIG_OFF
}CONFIG_ENABLE_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum CONFIG_AUDIO_TYPE">
typedef enum CONFIG_AUDIO_EN
{
    AUDIO_ANALOG,
    AUDIO_DIGITAL
}CONFIG_AUDIO_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum CONFIG_TO_SET_TYPE">
typedef enum CONFIG_TO_SET_EN
{
    SET_BT_NAME,
    SET_BATT_CONFIG,
    SET_BT_STATE_DISABLE_CONFIG,
    SET_BT_STATE_ENABLE_CONFIG,
    SET_AUDIO,
    SET_AUDIO_ANALOG,
    SET_DEEP_SLEEP_ON,
    SET_DEEP_SLEEP_OFF,
    SET_ENABLE_LED_ON,
    SET_ENABLE_LED_OFF,
    SET_ENABLE_BAT_INF_ON,
    SET_ENABLE_BAT_INF_OFF,
    SET_AUTOCONN_ON,
    SET_COD,
    SET_CODEC,
    SET_ENABLE_BATT_IND_ON,
    SET_MAX_REC,
    SET_PROFILES,
    SET_VREG_ROLE,
    SET_HFP_CONFIG
}CONFIG_TO_SET_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum CONFIG_TO_GET_TYPE">
typedef enum CONFIG_TO_GET_EN
{
    GET_BT_NAME,
    GET_BATT_CONFIG,
    GET_BT_STATE_CONFIG,
    GET_AUDIO,
    GET_AUDIO_ANALOG,
    GET_DEEP_SLEEP,
    GET_ENABLE_LED,
    GET_ENABLE_BAT_INF
}CONFIG_TO_GET_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum MODEL_STATUS_TYPE">
typedef enum MODEL_STATUS_EN
{
    MODEL_STATUS_NO_STARTED,
    IDLE,
    WRITING,
    WRITING_END,
    SENDING,
    SENDING_END,
    RECEIVING,
    RECEIVING_END,
    READING,
    READING_END
}MODEL_STATUS_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum MODEL_READY_STATUS_TYPE">
typedef enum MODEL_READY_STATUS_EN
{
    MODEL_READY_NO_STARTED,
    READY,
    NOTREADY
}MODEL_READY_STATUS_TYPE;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Enum EXPECTED_ANSWER_TYPE">
typedef enum EXPECTED_ANSWER_EN
{
    NO_ANSWER,
    SHORT_ANSWER_OK,        // Generalmente se usa esta
    AUTO_ANSWER_READY,
    STATUS_ANSWER,
    BATTERY_STATUS_ANSWER,
    GET_ANSWER_BATT_CONFIG,
    GET_ANSWER_NAME,
    GET_ANSWER_BT_STATE_CONFIG,
    GET_ANSWER_ENABLE_LED,
    GET_ANSWER_AUDIO,
    GET_ANSWER_AUDIO_ANALOG,
    GET_ANSWER_DEEP_SLEEP,
    GET_ANSWER_ENABLE_BAT_INF
}EXPECTED_ANSWER_TYPE;
//</editor-fold>
//</editor-fold>

/******************************* CHAR STRINGS *********************************/
//<editor-fold defaultstate="collapsed" desc="Char Strings">
//<editor-fold defaultstate="collapsed" desc="Struct AT_COMMANDS_BC127">
typedef struct
{
    char* AT_Command;
    char* Equal_Command;
    char* ON_Command;
    char* OFF_Command;
    char* Write_Command;
    char* Reset_Command;
    char* Get_Command;
    char* Set_Command;
    char* Enter_Data_Mode_Command;
    char* Battery_Status_Command;
    char* Batt_Config_Command;
    char* Deep_Sleep_Command;
    char* Enable_Led_Command;
    char* Name_Command;
    char* Status_Command;
    char* State_Command;
    char* TX_Power_Command;
    char* BT_State_Config_Dis_Command;
    char* BT_State_Config_En_Command;
    char* Audio_Command;
    char* Audio_Analog_Command;
    char* Call_Command;
    char* Close_Command;
    char* Restore_Command;
    char* Power_Command;
    char* Enable_Batt_Ind_Command;
    char* Autoconnect_ON_Command;
    char* COD_Command;
    char* CODEC_Command;
    char* MAX_REC_Command;
    char* PROFILES_Command;
    char* VREG_ROLE_Command;
    char* HFP_CONFIG_Command;
    char* BATTERY_INDICATOR_0_Command;
    char* BATTERY_INDICATOR_1_Command;
    char* BATTERY_INDICATOR_2_Command;
    char* BATTERY_INDICATOR_3_Command;
    char* BATTERY_INDICATOR_4_Command;
    char* BATTERY_INDICATOR_5_Command;
    char* BATTERY_INDICATOR_6_Command;
    char* BATTERY_INDICATOR_7_Command;
    char* BATTERY_INDICATOR_8_Command;
    char* BATTERY_INDICATOR_9_Command;
    char* VERSION_NAME_Command;
    char* ENABLE_BATT_IND_ON_Command;
    char* Space_Command;
    char* END_Command;
    char* RETURN_Command;
}AtCommandBTPlatform;

extern AtCommandBTPlatform AT_COMMANDS_BC127;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct Battery_Status_Response">
typedef struct
{
    char* Fast_Charge_Response;
    char* No_Power_Response;
    char* Disabled_Response;
    char* Stanby_Response;
    char* Trickle_Response;
    char* Charge_in_Progress_Response;
    char* Charge_Complete_Response;
}Battery_Status_Response_Struct;

extern Battery_Status_Response_Struct Battery_Status_Response;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct ShortResponses">
typedef struct
{
    char* Ok_Response;
    char* Error_Response;
    char* Ready_Response;
}ShortResponseBTStruct;

extern ShortResponseBTStruct ShortResponses;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct AutonomusResponses">
typedef struct
{
    char* Ready_Response;
    char* A2DP_Start_Response;
    char* Close_OK_Response;
    char* Role_OK_Response;
    char* AVRCP_Play_Response;
    char* AVRCP_Pause_Response;
    char* SCO_Open_Response;
    char* SCO_Close_Response;
    char* CHRG_InProgess_Response;
    char* CHRG_Complete_Response;
}AutonomusResponseBTStruct;

extern AutonomusResponseBTStruct AutonomusResponses;
//</editor-fold>
//</editor-fold>

/***************************** COMMANDS Model ********************************/
//<editor-fold defaultstate="collapsed" desc="Command Model">

//<editor-fold defaultstate="collapsed" desc="Struct COMMAND_STRING_t">
#define TxVectorSize    80

struct COMMAND_STRING_str {
	char TX_VECTOR[TxVectorSize];
    char StringSize;
};

typedef struct COMMAND_STRING_str COMMAND_STRING_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct TX_UART_Data_t">
struct TX_UART_Data_str {
    COMMAND_STRING_t CommandString;
    char QtySent;
    CONFIG_STATUS_TYPE StatusTxData;
};

typedef struct TX_UART_Data_str TX_UART_Data_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct Command_t">
struct Command_str{
    TX_UART_Data_t          TxData;
    MODEL_STATUS_TYPE       InternalStatus;
    CONFIG_STATUS_TYPE      Status;
    EXPECTED_ANSWER_TYPE    ExpectedAnswerType;
    MODEL_READY_STATUS_TYPE CommandModelReady;
};

typedef struct Command_str Command_t;
//</editor-fold>

//</editor-fold>

/***************************** RX Vector Models *******************************/
//<editor-fold defaultstate="collapsed" desc="Rx Vector Models">

//<editor-fold defaultstate="collapsed" desc="Struct RX_INT_UART_Data_t">
#define RxIntVectorSize         80

struct RX_INT_UART_Data_str {
    char RX_VECTOR[RxIntVectorSize];
    char QtyReceived;
    MODEL_STATUS_TYPE ModelStatus;
    MODEL_READY_STATUS_TYPE ModelReady;
};

typedef struct RX_INT_UART_Data_str RX_INT_UART_Data_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct RX_BATT_STAT_Data_t">
#define BATT_STATUS_RESPONSE_SIZE       14
#define BATT_CHARGER_SIZE               13
#define BATT_LEVEL_SIZE                 4
#define BATT_STAT_RESPONSE_MAX_SIZE     BATT_STATUS_RESPONSE_SIZE+BATT_CHARGER_SIZE+BATT_LEVEL_SIZE

struct RxBattStatus_str{
    char BATT_STATUS_RESPONSE[BATT_STATUS_RESPONSE_SIZE];
    char BATT_CHARGER[BATT_CHARGER_SIZE];
    char BATT_LEVEL[BATT_LEVEL_SIZE];
};

union RxBattStatus_uni{
    struct RxBattStatus_str RxBattStatus_t;
    char RX_BATT_STAT_VECTOR[BATT_STAT_RESPONSE_MAX_SIZE];
};

struct RX_BATT_STAT_Data_str {
    union RxBattStatus_uni RxBattStatus_u;
    char QtyReceived;
    MODEL_STATUS_TYPE ModelStatus;
    MODEL_READY_STATUS_TYPE ModelReady;
};
typedef struct RX_BATT_STAT_Data_str RX_BATT_STAT_Data_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct RX_GET_Data_t">
#define GET_RESPONSE_MAX_SIZE             40

struct RX_GET_Data_str {
    char RX_GET_VECTOR[GET_RESPONSE_MAX_SIZE];
    char QtyReceived;
    MODEL_STATUS_TYPE ModelStatus;
    MODEL_READY_STATUS_TYPE ModelReady;
};
typedef struct RX_GET_Data_str RX_GET_Data_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct RX_STATUS_Data_t">
#define STATUS_RESPONSE_MAX_SIZE             70

struct RX_STATUS_Data_str {
    char RX_GET_VECTOR[GET_RESPONSE_MAX_SIZE];
    char QtyReceived;
    MODEL_STATUS_TYPE ModelStatus;
    MODEL_READY_STATUS_TYPE ModelReady;
};
typedef struct RX_STATUS_Data_str RX_STATUS_Data_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct RX_SHORT_Data_t">
#define SHORT_RESPONSE_MAX_SIZE             16

struct RX_SHORT_Data_str {
    char RX_SHORT_VECTOR[SHORT_RESPONSE_MAX_SIZE];
    char QtyReceived;
    MODEL_STATUS_TYPE ModelStatus;
    MODEL_READY_STATUS_TYPE ModelReady;
};
typedef struct RX_SHORT_Data_str RX_SHORT_Data_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct RxReceiveVector_t">
#define RxVectQty 4

struct RxReceiveVector_str{
    RX_INT_UART_Data_t          ReceiveVector0;
    RX_INT_UART_Data_t          ReceiveVector1;
    RX_INT_UART_Data_t          ReceiveVector2;
    RX_INT_UART_Data_t          ReceiveVector3;
    char VectorToWrite;
};

typedef struct RxReceiveVector_str RxReceiveVector_t;
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Struct RX_Data_t">
struct RX_Data_str {
    RxReceiveVector_t           ReceiveVector;
    EXPECTED_ANSWER_TYPE        AnswerType;
    MODEL_STATUS_TYPE           ModelStatus;
    MODEL_READY_STATUS_TYPE     ModelReady;
};
typedef struct RX_Data_str RX_Data_t;
//</editor-fold>
//</editor-fold>

/******************************** DATA Model **********************************/
//<editor-fold defaultstate="collapsed" desc="DATA models">

//<editor-fold defaultstate="collapsed" desc="Struct Config_Data_t">
struct Config_Data_str
{
    uint16_t    BATTERY_STATUS_LEVEL_C;
    uint16_t    BATTERY_STATUS_C;
    uint16_t    BATT_CONFIG_ENABLE_C;
    uint16_t    BATT_CONFIG_CRITICAL_LEVEL_C;
    uint16_t    BATT_CONFIG_CHARGE_LEVEL_C;
    uint16_t    BATT_CONFIG_THERMISTOR_LEVEL_C;
    uint16_t    BATT_CONFIG_CHARGING_CURRENT_C;
    uint16_t    DEEP_SLEEP_C;
    uint16_t    ENABLE_LED_C;
    uint16_t    STATE_ON_C;
    uint16_t    STATE_CONNECTED_C;
    uint16_t    STATE_CONNECTABLE_C;
    uint16_t    STATE_DISCOVERABLE_C;
    uint16_t    STATE_BLE_C;
    uint16_t    TX_POWER_DEFAULT_C;
    uint16_t    TX_POWER_MAX_C;
    uint16_t    BT_STATE_CONFIG_CONNECTABLE_C;
    uint16_t    BT_STATE_CONFIG_DISCOVERABLE_C;
    uint16_t    AUDIO_INPUT_C;
    uint16_t    AUDIO_OUTPUT_C;
    uint16_t    AUDIO_ANALOG_INPUT_GAIN_C;
    uint16_t    AUDIO_ANALOG_OUTPUT_GAIN_C;
    uint16_t    AUDIO_ANALOG_MIC_BIAS_C;
    uint16_t    AUDIO_ANALOG_ENABLE_PREAMP_C;
    uint16_t    GENERAL_SYSTEM_STATE_C;
    uint16_t    GENERAL_DEVICE_STATE_C;
};

typedef struct Config_Data_str Config_Data_t;
//</editor-fold>
//</editor-fold>

/********************************* BT Model ***********************************/
//<editor-fold defaultstate="collapsed" desc="Struct BT_Model_t">
struct BT_Model_str
{
    Command_t Command;
    RX_Data_t RX_Data;
    Config_Data_t Config;
};

typedef struct BT_Model_str BT_Model_t;
//</editor-fold>

/****************************** Timeout Model *********************************/
//<editor-fold defaultstate="collapsed" desc="Struct System_timeouts_t">
struct System_timeouts_str
{
    uint16_t STATUS_State_Period;    // Returns ON/OFF and CONNECTED/DISCONNECTED status
//    uint16_t BATT_State_Period;
//    uint16_t BUTTON_Period;
    uint16_t POWER_OFF_Period;
    uint16_t LED_PeriodCounter;
};

typedef struct System_timeouts_str System_timeouts_t;
//</editor-fold>

//*********************** FUNCIONES COMUNICACIÓN ******************************/
//<editor-fold defaultstate="collapsed" desc="Enable/Disable Read">
void BT_Enable_Read(void);
void BT_Disable_Read(void);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio de uso BT">
CONFIG_STATUS_TYPE setConfig(CONFIG_TO_SET_TYPE SetData, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE getConfig(CONFIG_TO_GET_TYPE GetData, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE setTxPower(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE BatteryStatus(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE CommStatus(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE saveConfigurations(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE PowerOnOff(CONFIG_ENABLE_TYPE OnOffState, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE resetSystem(BT_Model_t* BT_Mod);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio de comunicación BT">
void ResetBTLibrary(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE setStringToSend(Command_t* CommandModel, char* StringToAdd, uint8_t StringSize);
CONFIG_STATUS_TYPE CheckIfDataToSend(Command_t* CommandModel);
uint8_t GetDataToSend(Command_t* CommandModel);
CONFIG_STATUS_TYPE GetCommandModelStatus(Command_t* CommandModel);
CONFIG_STATUS_TYPE SetCommandModelStatus(Command_t* CommandModel, CONFIG_STATUS_TYPE ConfigStatus);
CONFIG_STATUS_TYPE SetInternalCommandModelStatus(Command_t* CommandModel, MODEL_STATUS_TYPE ModelStatus);
const void* InitCommandModel(Command_t* CommandModel);
void ClearCommandModel(Command_t* CommandModel);
CONFIG_STATUS_TYPE GeneralCommandProcess(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE startToSend(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE GeneralResponseReview(BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE CheckResponseType(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod);
void StopAnswerWaiting(BT_Model_t* BT_Mod);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="RxModel Services">
const void* InitRxModel(RX_Data_t* RxModel);
void ClearRXModel(RX_Data_t* RxModel);
void ClearVector(char* Vector, uint8_t size);
void ClearRxIntModel(RX_INT_UART_Data_t* RxIntRecModel);
//void SetAnswerRxModel(RX_Data_t* RxModel, EXPECTED_ANSWER_TYPE* AnswerTypeRec);
EXPECTED_ANSWER_TYPE GetAnswerRxModel(RX_Data_t* RxModel);
void ClearAnswerRxModel(RX_Data_t* RxModel);
MODEL_STATUS_TYPE GetRecVectorModelStatus(RX_Data_t* RxModel, uint8_t VectNmbr);
MODEL_READY_STATUS_TYPE GetRecVectorModelReady(RX_Data_t* RxModel, uint8_t VectNmbr);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Rx DATA Services">
void ManageNewReceivedData(RX_Data_t* RxModel, unsigned char RxNewData);
MODEL_STATUS_TYPE SetNewReceivedData(RX_INT_UART_Data_t* RxModel, uint8_t RxNewData);
void CheckForReadyBTInfo(RX_Data_t* RxModel);
CONFIG_STATUS_TYPE SetReceivedDataType(RX_INT_UART_Data_t* RxIntModel, RX_Data_t* RxModel);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio CheckResponses">
CONFIG_STATUS_TYPE CheckShortResponse(RX_INT_UART_Data_t* RxVectModel, char* ToCompare);
CONFIG_STATUS_TYPE CheckStatusResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE CheckBatteryStatusResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE CheckBatteryConfigResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE CheckBTStateConfigResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod);
CONFIG_STATUS_TYPE CheckBTNameResponse(RX_INT_UART_Data_t* RxVectModel, BT_Model_t* BT_Mod);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicio Varios">
CONFIG_STATUS_TYPE StringCompare(RX_INT_UART_Data_t* RxVectModel, char* ToCompare, uint8_t Start, uint8_t size);
uint8_t SearchSpace(RX_INT_UART_Data_t* RxVectModel, uint8_t Start, uint8_t VectSize);
//CONFIG_STATUS_TYPE ConvertAsciiDecimalToNumber(char* CharStringToConvert, uint16_t* FinalValue, uint8_t StringCharSize);
CONFIG_STATUS_TYPE ConvertAsciiDecimalToNumber(RX_INT_UART_Data_t* RxVectModel, uint8_t StringCharSTART, uint8_t StringCharSize, uint16_t* FinalValue);
uint8_t NumberToAsciiNumber(uint32_t Number, char* Vector);
CONFIG_STATUS_TYPE startStandarAssemblyString(Command_t* CommandModel);
CONFIG_STATUS_TYPE endStandarAssemblyString(Command_t* CommandModel);
//char hexToAscii(uint8_t Hex);
CONFIG_STATUS_TYPE ConvertAsciiCharIntoUint8Vector(char* CharStringToConvert, uint8_t* FinalVector, uint8_t StringCharSize);

//<editor-fold defaultstate="collapsed" desc="Servicios ConnectionTimeout">
void startConnectionTimeout(uint8_t Period);
void stopConnectionTimeout(void);
void setCommunicationTimeout(void);
void clearCommunicationTimeout(void);
uint8_t getCommunicationTimeoutStatus(void);
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Servicios UserTimeout">


//<editor-fold defaultstate="collapsed" desc="Servicios Button_Timeout">
void set_Button_Timeout();
void clear_Button_Timeout();
uint8_t get_Button_Timeout();
//</editor-fold>

//</editor-fold>

//</editor-fold>
/*****************************************************************************/

#endif // _ATCOMMANDBT_H_

