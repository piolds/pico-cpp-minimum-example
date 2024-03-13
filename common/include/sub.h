#include <vector>

#include "i2c.h"
using namespace common;

class II2C {
   public:
    virtual bool miso(const Config& config, char* buffer, int size) = 0;
    virtual bool mosi(const Config& config, char* buffer, int size) = 0;
};

class SubDevice : public II2C {
   public:
    inline bool mosi(const Config& config, char* buffer, int size) override {
        buffer_ = buffer;
        return true;
    }

    inline bool miso(const Config& config, char* buffer, int size) override {
        // I am the sub
        return false;
    }

    inline char* getBuffer() { return buffer_; }

    inline bool send(const Config& config, char* buffer, int size) {
        mainDevice_->miso(config, buffer, size);
        return true;
    }

    inline void registerMainDevice(II2C* mainDevice) {
        mainDevice_ = mainDevice;
    }

   private:
    II2C* mainDevice_;
    char* buffer_;
};

class MainDevice : public II2C {
   public:
    inline void registerSubDevice(II2C* observer) {
        subDevices_.push_back(observer);
    }

    inline char* getBuffer() { return buffer_; }

    inline bool send(const Config& config, char* buffer, int size) {
        for (auto subDevice : subDevices_) {
            subDevice->mosi(config, buffer, size);
        }
        return true;
    }

   private:
    bool miso(const Config& config, char* buffer, int size) override{
        buffer_ = buffer;
        return true;
    };

    bool mosi(const Config& config, char* buffer, int size) override {
        // I am the main
        return false;
    };

    std::vector<II2C*> subDevices_{};
    char* buffer_;
};
