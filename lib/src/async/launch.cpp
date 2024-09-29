#include <cent/bits/async/launch.hpp>

namespace cent::async {

Launcher launch(Task<void> t) { return {std::move(t)}; }
}  // namespace cent::async
