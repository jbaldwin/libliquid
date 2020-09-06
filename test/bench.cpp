#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <turbohttp/turbohttp.hpp>

#include <turbohttp/turbohttp.hpp>

#include <iostream>
#include <chrono>

TEST_CASE("Benchmark")
{

    // theres no transfer encoding so this is safe to re-use as input
    //std::string buffer =
    //  "GET /cookies HTTP/1.1\r\n"
    // "Host: 127.0.0.1:8090\r\n"
    // "Connection: keep-alive\r\n"
    // "Cache-Control: max-age=0\r\n"
    // "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    // "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 (KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17\r\n"
    // "Accept-Encoding: gzip,deflate,sdch\r\n"
    // "Accept-Language: en-US,en;q=0.8\r\n"
    // "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n"
    // "Cookie: name=wookie\r\n\r\n";
    std::string buffer =
        "GET /wp-content/uploads/2010/03/hello-kitty-darth-vader-pink.jpg HTTP/1.1\r\n"
        "Host: www.kittyhell.com\r\n"
        "User-Agent: Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10.6; ja-JP-mac; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3 Pathtraq/0.9\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
        "Accept-Language: ja,en-us;q=0.7,en;q=0.3\r\n"
        "Accept-Encoding: gzip,deflate\r\n"
        "Accept-Charset: Shift_JIS,utf-8;q=0.7,*;q=0.7\r\n"
        "Keep-Alive: 115\r\n"
        "Connection: keep-alive\r\n"
        "Cookie: wp_ozh_wsa_visits=2; wp_ozh_wsa_visit_lasttime=xxxxxxxxxx; __utma=xxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.xxxxxxxxxx.x; __utmz=xxxxxxxxx.xxxxxxxxxx.x.x.utmccn=(referral)|utmcsr=reader.livedoor.com|utmcct=/reader/|utmcmd=referral\r\n"
        "\r\n";
    constexpr size_t iterations = 10'000'000;

    auto start = std::chrono::steady_clock::now();
    turbo::http::request request{};
    for(size_t i = 0; i < iterations; ++i)
    {
        request.reset();
        request.parse(buffer);
    }
    auto end = std::chrono::steady_clock::now();

    auto total = end - start;
    auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(total).count();
    std::cout << "Total running time in ms: " << total_ms  << "\n";

    double requests_per_second = ((double)iterations) / total_ms * 1000;
    std::cout << "requests/sec: " << (uint64_t)requests_per_second << "\n";
    std::cout << "MegaBytes per second: " << (buffer.length() * requests_per_second) / 1024 / 1024 << "\n";

    REQUIRE(true);
}
