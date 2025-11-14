#ifndef CLCD_H
#define CLCD_H

/*---------------------------------------------------------
 * LCD Port Definitions
 *---------------------------------------------------------*/
#define CLCD_PORT               PORTD
#define CLCD_PORT_DIR           TRISD     /* Full data port direction */

#define CLCD_EN                 RC2
#define CLCD_RS                 RC1
#define CLCD_RW                 RC0

#define CLCD_BUSY               RD7       /* Busy flag bit */

/*---------------------------------------------------------
 * General Macros
 *---------------------------------------------------------*/
#define HI                      1
#define LO                      0

#define INPUT                   0xFF
#define OUTPUT                  0x00

/*---------------------------------------------------------
 * LCD Command Modes
 *---------------------------------------------------------*/
#define DATA_COMMAND            1
#define INSTRUCTION_COMMAND     0

/*---------------------------------------------------------
 * Display Addresses
 *---------------------------------------------------------*/
#define LINE1(x)                (0x80 + (x))
#define LINE2(x)                (0xC0 + (x))

/*---------------------------------------------------------
 * LCD Commands (Instruction Codes)
 *---------------------------------------------------------*/
#define CMD_FUNCTION_SET_8BIT_2LINE   0x38
#define CMD_CLEAR_DISPLAY             0x01
#define CMD_RETURN_HOME               0x02
#define CMD_DISPLAY_ON_CURSOR_OFF     0x0C

/* Startup "magic sequence" (as used in your init_clcd) */
#define LCD_STARTUP_MAGIC             0x33

/*---------------------------------------------------------
 * Clock Frequency (Required for __delay_ms/us)
 *---------------------------------------------------------*/
#define _XTAL_FREQ                   20000000UL

/*---------------------------------------------------------
 * Function Prototypes
 *---------------------------------------------------------*/
void init_clcd(void);
void clcd_write(unsigned char byte, unsigned char control_bit);
void clcd_print(const unsigned char *data, unsigned char addr);
void clcd_putch(const unsigned char data, unsigned char addr);
void clcd_clear(void);

#endif
