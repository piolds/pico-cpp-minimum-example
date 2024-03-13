#include <Graphic.h>
#include <dsp/basic_math_functions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "hello.h"
#include "sub.h"

TEST(a, b) { ASSERT_EQ(returnHello(), "hello"); }

TEST(b, c) {
    unsigned char* buf;
    auto g = SSD1306::Graphic(buf, 1, 2);
    std::cout << g.getHeight();
}

TEST(dsp, arm_add_f32) {
    float32_t first[4] = {1, 2, 5, 7};
    float32_t second[4] = {1, 2, 3, 10};
    float32_t result[4] = {};
    float32_t expectedResult[4] = {2, 4, 8, 17};

    arm_add_f32(first, second, result, 4);

    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(result[i], expectedResult[i]);
    }
}

TEST(I2C, hello_i2c_stub) {
    static constexpr char* message = "hello world";
    auto main = MainDevice{};
    auto sub = SubDevice{};

    main.registerSubDevice(&sub);
    sub.registerMainDevice(&main);

    Config c{0x1, 2};
    main.send(c, message, 4);

    ASSERT_EQ(sub.getBuffer(), message);
}

