#include "I2C.h"
#include "hardware/i2c.h"

class MyI2c : public I2C {
    inline bool init() override { i2c_init(); };
    inline bool send(const Config& config, const char* buffer,
                     int size) override{
        i2c_write_blocking();
    };
    inline bool receive(const Config& config, char* buffer, int size) override {
    }
};
