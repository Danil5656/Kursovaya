#include "HTTPClient.h"

string HTTPClient::get_server()
{
    return server;
}
string HTTPClient::get_path()
{
    return path;
}

void HTTPClient::set_url(const string& url)
{
    if (url.substr(0, 7) == "HTTP://" || url.substr(0, 7)== "http://") {
        server = url.substr(7, url.find("/", 7) - 7);
        path = url.substr(7 + server.size());
        if (path.empty())
            path = "/";
    } else
        throw Error("Incorrect url");
}

void HTTPClient::set_file(const string& f_path)
{
    file_path = f_path;
    ifstream file_check;
    file_check.open(file_path);
    if (!file_check.is_open()) {
        file_check.close();
        throw Error("File not found or not opened");
    }
    file_check.close();
}

void HTTPClient::connect()
{
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock < 0) {
        close(client_sock);
        throw Error("Error open socket");
        exit(1);
    }
    int rc;
    addrinfo* server_addr;
    rc = getaddrinfo(server.c_str(), "http", nullptr,&server_addr);
    if (rc != 0)
        throw Error("Getaddrinfo error");
    client_sock = socket(server_addr->ai_family,server_addr->ai_socktype, server_addr->ai_protocol);
    if (client_sock <= 0)
        throw Error("Invalid socket");
    rc = ::connect(client_sock, server_addr->ai_addr,server_addr->ai_addrlen);
    if (rc == -1)
        throw Error("Connection error");
}

int HTTPClient::parse_headers(const string& head_buf_str,int& res_code, string& loc)
{
    int ret;
    size_t head_size = head_buf_str.find("\r\n\r\n") + 4;
    if (head_size == string::npos)
        return -1;
    string head_str(head_buf_str, 0, head_size);
    ret = sscanf(head_str.substr(0, head_str.find("\r\n")).c_str(),"HTTP/1.%*[01] %3u", &res_code);
    if (ret != 1)
        return -1;
    size_t loc_pos = head_str.find("Location: ");
    if (loc_pos != string::npos)
        loc = head_str.substr(loc_pos + 10,head_str.find("\r\n", loc_pos) - loc_pos - 10);
    return head_size;
}

int HTTPClient::request()
{
    string client_request = "GET " + path + " HTTP/1.1\r\nHost: " + server + "\r\nConnection: close\r\n\r\n";
    int ret, headers_size, recv_head_count, http_code;
    string location;
    char recv_head_buf[1024];
    connect();
    ret = send(client_sock, client_request.c_str(),client_request.size(), 0);
    if (ret == -1)
        throw Error("Sending error");
    recv_head_count = recv(client_sock, recv_head_buf, sizeof(recv_head_buf), 0);
    if (recv_head_count == -1)
        throw Error("Recv error");
    headers_size = parse_headers(string(recv_head_buf,recv_head_count), http_code, location);
    if (headers_size == -1)
        throw Error("Parse header error");
    if (!location.empty()) {
        set_url(location);
        return request();
    }

    if (http_code == 200) {
        recv_count = recv_head_count - headers_size;
        memcpy(recv_buf, recv_head_buf + headers_size, recv_count);
        save_to_file();
    }
    return http_code;
}

void HTTPClient::save_to_file()
{
    ofstream file(file_path.c_str(), ofstream::out |ofstream::binary);
    while (recv_count > 0) {
        file.write(recv_buf, recv_count);
        if (!file.good())
            throw Error("Write to file error");
        recv_count = recv(client_sock, recv_buf, sizeof(recv_buf), 0);
        if (recv_count == -1)
            throw Error("Recv error");
    }
    file.close();
}
