#include "bsp.h"
#include "gvars.h"
#include "tm1638.h"
#include "txtzyme.h"

//proto thread
#include "pt.h"

static struct pt pt1, pt2;

static int ser_thread(struct pt *pt)
{
    static unsigned char ch, i;
    static unsigned char * p;
    static unsigned char txt_buf[64];

    PT_BEGIN(pt);
    while (1)
    {
        p = txt_buf;
        i = 0;

        while (1)
        {
            PT_WAIT_UNTIL(pt, uart_available());
            ch = uart_getc();
            if (ch >= ' ' && ch <= '~') //human readable ASCII
            {
                *p++ = ch;  //save to txt_buf
                i++;
            }
            else if (ch == '\r' || ch == '\n')
            {
                *p = 0; //end cmd string
                textEval(txt_buf);
                break;
            }
            else if (i >= 64)
            { //overflow protection
                break;
            }
        }

    }
    PT_END(pt);
}

static int button_thread(struct pt * pt)
{
    PT_BEGIN(pt);
    while (1)
    {
        PT_WAIT_UNTIL(pt, !(P2IN&TEST_PIN));
        delay_mS(2);
        if (!(P2IN & TEST_PIN))
        {
            WDTCTL = WDTPW + WDTCNTCL + (WDTCTL & 0x00ff); //clear watchdog timer
            //x = TA1R;
            if (P1IN & LED_PIN){
                P1OUT &= ~LED_PIN;
            }else{
                P1OUT |= LED_PIN;
            }
        }
        PT_WAIT_UNTIL(pt, (P2IN&TEST_PIN));
    }
    PT_END(pt);
}




/**
 * main.c
 */
int main(void)
{


    init_SYS();
    init_UART();
    init_GPIO();

    uart_puts((char *) "v0.1\n\rcircuit4u-medium.github.io/home\n\r");


    //go to sleep mode
    P1OUT &= ~LED_PIN;
    __bis_SR_register(LPM3_bits | GIE);

    //out of sleep mode
    P1OUT |= LED_PIN;
    WDTCTL = WDTPW + WDTCNTCL + WDTSSEL_1 + WDTIS_2; //arm watchdog timeout about 4mins


    PT_INIT(&pt1);
    PT_INIT(&pt2);
    while (1)
    {
        ser_thread(&pt1);
        button_thread(&pt2);
    }

    return 0;
}
