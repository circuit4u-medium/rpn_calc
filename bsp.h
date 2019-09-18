#ifndef BSP_H_
#define BSP_H_

#include <msp430.h>

void init_SYS()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5; // Disable the GPIO power-on default high-impedance mode
                          // to activate 1previously configured port settings
    __bis_SR_register(SCG0);                          // disable FLL
    CSCTL3 |= SELREF__REFOCLK;               // Set REFO as FLL reference source
    CSCTL0 = 0;                              // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);             // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_3;                     // Set DCO = 8MHz
    CSCTL2 = FLLD_0 + 243;                   // DCODIV = 8MHz
    __delay_cycles(3);
    __bic_SR_register(SCG0);                 // enable FLL
    while (CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1))
        ; // Poll until FLL is locked

    CSCTL4 = SELMS__DCOCLKDIV | SELA__REFOCLK; // set default REFO(~32768Hz) as ACLK source, ACLK = 32768Hz
                                               // default DCODIV as MCLK and SMCLK source
}

//time delay functions (blocking function)

#define delay_1ms __delay_cycles(8000)
#define delay_1us __delay_cycles(4)

void delay_uS(int i)
{
    while (i)
    {
        delay_1us;
        i--;
    }
}

void delay_mS(long i)
{
    while (i)
    {
        delay_1ms;
        i--;
    }
}

#define RXD     BIT5    // Receive Data (RXD) at P1.5
#define TXD     BIT4    // Transmit Data (TXD) at P1.4

//serial
void init_UART() //115200
{

    // Configure UART pins
    P1SEL0 |= RXD | TXD;                  // set 2-UART pin as second function

    // Configure UART
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;

    UCA0BR0 = 4;                             // 8000000/16/115200
    UCA0BR1 = 0x00;
    UCA0MCTLW = 0x55 | UCOS16 | UCBRF_1;

    UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI

}

void uart_putc(unsigned char c)
{
    while (!(UCA0IFG & UCTXIFG))
        ;              // USCI_A0 TX buffer ready?
    UCA0TXBUF = c;                          // TX
}

void uart_puts(const char *str)                 // Output a string
{
    while (*str)
        uart_putc(*str++);
}

int uart_available()
{
    return (UCA0IFG & UCRXIFG);
}

unsigned char uart_getc()
{
    //while (!(UCA0IFG & UCRXIFG))
    ;
    // USCI_A0 RX buffer ready?
    return UCA0RXBUF;
}

//gpio
#define LED_PIN BIT0  //p1.0

#define TEST_PIN BIT3 //p2.3
#define POWER_PIN BIT7 //p2.7

//tm1638
#define STB_PIN BIT1 //p1.1
#define CLK_PIN BIT2 //p1.2
#define DIO_PIN BIT3 //p1.3

#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~POWER_PIN;
//    __bic_SR_register_on_exit(LPM3_bits);                 // Exit LPM3

    if (P1IN & LED_PIN)
    {
        P1OUT &= ~LED_PIN;
        //reset
        WDTCTL = 0xDEAD;
    }
    else
    {
        __bic_SR_register_on_exit(LPM3_bits);                 // Exit LPM3
    }
}

void init_GPIO()
{
    P1DIR |= (LED_PIN | STB_PIN | CLK_PIN | DIO_PIN);
    P1OUT |= (LED_PIN | STB_PIN);

    //pull-up
    P2REN |= (TEST_PIN | POWER_PIN);
    P2OUT |= (TEST_PIN | POWER_PIN);

    P2IES |= POWER_PIN;
    P2IE |= POWER_PIN;
    P2IFG &= ~POWER_PIN;

}

#endif /* BSP_H_ */
