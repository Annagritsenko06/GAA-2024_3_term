#include "pch.h"
#include "LexAnalysis.h"
#include "Graphs.h"
#include<stack>
#include<string>
#include<cstring>


namespace Lexer
{
	Graph graphs[N_GRAPHS] =	// соответствие лексем и токенов
	{
		
		{ LEX_SEPARATORS, FST::FST(GRAPH_SEPARATORS)) },
		{ LEX_LITERAL, FST::FST(GRAPH_V_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_INT_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_STRING_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_SMBL_LITERAL) },
		{ LEX_LITERAL, FST::FST(GRAPH_FALSE) },
		{ LEX_LITERAL, FST::FST(GRAPH_TRUE) },
		{ LEX_GOUTLAW, FST::FST(GRAPH_GOUTLAW) },
		{ LEX_MAIN, FST::FST(GRAPH_MAIN) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_NUMBER) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_STRING) },
		{ LEX_FUNCTION, FST::FST(GRAPH_FUNCTION) },
		{ LEX_LITERAL, FST::FST(GRAPH_B_LITERAL) },
		{ LEX_RETURN, FST::FST(GRAPH_RETURN) },
		{ LEX_WRITE, FST::FST(GRAPH_WRITE) },
		{ LEX_CONDITION, FST::FST(GRAPH_CONDITION) },
		{ LEX_CYCLE, FST::FST(GRAPH_CYCLE) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_TBOOL) },
		{ LEX_ID_TYPE, FST::FST(GRAPH_SMBL) },
		{ LEX_ID, FST::FST(GRAPH_ID) }

	};

	char* getScopeName(IT::IdTable idtable, char* prevword) // имя текущей области видимости (если находимся внутри блока)
	{
		char* a = new char[5];
		a[0] = 'm';
		a[1] = 'a';
		a[2] = 'i';
		a[3] = 'n';
		a[4] = '\0';
		if (strcmp(prevword, MAIN) == 0)
			return a;
		for (int i = idtable.size - 1; i >= 0; i--)
			if (idtable.table[i].idtype == IT::IDTYPE::F)
				return idtable.table[i].id;
		return nullptr;		// не найдено имя функции
	}

	int getLiteralIndex(IT::IdTable ittable, char* value, IT::IDDATATYPE type) // получаем индекс повторного литерала(по значению)
	{
		
		
		int intValue = atoi(value); 
		bool boolValue = (intValue != 0);
		
		for (int i = 0; i < ittable.size; i++)
		{
			if (ittable.table[i].idtype == IT::IDTYPE::L && ittable.table[i].iddatatype == type)
			{
				switch (type)
				{
				case IT::IDDATATYPE::TBOOL:
					// Обработка булевого литерала
					if (ittable.table[i].value.vbool == boolValue) // Сравниваем булевое значение
					{
						return i; // Токен найден
					}
					break;
				case IT::IDDATATYPE::NUM: // Проверка для числовых литералов
					if (ittable.table[i].value.vint == intValue) {
						return i;
					}
					break;

				case IT::IDDATATYPE::STR:
				{
					char buf[STR_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++) // строковый литерал без кавычек
						buf[j - 1] = value[j];
					buf[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp(ittable.table[i].value.vstr.str, buf) == 0)
						return i;
				}
				break;

				case IT::IDDATATYPE::SMBL:
				{
					char buf_2[SMBL_MAXSIZE];
					for (unsigned j = 1; j < strlen(value) - 1; j++) // символьный литерал без кавычек
						buf_2[j - 1] = value[j];
					buf_2[strlen(value) - 2] = IN_CODE_NULL;
					if (strcmp(ittable.table[i].value.vsmbl.smbl, buf_2) == 0)
						return i;
				}
				break;

				
					
				}
			}
		}
		return NULLIDX_TI; // Не найден
	}
	IT::IDDATATYPE getType(char* curword, char* idtype) {
		// Проверка на nullptr
		if (curword == nullptr || idtype == nullptr) {
			if (!strcmp(TYPE_STRING, idtype))
				return IT::IDDATATYPE::STR;  // Строковый идентификатор
			if (!strcmp(TYPE_SMBL, idtype))
				return IT::IDDATATYPE::SMBL;  // Символьный идентификатор
			if (!strcmp(TYPE_TBOOL, idtype))
				return IT::IDDATATYPE::TBOOL;  // Булевый идентификатор
			if (!strcmp(TYPE_NUMBER, idtype))
				return IT::IDDATATYPE::NUM;    // Числовой идентификатор
			else
			return IT::IDDATATYPE::UNDEF; // Возвращаем неопределенный тип
		}
		size_t len = strlen(curword);
		// Проверка на литералы true и false
		if (strcmp(curword, "true") == 0 || strcmp(curword, "false") == 0) {
			return IT::IDDATATYPE::TBOOL; // Булевый идентификатор
		}
		
		if (len == 7 && curword[4] == '\'' && curword[6] == '\'') { // Символьный литерал вида 'x'
			return IT::IDDATATYPE::SMBL;
		}

		// Проверка на строковый литерал
		if (len > 1 && curword[0] == '"' && curword[len - 1] == '"') { // Строка вида "text"
			return IT::IDDATATYPE::STR;
		}
		if (len > 2 && curword[0] == '0' && (curword[1] == 'x')) {
			for (size_t i = 2; i < len; ++i) {
				if (!isxdigit(curword[i])) {
					return IT::IDDATATYPE::UNDEF; // Неопределенный тип, если невалидные символы
				}
			}
			return IT::IDDATATYPE::NUM; // Шестнадцатеричный литерал — числовой тип
		}

		// Проверки для других типов
		if (!strcmp(TYPE_STRING, idtype))
			return IT::IDDATATYPE::STR;  // Строковый идентификатор
		if (!strcmp(TYPE_SMBL, idtype))
			return IT::IDDATATYPE::SMBL;  // Символьный идентификатор
		if (!strcmp(TYPE_TBOOL, idtype))
			return IT::IDDATATYPE::TBOOL;  // Булевый идентификатор
		if (!strcmp(TYPE_NUMBER, idtype))
			return IT::IDDATATYPE::NUM;    // Числовой идентификатор

		// Проверка на числовой литерал
		if (isdigit(*curword) || *curword == LEX_MINUS)
			return IT::IDDATATYPE::NUM;

		// Проверка на строковый литерал
		if (*curword == IN_CODE_QUOTE) {
			return IT::IDDATATYPE::STR;

		}

		return IT::IDDATATYPE::UNDEF; // Неопределенный тип
	}

	int getIndexInLT(LT::LexTable& lextable, int itTableIndex)		// индекс первого вхождения идентификатора в таблице лексем
	{
		if (itTableIndex == NULLIDX_TI)		// если идентификатор встречается впервые
			return lextable.size;
		for (int i = 0; i < lextable.size; i++)
			if (itTableIndex == lextable.table[i].idxTI)
				return i;
		return NULLIDX_TI;
	}


	bool isLiteral(char* id) // проверка, является ли токен литералом
	{
		// Проверка на литералы true и false
		if (strcmp(id, "true") == 0 || strcmp(id, "false") == 0)
			return true;

		// Проверка на числовые литералы или символы
		if (isdigit(*id) || *id == IN_CODE_QUOTE || *id == LEX_MINUS)
			return true;

		// Проверка на шестнадцатеричные литералы
		if (id[0] == '0' && (id[1] == 'x' || id[1] == 'X')) {
			// Проверяем, что после '0x' идут только допустимые символы
			for (char* ptr = id + 2; *ptr != '\0'; ++ptr) {
				if (!isxdigit(*ptr)) // isxdigit проверяет на символы 0-9, A-F, a-f
					return false;
			}
			return true;
		}

		return false;
	}


	IT::STDFNC getStandFunction(char* id)
	{
		if (!strcmp(STRCOPY, id))
			return IT::STDFNC::F_STRCOPY;
		if (!strcmp(LENGHT, id))
			return IT::STDFNC::F_LENGHT;
		return IT::STDFNC::F_NOT_STD;
	}

	char* getNextLiteralName()		// сгенерировать следующее имя литерала
	{
		static int literalNumber = 1;
		char* buf = new char[SCOPED_ID_MAXSIZE], num[3];
		strcpy_s(buf, MAXSIZE_ID, "LTRL");
		_itoa_s(literalNumber++, num, 10);
		strcat(buf, num);
		return buf;
	}

	IT::Entry* getEntry(		// формирует и возвращает строку ТИ
		Lexer::LEX& tables,		// ТЛ + ТИ
		char lex,				// лексема
		char* id,				// идентификатор
		char* idtype,			// предыдущая (тип)
		bool isParam,			// признак параметра функции
		bool isFunc,			// признак функции
		Log::LOG log,			// протокол
		int line,				// строка в исходном тексте
		bool& lex_ok)			// флаг ошибки(по ссылке)
	{

		IT::IDDATATYPE type = getType(id, idtype);	// тип данных
		int index = IT::isId(tables.idtable, id);	// индекс существующего идентификатора или -1
		if (lex == LEX_LITERAL)
			index = getLiteralIndex(tables.idtable, id, type);
		if (index != NULLIDX_TI)
			return nullptr;		// уже существует

		IT::Entry* itentry = new IT::Entry;
		itentry->iddatatype = type;		// сохраняем тип данных

		itentry->idxfirstLE = getIndexInLT(tables.lextable, index);		// индекс первой строки в таблице лексем 

		if (lex == LEX_LITERAL) // литерал
		{
			bool int_ok = IT::SetValue(itentry, id);
			if (!int_ok)
			{
				Log::writeError(log.stream, Error::GetError(313, line, 0)); // превышен максимальный размер числового литерала
				lex_ok = false;
			}

			// Проверка для строковых литералов
			if (itentry->iddatatype == IT::IDDATATYPE::STR && itentry->value.vstr.len == 0)
			{
				Log::writeError(log.stream, Error::GetError(310, line, 0)); // пустой строковый литерал
				lex_ok = false;
			}
			if (itentry->iddatatype == IT::IDDATATYPE::SMBL && itentry->value.vsmbl.smbl_len == 0)
			{
				Log::writeError(log.stream, Error::GetError(310, line, 0)); // пустой строковый литерал
				lex_ok = false;
			}

			// Проверка для булевых литералов
			if (itentry->iddatatype == IT::IDDATATYPE::TBOOL)
			{
				if (itentry->value.vbool != 0 && itentry->value.vbool != 1)
				{
					Log::writeError(log.stream, Error::GetError(320, line, 0)); // неверное значение для булевого литерала
					lex_ok = false;
				}
			}

			strcpy_s(itentry->id, getNextLiteralName());
			itentry->idtype = IT::IDTYPE::L;
		}
		else	// если идентификатор
		{
			switch (type)
			{
			case IT::IDDATATYPE::STR:	// если строковый, то копируем литерал
				strcpy_s(itentry->value.vstr.str, "");
				itentry->value.vstr.len = STR_DEFAULT;
				break;
			
			case IT::IDDATATYPE::NUM:	// если целочисленный, то записываем
				itentry->value.vint = NUM_DEFAULT;
				break;
			case IT::IDDATATYPE::SMBL:	// если строковый, то копируем литерал
				strcpy_s(itentry->value.vsmbl.smbl, "");
				itentry->value.vstr.len = SMBL_DEFAULT;
				break;
			case IT::IDDATATYPE::TBOOL:	// если булевый, то записываем
				itentry->value.vbool = BOOL_DEFAULT;
				break;
			}

			if (isFunc)
			{
				switch (getStandFunction(id))
				{
				case IT::STDFNC::F_STRCOPY:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = STRCOPY_TYPE;
					itentry->value.params.count = STRCOPY_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[STRCOPY_PARAMS_CNT];
					for (int k = 0; k < STRCOPY_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::STRCOPY_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_LENGHT:
				{
					itentry->idtype = IT::IDTYPE::S;
					itentry->iddatatype = LENGHT_TYPE;
					itentry->value.params.count = LENGHT_PARAMS_CNT;
					itentry->value.params.types = new IT::IDDATATYPE[LENGHT_PARAMS_CNT];
					for (int k = 0; k < LENGHT_PARAMS_CNT; k++)
						itentry->value.params.types[k] = IT::LENGHT_PARAMS[k];
					break;
				}
				case IT::STDFNC::F_NOT_STD:
					itentry->idtype = IT::IDTYPE::F;
					break;
				}
			}
			else if (isParam)
				itentry->idtype = IT::IDTYPE::P;
			else
				itentry->idtype = IT::IDTYPE::V;

			strncpy_s(itentry->id, id, SCOPED_ID_MAXSIZE);
		}

		int i = tables.lextable.size;	 // индекс в ТЛ текущего идентификатора
		if (i > 1 && itentry->idtype == IT::IDTYPE::V && tables.lextable.table[i - 2].lexema != LEX_GOUTLAW /*&& tables.lextable.table[i - 2].lexema ==LEX_CONDITION*/)
		{
			Log::writeError(log.stream, Error::GetError(304, line, 0));		// в объявлении отсутствует ключевое слово new  itentry->iddatatype != STRCOPY_TYPE&& itentry->iddatatype != LENGHT_TYPE
			lex_ok = false;
		}
		if (i > 1 && itentry->idtype == IT::IDTYPE::F && tables.lextable.table[i - 1].lexema != LEX_FUNCTION)
		{
			Log::writeError(log.stream, Error::GetError(303, line, 0));		// в объявлении не указан тип функции
			lex_ok = false;
		}
		if (itentry->iddatatype == IT::IDDATATYPE::UNDEF)
		{
			
			Log::writeError(log.stream, Error::GetError(300, line, 0));		// невозможно определить тип
			lex_ok = false;
		}

		return itentry;
	}

	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm)		// анализ действий при встрече соответствующих лексем
	{
		static bool lex_ok = true;
		tables.lextable = LT::Create(MAXSIZE_LT);
		tables.idtable = IT::Create(MAXSIZE_TI);

		bool isParam = false, isFunc = false;
		int enterPoint = NULL;
		char curword[STR_MAXSIZE], nextword[STR_MAXSIZE];	// один символ из исходного кода
		int curline;
		std::stack <char*> scopes;			// стек для хранения имени текущей области видимости

		for (int i = 0; i < In::InWord::size; i++)
		{
			strcpy_s(curword, in.words[i].word);
			if (i < In::InWord::size - 1)
				strcpy_s(nextword, in.words[i + 1].word);
			curline = in.words[i].line;		// берем исходную строку из структуры
			isFunc = false;
			int idxTI = NULLIDX_TI;

			for (int j = 0; j < N_GRAPHS; j++)
			{
				FST::FST fst(curword, graphs[j].graph);
				if (FST::execute(fst))
				{
					char lexema = graphs[j].lexema;		// одна лексема из таблицы
					switch (lexema)
					{
					case LEX_MAIN:
						enterPoint++;
						break;
					case LEX_SEPARATORS:
					{
						switch (*curword)
						{
						case LEX_LEFTHESIS:		// открывающая скобка - параметры  функции
						{
							isParam = true;
							if (*nextword == LEX_RIGHTTHESIS || ISTYPE(nextword))
							{
								char* functionname = new char[MAXSIZE_ID];
								char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
								if (scopename == nullptr)
									break;
								strcpy_s(functionname, MAXSIZE_ID, scopename);
								scopes.push(functionname);
							}
							break;
						}
						case LEX_RIGHTTHESIS:	// закрывающая скобка
						{
							isParam = false;
							if (*in.words[i - 1].word == LEX_LEFTHESIS || (i > 2 && (tables.lextable.table[tables.lextable.size - 2].lexema == LEX_ID_TYPE)))
								scopes.pop();
							break;

						}
						case LEX_LEFTBRACE:		// начало области видимости
						{
							if (i > 0 && *in.words[i - 1].word == LEX_ISFALSE || *in.words[i - 1].word == LEX_ISTRUE || *in.words[i - 1].word == LEX_CYCLE)
								break;
							char* functionname = new char[MAXSIZE_ID];
							char* scopename = getScopeName(tables.idtable, in.words[i - 1].word);
							if (scopename == nullptr)  break;		// получаем имя этой области
							strcpy_s(functionname, MAXSIZE_ID, scopename);
							scopes.push(functionname);
							break;
						}
						case LEX_BRACELET:		// конец области видимости
						{
							if (*in.words[i + 1].word == LEX_ID_TYPE || /**in.words[i + 1].word == LEX_SCRIPT ||*/ *in.words[i + 1].word == LEX_MAIN)
								if (!scopes.empty())
									scopes.pop();
							break;
						}
						}
						lexema = *curword;
						break;
					}
					case LEX_ID:
					case LEX_LITERAL:
					{
						char id[STR_MAXSIZE] = "";
						idxTI = NULLDX_TI;		// индекс идентификатора в ТИ
						if (*nextword == LEX_LEFTHESIS)
							isFunc = true;					// идентификатор функции
						char* idtype = (isFunc && i > 1) ?	// тип идентификатора
							in.words[i - 2].word : in.words[i - 1].word;		// пропускаем ключевое слово function
						if (!isFunc && !scopes.empty())
							strncpy_s(id, scopes.top(), MAXSIZE_ID);
						strncat(id, curword, MAXSIZE_ID);
						if (isLiteral(curword))
							strcpy_s(id, curword);	// литерал заменяется своим значением

						IT::Entry* itentry = getEntry(tables, lexema, id, idtype, isParam, isFunc, log, curline, lex_ok);

						if (itentry != nullptr)		// первая встреча - объявление
						{
							if (isFunc)		// если функция - сохранить список параметров
							{
								if (getStandFunction(id) == IT::STDFNC::F_NOT_STD)	// стандартная функция или нет
								{
									itentry->value.params.count = NULL;
									itentry->value.params.types = new IT::IDDATATYPE[MAX_PARAMS_COUNT];
									for (int k = i; in.words[k].word[0] != LEX_RIGHTTHESIS; k++)
									{
										if (k == In::InWord::size || in.words[k].word[0] == LEX_SEPARATOR)
											break;
										if (ISTYPE(in.words[k].word))
										{
											if (itentry->value.params.count >= MAX_PARAMS_COUNT)
											{
												Log::writeError(log.stream, Error::GetError(306, curline, 0));
												lex_ok = false;
												break;
											}
											itentry->value.params.types[itentry->value.params.count++] = getType(NULL, in.words[k].word);
											//std::cout << itentry->value.params.count;
												
										}
									}
									
								}
							}
							IT::Add(tables.idtable, *itentry);
							idxTI = tables.idtable.size - 1;
						}
						else     // повторный идентификатор
						{
							int i = tables.lextable.size - 1;	// попытка переопределить идентификатор
							if (i > 0 && tables.lextable.table[i - 1].lexema == LEX_GOUTLAW || tables.lextable.table[i].lexema == LEX_GOUTLAW
								|| tables.lextable.table[i - 1].lexema == LEX_FUNCTION || tables.lextable.table[i].lexema == LEX_FUNCTION
								|| tables.lextable.table[i - 1].lexema == LEX_ID_TYPE || tables.lextable.table[i].lexema == LEX_ID_TYPE
								/*|| tables.lextable.table[i - 1].lexema == LEX_SCRIPT || tables.lextable.table[i].lexema == LEX_SCRIPT*/
								|| tables.lextable.table[i - 1].lexema == LEX_LITERAL_NUMO)
							{
								Log::writeError(log.stream, Error::GetError(305, curline, 0));
								lex_ok = false;
							}
							idxTI = IT::isId(tables.idtable, id);	// индекс существующего идентификатора
							if (lexema == LEX_LITERAL || lexema == LEX_LITERAL_NUMO)
								idxTI = getLiteralIndex(tables.idtable, curword, getType(id, in.words[i - 1].word)); // или литерала
						}
					}
					break;
					}

					LT::Entry* ltentry = new LT::Entry(lexema, curline, idxTI);	
					LT::Add(tables.lextable, *ltentry);			// каждую лексему добавляем в таблицу лексем
					break;
				}
				else if (j == N_GRAPHS - 1)		 // ошибка лексического анализатора: цепочка не распознана
				{
					Log::writeError(log.stream, Error::GetError(201, curline, 0));
					//throw ERROR_THROW(201);
					lex_ok = false;
				}
			}
		}

		if (enterPoint == NULL)		// не найдена точка входа
		{
			Log::writeError(log.stream, Error::GetError(301));
			//ERROR_THROW
			lex_ok = false;
		}
		if (enterPoint > 1)		//больше одной точки входа
		{
			Log::writeError(log.stream, Error::GetError(302));
			lex_ok = false;
		}
		return lex_ok;
	}
};