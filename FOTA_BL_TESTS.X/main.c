/*
 * File:   main.c
 * Author: Pratik
 *
 * Created on February 25, 2023, 12:41 PM
 */

//#include "myconfig.h"
//#include "../mcc_generated_files/system.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/memory/flash.h"
#define FCY 8000000UL
#include <libpic30.h>

#define     BOOTLOADER_START_ADDRESS    0x800
#define     APPLICATION_START_ADDRESS   0x2000

#define     PRINT(x)    uartTx(x,sizeof(x)-1)

static uint8_t fletcher1 = 0, fletcher2 = 0;
static uint16_t calc_csum = 0;
static uint16_t expected_csum = 0;
void update_csum (uint8_t *data)
{
    uint16_t i;

    for (i = 0; i < 512; i++)
    {
        fletcher1 = (fletcher1 + data[i]) % 255;
        fletcher2 = (fletcher2 + fletcher1) % 255;
    }
    calc_csum = fletcher2*256 + fletcher1;
}

static uint16_t hextoint (char *hex)
{
    uint8_t i;
    uint16_t val;

    for (i=0; i<4; i++)
    {
        if ((hex[i]>='0') && (hex[i]<='9'))
            hex[i] -= '0';
        else if ((hex[i]>='a') && (hex[i]<='f'))
            hex[i] = (hex[i]-'a')+10;
        else if ((hex[i]>='A') && (hex[i]<='F'))
            hex[i] = (hex[i]-'A')+10;
    }
    val = hex[0]*4096 + hex[1]*256 + hex[2]*16 + hex[3];
    return val;
}

static void inttohex (char *hex, uint16_t num)
{
    char i;
    hex[0] = (num & 0xF000)>>12;
    hex[1] = (num & 0x0F00)>>8;
    hex[2] = (num & 0x00F0)>>4;
    hex[3] = num & 0x000F;

    for (i=0; i<4; i++)
    {
        if ((hex[i] >=0) && (hex[i] <=9))
            hex[i] += '0';
        else if (((hex[i] >=10) && (hex[i] <=15)))
            hex[i] = hex[i] - 10 + 'a';
    }
}

void uartTx (char *data, uint16_t len)
{
    uint16_t i;
    for (i=0; i<len; i++)
    {
        // Wait for transmit buffer to be empty
        while (!U2STAbits.TRMT);
        // Transmit byte
        U2TXREG = data[i];
    }
    // Wait for transmit buffer to be empty
    while (!U2STAbits.TRMT);
}

// Returns how many received before idle period caused timeout
uint16_t uartRx (char *data, uint16_t len)
{
    uint16_t i;
    
    data[0] = U2RXREG;  // Clear pending UART data
    if ((U2STAbits.OERR == 1))
            U2STAbits.OERR = 0;
    
    // Clear receive array to avoid false matches
    for (i=0; i<len; i++)
        data[i] = 0xFF;
    
    // Init timeout timer
    PR1 = 0xFFFF;
    TMR1_Counter16BitSet (0);
    TMR1_Start ();
    
    for (i=0; i<len; i++)
    {
        // Wait for data to be received
        while(!(U2STAbits.URXDA == 1))
            if (TMR1_Counter16BitGet() > 0xC000)
            {
                TMR1_Stop ();
                return i;
            }

        // Read received data, reset timeout timer
        if ((U2STAbits.OERR == 1))
            U2STAbits.OERR = 0;
        *data++ = U2RXREG;
        TMR1_Counter16BitSet (0);
    }
    return len;
}

// Check if source matches "OK\r", return 1 on match
// Returns 0 on mismatch
char is_response (char *rx, char *ref, uint8_t size)
{
    uint8_t i;
    for (i=0; i<size; i++)
    {
        if (rx[i] != ref[i])
            break;
    }
    if (i==size) return 1;
    else return 0;
}

// Address must be a multiple of 0x400 for page erase
// Each erased page can hold 8 sets of progbuf[128]
void eraseFlash (uint32_t pageaddr)
{
    uint32_t flash_storage_address;
    
    // Erase page
    flash_storage_address = FLASH_GetErasePageAddress((uint32_t)pageaddr);
    
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    FLASH_ErasePage(flash_storage_address);
    FLASH_Lock();
}

