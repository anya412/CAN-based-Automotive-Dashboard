
#include "sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "digital_keypad.h"

uint16_t get_speed(int index)
{
    // Implement the speed function
    uint16_t speed;
//    if(index > 1 && index < 8)
            speed = read_adc(CHANNEL4) / 10.33;
//    else
//        speed = 0;

    if(index == 7)
        speed /= 5;
    
    return speed;
}

unsigned char get_gear_pos()
{
    // Implement the gear function
    static unsigned char index = 0, key;
    key = read_digital_keypad(STATE_CHANGE);
    if(key != ALL_RELEASED)
    {
        if(key == SWITCH1)
        {
            if(index < 7)
                index++;
            else if(index == 8)
                index = 1;
        }
        else if(key == SWITCH2)
        {
            if(index == 8)
                index = 1;
            else if(index > 1)
                index--;
        }
        else if(key == SWITCH3)
        {
            index = 8;
        }
    }
    return index;
}