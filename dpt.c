#include "dpt.h"

#include <stdbool.h>

#define RESET_PAYLOAD(x)                                                                           \
    for (int pi = 0; pi < (x); ++pi)                                                               \
    payload[pi] = 0

void uint8_to_payload(uint8_t *payload, size_t payload_length, int index, uint8_t value,
                      uint8_t mask)
{
    RESET_PAYLOAD(index + 1);
    payload[index] = (payload[index] & ~mask) | (value & mask);
}

void int16_to_payload(uint8_t *payload, size_t payload_length, int index, int16_t value,
                      uint16_t mask)
{
    RESET_PAYLOAD(index + 2);
    payload[index] = (payload[index] & (~mask >> 8)) | ((value >> 8) & (mask >> 8));
    payload[index + 1] = (payload[index + 1] & ~mask) | (value & mask);
}

void float16_to_payload(uint8_t *payload, size_t payload_length, int index, double value,
                        uint16_t mask) // mask = 0xFFFF
{
    bool wasNegative = false;
    if (value < 0)
    {
        wasNegative = true;
        value *= -1;
    }

    value *= 100.0;
    unsigned short exponent = 0;

    if (value > 2048)
        exponent = ceil(log2(value) - 11.0);

    short mantissa = roundf(value / (1 << exponent));

    if (wasNegative)
        mantissa *= -1;

    int16_to_payload(payload, payload_length, index, mantissa, mask);
    uint8_to_payload(payload, payload_length, index, exponent << 3, 0x78 & (mask >> 8));
}

uint16_t payload_to_uint16(const uint8_t *payload, int index)
{
    return ((((uint16_t)payload[index]) << 8) & 0xFF00) | (((uint16_t)payload[index + 1]) & 0x00FF);
}

double payload_to_float16(const uint8_t *payload, int index)
{
    uint16_t mantissa = payload_to_uint16(payload, index) & 0x87FF;
    if (mantissa & 0x8000)
        return ((~mantissa & 0x07FF) + 1.0) * -0.01 * (1 << ((payload[index] >> 3) & 0x0F));

    return mantissa * 0.01 * (1 << ((payload[index] >> 3) & 0x0F));
}
