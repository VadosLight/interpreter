/*
  interface.h
  Базовый абстрактный класс команд языка
*/ 

#pragma once
#include <iosfwd>
#include <map>
#include "lexical_analizer.h"

class Command {
public: 
    virtual ~Command() {};
    virtual bool Process(Parser&) = 0;
};
