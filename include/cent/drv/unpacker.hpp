#pragma once

#include <memory>

#include "cent/def.hpp"

namespace cent::drv {

/**
 * Class capable of extracting various packages, like .tar.gz
 */
class Unpacker {
 public:
    virtual ~Unpacker() {}

    virtual void unpack(const stdfs::path& src, const stdfs::path& dst) = 0;
};

std::unique_ptr<Unpacker> default_unpacker();
}  // namespace cent::drv