/*
 * tm1638.h
 *
 *  Created on: Sep 16, 2019
 *      Author: zh1010
 */

#ifndef TM1638_H_
#define TM1638_H_



void start()
{
    P1OUT &= ~STB_PIN;
    delay_uS(10);
}

void end()
{
    P1OUT |= STB_PIN;
    delay_uS(10);
}

//display_off = 128; display_on = 143
//start_write_reg = 64; start_addr = 192; end_addr = 207
//start_read_key = 66;

void read_4_byte(unsigned char * data_array) //always read 4 bytes
{
    int i, j;
    unsigned char temp;
    //switch to input
    P1DIR &= ~DIO_PIN;
    for (j = 0; j < 4; j++)
    {
        temp = 0;
        for (i = 0; i < 8; i++)
        {
            P1OUT &= ~CLK_PIN;
            delay_uS(10);
            P1OUT |= CLK_PIN;
            temp |= ((P1IN & DIO_PIN) ? 1 : 0) << i;
            delay_uS(10);
        }
        data_array[j] = temp;
    }
    //switch to output
    P1DIR |= DIO_PIN;

}

void send_byte(unsigned char data_byte)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        P1OUT &= ~CLK_PIN;
        delay_uS(10);
        if (data_byte & 0x01)
        {
            P1OUT |= DIO_PIN;
        }
        else
        {
            P1OUT &= ~DIO_PIN;
        }
        data_byte >>= 1;
        P1OUT |= CLK_PIN;
        delay_uS(10);
    }

}

void send_2_byte(unsigned char data_byte)
{
    send_byte(data_byte);
    send_byte(0); //set SEG9, SEG10 = 0
}

#endif /* TM1638_H_ */
