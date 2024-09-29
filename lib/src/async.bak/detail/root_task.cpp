#pragma once

#include <cent/bits/async/detail/executor.hpp>
#include <cent/bits/async/detail/root_task.hpp>
#include <cent/bits/util/assert.hpp>

namespace cent::async::detail {

void RootTask::enable() {
    CENT_ASSERT(m_exec);
    auto& enabled = m_exec->m_enabled;
    auto iter =
        std::find_if(enabled.begin(), enabled.end(),
                     [&](const auto& v) { return std::addressof(v) == this; });
    CENT_ASSERT(iter != enabled.end());
}

}  // namespace cent::async::detail
