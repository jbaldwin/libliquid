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
