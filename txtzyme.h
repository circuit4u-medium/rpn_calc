#ifndef TXTZYME_H_
#define TXTZYME_H_

long x = 0;
long y = 0;
long z = 0;
long t = 0;

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

void crlf(void)                  // send a crlf
{
    uart_puts("\n\r");
}

void textEval(unsigned char *buf)
{
    long temp;
    char ch;
    int k;
    unsigned char *loop;

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
        }

    }
}

#endif /* TXTZYME_H_ */
