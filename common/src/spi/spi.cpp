#include "spi.hpp"
#include "hardware/gpio.h"
#include "pico/time.h"
#include <stdlib.h>
#include <string.h>

using namespace common::spi;

Spi4::Spi4(spi_inst_t* spi, uint32_t baudRate)
    : spiPtr_(spi), baudRate_(baudRate) {
    buffer_ = (uint8_t*)malloc(buffer_len());
}

void Spi4::init() {
    gpio_init_mask(1 << pins.SCLK | 1 << pins.MOSI | 1 << pins.CS |
                   1 << pins.DC | 1 << pins.RES);

    // rst, leave it low (reset) first
    gpio_set_dir(pins.RES, GPIO_OUT);
    gpio_put(pins.RES, 0);

    spi_init(spiPtr_, baudRate_);

    // configure sdo and sclk to pull from spi

    gpio_set_function(pins.MOSI, GPIO_FUNC_SPI);
    gpio_set_function(pins.SCLK, GPIO_FUNC_SPI);

    gpio_set_dir(pins.CS, GPIO_OUT);
    gpio_put(pins.CS, 0);

    gpio_set_dir(pins.DC, GPIO_OUT);
    gpio_put(pins.DC, 0);

    send_init_commands();
}

void Spi4::send_command(uint8_t command) {
    gpio_put(pins.DC, 0);  
    spi_write_blocking(spiPtr_, &command, 1);
}

void Spi4::send_commands(const uint8_t* commands, size_t len) {
    gpio_put(pins.DC, 0);  // low for command mode
    spi_write_blocking(spiPtr_, commands, len);
}

void Spi4::send_init_commands() {
    // reset the display by pulling rst low then high
    gpio_put(pins.RES, 0);
    sleep_ms(10);
    gpio_put(pins.RES, 1);

    // it's dangerous to go alone, take this:
    // https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf

    // SSD1306_SWITCHCAPVCC to generate from a 3.3v source
    // or SSD1306_EXTERNALVCC for external vcc
    uint8_t vccstate = SSD1306_SWITCHCAPVCC;

    send_command(SSD1306_DISPLAYOFF);

    send_command(SSD1306_SETDISPLAYCLOCKDIV);
    send_command(0x80);  // the suggested ratio 0x80

    send_command(SSD1306_SETMULTIPLEX);
    send_command(height_ - 1);

    static const uint8_t init2[] = {SSD1306_SETDISPLAYOFFSET,    // 0xD3
                                    0x0,                         // no offset
                                    SSD1306_SETSTARTLINE | 0x0,  // line #0
                                    SSD1306_CHARGEPUMP};
    send_commands(init2, sizeof(init2) / sizeof(init2[0]));
    send_command((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);

    static const uint8_t init3[] = {SSD1306_MEMORYMODE,  // 0x20
                                    0x00,                // 0x0 act like ks0108
                                    SSD1306_SEGREMAP | 0x1, SSD1306_COMSCANDEC};
    send_commands(init3, sizeof(init3) / sizeof(init3[0]));

    uint8_t comPins = 0x02;
    uint8_t contrast = 0x8F;

    if ((width_ == 128) && (height_ == 64)) {
        comPins = 0x12;
        contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
    } else {
        // Other screen varieties -- TBD
    }

    send_command(SSD1306_SETCOMPINS);
    send_command(comPins);
    send_command(SSD1306_SETCONTRAST);
    send_command(contrast);

    // This command is used to set the duration of the pre-charge period. The
    // interval is counted in number of DCLK, where RESET equals 2 DCLKs.
    send_command(SSD1306_SETPRECHARGE);  // 0xd9
    send_command((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);

    static const uint8_t init5[] = {
        SSD1306_SETVCOMDETECT,  // 0xDB
        0x40,
        SSD1306_DISPLAYALLON_RESUME,  // 0xA4 - start using gddram
        // SSD1306_DISPLAYALLON,  // 0xA5 - ignore gddram, set everything on
        SSD1306_NORMALDISPLAY,  // 0xA6
        SSD1306_DEACTIVATE_SCROLL,
        SSD1306_DISPLAYON  // Main screen turn on
    };
    send_commands(init5, sizeof(init5) / sizeof(init5[0]));
}

void Spi4::draw_pixel(int16_t x, int16_t y, SSD1306PixelColor color) {
    assert(x >= 0 && x < width_ && y >= 0 && y < height_);

    switch (color) {
        case SSD1306_COLOR_ON:
            buffer_[x + (y / 8) * width_] |= (1 << (y & 7));
            break;
        case SSD1306_COLOR_OFF:
            buffer_[x + (y / 8) * width_] &= ~(1 << (y & 7));
            break;
        case SSD1306_COLOR_INVERTED:
            buffer_[x + (y / 8) * width_] ^= (1 << (y & 7));
            break;
        default:
            break;
    }
}

void Spi4::write() {
    static const uint8_t dlist1[] = {
        SSD1306_PAGEADDR,
        0x0,                       // Page start address
        0xFF,                      // Page end (not really, but works here)
        SSD1306_COLUMNADDR, 0x0};  // Column start address

    send_commands(dlist1, sizeof(dlist1) / sizeof(dlist1[0]));
    send_command(width_ - 1);  // Column end address

    gpio_put(pins.DC, 1);
    // bring data/command high since we're sending data now

    spi_write_blocking(spiPtr_, buffer_, buffer_len());
}

void Spi4::clear() { memset(buffer_, 0, buffer_len()); }