// Aligned with 128-word segment of erase page, unused data is 0xFFFF FFFF
void writeFlash (uint32_t memaddr, uint32_t *memdata)
{
    uint8_t i;
    // Unlock flash ops
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    for (i=0; i<64; i++)
        FLASH_WriteDoubleWord24(memaddr + 4*i, memdata[2*i], memdata[2*i+1]);

    FLASH_Lock();
}

void writeStatus (uint32_t memaddr, uint32_t fw_csum, uint32_t fw_size)
{
    FLASH_Unlock(FLASH_UNLOCK_KEY);
    FLASH_WriteDoubleWord24((uint32_t) memaddr, fw_csum&0x00FFFFFF, fw_size&0x00FFFFFF);
    FLASH_Lock();
}

// 0 = fresh flash, proceed to boot
// 1 = invalid app in storage, cannot boot
// 2 = valid app in storage
uint8_t can_boot (void)
{
    // csum stores 16-bit checksum and 8-bit status
    uint32_t csum = FLASH_ReadWord24((uint32_t)0x1800);
    uint32_t size = FLASH_ReadWord24((uint32_t)0x1802);

    // Factory app
    // TODO: Allow for factory app during deployment
    if ((csum&0xFFFFFF) == 0xFFFFFF)
        return 1;
    
    if ((csum & 0x00FF0000) == 0x00AA0000)
        return 2;

    return 1;
}

SYSTEM_deInitialize(void)
{
    // Timer de-init
    T1CON = 0x0000;
    PR1 = 0xFFFF;
    TMR1 = 0x0000;
    
    // UART disable
    U2MODEbits.UARTEN = 0;
    U2STAbits.UTXEN = 0;

    // Clock de-init
    OSCFDIV = 0;
    OSCDIV = 1;


    // Pin manager de-init
    TRISB = 0xFFFF;
    TRISA = 0xFFFF;
    LATB = 0;
    ANSB = 0xFFFF;
    ANSA = 0xFFFF;
    RPINR19 = 0x3F3F;
    RPOR1 = 0x0000;
}

