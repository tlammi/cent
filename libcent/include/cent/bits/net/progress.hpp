#pragma once

namespace cent::net {

/**
 * \brief Passed to a progress callback
 * */
struct Progress {
    long dltotal;  // how much to download
    long dlnow;    // how much downloaded
    long ultotal;  // how much to upload
    long ulnow;    // how much uploaded
};

}  // namespace cent::net
