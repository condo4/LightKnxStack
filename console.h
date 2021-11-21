#ifndef __CONSOLE__
#define __CONSOLE__

/* Debug console managment,
 * Written to be very minimalist
 * to be used also for debuging ISR
 *
 * This is why we don't use sprintf macro
 * here.
 */


#ifndef CONSOLE_UART
/* Case with STM32L432KC */
#define CONSOLE_UART USART2
#endif

void console_start_next_xfer();

void console_print_hex(uint8_t c);
void console_print_int(int i);
void console_print_bytes(uint8_t *bytes, uint8_t len);
void console_print_char(unsigned char c);
void console_print_string(const char *str);

#endif
