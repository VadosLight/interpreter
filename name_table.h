/*
  name_table.h
  Интерфейс таблицы имён интерпретатора
*/ 

#pragma once

#include "interface.h"



class NameTable {
    //Хранит типы комманд
  std::map<std::string, Command *> commands;
  //Хранит объявленные переменные и их значения
  std::map<std::string, float>     variables;
public:
  ~NameTable();
  // Регистрация команды языка в таблице имен
  void RegisterCommand(std::string const& name, Command *);
  // Выполнить команду, если это возможно
  bool ProcessCommand(Parser&);
  //Устанавливает значение переменной по имени 
  void  SetVariable(std::string const& name, float value); 
  //Возвращает значение переменной по имени
  float GetVariable(std::string const& name);
};

extern NameTable NT;