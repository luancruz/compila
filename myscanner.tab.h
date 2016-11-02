/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_MYSCANNER_TAB_H_INCLUDED
# define YY_YY_MYSCANNER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_NUMBER = 258,
    T_NAME = 259,
    caracterInvalido = 260,
    T_AND = 261,
    T_NOT = 262,
    T_OR = 263,
    T_ELSEIF = 264,
    T_WHILE = 265,
    T_DO = 266,
    T_FUNCTION = 267,
    T_END = 268,
    T_FOR = 269,
    T_ELSE = 270,
    T_IF = 271,
    T_THEN = 272,
    T_RETURN = 273,
    T_LOCAL = 274,
    T_NIL = 275,
    T_PLUS = 276,
    T_MINUS = 277,
    T_TIMES = 278,
    T_DIV = 279,
    T_COMMA = 280,
    T_OPENPAR = 281,
    T_CLOSEPAR = 282,
    T_SEMICOL = 283,
    T_ASSIGN = 284,
    T_EQ = 285,
    T_NEQ = 286,
    T_LTEQ = 287,
    T_GTEQ = 288,
    T_LT = 289,
    T_GT = 290,
    T_PRINT = 291,
    FOR = 292,
    FUNCTION = 293,
    LOCAL = 294,
    MENOS = 295,
    CALL = 296
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 155 "myscanner.y" /* yacc.c:1909  */

	int valor;
    	char* variavel;
	noTipo *PointerTipoNo;

#line 102 "myscanner.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_MYSCANNER_TAB_H_INCLUDED  */
