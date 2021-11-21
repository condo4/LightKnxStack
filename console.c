#include "ring_buffer.h"
#include <main.h>
#include <gpio.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>

extern UART_HandleTypeDef huart2;
#define CONSOLE_HUART &huart2
#define CONSOLE_BUFFER_LEN 4096

volatile static uint8_t console_buffer_data_space[CONSOLE_BUFFER_LEN];
static ring_buffer console_buffer = { .buffer = console_buffer_data_space,
                                      .head = 0,
                                      .tail = 0,
                                      .maxlen = CONSOLE_BUFFER_LEN,
                                      .overflow = 0 };

volatile static int console_xfer_in_progress = 0;
static uint8_t console_data_to_xfer = 0;

char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void console_start_next_xfer()
{
    if (!ring_buffer_empty(&console_buffer))
    {
        ring_buffer_pop(&console_buffer, &console_data_to_xfer);
        HAL_UART_Transmit_IT(CONSOLE_HUART, &console_data_to_xfer, 1);
        console_xfer_in_progress = 1;
    }
    else
    {
        console_xfer_in_progress = 0;
    }
}

static inline void console_send_char(unsigned char c)
{
    ring_buffer_push(&console_buffer, c);
    if (!console_xfer_in_progress)
        console_start_next_xfer();
}

void console_print_int(int i)
{
    if (i > 10)
        console_print_int(i / 10);
    console_send_char(hex[i % 10]);
}

void console_print_hex(uint8_t c)
{
    console_send_char(hex[(c & 0xF0) >> 4]);
    console_send_char(hex[c & 0x0F]);
}

void console_print_bytes(uint8_t *bytes, uint8_t len)
{
    int i;
    console_send_char('{');
    for (i = 0; i < len - 1; i++)
    {
        console_print_hex(bytes[i]);
        console_send_char(' ');
    }
    console_print_hex(bytes[i]);
    console_send_char('}');
}

void console_print_char(unsigned char c)
{
    console_send_char(c);
}

void console_print_string(const char *str)
{
    while (*str != 0)
    {
        console_print_char(*(str++));
    }
}

int _write(int file, char *ptr, int len)
{
    if (file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        int i = 0;
        for (i = 0; i < len; i++)
        {
            if (ptr[i] == '\n')
            {
                while (ring_buffer_push(&console_buffer, '\r') == -1)
                    ;
            }
            while (ring_buffer_push(&console_buffer, (uint8_t)(ptr[i])) == -1)
                ;
            if (!console_xfer_in_progress)
                console_start_next_xfer();
        }
        return i;
    }
    errno = EIO;
    return -1;
}
