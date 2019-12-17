/*
  executor.h
  Интерфейс виртуальной машины
*/ 

#pragma once
#include <iosfwd>

void execute_script(std::istream& in);