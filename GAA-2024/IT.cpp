#include "pch.h"
#include "Header.h"
#include <iomanip>

// отрисовка в консоли границ таблицы
#define W(x, y)\
		<< std::setw(x) << (y) <<
#define STR(n, line, type, id)\
		"|" W(4,n) " |  " W(5,line) "    |" W(17,type) " |  " W(SCOPED_ID_MAXSIZE, id) " |"


namespace IT
{
	IdTable Create(int size)
	{
		if (size > MAXSIZE_TI)
			throw ERROR_THROW(203);
		IdTable idtable;
		idtable.table = new Entry[idtable.maxsize = size];
		idtable.size = NULL;
		return idtable;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
			throw ERROR_THROW(203);
		idtable.table[idtable.size++] = entry;
	}

	int isId(IdTable& idtable, char id[SCOPED_ID_MAXSIZE])		// возвращает номер строки(если есть), TI_NULLIDX(если нет)
	{
		for (int i = 0; i < idtable.size; i++)
			if (strcmp(idtable.table[i].id, id) == 0)
				return i;
		return NULLIDX_TI;
	}

	bool SetValue(IT::IdTable& idtable, int index, char* value)
	{
		return SetValue(&(idtable.table[index]), value);
	}

	bool SetValue(IT::Entry* entry, char* value) // установка значения переменной
	{
		bool rc = true;

		switch (entry->iddatatype) {
		case NUM:
		{
			int temp = 0;
			bool isBinary = true;
			
			for (size_t i = 0; i < strlen(value); i++) {
				if (value[i] != '0' && value[i] != '1') {
					isBinary = false;
					break;
				}
			}

			if (isBinary) {
				
				temp = strtol(value, nullptr, 2);
			}
			else if (strncmp(value, "0x", 2) == 0) {
				
				temp = strtol(value, nullptr, 16);
			}
			else {
			
				temp = atoi(value);
			}

			if (temp > NUM_MAXSIZE || temp < NUM_MINSIZE) {
				if (temp > NUM_MAXSIZE)
					temp = NUM_MAXSIZE;
				if (temp < NUM_MINSIZE)
					temp = NUM_MINSIZE;
				rc = false;
			}
			entry->value.vint = temp;
			break;
		}
		case STR:
		{
			for (unsigned i = 1; i < strlen(value) - 1; i++) { // без кавычек
				entry->value.vstr.str[i - 1] = value[i];
			}
			entry->value.vstr.str[strlen(value) - 2] = '\0';
			entry->value.vstr.len = strlen(value) - 2;
			break;
		}
		case SMBL:
		{
			// Проверка: длина должна быть 3 ('o')
			if (strlen(value) == 7 && value[4] == '\'' && value[6] == '\'') {
				entry->value.vsmbl.smbl[0] = value[5]; // Извлекаем символ внутри кавычек
				entry->value.vsmbl.smbl[1] = '\0';     // Завершающий символ строки
				entry->value.vsmbl.smbl_len = 1;       // Длина символа
			}
			else {
				rc = false; // Если формат некорректен, установка значения невозможна
			}
			break;
		}
		case TBOOL:
		{
			// Пробуем преобразовать значение в целое число
			int boolTemp = atoi(value); // Преобразуем строку в целое число
			if (boolTemp == 0) {
				entry->value.vbool = false; // 0 - это false
				break;
			}
			else if (boolTemp == 1) {
				entry->value.vbool = true;  // 1 - это true
				break;
			}

		}
		default:

			rc = false; // Неподдерживаемый тип данных
			break;
		}

		return rc;
	}

	void writeIdTable(std::ostream* stream, IT::IdTable& idtable)
	{
		*stream << "=============================  ТАБЛИЦА ИДЕНТИФИКАТОРОВ  ============================\n" << std::endl;
		*stream << "|  N  |СТРОКА В ТЛ| ТИП ИДЕНТИФИКАТОРА |        ИМЯ        | ЗНАЧЕНИЕ (ПАРАМЕТРЫ)" << std::endl;

		for (int i = 0; i < idtable.size; i++)
		{
			IT::Entry* e = &idtable.table[i];
			char type[50] = "";

			switch (e->iddatatype)
			{
			case IT::IDDATATYPE::NUM:
				strcat(type, "  bint ");
				break;
			case IT::IDDATATYPE::STR:
				strcat(type, " string  ");
				break;
			case IT::IDDATATYPE::SMBL:
				strcat(type, " osymb  ");
				break;
			case IT::IDDATATYPE::TBOOL:
				strcat(type, " tbool  ");
				break;
			case IT::IDDATATYPE::UNDEF:
				strcat(type, "UNDEFINED");
				break;
			}

			switch (e->idtype)
			{
			case IT::IDTYPE::V:
				strcat(type, "  variable");
				break;
			case IT::IDTYPE::F:
				strcat(type, "  function");
				break;
			case IT::IDTYPE::P:
				strcat(type, " parameter");
				break;
			case IT::IDTYPE::L:
				strcat(type, "   literal");
				break;
			case IT::IDTYPE::S:
				strcat(type, "  LIB FUNC");
				break;
			default:
				strcat(type, "UNDEFINED ");
				break;
			}

			*stream << STR(i, e->idxfirstLE, type, e->id);

			// Output value based on type
			if (e->idtype == IT::IDTYPE::L || (e->idtype == IT::IDTYPE::V && e->iddatatype != IT::IDDATATYPE::UNDEF))
			{
				if (e->iddatatype == IT::IDDATATYPE::NUM)
					*stream << e->value.vint;
				else if (e->iddatatype == IT::IDDATATYPE::STR)
					*stream << "{" << (int)e->value.vstr.len << "}" << e->value.vstr.str;
				else if (e->iddatatype == IT::IDDATATYPE::SMBL)
					*stream << "{" << (int)e->value.vsmbl.smbl_len << "}" << e->value.vsmbl.smbl;
				else if (e->iddatatype == IT::IDDATATYPE::TBOOL)
					*stream << (e->value.vbool ? "true" : "false");
			}

			if (e->idtype == IT::IDTYPE::F || e->idtype == IT::IDTYPE::S)
			{
				for (int j = 0; j < e->value.params.count; j++)
				{
					*stream << " P" << j << ":";
					switch (e->value.params.types[j])
					{
					case IT::IDDATATYPE::NUM:
						*stream << "BINT |";
						break;
					case IT::IDDATATYPE::STR:
						*stream << "STRING |";
						break;
					case IT::IDDATATYPE::SMBL:
						*stream << "OSYMB |";
						break;
					case IT::IDDATATYPE::TBOOL:
						*stream << "TBOOL |";
						break;
					case IT::IDDATATYPE::PROC:
					case IT::IDDATATYPE::UNDEF:
						*stream << "UNDEFINED";
						break;
					}
				}
			}
			*stream << std::endl;
		}
		*stream << "\n===================================================================================\n\n";
	}
};
