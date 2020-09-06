libturbohttp - C++17 HTTP/1.1 Request and Response stateful parser.
===================================================================

[![CI](https://github.com/jbaldwin/libturbohttp/workflows/build/badge.svg)](https://github.com/jbaldwin/libturbohttp/workflows/build/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/jbaldwin/libturbohttp/badge.svg?branch=master)](https://coveralls.io/github/jbaldwin/libturbohttp?branch=master)
[![language][badge.language]][language]
[![license][badge.license]][license]

[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow.svg
[badge.license]: https://img.shields.io/badge/license-Apache--2.0-blue

[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/Apache_License

**libturbohttp** is a C++17 HTTP/1.1 Request and Response stateful parser with zero memory allocations.  The intended purpose of this library is to provide a high quality and extremely fast stateful HTTP/1.1 parser with modern C++ coding standards.

**libturbohttp** is licensed under the Apache 2.0 license.

# Overview #
* Modern C++17 API
* Stateful parser, continue parsing where the previous call left off at when partial requests and responses are provided.
* Zero allocation parsing, The request and response objects can be created on the stack and do not allocate any memory when parsing.
* Custom maximum number of headers for request and response objects, default is 16.

# Usage #

## Examples

### Parsing an HTTP Request

Below is an example of parsing two HTTP requests, a GET and a POST.  It re-uses the parser inbetween each request and prints out the relevant information parsed.

```C++
#include <iostream>
#include <turbohttp/turbohttp.hpp>

int main()
{
    std::string http_request_data =
        "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 Pathtraq/0.9\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n"
        "Accept-Encoding: gzip,deflate\r\n"
        "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n"
        "Keep-Alive: 115\r\n"
        "Connection: keep-alive\r\n"
        "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; __utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; __utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"
        "\r\n";

    using namespace turbo::http;
    request parser{};
    if(parser.parse(http_request_data) == request_parse_result::complete)
    {
        std::cout << "Method: " << to_string(parser.http_method()) << "\n";
        std::cout << "URI: " << parser.http_uri() << "\n";
        std::cout << "Version: " << to_string(parser.http_version()) << "\n";
        std::cout << "Headers:\n";
        parser.http_header_for_each([](std::string_view name, std::string_view value) {
            std::cout << "    " << name << ": " << value << "\n";
        });
    }

    std::cout << "\n\n";

    std::string post_data = "data blob for upload";
    http_request_data =
        "POST /upload HTTP/1.1\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 Pathtraq/0.9\r\n"
        "Connection: keep-alive\r\n"
        "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; __utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; __utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"
        "Content-Length: " + std::to_string(post_data.size());
    http_request_data.append("\r\n\r\n");
    http_request_data.append(post_data);

    parser.reset();
    if(parser.parse(http_request_data) == request_parse_result::complete)
    {
        std::cout << "Method: " << to_string(parser.http_method()) << "\n";
        std::cout << "URI: " << parser.http_uri() << "\n";
        std::cout << "Version: " << to_string(parser.http_version()) << "\n";
        std::cout << "Headers:\n";
        parser.http_header_for_each([](std::string_view name, std::string_view value) {
            std::cout << "    " << name << ": " << value << "\n";
        });
        std::cout << "Body: " << parser.http_body().value_or("") << "\n";
    }

    return 0;
}
```

### Parsing with partial incoming data.

It is great when the entire HTTP request arrives at the same time, but it is very likely that requests will be split across `recv()` calls.  The parser for both requests and responses will allow for partial parsing and continue where left off when given additional data.  Its important that the data passed into `parse()` be contiguous, e.g. it shouldn't be provided separate buffers for each partial part received.  Below is pseudo code on how to possibly handle partial requests.

```C++
using namespace turbo::http;

bool done = false;
std::string data;
data.reserve(1024 * 8); // Recommend having a larger buffer to append into.
request parser{};
while(!done)
{
    data += recv(socket); // receive data on the socket and append to the buffer.
    switch(parser.parse(data))
    {
        case request_parse_result::complete:
            // Full http request has been received, break out of parse loop.
            done = true;
            break;
        case request_parse_result::incomplete:
            // Continue to recv more data, the http request is incomplete.
            break;
        case request_parse_result::method_unknown:
        case request_parse_result::http_version_malformed:
        case request_parse_result::http_version_unknown:
        case request_parse_result::maximum_headers_exceeded:
        case request_parse_result::chunk_malformed:
            // Request is malformed in some manner, handle error
            handle_error();
            break;
    }
}
``

## Requirements
* C++17
* gcc or clang
* Tested on ubuntu and fedora.

## Instructions

### Building
TODO

### CMake Projects
TODO

### Running Tests
TODO

## Benchmarks
TODO

## Support

File bug reports, feature requests and questions using [GitHub Issues](https://github.com/jbaldwin/libturbohttp/issues)

Copyright © 2018-2020, Josh Baldwin