#include <UnitTest++/UnitTest++.h>
#include "/home/kali/Kursovaya/1/HTTPClient.h"
#include "/home/kali/Kursovaya/1/HTTPClient.cpp"
SUITE(SetUrlTest)
{
    TEST(HttpUrlTest1) {
        HTTPClient hc;
        hc.set_url("http://moodle.pnzgu.ru/my/");
        CHECK_EQUAL(hc.get_server(), "moodle.pnzgu.ru");
        CHECK_EQUAL(hc.get_path(),"/my/");
    }
    TEST(HttpUrlTest2) {
        HTTPClient hc;
        hc.set_url("http://pnzgu.ru/");
        CHECK_EQUAL(hc.get_server(), "pnzgu.ru");
        CHECK_EQUAL(hc.get_path(), "/");
    }
    TEST(HttpUrlTest3) {
        HTTPClient hc;
        hc.set_url("http://pnzgu.ru");
        CHECK_EQUAL(hc.get_server(), "pnzgu.ru");
        CHECK_EQUAL(hc.get_path(), "/");
    }

    TEST(HttpsUrlTest) {
        HTTPClient hc;
        CHECK_THROW(hc.set_url("https://github.com/"),Error);
    }

    TEST(FtpUrlTest) {
        HTTPClient hc;
        CHECK_THROW(hc.set_url("ftp://ftp.startrekftp.ru/"),Error);
    }

    TEST(RelativeUrlTest1) {
        HTTPClient hc;
        CHECK_THROW(hc.set_url("/moodle.pnzgu.ru"), Error);
    }
    TEST(RelativeUrlTest2) {
        HTTPClient hc;
        CHECK_THROW(hc.set_url("moodle.pnzgu.ru/my/"),Error);
    }

}

SUITE(SetFileTest)
{
    TEST(FileTest1) {
        HTTPClient hc;
        hc.set_file("/home/kali/Kursovaya/Test/test.txt");
        CHECK(true);
    }
    TEST(FileTest2) {
        HTTPClient hc;
        hc.set_file("/home/kali/Kursovaya/Test/test.html");
        CHECK(true);
    }

    TEST(FileTest3) {
        HTTPClient hc;
        CHECK_THROW(hc.set_file("/home/kali/Kursovaya/Test/"),Error); // папка
    }

    TEST(FileTest4) {
        HTTPClient hc;
        CHECK_THROW(hc.set_file("/home/kali/Kursovaya/Test/net_takogo_faila.12342"),Error); // несуществующий файл
    }

    TEST(FileTest5) {
        HTTPClient hc;
        hc.set_file("/home/kali/Kursovaya/Test/test.pdf");
        CHECK(true);

    }
    
}

SUITE(ParseHeadersTest)
{
    TEST(ResponseCode200) {
        int http_code;
        std::string loc, http_str = "HTTP/1.0 200 OK\r\n\r\n";
        int ret = HTTPClient::parse_headers(http_str,http_code, loc);
        CHECK_EQUAL(ret, 19);
        CHECK_EQUAL(http_code, 200);
        CHECK(loc.empty());
    }
    TEST(ResponseCode301) {
        int http_code;
        std::string loc, http_str = "HTTP/1.1 301 Moved Permanently\r\nServer: Apache\r\nLocation: http://ya.ru\r\nContent-Language: ru\r\n\r\n";
        int ret = HTTPClient::parse_headers(http_str,http_code, loc);
        CHECK_EQUAL(ret, 96);
        CHECK_EQUAL(http_code, 301);
        CHECK_EQUAL(loc, "http://ya.ru");
    }

    TEST(ResponseLocationNotHeader) {
        int http_code;
        string loc, http_str = "HTTP/1.1 200 OK\r\nServer: Apache\r\n\r\nLocation: http://ya.ru\r\n";
        int ret = HTTPClient::parse_headers(http_str,http_code, loc);
        CHECK_EQUAL(ret, 35);
        CHECK_EQUAL(http_code, 200);
        CHECK(loc.empty());
    }
    TEST(IncorrectResponse) {
        int http_code;
        string loc, http_str = "HTTP/1.1 200 OK\r\n";
        int ret = HTTPClient::parse_headers(http_str,http_code, loc);
        CHECK_EQUAL(ret, -1);
    }
}
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
