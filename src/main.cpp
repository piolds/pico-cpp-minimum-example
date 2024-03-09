#include <pico/stdlib.h>
#include <stdio.h>

int main() {
    stdio_init_all();
    const uint8_t led = PICO_DEFAULT_LED_PIN;
    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);

    for (;;) {
        uint8_t state = gpio_get(led);
        gpio_put(led, !state);
        sleep_ms(500);
    }
    return 0;
}
