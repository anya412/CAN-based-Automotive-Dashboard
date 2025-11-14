#include <xc.h>
#include "digital_keypad.h"

void init_digital_keypad(void)
{
    // Set keypad pins (defined by INPUT_PINS) as input
    TRISC = TRISC | INPUT_PINS;
}

unsigned char read_digital_keypad(unsigned char detection_type)
{
    static unsigned char once = 1; // Flag to detect single press in STATE_CHANGE mode

    if (detection_type == STATE_CHANGE)
    {
        // If any key is pressed and it's the first detection (once == 1)
        if (((KEY_PORT & INPUT_PINS) != ALL_RELEASED) && once)
        {
            once = 0; // Block further detections until key is released

            return (KEY_PORT & INPUT_PINS); // Return the pressed key
        }
        // Reset the flag when all keys are released
        else if ((KEY_PORT & INPUT_PINS) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    else if (detection_type == LEVEL)
    {
        // Directly return current key status (pressed or not)
        return (KEY_PORT & INPUT_PINS);
    }

    return 0xFF; // No key detected
}