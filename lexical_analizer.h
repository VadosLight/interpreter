/*
  lexical_analizer.h
  Интерфейс лексического анализатора
*/ 

#pragma once

// Типы лексем, используемых калькулятором
enum  LexemType {
  LT_Unknown,//нераспознанная лексема
  LT_Number, //Число
  LT_End, //Конец файла
  LT_Delimiter, //разделители +-*/();=<>
  LT_Identifier, //Имя
  LT_Label, //предположительно, это для goto
  LT_String, //строка внутри кавычек
  LT_EOL, // Конец строки
};

// Структура лексемы

//  Лексема - это последовательность допустимых символов языка программирования, имеющая смысл для интерпретатора.
struct Lexem {
  LexemType   type;         // тип
  float       value;        // значение    (если тип TT_Number)
  char        delimiter;    // разделитель (если тип TT_Delimiter)
  std::string name;         // имя         (если тип TT_Identifier)
};

// Парсер входного текста, разделяющий его на лексемы
class Parser {
  // Есть опасность копирования содержимого класса конструктором
  // копии или оператором присваивания, сгенерированными по умолчанию
  // при неправильном использовании объектов класса Parser.
  // Объявляя их в закрытой части класса Parser я подавляю возможность
  // автоматической генерации таких методов.
  Parser(const Parser&);
  Parser& operator = (const Parser&);

public:
  class Holder {
    std::istream::pos_type  pos;
    unsigned int            line_number;
    friend class Parser;
  };

  std::istream& in;
  Lexem         last;
  int           line_number;


public:  
  Parser(std::istream& str);
  Lexem get_lexem();            // считать следующую лексему
  Lexem get_last();             // вернуть предыдущую лексему

  bool  SkipUntilEOL();

  Holder  Hold() const;
  void    Fetch(Holder);
  void    Reset();
};

