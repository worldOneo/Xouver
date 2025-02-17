﻿using System;
using System.Collections.Generic;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Lexing {
    public enum TokenType {
        Int, Float,
        Char, String,
        Identifier,
        Plus, Minus, Div, Mul, Not,
        Increase, Decrease,
        CmpEquals, NotEq, Greater, Less, GreaterEq, LessEq,
        Equals, PlusEq, MinusEq, DivEq, MulEq,
        L_Arrow, R_Arrow,
        L_Brack, R_Brack,
        L_Paren, R_Paren,
        L_Brace, R_Brace,
        Semicolon, Comma, Period,
        Eof
    }
}
