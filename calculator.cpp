/*
 	 Calculator.cpp 
   ������, ���������� �� ���������� ���������.
   ������ ��������� ������� � ������� evaluate_expression.
   ��� ������������� ���������� � ���������� � ������� float.
 
   ������ ��������:
      � ������ ������������� ������ ������������� ���������� ���� 
    const char *
*/ 

#include "stdafx.h"
#include <math.h>
#include "calculator.h"
#include "name_table.h"

// ������ ���������� - �.�. ���������� ���������� �������� ���������
float prim(Parser& parser) {
  parser.get_lexem();

  switch (parser.get_last().type) {
    case LT_Number: 
      {
        float v = parser.get_last().value;
        parser.get_lexem();
        return v; 
      }
    case LT_Identifier:
      {
        std::string name(parser.get_last().name);
        parser.get_lexem();
        return NT.GetVariable(name); 
      }
      break;
    case LT_Delimiter:
      switch(parser.get_last().delimiter) {
    case '-':
      return -prim(parser);
    case '(': 
      {
        float e=evaluate_expression(parser);
        if (parser.get_last().type == LT_Delimiter && parser.get_last().delimiter == ')' ) {
          parser.get_lexem();
          return e;
        } else
          throw "\')\' expected";
      }
    default:
      throw "primary expected";
      }
    default:
      throw "primary expected";
  }
}

// ������������ ���������� � �������
float powterm(Parser& parser)
{
    float left = prim(parser);

    for (;;)
    {
        if (parser.get_last().type == LT_Delimiter)
        {
            switch (parser.get_last().delimiter)
            {
            case '^':
            {
                float a = powterm(parser);
                if (a != 0)
                {                  
                        left = pow(left, a);         
                }
                else
                {
                    throw "Error: Uncertainty";
                }
                break;
            }
            default:
                return left;
            }
        }
        else
            return left;
    }
    return left;
}

// ������������ ��������� � �������
float term(Parser& parser) {
	float left = powterm(parser);
	for (;;) {
		if (parser.get_last().type == LT_Delimiter) {
			switch (parser.get_last().delimiter)
			{
			case '*':
				left *= powterm(parser);
				break;
			case '/':
			{ const float precision = 1.0e-5f;
			float d = powterm(parser);
			if (fabs(d) > precision) {
				left /= d;
			}
			else {
				throw "Divide by zero";
			}
			}
			break;
			default:
				return left;
			}
		}
		else
			return left;
	}
	return left;
}

// ������������ �������� � ���������
float plus_minus(Parser& parser) {
  float left = term(parser);

  for (;;) {
    if (parser.get_last().type == LT_Delimiter) {
      switch (parser.get_last().delimiter)
      {
      case '+':
        left+=term(parser);
        break;
      case '-':
        left-=term(parser);
        break;
      default:
        return left;
      }
    } else
      return left;
  }
  return left;
}


// ��������� ��������� - ���������� ���������
float evaluate_expression(Parser& parser) {
  float left = plus_minus(parser);

  for (;;) {
    if (parser.get_last().type == LT_Delimiter) {
      switch (parser.get_last().delimiter)
      {
      case '<':
        left = left < plus_minus(parser);
        break;
      case '>':
        left = left > plus_minus(parser);
        break;
      case '=':
        left = left == plus_minus(parser);
        break;
      default:
        return left;
      }
    } else
      return left;
  }
  return left;
}

