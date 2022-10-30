#pragma once

#include <string>
#include <string_view>

namespace cent::net {

class HttpSession {
 public:
    HttpSession() = default;
    virtual ~HttpSession() {}

    HttpSession(HttpSession&&) = default;
    HttpSession& operator=(HttpSession&&) = default;

    HttpSession(const HttpSession&) = delete;
    HttpSession& operator=(const HttpSession&) = delete;
    /**
     * Tell the session that this header field should be captured and accessible
     * using \a capture_header_field
     *
     * \param field Field to capture
     */
    virtual void capture_header_field(std::string_view field) = 0;

    [[deprecated]] virtual std::string& header_field(
        std::string_view field) = 0;

    /**
     * Access the header fields
     *
     * The header field has to be configured in \a capture_header_field before
     * performing requests
     *
     * \param field Field to access
     * \return Header value or "" if not set
     */
    virtual const std::string& header_field(std::string_view field) const = 0;

    /**
     * Set outgoing request header field
     *
     * \param field Field name
     * \param value Header value
     */
    virtual void set_header_field(std::string_view field,
                                  std::string_view value) = 0;

    /**
     * Access the response body
     *
     * \return Body value
     */
    virtual std::string_view get_body() = 0;

    /**
     * Perform a get request
     *
     * \param url URLto access
     * \return Status code
     */
    virtual int get(std::string_view url) = 0;
};
}  // namespace cent::net