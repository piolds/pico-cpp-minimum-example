
namespace common {

enum class Direction { MOSI, MISO };
class Config {
   public:
    // Device address
    unsigned char address;

    // Bus frequency
    unsigned long frequency;

    Config() {
        address = 0;
        frequency = 0;
    }

    Config(unsigned char address, unsigned int frequency) {
        this->address = address;
        this->frequency = frequency;
    }
};

class I2C {
   public:
    virtual ~I2C(){};
    // Open the device for reading and/or writing
    virtual bool init() { return false; };
    // Send data to device
    virtual bool send(const Direction& direction, const Config& config,
                      char* buffer, int size) {
        return false;
    };
    // Receive data from device
    virtual bool receive(const Direction& direction, const Config& config,
                         char* buffer, int size) {
        return false;
    };
};

}  // namespace common
