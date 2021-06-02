
/**
 * @file HTTPClient.h
 * @author Соколов Д.А.
 * @version 1.0
 * @date 12.05.21
 * @copyright ИБСТ ПГУ
 * @brief Описание класса HTTPClient
 */
#pragma once
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "Exception.h"
using namespace std;
///@brief Класс, который реализует клиента службы HTTP
class HTTPClient
{
private:
    char recv_buf[1024 * 4096]; ///< атрибут, хранящий буфер для приёма данных от сервера
    int recv_count;  ///< атрибут, хранящий количество байт принятых данных в буфер "recv_buf"
    string server;  ///< атрибут, хранящий адрес сервера
    string path; ///<  атрибут, хранящий путь к файлу на сервере
    string file_path; ///<  атрибут, хранящий путь до файла, в котором будет сохранён результат
    int client_sock; ///< атрибут для сокета
    /** @brief Устанавливает соединение с сервером
    * @throw Error в случае ошибки в открытии сокета и в случае ошибки с установкой соединения с сервером
    */
    void connect();
    /** @brief Метод, записывающий полученные с сервера данные в файл, который был указан пользователем при запуске
      *  @details Записывает определённо количество байт из буффера "recv_buf" в файл "file_path".
     * Данный процесс происходит циклично. Запись данных в файл происходит до тех пор, пока на сокете не закончатся данные
     * @throw Error в случае ошибки при записи данных файл и в случае ошибки в обмене данных с сервером
     */
    void save_to_file();
public:
    /// @brief Конструктор по умолчанию
    HTTPClient() = default;
    /// @brief Деструктор по умолчанию
    ~HTTPClient() = default;
    /** @brief Геттер для получения адреса сервера
        @return std::string, в которой хранится адрес сервера
     * */
    string get_server();
    /** @brief Геттер для получения пути файла на сервере
        @return std::string, в которой путь файла на сервере
     * */
    string get_path();
    /** @brief Метод, выполняющий валидацию url-адреса и заполняющий атрибуты "server" и "path"
        @param std::string url - url-aдрес, который указал пользователь при запуске
        @throw Error, если url-aдрес некорректный
     * */
    void set_url(const string& url);
    /** @brief Метод, выполняющий валидацию файла, где должен сохраниться результат. Также метод заполняет атрибут "file_path"
        @param std::string f_path - путь до файла, который указал пользователь при запуске.
        @throw Error, если файл не найден или не открыт
     * */
    void set_file(const string& f_path);
    /**@brief Статический метод, выполняющий разбор заголовков
     * @param std::string head_buf_str - строка с данными, полученная от сервера.
     * @param http_code - целочисленный http код ответа
     * @param std::string loc -значение заголовка "Location"
     * @return размер заголовков в байтах
     */
    static int parse_headers(const string& head_buf_str, int& http_code, string& loc);
    /**@brief Метод, выполняющий HTTP запрос и сохраняющий результат в файл, который был указан пользователем
    * @throw Error в случае ошибок в работе с сервером
    * @return возвращает ответа сервера (http код)
    * */
    int request();
};
