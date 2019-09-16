#include "bsp.h"
#include "txtzyme.h"

/**
 * main.c
 */
int main(void)
{
    unsigned char ch, i;
    unsigned char * p;
    unsigned char txt_buf[64];

    init_SYS();
    init_UART();
    init_GPIO();

    uart_puts("Hello World!\n\r");

    while (1)
    {
        p = txt_buf;
        i = 0;

        while (1)
        {
            ch = uart_getc();
            if (ch >= ' ' && ch <= '~') //human readable ASCII
            {
                *p++ = ch;  //save to txt_buf
                i++;
            }else if(ch=='\r' || ch=='\n'){
                textEval(txt_buf);
                break;
            }else if(i>=64){ //overflow protection
                break;
            }
        }

    }

    return 0;
}
