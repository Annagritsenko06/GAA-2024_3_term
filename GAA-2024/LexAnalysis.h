#pragma once
#include "LT.h"
#include "In.h"
#include "Log.h"
#include "FST.h"

// �������� ������� �������
#define TYPE_NUMBER		"bint"
#define TYPE_STRING		"string"
#define TYPE_SMBL		"osymb"
#define TYPE_TBOOL      "tbool"
#define TYPE_TRUE       "true"
#define TYPE_FALSE      "false"
#define STRCOPY			"strcopy"
#define MAIN			"main"
#define LENGHT			"mystrlen"


#define ISTYPE(str) ( !strcmp(str, TYPE_NUMBER) || !strcmp(str, TYPE_STRING) ||!strcmp(str, TYPE_SMBL) || !strcmp(str, TYPE_TBOOL))

namespace Lexer
{
	struct LEX
	{
		LT::LexTable lextable;
		IT::IdTable	idtable;
		LEX() {}
	};
	struct Graph
	{
		char lexema;
		FST::FST graph;
	};


	IT::Entry* getEntry		// ��������� � ���������� ������ ��
	(
		Lexer::LEX& tables,				// �� + ��
		char lex,						// �������
		char* id,						// �������������
		char* idtype,					// ���������� (���)
		bool isParam,					// ������� ��������� �������
		bool isFunc,					// ������� �������
		Log::LOG log,					// ��������
		int line,						// ������ � �������� ������
		bool& rc_err					// ���� ������(�� ������)
	);

	struct ERROR_S			// ��� ���������� ��� throw ERROR_THROW | ERROR_THROW_IN
	{
		int id;
		char message[ERROR_MAXSIZE_MESSAGE];	// ���������			
		struct
		{
			short line = -1;			//����� ������ (0, 1, 2, ...)
			short col = -1;				//����� ������� � ������(0, 1, 2, ...)
		} position;
	};
	bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);
	int	getIndexInLT(LT::LexTable& lextable, int itTableIndex);			// ������ ������ ������� � ������� ������
};

