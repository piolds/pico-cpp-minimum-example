#include <pico/stdlib.h>
#include <stdio.h>

#include <spi.hpp>

int main() {
    stdio_init_all();
    const uint8_t led = PICO_DEFAULT_LED_PIN;
    gpio_init(led);
    gpio_set_dir(led, GPIO_OUT);

    common::spi::Spi4 spi(spi0, 8000 * 1000);
    spi.init();

    spi.clear();
    spi.write();

    spi.draw_pixel(10, 12, common::spi::SSD1306_COLOR_ON);
    spi.write();

    uint8_t x = 1;
    uint8_t y = 1;
    for (;;) {
        uint8_t state = gpio_get(led);
        gpio_put(led, !state);
    }
    return 0;
}
