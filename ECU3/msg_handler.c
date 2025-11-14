#include <xc.h>
#include <string.h>
#include "msg_handler.h"
#include "msg_id.h"
#include "can.h"
#include "clcd.h"

/* Global Timer Counter (incremented in Timer0 ISR) */
unsigned long int g_timer_ticks;

/* Event strings for gear display */
static const unsigned char g_gear_labels[9][3] =
{
    "ON", "GN", "G1", "G2", "G3", "G4", "G5", "Gr", "C_"
};

/*---------------------------------------------------------
 * Display fixed labels on LCD
 *---------------------------------------------------------*/
void display_labels(void)
{
    clcd_print("SP",  LINE1(0));
    clcd_print("GR",  LINE1(4));
    clcd_print("RPM", LINE1(8));
    clcd_print("IND", LINE1(13));
}

/*---------------------------------------------------------
 * SPEED Handler
 *---------------------------------------------------------*/
void handle_speed_data(uint8_t *data, uint8_t len)
{
    clcd_putch(data[0], LINE2(0));
    clcd_putch(data[1], LINE2(1));
}

/*---------------------------------------------------------
 * GEAR Handler
 *---------------------------------------------------------*/
void handle_gear_data(uint8_t *data, uint8_t len)
{
    if (*data < 9)
    {
        clcd_print(g_gear_labels[*data], LINE2(4));
    }
}

/*---------------------------------------------------------
 * RPM Handler
 *---------------------------------------------------------*/
void handle_rpm_data(uint8_t *data, uint8_t len)
{
    clcd_print(data, LINE2(8));
}

/*---------------------------------------------------------
 * INDICATOR Handler
 *  - Controlled flashing using g_timer_ticks
 *---------------------------------------------------------*/
void handle_indicator_data(uint8_t *data, uint8_t len)
{
    int indicator = *data;

    if (g_timer_ticks <= 10000)
    {
        if (indicator == e_ind_off)
        {
            LEFT_IND_OFF();
            RIGHT_IND_OFF();
            clcd_putch(' ', LINE2(14));
            clcd_putch(' ', LINE2(15));
        }
        else if (indicator == e_ind_left)
        {
            LEFT_IND_ON();
            RIGHT_IND_OFF();
            clcd_putch('<', LINE2(14));
            clcd_putch(' ', LINE2(15));
        }
        else if (indicator == e_ind_right)
        {
            LEFT_IND_OFF();
            RIGHT_IND_ON();
            clcd_putch(' ', LINE2(14));
            clcd_putch('>', LINE2(15));
        }
        else if (indicator == e_ind_hazard)
        {
            LEFT_IND_ON();
            RIGHT_IND_ON();
            clcd_putch('<', LINE2(14));
            clcd_putch('>', LINE2(15));
        }
    }
    else
    {
        LEFT_IND_OFF();
        RIGHT_IND_OFF();
        clcd_putch(' ', LINE2(14));
        clcd_putch(' ', LINE2(15));
    }
}

/*---------------------------------------------------------
 * CAN Message Processing Logic
 *---------------------------------------------------------*/
void process_canbus_data(void)
{
    uint8_t data[5];
    uint8_t len = 0;
    uint16_t msg_id;

    static uint8_t collision_flag = 0;

    can_receive(&msg_id, data, &len);

    if (collision_flag == 0)
    {
        if (msg_id == SPEED_MSG_ID)
        {
            handle_speed_data(data, len);
        }
        else if (msg_id == GEAR_MSG_ID)
        {
            handle_gear_data(data, len);

            /* Collision event detected */
            if (*data == 8)
            {
                collision_flag = 1;

                clcd_clear();
                clcd_print("Collision !",     LINE1(0));
                clcd_print("Vehicle Damaged", LINE2(0));
            }
        }
        else if (msg_id == RPM_MSG_ID)
        {
            handle_rpm_data(data, len);
        }
        else if (msg_id == INDICATOR_MSG_ID)
        {
            handle_indicator_data(data, len);
        }
    }
    else /* collision_flag == 1 */
    {
        if (msg_id == GEAR_MSG_ID)
        {
            /* Collision cleared */
            if (*data != 8)
            {
                collision_flag = 0;

                clcd_clear();
                display_labels();
            }
        }
    }
}
