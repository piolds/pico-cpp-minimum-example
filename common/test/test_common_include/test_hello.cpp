#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Graphic.h"
#include "hello.h"


TEST(a, b) { ASSERT_EQ(returnHello(), "hello"); }

TEST(b, c) {
    unsigned char* buf;
    auto g = SSD1306::Graphic(buf, 1, 2);
    std::cout << g.getHeight();
}
