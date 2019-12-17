/*
  name_table.cpp 
  Таблица имён интерпретатора.
  Модуль отвечает за хранение имён команд и переменных.
*/ 

#include "stdafx.h"
#include "name_table.h"

NameTable NT;

NameTable::~NameTable()
{
  // Таблица имен "владеет" объектами команд

    //Нельзя менять данные, на которые указывает const_iterator. Компилятор генерит более быстрый код.
  std::map<std::string, Command *>::const_iterator it(commands.begin());
  for ( ; it != commands.end(); ++it )
    delete (it->second);
}

//Регистрация комманд
void NameTable::RegisterCommand(std::string const& name, Command *cmd) {
  commands[name]=cmd;
}
/*Исполнитель комманд
*/
bool NameTable::ProcessCommand(Parser& parser)
{
    //Нельзя менять данные, на которые указывает const_iterator. Компилятор генерит более быстрый код.
  std::map<std::string, Command *>::const_iterator it(commands.begin());
  for ( ; it != commands.end(); ++it ) {
    if (it->second->Process(parser))
      return true;
  }

  return false;
}

/* Создаёт или устанавливает значение переменным. 
    в качестве контейнера используется map для обращения по названию переменной
    */
void  NameTable::SetVariable(std::string const& name, float value)
{
  variables[name]=value;
}
//Возвращает значение переменных по их имени
float NameTable::GetVariable(std::string const& name)
{
  return variables[name];
}
