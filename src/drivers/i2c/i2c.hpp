#ifndef I2C_HPP
#define I2C_HPP

#include <cstddef>
#include <cstdint>
#include <string_view>

class II2c
{
public:
    virtual ~II2c() = default;

    virtual bool init() = 0;
    virtual bool open(std::string_view bus) = 0;
    virtual void close() = 0;

    virtual bool isOpen() const = 0;

    virtual bool setSlave(uint8_t address) = 0;
    virtual bool probe(uint8_t address) = 0;

    virtual bool write(const uint8_t* data,
                       size_t length) = 0;

    virtual bool read(uint8_t* data,
                      size_t length) = 0;

    virtual bool writeRead(const uint8_t* tx,
                           size_t txLength,
                           uint8_t* rx,
                           size_t rxLength) = 0;
};

class LinuxI2c final : public II2c
{
public:
    LinuxI2c() = default;
    ~LinuxI2c() override;

    LinuxI2c(const LinuxI2c&) = delete;
    LinuxI2c& operator=(const LinuxI2c&) = delete;

    bool init() override;

    bool open(std::string_view bus) override;

    void close() override;

    bool isOpen() const override;

    bool setSlave(uint8_t address) override;

    bool probe(uint8_t address) override;

    bool write(const uint8_t* data,
               size_t length) override;

    bool read(uint8_t* data,
              size_t length) override;

    bool writeRead(const uint8_t* tx,
                   size_t txLength,
                   uint8_t* rx,
                   size_t rxLength) override;

private:
    int fd_{-1};

    uint8_t currentSlave_{0xff};
};

#endif