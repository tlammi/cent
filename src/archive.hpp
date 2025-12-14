#pragma once

#include <functional>

#include "c_str.hpp"
#include "concepts.hpp"
#include "io/file.hpp"

extern "C" struct archive;
extern "C" struct archive_entry;

namespace cent::archive {

template <class T>
concept entry_output =
    requires(T t, size_t s) {
        { t.resize(s) };
    } && (character_type<typename T::value_type> ||
          std::same_as<typename T::value_type, std::byte>);

class Entry {
    friend class Archive;

 public:
    CStr path() const noexcept;
    size_t size() const noexcept;

    Entry(const Entry&) = delete;
    Entry& operator=(const Entry&) = delete;

    Entry(Entry&&) = delete;
    Entry& operator=(Entry&&) = delete;

    constexpr ~Entry() noexcept = default;

    template <entry_output T>
    Entry& operator>>(T& out) {
        auto sz = size();
        if (sz > 0) {
            out.resize(size());
            auto count = read(out);
            assert(count == out.size());
        } else {
            out.clear();
            auto buf = std::vector<std::byte>(1024, std::byte{});
            while (true) {
                auto count = read(buf);
                if (!count) break;
                auto orig_size = out.size();
                out.resize(orig_size + count);
                std::copy(buf.begin(), buf.begin() + count,
                          out.begin() + orig_size);
            }
        }
        return *this;
    }

    size_t read(std::span<std::byte> buf);

 private:
    constexpr Entry() noexcept = default;
    constexpr Entry(::archive* a, ::archive_entry* e) noexcept
        : m_a(a), m_e(e) {}
    ::archive* m_a{};
    ::archive_entry* m_e{};
};

class Archive {
 public:
    constexpr Archive() noexcept = default;

    explicit Archive(std::span<const std::byte> buf);

    ~Archive();

    CStr compression_name() const noexcept;

    class iterator {
        friend Archive;

     public:
        using value_type = Entry;
        using reference = Entry&;
        using difference_type = std::ptrdiff_t;

        constexpr iterator() noexcept = default;

        iterator& operator++();
        iterator operator++(int) {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        reference operator*() const noexcept { return *m_e; }

        bool operator==(std::default_sentinel_t) const noexcept;

     private:
        constexpr explicit iterator(Entry* e) noexcept : m_e(e) {}
        Entry* m_e;
    };

    auto begin() noexcept {
        auto it = iterator{&m_entry};
        ++it;
        return it;
    }
    constexpr auto end() const noexcept { return std::default_sentinel; }

 private:
    Entry m_entry{};
};

#if 0
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
    static ::archive* init_archive();

 public:
    constexpr Extractor() noexcept = default;
    template <range_of<std::span<const std::byte>> Range>
    explicit Extractor(Range&& range) : m_arch(init_archive()) {
        auto it = range.begin();
        auto end = range.end();
        if (it != end) m_buf = *it;
        m_next = [it, end] mutable -> std::span<const std::byte> {
            ++it;
            if (it == end) return {};
            return *it;
        };
    }
    explicit Extractor(std::span<const std::byte> buffer)
        : m_arch(init_archive()) {
        m_buf = buffer;
        m_next = [] -> std::span<const std::byte> { return {}; };
    }

    explicit Extractor(io::File& f);

    Extractor(const Extractor&) = delete;
    Extractor& operator=(const Extractor&) = delete;

    Extractor(Extractor&&) = delete;
    Extractor& operator=(Extractor&&) = delete;

    ~Extractor() {}

    constexpr auto end() const noexcept { return std::default_sentinel; }

 private:
    ::archive* m_arch{};
    ::archive_entry* m_entry{};
    std::span<const std::byte> m_buf{};
    std::function<std::span<const std::byte>()> m_next{};
};
#endif

}  // namespace cent::archive
