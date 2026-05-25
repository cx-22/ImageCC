#include <stdint.h>
#include <emscripten/emscripten.h>

EMSCRIPTEN_KEEPALIVE
void addImage(uint8_t* input, uint8_t* output, int height, int width, int val)
{
    int size = height * width;

    for (int i = 0; i < size; i++)
    {
        int result = input[i] + val;

        // clamp to 0–255 (important for images)
        if (result > 255) result = 255;
        if (result < 0) result = 0;

        output[i] = (uint8_t)result;
    }
}