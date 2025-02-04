#include "pch.h"
#include "Header.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
// отрисовка границ таблицы лога в файле
#define W(x, y)  << std::setw(x) << (y) <<
#define STR(n, line, type, id)\
	"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace Log
{
    LOG getstream(wchar_t  logfile[])
    {
        LOG stream;
        stream.stream = new std::ofstream;
        stream.stream->open(logfile);
        if (!stream.stream->is_open())
            throw ERROR_THROW(103);		// ошибка при создании файла протокола
        wcscpy_s(stream.logfile, logfile);
        return stream;
    }

    void writeLog(LOG& log) {
        char buffer[80];
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        const char* format = "%d.%m.%Y %H:%M:%S";
        strftime(buffer, 80, format, timeinfo);

        if (log.stream != nullptr && log.stream->is_open()) {
            *log.stream << "\n================================  Протокол работы  ================================\n"
                << "===========================  Дата: " << buffer << "  ===========================\n\n";
            //log.stream->flush();  // Принудительная запись
        }
        else {
            std::cerr << "Ошибка: файл не открыт или указатель равен nullptr!" << std::endl;
        }
    }




    void writeLine(std::ostream* stream, char* c, ...) {  // вывести в протокол конкатенацию строк
        if (stream == nullptr) {
            std::cerr << "Ошибка: указатель на поток равен nullptr!" << std::endl;
            return;
        }

        // Обработка переменных аргументов
        va_list args;
        va_start(args, c);

        // Определяем начальный размер для result
        size_t initial_size = 15;
        char* result = new char[initial_size];
        if (result == nullptr) {
            std::cerr << "Ошибка: не удалось выделить память!" << std::endl;
            return;
        }

        result[0] = '\0';  // Инициализация строки
        size_t size = 0;

        // Конкатенация строк
        char* ptr = c;
        while (ptr != nullptr && strcmp(ptr, "") != 0) {
            size_t slen = strlen(ptr);
            if (size + slen + 1 > initial_size) {  // +1 для нуль-терминатора
                size_t new_size = size + slen + 1;
                char* temp = new char[new_size];
                if (temp == nullptr) {
                    std::cerr << "Ошибка: не удалось выделить память!" << std::endl;
                    delete[] result;  // Освобождение ранее выделенной памяти
                    return;
                }
                strcpy(temp, result);
                delete[] result;
                result = temp;
                initial_size = new_size;
            }
            strcat(result, ptr);
            size += slen;
            ptr = va_arg(args, char*);  // Получаем следующий аргумент
        }

        va_end(args);

        *stream << result << std::endl;  // Выводим результат
        stream->flush();  // Принудительная запись в файл
        delete[] result;  // Освобождаем память
    }

    void writeParm(LOG& log, Parm::PARM& parm) {
        if (log.stream == nullptr) {
            std::cerr << "Ошибка: указатель на поток равен nullptr!" << std::endl;
            return;
        }

        char inTxt[PARM_MAX_SIZE], outTxt[PARM_MAX_SIZE], logTxt[PARM_MAX_SIZE];
        wcstombs(inTxt, parm.in, wcslen(parm.in) + 1);
        wcstombs(outTxt, parm.out, wcslen(parm.out) + 1);
        wcstombs(logTxt, parm.log, wcslen(parm.log) + 1);

        *log.stream << "\n================  Параметры  ================";
        *log.stream << "\n-in: " << inTxt
            << "\n-out: " << outTxt
            << "\n-log: " << logTxt;
        log.stream->flush();  // Принудительная запись в файл
    }

    void writeIn(std::ostream* stream, In::IN& in) {
        if (stream == nullptr) {
            std::cerr << "Ошибка: указатель на поток равен nullptr!" << std::endl;
            return;
        }

        *stream << "\n=============  Исходные данные  =============";
        *stream << "\nКоличество символов: " << std::setw(3) << in.size
            << "\nПроигнорировано: " << std::setw(3) << in.ignor
            << "\nКоличество строк: " << std::setw(3) << in.lines << "\n\n";
        stream->flush();  // Принудительная запись в файл
    }

    void writeError(std::ostream* stream, Error::ERROR e) {
        if (stream == nullptr) {
            if (e.position.col == -1 || e.position.line == -1)
                std::cout << std::endl << "Ошибка N" << e.id << ": " << e.message << std::endl;
            else if (e.position.col == 0)
                std::cout << std::endl << "Ошибка N" << e.id << ": " << e.message
                << " Строка: " << e.position.line << std::endl;
            else
                std::cout << std::endl << "Ошибка N" << e.id << ": " << e.message
                << " Строка: " << e.position.line
                << " Позиция в строке: " << e.position.col << std::endl;
            system("pause");
        }
        else {
            if (e.position.col == -1 || e.position.line == -1)
                *stream << std::endl << "Ошибка N" << e.id << ": " << e.message;
            else if (e.position.col == 0)
                *stream << std::endl << "Ошибка N" << e.id << ": " << e.message
                << " Строка: " << e.position.line;
            else
                *stream << std::endl << "Ошибка N" << e.id << ": " << e.message
                << " Строка: " << e.position.line
                << " Позиция в строке: " << e.position.col;
            *stream << std::endl;
            stream->flush();  // Принудительная запись в файл
        }
    }

    void writeWords(std::ostream* stream, In::InWord* words) {
        if (stream == nullptr) {
            std::cerr << "Ошибка: указатель на поток равен nullptr!" << std::endl;
            return;
        }

        *stream << " ===================  ТАБЛИЦА СЛОВ  ===================" << std::endl;
        for (int i = 0; i < words->size; i++)
            *stream << std::setw(2) << i << std::setw(3) << words[i].line << " |  " << words[i].word << std::endl;
        *stream << "\n=============================================================================\n\n";
        stream->flush();  // Принудительная запись в файл
    }

    void Close(LOG& log) {
        if (log.stream != nullptr) {
            log.stream->close();
        }
        else {
            std::cerr << "Ошибка: указатель на поток равен nullptr!" << std::endl;
        }
    }
}