#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "msg_id.h"
#include "sensor.h"
#include "digital_keypad.h"
#include "can.h"
#include "string.h"

unsigned long int timer_count;

#define _XTAL_FREQ 200000

void init_config()
{
    init_adc();
    init_digital_keypad();
    init_can();
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

char * my_itoa(unsigned int num, unsigned char* str, unsigned int base)
{
    int i = 0;
    if (num == 0)
    {
        str[i++] = '0';
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = rem + '0';
        num = num / base;
    }
    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

int main()
{
    init_config();
    unsigned int speed = 0;
    unsigned char gear_pos = 0;
    unsigned char data[5] = {0x00};
    while(1)
    {
        __delay_ms(10);
        
        gear_pos = get_gear_pos();
        can_transmit(GEAR_MSG_ID, &gear_pos, 1);
       
        __delay_ms(10);
        
        speed = get_speed(gear_pos);
        my_itoa(speed, data, 10);
        can_transmit(SPEED_MSG_ID, data, 3);
    }
}