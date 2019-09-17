#ifndef TXTZYME_H_
#define TXTZYME_H_

long x = 0;
long y = 0;
long z = 0;
long t = 0;

unsigned char data_array[8];

void printnum(long num)
{
    if (num < 0)
    {
        num = -num;
        uart_putc('-');
    }
    if (num / (unsigned long) 10 != 0)
        printnum(num / (unsigned long) 10);
    uart_putc((char) (num % (unsigned long) 10) + '0');
    return;
}

void printnum_to_array(unsigned long num)
{
    int j;
    for (j = 7; j >= 0; j--)
    {
        data_array[j] = (num % (unsigned long) 10);
        num = num / (unsigned long) 10;
    }
}


void crlf(void)                  // send a crlf
{
    uart_puts("\n\r");
}

void textEval(unsigned char *buf)
{
    long temp;
    char ch;
    int k,j;
    unsigned char *loop;

    //temp flag
    char flag;

    while ((ch = *buf++))
    {
        switch (ch)
        {

        case 'v':
            uart_puts((char *) "v0.1\n\rcircuit4u-medium.github.io/home\n\r");
            break;

        case 'h':
            P1OUT |= LED_PIN;
            break;

        case 'l':
            P1OUT &= ~LED_PIN;
            break;

        case '0' ... '9':
            x = ch - '0';
            while (*buf >= '0' && *buf <= '9')
            {
                x = x * 10 + (*buf++ - '0');     // If  a number store it in "x"
            }
            break;

        case 'p':
            printnum(x);                        // print long integer
            crlf();
            break;
        case '{':
            k = x;
            loop = buf;
            while ((ch = *buf++) && ch != '}')
                ;

        case '}':
            if (k)
            {
                k--;
                buf = loop;
            }
            break;

        case 'm':
            delay_mS(x);
            break;

            //rpn
        case ' ': //shift-in
            t = z;
            z = y;
            y = x;
            break;
        case '~': //swap
            temp = x;
            x = y;
            y = temp;
            break;
        case '`': //drop
            x = y;
            y = z;
            z = t;
            break;
        case '+':
            x = x + y;
            y = z;
            z = t;
            break;
        case '-':
            x = x - y;
            y = z;
            z = t;
            break;
        case '*':
            x = x * y;
            y = z;
            z = t;
            break;
        case '_':
            x = 0;
            while (*buf >= '0' && *buf <= '9')
            {

                x = x * 10 + (*buf++ - '0');
            }
            x = -x;
            break;
        case '?':
            uart_puts("STK: ");
            crlf();
            uart_puts("t: ");
            printnum(t);
            crlf();
            uart_puts("z: ");
            printnum(z);
            crlf();
            uart_puts("y: ");
            printnum(y);
            crlf();
            uart_puts("x: ");
            printnum(x);
            crlf();
            uart_puts("ARRY: ");
            for (j = 0; j < 8; j++)
            {
                printnum(data_array[j]);
                uart_putc(' ');
            }
            crlf();
            break;

        case 'd':  //std on
            start();
            break;
        case 'f': //std off
            end();
            break;
        case 's': //send data
            send_byte(x);
            break;
        case 'r': //read keyscan data
            read_4_byte(data_array);
            break;
        case 'o': //write digits to 7-segment
            printnum_to_array(x);
            flag = 1; //signal leading zeros
            for(j=0;j<8;j++){
                if(flag){
                    if(data_array[j]==0){
                        send_2_byte(0);
                    }else{
                        flag = 0;
                        send_2_byte(digitToSegment[data_array[j]]);
                    }
                }else{
                    send_2_byte(digitToSegment[data_array[j]]);
                }
            }
            break;
        }

    }
}

#endif /* TXTZYME_H_ */
