#pragma once

#include <cent/c_str.hpp>
#include <filesystem>
#include <utility>

extern "C" struct archive;
extern "C" struct archive_entry;

namespace cent::archive {

class Entry {
    friend class Extractor;

 public:
    Entry(const Entry&) = delete;
    Entry& operator=(const Entry&) = delete;

    Entry(Entry&&) = default;
    Entry& operator=(Entry&&) = default;
    ~Entry() = default;

    CStr path() const noexcept;
    size_t size() const noexcept;

    Entry& operator>>(std::span<std::byte>& buf);

 private:
    constexpr Entry() noexcept = default;
    Entry(::archive* ar, ::archive_entry* entry) noexcept
        : m_a(ar), m_e(entry) {}
    ::archive* m_a;
    ::archive_entry* m_e{};
};

class Extractor {
 public:
    Extractor();
    Extractor(const Extractor&) = delete;
    Extractor& operator=(const Extractor&) = delete;

    Extractor(Extractor&&) noexcept = default;
    Extractor& operator=(Extractor&&) noexcept = default;

    ~Extractor();

    Entry* operator()(std::span<const std::byte> chunk);

 private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}  // namespace cent::archive