uint32_t progbuf[128];
int main(void) {
    //asm("goto 0x2000");
    char ch, retry;
    uint16_t i;
    uint32_t binsize, binctr, rowaddr, temp;
    char rx [32];

    // Update pending from app?
    if ((can_boot() == 0) || (can_boot() == 2))
        if ((FLASH_ReadWord24((uint32_t)0x1804) & 0x00FFFFFF) == 0x00FFFFFF)
            goto skipall;              // Boot factory or installed app

    // Initialize system peripherals
    SYSTEM_Initialize();

    // Main bootloader failsafe loop
    while (1)
    {
        // Attempt to communicate with app, assuming SPP link active
        // Wait for app to acknowledge FOTA command, timeout 4 sec x 2 attempts
        __delay_ms (50);
        for (retry=0; retry<2; retry++)
        {
            uartTx ("###FOTA###", 10);
            i = uartRx (rx, 11);
            if (is_response(rx, "<<<ready>>>", 11))
                break;      // App is in FOTA mode - confirmed
        }

        if (retry == 2)
        {
            // No answer from phone app, check other options
            if ((can_boot() == 0) || (can_boot() == 2))
            {
                // Valid app exists, boot that
                // Clear pending OTA flag
                expected_csum = FLASH_ReadWord24((uint32_t)0x1800) & 0x0000FFFF;
                binsize = FLASH_ReadWord24((uint32_t)0x1802) & 0x00FFFFFF;
                eraseFlash (0x1800);
                writeStatus (0x1800, 0x00AA0000 + expected_csum, binsize);
                asm ("reset");
            }

            // No valid app, and no response from phone app
            // Keep attempting SPP link
            while (1)
            {
                uartTx ("ENTER_DATA_MODE 15\r", 19);
                i = uartRx (rx, 13); // possible response is ERROR 0x0000\r, or OK\r
                if (is_response(rx, "OK\r", 3))
                    break;
                if (is_response(rx, "ERROR", 5))
                {
                    __delay_ms (3000);              // SPP cannot open, retry after 3 sec
                    continue;
                }
                // No response was received, maybe SPP is connected already?
                if (i == 0)
                    break;
            }

            // SPP connection is established here for sure
            // Wait for app to acknowledge FOTA command, timeout 4 sec x 10 attempts
            __delay_ms (500);
            for (retry=0; retry<10; retry++)
            {
                uartTx ("###FOTA###", 10);
                i = uartRx (rx, 11);
                if (is_response(rx, "<<<ready>>>", 11))
                    break;
            }
            if (retry == 10)
                continue;       // Restart from beginning
        }
        
        // App has responded with ready signal, move to other operations
        // Send ###BINV xxxx###, fetch expected csum
        __delay_ms (50);
        i = FLASH_ReadWord24((uint32_t)0x1800) & 0xFFFF;
        char tx[] = "###BINV ----###";
        inttohex (&tx[8], i);
        for (retry=0; retry<10; retry++)
        {
            uartTx (tx, 15);
            uartRx (rx, 10);
            if ((is_response(rx, "<<<", 3)) && (is_response (&rx[7], ">>>", 3)))
                break;
        }
        if (retry == 10)
            continue;       // Restart from beginning
        
        // Checksum is ready to compare
        expected_csum = hextoint (&rx[3]);
        
        for (retry=0; retry<10; retry++)
        {
            __delay_ms (500);
            uartTx ("###SIZE###", 10);
            uartRx (rx, 12);
            if ((is_response(rx, "<<<", 3)) && (is_response (&rx[9], ">>>", 3)))
                break;
        }
        if (retry == 10)
            continue;       // Restart from beginning
        
        // Obtained size at this point
        binsize = (uint32_t)(rx[3]-'0')*(uint32_t)100000;
        binsize = binsize + (uint32_t)(rx[4]-'0')*(uint32_t)10000;
        binsize = binsize + (uint32_t)(rx[5]-'0')*(uint32_t)1000;
        binsize = binsize + (uint32_t)(rx[6]-'0')*(uint32_t)100;
        binsize = binsize + (uint32_t)(rx[7]-'0')*(uint32_t)10;
        binsize = binsize + (uint32_t)(rx[8]-'0');
        
        // About to enter flash modification process, set status
        eraseFlash (0x1800);
        writeStatus (0x1800, 0x00110000, binsize);
        // Clear checksum
        fletcher1 = 0;
        fletcher2 = 0;

        for (binctr=0; binctr<binsize; binctr+=512)
        {
            rowaddr = binctr/2;
            // Initialize buffer first
            for (i=0; i<128; i++)
                progbuf[i] = 0xFFFFFFFF;

            uartTx ("###SEND###", 10);
            i = uartRx ((char *)progbuf, 128*4);
            if (i != 512)
            {
                uartTx ("###FAIL-TOUT###", 15);
                break;
            }
            // Update checksum
            update_csum ((uint8_t *)progbuf);
            // TODO: remove debug command here
            tx[0]="$"; tx[1]="$"; tx[2]="$";
            inttohex (&tx[3], calc_csum);
            tx[7]="$"; tx[8]="$"; tx[9]="$";
            uartTx (tx, 10);
            
            // Acquired 128 bytes, process them
            // Do not remove reset vector
            if (rowaddr == 0)
            {
                progbuf[0] = 0x040000 + BOOTLOADER_START_ADDRESS;
                //progbuf[0] = 0x040000 + 0x2000;
                progbuf[1] = 0x000000;
            }
            // Do not overwrite bootloader area
            if ((rowaddr >= BOOTLOADER_START_ADDRESS) && (rowaddr < APPLICATION_START_ADDRESS))
            {
                ;       // Do nothing to bootloader code
            }
            else
            {
                // Valid app flash area? erase page if needed
                if ((rowaddr % 0x800) == 0)
                    eraseFlash (rowaddr);           // Erase next 8 blocks of 128 progbuf's
                writeFlash (rowaddr, progbuf);
            }
        }

        // Exit because of timeout?
        if (i != 512)
            continue;
        
        // Verify and flash here
        if (expected_csum != (fletcher2*256 + fletcher1))
        {
            // Problem with the app binary!
            uartTx ("###FAIL-CSUM###", 15);
            continue;
        }
        else
        {
            // App is valid and flashed properly, pending OTA flag is clear
            eraseFlash (0x1800);
            writeStatus (0x1800, 0x00AA0000 + expected_csum, binsize);
        }
        
        // Done
        uartTx ("###EXIT###", 10);
        // reboot now
        asm ("reset");
        
        // Only get here if everything checks out OK
        break;
    }
    
    // Jump to app start address
    //SYSTEM_deInitialize ();
    skipall:
    asm("goto 0x2000");
    return 0;
}
