/*
  lexical_analizer.cpp 
  ����������� ����������.
  ������ �������� �� ��������� ������ �� �������� ������.
*/ 

#include "stdafx.h"
#include "lexical_analizer.h"

Parser::Parser(std::istream& in)
: in(in), line_number(1) {
  last.type=LT_Unknown;
  in >> std::noskipws;//noskipws - ��������� ������� ������ �����������
}

// ���������� ���������� ��������� �������
// ���� �� ���� ������� ������� ������� - �������� get_token
// ��� ������ �������

//������� ����������� ���������� �������
Lexem Parser::get_last()  {
  if (last.type==LT_Unknown)
    return get_lexem();
  return last;
}

// ������ ������� �� ������
Lexem Parser::get_lexem() 
{
  char c;
  in >> c; //��������� ������ ������
  while ( c == ' ' || c=='\t' ) { //���� ��������� ��� ������, �� ���������� �� � in
    in >> c;
    if (!in) //���� �� ������ (��� ��� ������), �� ����� "����������" �� ���� ��� ���� � ������������������
      break;
  }
  
  //����� ������
  if ( c == '\n' ) {
    last.type=LT_EOL;
    ++line_number; //���������� � �������� ����� 1
    // ���������� ������ ������� �����
    //std::cout << "[" << line_number << "]";
    return last;
  }

  if (!in) { // ����� ������
    last.type=LT_End;
    return last;
  }
  
  // ������ � ��������
  if (c == '\"') {
    last.name="";
    in >> c;
    while (c != '\"') {
      last.name+=c;
      in >> c;
      if (c == '\n' || !in)
        throw "unpaired quotation marks";
    }
    last.type=LT_String;
    return last;
  }

  // �����������
  const std::string delimiters("+-*^/();=<>");
  std::string::size_type pos=delimiters.find(c);//���� ����������� ������ � ���������� ������, �� ���������� ��� �������

  if (pos != std::string::npos) { // npos - ��� �������������� ������� � string
    last.type=LT_Delimiter; 
    last.delimiter=c;
    return last;
  }

  // �������� ��������
  if ( isdigit(c) || c=='.' ) { //�������� �� �� ������ ��� ��� (� ������ ������� �����)
    in.putback(c);
    in >> last.value;
    last.type = LT_Number;
    return last;
  }

  // �������������
  if ( isalpha(c) ) { // �������� �� �� �������� ��� ��������� ������ ��������.
    last.name="";
    while (isalpha(c)) {
      last.name+=c;
      in >> c;
    }
    in.putback(c);
    last.type = LT_Identifier;
    return last;
  }

  throw "Unexpected lexem";
}

//������� ��������� ������� ���������
Parser::Holder Parser::Hold() const
{
  Holder holder;
  holder.pos = in.tellg(); //� ������� ������� tellg() � tellp() �������������� ����� ���������� ������� ������� ���������� ����� � ������
  holder.line_number = line_number; //���������� ����� �������� ������
  return holder;
}

//������� ��������� ������� ��������� 
void Parser::Fetch(Holder holder)
{
  in.seekg(holder.pos); /*������� seekg() ������������ � �������� �����. 
                        ��� ������� ��������� "get" ��� �������� ������ �� offset ������,
                        ��� ����������� ��������� "get" � ������� position.*/
  line_number = holder.line_number;
}

//������� ����������� ��������� � ������
void Parser::Reset(){
  in.clear();
  in.seekg(0);
}

//���������� �� �� ����� ������
bool Parser::SkipUntilEOL() { 
  while (get_lexem().type != LT_EOL) {
    if (get_lexem().type == LT_End  )
      return false;
  }
  get_lexem();//�������� ��������� ��������� �������
  return true;
}
