#include <xc.h>
#include "adc.h"
#include "digital_keypad.h"
#include "ssd.h"
#include "sensor.h"
#include "msg_id.h"
#include "can.h"


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

void main(void) {
    init_config();
    
    unsigned char indicator;
    unsigned int adc;
    
    unsigned char data[5] = {0x00};
    
    while(1)
    {
        indicator = process_indicator();
        adc = get_rpm();
        
        can_transmit(INDICATOR_MSG_ID, &indicator, 1);
        __delay_ms(10);
        
        my_itoa(adc, data, 10);
        can_transmit(RPM_MSG_ID, data, 5);
        __delay_ms(10);     
    }
    return;
}