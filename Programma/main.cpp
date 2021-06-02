#include "HTTPClient.h"

int main(int argc, char** argv)
{
    try {
        if (argc != 3)
            throw Error("Invalid arguments");
        HTTPClient http_cnt;
        http_cnt.set_url(argv[1]);
        http_cnt.set_file(argv[2]);
        int res = http_cnt.request();
        if (res == 200) {
            cout << "File saved" << endl;
        } else
            throw Error(string("Failed to load file:server response code ") + to_string(res));
    } catch (Error& e) {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}
