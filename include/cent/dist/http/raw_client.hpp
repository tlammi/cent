#pragma once

#include <cent/dist/http/session.hpp>
#include <memory>

namespace cent::dist::http {

class RawClient {};

std::unique_ptr<RawClient> make_raw_client(Session* sess);

}  // namespace cent::dist::http
