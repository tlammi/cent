#pragma once

namespace cent::dist {

class TransportConsumer {
 public:
    virtual bool on_status(uint16_t code) noexcept = 0;

 protected:
    ~TransportConsumer() = default;
};

class Transport {
 public:
};
}  // namespace cent::dist
