/*
  lexical_analizer.cpp 
  Лексический анализатор.
  Модуль отвечает за выделение лексем из входного текста.
*/ 

#include "stdafx.h"
#include "lexical_analizer.h"

Parser::Parser(std::istream& in)
: in(in), line_number(1) {
  last.type=LT_Unknown;
  in >> std::noskipws;//noskipws - отключает пропуск пустых пространств
}

// Возвращает предыдущую считанную лексему
// Если не было считано никакой лексемы - вызывает get_token
// для чтения лексемы

//Функция возвращения предыдущей лексемы
Lexem Parser::get_last()  {
  if (last.type==LT_Unknown)
    return get_lexem();
  return last;
}

// Чтение лексемы из потока
Lexem Parser::get_lexem() 
{
  char c;
  in >> c; //Считывает символ потока
  while ( c == ' ' || c=='\t' ) { //если табуляция или пробел, то записываем их в in
    in >> c;
    if (!in) //если ин пустой (таб или пробел), то будем "пропускать" их пока они есть в последовательности
      break;
  }
  
  //Конец строки
  if ( c == '\n' ) {
    last.type=LT_EOL;
    ++line_number; //прибавляем к счетчику строк 1
    // Отладочная печать номеров строк
    //std::cout << "[" << line_number << "]";
    return last;
  }

  if (!in) { // Конец потока
    last.type=LT_End;
    return last;
  }
  
  // Строка в кавычках
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

  // Разделители
  const std::string delimiters("+-*^/();=<>");
  std::string::size_type pos=delimiters.find(c);//Если разделитель найден в предыдущей строке, то возвращаем его позицию

  if (pos != std::string::npos) { // npos - это несуществующая позиция в string
    last.type=LT_Delimiter; 
    last.delimiter=c;
    return last;
  }

  // Числовое значение
  if ( isdigit(c) || c=='.' ) { //является ли он числом или нет (с учетом дробной части)
    in.putback(c);
    in >> last.value;
    last.type = LT_Number;
    return last;
  }

  // идентификатор
  if ( isalpha(c) ) { // является ли он строчной или прописной буквой алфавита.
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

//Функция получения позиции указателя
Parser::Holder Parser::Hold() const
{
  Holder holder;
  holder.pos = in.tellg(); //с помощью функций tellg() и tellp() соответственно можно определить текущую позицию указателей ввода и вывода
  holder.line_number = line_number; //записываем новое значение строки
  return holder;
}

//Функция установки позиции указателя 
void Parser::Fetch(Holder holder)
{
  in.seekg(holder.pos); /*Функция seekg() используется с потоками ввода. 
                        Она смещает указатель "get" для текущего потока на offset байтов,
                        или располагает указатель "get" в позицию position.*/
  line_number = holder.line_number;
}

//Функция перемещения указателя в начало
void Parser::Reset(){
  in.clear();
  in.seekg(0);
}

//Пропускаем всё до конца строки
bool Parser::SkipUntilEOL() { 
  while (get_lexem().type != LT_EOL) {
    if (get_lexem().type == LT_End  )
      return false;
  }
  get_lexem();//начинаем обработку следующей лексемы
  return true;
}
