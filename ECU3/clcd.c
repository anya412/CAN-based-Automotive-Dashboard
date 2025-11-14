/***********************************************************************
 *  File name   : clcd.c
 *  Description : Driver for Character LCD (CLCD) in 8-bit mode.
 *                Handles instruction/data writes, initialization,
 *                character output, string output, and screen clearing.
 *
 *                Functions:
 *                - clcd_write()
 *                - init_clcd()
 *                - clcd_print()
 *                - clcd_putch()
 *                - clcd_clear()
 *
 ***********************************************************************/

#include <xc.h>
#include "clcd.h"

/*----------------------------------------------------------------------
 *  Function : clcd_write
 *  Description :
 *      Sends one byte (command or data) to the CLCD controller.
 *      Automatically waits for the busy flag before returning.
 *
 *      value       → Byte to send (command/data)
 *      control_bit → INSTRUCTION_COMMAND or DATA_COMMAND
 *----------------------------------------------------------------------*/
void clcd_write(unsigned char value, unsigned char control_bit)
{
    /* Select Command/Data register */
    CLCD_RS = control_bit;

    /* Put value on data bus */
    CLCD_PORT = value;

    /* Enable pulse (min 200ns) */
    CLCD_EN = HI;
    CLCD_EN = LO;

    /* Configure port for reading busy flag */
    PORT_DIR = INPUT;
    CLCD_RW  = HI;
    CLCD_RS  = INSTRUCTION_COMMAND;

    /* Wait until CLCD is ready */
    do
    {
        CLCD_EN = HI;
        CLCD_EN = LO;
    }
    while (CLCD_BUSY);

    /* Restore data direction to write mode */
    CLCD_RW  = LO;
    PORT_DIR = OUTPUT;
}

/*----------------------------------------------------------------------
 *  Function : init_clcd
 *  Description :
 *      Initializes CLCD controller according to datasheet requirements.
 *      Performs startup delay, function set, display setup, and clear.
 *----------------------------------------------------------------------*/
void init_clcd(void)
{
    /* Configure Datapath */
    TRISD = 0x00;          /* PortD → Output for CLCD Data */
    TRISC &= 0xF8;         /* RC0, RC1, RC2 → Control pins */

    CLCD_RW = LO;

    /* Mandatory power-on delay */
    __delay_ms(30);

    /* Initialization sequence (as per HD44780 datasheet) */
    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(4100);

    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(100);

    clcd_write(EIGHT_BIT_MODE, INSTRUCTION_COMMAND);
    __delay_us(1);

    CURSOR_HOME;
    __delay_us(100);

    TWO_LINE_5x8_MATRIX_8_BIT;
    __delay_us(100);

    CLEAR_DISP_SCREEN;
    __delay_us(500);

    DISP_ON_AND_CURSOR_OFF;
    __delay_us(100);
}

/*----------------------------------------------------------------------
 *  Function : clcd_print
 *  Description :
 *      Prints a null-terminated string on the CLCD.
 *
 *      str  → Pointer to string
 *      addr → Display address (LINE1(x), LINE2(x), etc.)
 *----------------------------------------------------------------------*/
void clcd_print(const unsigned char *str, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND);

    while (*str != '\0')
    {
        clcd_write(*str++, DATA_COMMAND);
    }
}

/*----------------------------------------------------------------------
 *  Function : clcd_putch
 *  Description :
 *      Writes a single character at a given CLCD address.
 *
 *      ch   → Character to display
 *      addr → Display address
 *----------------------------------------------------------------------*/
void clcd_putch(const unsigned char ch, unsigned char addr)
{
    clcd_write(addr, INSTRUCTION_COMMAND);
    clcd_write(ch,  DATA_COMMAND);
}

/*----------------------------------------------------------------------
 *  Function : clcd_clear
 *  Description :
 *      Clears the display and returns cursor to Home position.
 *----------------------------------------------------------------------*/
void clcd_clear(void)
{
    clcd_write(0x01, INSTRUCTION_COMMAND);
}
