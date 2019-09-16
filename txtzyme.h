

#ifndef TXTZYME_H_
#define TXTZYME_H_



void textEval(unsigned char *buf)
{
    char ch;

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
        }
    }
}

#endif /* TXTZYME_H_ */
