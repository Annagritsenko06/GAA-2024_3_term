#include "pch.h"
#include "Header.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
// ��������� ������ ������� ���� � �����
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
            throw ERROR_THROW(103);		// ������ ��� �������� ����� ���������
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
            *log.stream << "\n================================  �������� ������  ================================\n"
                << "===========================  ����: " << buffer << "  ===========================\n\n";
            //log.stream->flush();  // �������������� ������
        }
        else {
            std::cerr << "������: ���� �� ������ ��� ��������� ����� nullptr!" << std::endl;
        }
    }




    void writeLine(std::ostream* stream, char* c, ...) {  // ������� � �������� ������������ �����
        if (stream == nullptr) {
            std::cerr << "������: ��������� �� ����� ����� nullptr!" << std::endl;
            return;
        }

        // ��������� ���������� ����������
        va_list args;
        va_start(args, c);

        // ���������� ��������� ������ ��� result
        size_t initial_size = 15;
        char* result = new char[initial_size];
        if (result == nullptr) {
            std::cerr << "������: �� ������� �������� ������!" << std::endl;
            return;
        }

        result[0] = '\0';  // ������������� ������
        size_t size = 0;

        // ������������ �����
        char* ptr = c;
        while (ptr != nullptr && strcmp(ptr, "") != 0) {
            size_t slen = strlen(ptr);
            if (size + slen + 1 > initial_size) {  // +1 ��� ����-�����������
                size_t new_size = size + slen + 1;
                char* temp = new char[new_size];
                if (temp == nullptr) {
                    std::cerr << "������: �� ������� �������� ������!" << std::endl;
                    delete[] result;  // ������������ ����� ���������� ������
                    return;
                }
                strcpy(temp, result);
                delete[] result;
                result = temp;
                initial_size = new_size;
            }
            strcat(result, ptr);
            size += slen;
            ptr = va_arg(args, char*);  // �������� ��������� ��������
        }

        va_end(args);

        *stream << result << std::endl;  // ������� ���������
        stream->flush();  // �������������� ������ � ����
        delete[] result;  // ����������� ������
    }

    void writeParm(LOG& log, Parm::PARM& parm) {
        if (log.stream == nullptr) {
            std::cerr << "������: ��������� �� ����� ����� nullptr!" << std::endl;
            return;
        }

        char inTxt[PARM_MAX_SIZE], outTxt[PARM_MAX_SIZE], logTxt[PARM_MAX_SIZE];
        wcstombs(inTxt, parm.in, wcslen(parm.in) + 1);
        wcstombs(outTxt, parm.out, wcslen(parm.out) + 1);
        wcstombs(logTxt, parm.log, wcslen(parm.log) + 1);

        *log.stream << "\n================  ���������  ================";
        *log.stream << "\n-in: " << inTxt
            << "\n-out: " << outTxt
            << "\n-log: " << logTxt;
        log.stream->flush();  // �������������� ������ � ����
    }

    void writeIn(std::ostream* stream, In::IN& in) {
        if (stream == nullptr) {
            std::cerr << "������: ��������� �� ����� ����� nullptr!" << std::endl;
            return;
        }

        *stream << "\n=============  �������� ������  =============";
        *stream << "\n���������� ��������: " << std::setw(3) << in.size
            << "\n���������������: " << std::setw(3) << in.ignor
            << "\n���������� �����: " << std::setw(3) << in.lines << "\n\n";
        stream->flush();  // �������������� ������ � ����
    }

    void writeError(std::ostream* stream, Error::ERROR e) {
        if (stream == nullptr) {
            if (e.position.col == -1 || e.position.line == -1)
                std::cout << std::endl << "������ N" << e.id << ": " << e.message << std::endl;
            else if (e.position.col == 0)
                std::cout << std::endl << "������ N" << e.id << ": " << e.message
                << " ������: " << e.position.line << std::endl;
            else
                std::cout << std::endl << "������ N" << e.id << ": " << e.message
                << " ������: " << e.position.line
                << " ������� � ������: " << e.position.col << std::endl;
            system("pause");
        }
        else {
            if (e.position.col == -1 || e.position.line == -1)
                *stream << std::endl << "������ N" << e.id << ": " << e.message;
            else if (e.position.col == 0)
                *stream << std::endl << "������ N" << e.id << ": " << e.message
                << " ������: " << e.position.line;
            else
                *stream << std::endl << "������ N" << e.id << ": " << e.message
                << " ������: " << e.position.line
                << " ������� � ������: " << e.position.col;
            *stream << std::endl;
            stream->flush();  // �������������� ������ � ����
        }
    }

    void writeWords(std::ostream* stream, In::InWord* words) {
        if (stream == nullptr) {
            std::cerr << "������: ��������� �� ����� ����� nullptr!" << std::endl;
            return;
        }

        *stream << " ===================  ������� ����  ===================" << std::endl;
        for (int i = 0; i < words->size; i++)
            *stream << std::setw(2) << i << std::setw(3) << words[i].line << " |  " << words[i].word << std::endl;
        *stream << "\n=============================================================================\n\n";
        stream->flush();  // �������������� ������ � ����
    }

    void Close(LOG& log) {
        if (log.stream != nullptr) {
            log.stream->close();
        }
        else {
            std::cerr << "������: ��������� �� ����� ����� nullptr!" << std::endl;
        }
    }
}