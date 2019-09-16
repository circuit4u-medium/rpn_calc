#ifndef TXTZYME_H_
#define TXTZYME_H_

long x = 0;

void printnum(long num)
{
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
        }

    }
}

#endif /* TXTZYME_H_ */
