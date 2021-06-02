/**
 * @file Exception.h
 * @author Соколов Д.А.
 * @version 1.0
 * @date 12.05.21
 * @copyright ИБСТ ПГУ
 * @brief Описание класса Error
 */
#pragma once
#include <string>
#include <stdexcept>
using namespace std;
/// @brief Класс для обработки ошибок
class Error : public invalid_argument
{
public:
    /// @brief Запрещающий конструктор без параметров
    Error() = delete;
    /** @brief Конструктор с параметром
     @param std::string what - обычная строка, которая хранит описание ошибки.
     @details Данный конструктор перегружается вызовом конструктора базового класса с названием "invalid_argument"
    */
    Error(const string what) : invalid_argument(what) {}
};
