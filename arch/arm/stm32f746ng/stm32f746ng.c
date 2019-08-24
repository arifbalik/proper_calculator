

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>

#include "../../../euler/inc/euler.h"

void console_puts(char *s);
int console_gets(char *s, int len);

#define CONSOLE_UART USART6

static void clock_setup(void)
{
	//rcc_clock_setup_in_hsi_out_48mhz();
	rcc_periph_clock_enable(RCC_GPIOI);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_USART6);
}

static void usart_setup(void)
{
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO6);
	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO7);
	gpio_set_af(GPIOC, GPIO_AF8, GPIO6 | GPIO7);

	usart_set_baudrate(CONSOLE_UART, 9600);
	usart_set_databits(CONSOLE_UART, 8);
	usart_set_stopbits(CONSOLE_UART, USART_STOPBITS_1);
	usart_set_parity(CONSOLE_UART, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_UART, USART_FLOWCONTROL_NONE);
	usart_set_mode(CONSOLE_UART, USART_MODE_TX_RX);

	usart_enable(CONSOLE_UART);
}

/*
 * void console_puts(char *s)
 *
 * Send a string to the console, one character at a time, return
 * after the last character, as indicated by a NUL character, is
 * reached.
 */
void console_puts(char *s)
{
	while (*s != '\000') {
		usart_send_blocking(CONSOLE_UART, *s);
		/* Add in a carraige return, after sending line feed */
		if (*s == '\n') {
			usart_send_blocking(CONSOLE_UART, '\r');
		}
		s++;
	}
}

/*
 * int console_gets(char *s, int len)
 *
 * Wait for a string to be entered on the console, limited
 * support for editing characters (back space and delete)
 * end when a <CR> character is received.
 */
int console_gets(char *s, int len)
{
	char *t = s;
	char c;

	console_puts(">>>"); //prompt

	*t = '\000';
	/* read until a <CR> is received */
	while ((c = usart_recv_blocking(CONSOLE_UART)) != '\r') {
		if ((c == '\010') || (c == '\127')) {
			if (t > s) {
				/* send ^H ^H to erase previous character */
				console_puts("\010 \010");
				t--;
			}
		} else {
			*t = c;
			usart_send_blocking(CONSOLE_UART, c);
			if ((t - s) < len) {
				t++;
			}
		}
		/* update end of string with NULL */
		*t = '\000';
	}
	console_puts("\n");
	return t - s;
}

int main(void)
{
	char buf[MAX_QUERY_LENGTH];
	ersl_t euler;
	clock_setup();
	usart_setup();

	while (1) {
		if (console_gets(buf, 128)) {
			euler.ascii = buf;
			parse_query(&euler);
		}
	}
}
