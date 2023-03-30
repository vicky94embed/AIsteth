/**
  Generated main.c file from MPLAB Code Configurator

  @Company
 BSB Engineering

  @File Name
    main.c

  @Summary
 This is the main fie for the HeartBeat device project

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.167.0
        Device            :  PIC24FJ256GA702
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.35
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "xc.h"
#include "bsp/bsp.h"
#include "SourceFiles/AtCommandBT.h"
#include"mcc_generated_files/uart1.h"


extern BT_Model_t BT_Model;

void CheckStatus();

// n11k
/*
                         Main application
 */
int main(void)
{
    
    USER_CONFIG_STATUS_TYPE Error = USER_OK;
    
    int flag=0;
    
    // initialize the device
    bspInit(); // initialization complete
    bsp_set_button_OUT_Release(); // Button released
    ledDriverInit(); // All led off

    bsp_Status_Refresh(); // Update the overall status by querying STATUS and BATTERY
    bsp_set_Status_Timeout(&System_timeouts, EACH500ms); // Set next status refresh // EACH500ms
    
   
    set_device_State(&BT_Model.Config, START_UP); // Set the status as START UP
     
    
    while (1)
    {
        
        switch(get_device_State(&BT_Model.Config)) // General state switch
        {
            case START_UP: default: // Start after sleep
               
                if(bsp_get_Status_Timeout(&System_timeouts) == 0) // Do nothing until the timeout expires
                {   
                             
                    if(USER_OK != bsp_CheckStatus()) // IF the STATUS request fails then send it to sleep (it only fails if it is off)
                    {
                        // Send the module to sleep
                      set_device_State(&BT_Model.Config, GO_TO_SLEEP_UC);
                        
                    }
                    else
                    {
                        // Queries the name of the device to see if it is configured
                        bsp_CheckConfig();
                        // Check battery status
                        bsp_CheckBattery();
                        // Analyze module status not turned off
                        bsp_analize_system_State(&BT_Model.Config);
                        
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);  // EACH25s
                      
                        // With the module status set, go to ON mode
                        set_device_State(&BT_Model.Config, ON_UC);
                    }
                }
                break;
            case ON_UC:
                
                switch(get_system_State(&BT_Model.Config)) // Module status switch on
                {
                    case SLEEP_NOCHARGING: // Turn off by itself after a while
                       // LEDs off
               
                        LedRefresh(NONE_LED, EACH1s, NOms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);  // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);  // EACH25s
                        break;
                        
                    case SLEEP_CHARGING:
                        // Red led on
                        LedRefresh(RED_LED, EACH1s, NOms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s); // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH25s);   //EACH25s
                        break;
                        
                    case SLEEP_CHARGED:
                        // Green led on
                      
                        LedRefresh(GREEN_LED, EACH1s, NOms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);   // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH25s);     //EACH25s
                        break;
                   
                        
                    case ON_DISC_NOCHARGING_BATOK:
                         
                        if(flag==1) // To reset device and rediscover device when connected device lost connection
                        {
                            flag=0;
                            bsp_Function_To_Use(USER_RESET, 2);
                        }
                        
                         // Blue led blinking short
                        LedRefresh(BLUE_LED, EACH100ms, EACH1s, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);    // EACH1s
                        }
                        // Check automatic shutdown timeout
                        if(bsp_get_PowerOff_Timeout(&System_timeouts) == 0) // Shutdown due to inactivity
                        {
                            // After shutdown timeout send to sleep
                            if(USER_OK != bsp_Function_To_Use(USER_POWER_OFF, ChancesQty))
                            {
                                Error = USER_FAILED;
                                // If for some reason it doesn't turn off reset the 30 second timer
                                bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);  // CHANGED //EACH25s
                            }
                            // Reset the counter to check the state of the system
                            bsp_clear_Status_Timeout(&System_timeouts);
                        }
                        break;
                        
                    case ON_DISC_NOCHARGING_BATLOW:
                        // red led flashing short       
                        LedRefresh(RED_LED, EACH100ms, EACH500ms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);  // // EACH1s
                        }
                        // Check automatic shutdown timeout
                        if(bsp_get_PowerOff_Timeout(&System_timeouts) == 0) // Shutdown due to inactivity
                        {
                            // After shutdown timeout send to sleep
                            if(USER_OK != bsp_Function_To_Use(USER_POWER_OFF, ChancesQty))
                            {
                                Error = USER_FAILED;
                                // If for some reason it doesn't turn off reset the 30 second timer
                                bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s); //EACH25s
                            }
                            // Reset the counter to check the state of the system
                            bsp_clear_Status_Timeout(&System_timeouts);
                        }
                        break;
                        
                    case ON_DISC_CHARGING:
                       // Red led blinking long
                       
                        LedRefresh(RED_LED, EACH100ms, EACH1s, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);  // // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH25s);    // EACH25s
                        break;
                        
                        
                    case ON_DISC_CHARGED:
                        // Green led blinking long
                       // red_led_test();
                        LedRefresh(GREEN_LED, EACH100ms, EACH1s, &System_timeouts);
                        // Setea timeout para refresco de estado
                 //      if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                 //      {
                            // Refresca el estado cada un tiempo
                 //          bsp_Status_Refresh();
                            // El tiempo lo setea cada estado particularmente
                 //          bsp_set_Status_Timeout(&System_timeouts, EACH1s);
                  //      }
                        // Reinicia timeout de apagado automático
                  //     bsp_set_PowerOff_Timeout(&System_timeouts, EACH25s);
                        
                        
                         if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                           // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);   // EACH1s
                        }
                        // Check automatic shutdown timeout
                        if(bsp_get_PowerOff_Timeout(&System_timeouts) == 0) // Shutdown due to inactivity
                        {
                            // After shutdown timeout send to sleep
                            if(USER_OK != bsp_Function_To_Use(USER_POWER_OFF, ChancesQty))
                            {
                                Error = USER_FAILED;
                                // // If for some reason it doesn't turn off reset the 60 second timer
                                bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);  //CHANGED //EACH25s
                            }
                            // Resetea el contador para que revise el estado del sistema
                            bsp_clear_Status_Timeout(&System_timeouts);
                        }
                        
                        break;
                        
                    case ON_CON_NOCHARGING_BATOK:
                        
                      // Blue led on
                   
                        flag=1; // rediscovery_flag
                        
                        LedRefresh(BLUE_LED, EACH1s, NOms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);    // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);    // EACH25s
                        break;
                        
                    case ON_CON_NOCHARGING_BATLOW:
                       // Red led on
                        
                        flag=1; // rediscovery _flag
                        
                  //    LedRefresh(RED_LED, EACH1s, EACH150ms, &System_timeouts);  // note: redled blink after connected also with less led off time(150ms).)
                         LedRefresh(RED_LED, EACH1s, NOms, &System_timeouts);  // note: redled blink after connected also with less led off time(150ms).)
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);   // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);   //EACH25s
                        break;
                        
                    case ON_CON_CHARGING:
                        // Red led on
                        
                        LedRefresh(RED_LED, EACH1s, NOms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);   // EACH1s
                        }
                       // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH25s);    //EACH25s
                        break;
                        
                    case ON_CON_CHARGED:
                        // Green led on
                        
                        flag=1; // rediscovery_flag
                        
                        LedRefresh(GREEN_LED, EACH1s, NOms, &System_timeouts);
                        // Set timeout for state refresh
                        if(bsp_get_Status_Timeout(&System_timeouts) == 0)
                        {
                            // Refresh the state every once in a while
                            bsp_Status_Refresh();
                            // The time is set by each state individually
                            bsp_set_Status_Timeout(&System_timeouts, EACH1s);  // EACH1s
                        }
                        // Reset auto shutdown timeout
                        bsp_set_PowerOff_Timeout(&System_timeouts, EACH60s);    //EACH25s
                        break;
                        
                        
                    case TURN_OFF_NOCHARGING: // If this mode is activated, the module is not responding, I'm going to put the micro to sleep
                        
                        set_device_State(&BT_Model.Config, GO_TO_SLEEP_UC);
                        break;
                    default:
                        break;
                }
                break;
                
            case GO_TO_SLEEP_UC:
              // LEDs off
               
                LedRefresh(NONE_LED, EACH1s, NOms, &System_timeouts);
                // Enable Weak from button
                bsp_EX_INT0_InterruptEnable();
                // Enable Weak from UART
                bsp_UART1_Weak_Enable ();
                // Set the general state for when it wakes up
                set_device_State(&BT_Model.Config, SLEEP_UC);
                // put the module to sleep
                Sleep();
                break;
            case SLEEP_UC:
                break;
        }
    }
    return 1;
}

/**
 End of File
*/

