

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>
#include "../../../parser/includes.h"

void console_putc(char c);
char console_getc(void);
void console_puts(char *s);
int console_gets(char *s, int len);

#define CONSOLE_UART USART6

void delay(void);
void delay(void)
{
	for (int i = 0; i < 800000; i++) {
		__asm__("nop");
	}
}

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

	/* Setup UART parameters. */
	usart_set_baudrate(CONSOLE_UART, 9600);
	usart_set_databits(CONSOLE_UART, 8);
	usart_set_stopbits(CONSOLE_UART, USART_STOPBITS_1);
	usart_set_parity(CONSOLE_UART, USART_PARITY_NONE);
	usart_set_flow_control(CONSOLE_UART, USART_FLOWCONTROL_NONE);
	usart_set_mode(CONSOLE_UART, USART_MODE_TX_RX);

	/* Finally enable the USART. */
	usart_enable(CONSOLE_UART);
}

static void gpio_setup(void)
{
	gpio_set(GPIOI, GPIO1);

	/* Setup GPIO6 and 7 (in GPIO port A) for LED use. */
	gpio_mode_setup(GPIOI, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO1);
}

/*
 * Some definitions of our console "functions" attached to the
 * USART.
 *
 * These define sort of the minimum "library" of functions which
 * we can use on a serial port.
 */

/*
 * console_putc(char c)
 *
 * Send the character 'c' to the USART, wait for the USART
 * transmit buffer to be empty first.
 */
void console_putc(char c)
{
	gpio_toggle(GPIOI, GPIO1);
	usart_send_blocking(CONSOLE_UART, c);
}

/*
 * char = console_getc(int wait)
 *
 * Check the console for a character. If the wait flag is
 * non-zero. Continue checking until a character is received
 * otherwise return 0 if called and no character was available.
 */
char console_getc()
{
	gpio_toggle(GPIOI, GPIO1);
	return usart_recv_blocking(CONSOLE_UART);
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
		console_putc(*s);
		/* Add in a carraige return, after sending line feed */
		if (*s == '\n') {
			console_putc('\r');
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

	*t = '\000';
	/* read until a <CR> is received */
	while ((c = console_getc()) != '\r') {
		if ((c == '\010') || (c == '\127')) {
			if (t > s) {
				/* send ^H ^H to erase previous character */
				console_puts("\010 \010");
				t--;
			}
		} else {
			*t = c;
			console_putc(c);
			if ((t - s) < len) {
				t++;
			}
		}
		/* update end of string with NUL */
		*t = '\000';
	}
	return t - s;
}

/*
 * Set up the GPIO subsystem with an "Alternate Function"
 * on some of the pins, in this case connected to a
 * USART.
 */
int main(void)
{
	char buf[128];
	int len;

	clock_setup(); /* initialize our clock */
	gpio_setup();
	usart_setup();

	while (1) {
		console_puts(">>>");
		len = console_gets(buf, 128);
		console_putc('\n');
		if (len) {
			cmd(buf);
		}
	}
}
