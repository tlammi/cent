#pragma once

#include <boost/url.hpp>
#include <cent/name.hpp>

namespace cent::dist {

using Url = boost::urls::url;
using UrlView = boost::urls::url_view;

Url manifest_url(NameView nm);

}  // namespace cent::dist
