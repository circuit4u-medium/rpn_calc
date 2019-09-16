#include "bsp.h"


/**
 * main.c
 */
int main(void)
{
	init_SYS();
	init_UART();

	uart_puts("Hello World!\n\r");

	
	return 0;
}
