#pragma once

#include "i2c.h"
#include <vector>
using namespace common;

class SubDevice;

class SubTx {
   public:
    virtual bool receive(const Config& config, char* buffer, int size) = 0;
};

class MainDevice : public SubTx {
   public:
    inline void registerSubDevice(SubDevice* observer) {
        subDevices_.push_back(observer);
    }

    inline bool miso(const Config& config, char* buffer, int size) {
        buffer_ = buffer;
        return true;
    }

    inline char* getBuffer() { return buffer_; }

    inline bool send(const Config& config, char* buffer, int size) {
        for (auto subDevice : subDevices_) {
            subDevice->mosi(config, buffer, size);
        }
        return true;
    }

   private:
    std::vector<SubDevice*> subDevices_{};
    char* buffer_;
};
