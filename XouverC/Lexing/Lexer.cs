using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Runtime.Intrinsics.X86;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Lexing {
    public class Lexer {
        private const string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        private const string DIGITS = "0123456789";

        private List<Token> tokens;
        private string code;

        private int ptr;
        private char current;

        private int line;

        public Lexer(string code) {
            this.code = code;
            ptr = 0;
            line = 1;
            tokens = new List<Token>();
            current = code[0];
        }

        public Token[] Lex() {
            tokens = new List<Token>();

            while (ptr < code.Length) {
                if ((LETTERS + '_').Contains(current)) {
                    tokens.Add(new Token(TokenType.Identifier, MakeIdentifier(), line));
                    continue;
                }
                else if (DIGITS.Contains(current)) {
                    tokens.Add(MakeNumber());
                    continue;
                }
                else if (current == '"')
                    tokens.Add(new Token(TokenType.String, MakeString(), line));
                else if (current == '\'')
                    tokens.Add(new Token(TokenType.Char, MakeChar(), line));
                else if (current == '(')
                    tokens.Add(new Token(TokenType.L_Paren, '(', line));
                else if (current == ')')
                    tokens.Add(new Token(TokenType.R_Paren, ')', line));
                else if (current == '[')
                    tokens.Add(new Token(TokenType.L_Brack, '[', line));
                else if (current == ']')
                    tokens.Add(new Token(TokenType.R_Brack, ']', line));
                else if (current == '{')
                    tokens.Add(new Token(TokenType.L_Brace, '{', line));
                else if (current == '}')
                    tokens.Add(new Token(TokenType.R_Brace, '}', line));
                else if (current == '+')
                    switch (Advance()) {
                        case '=':
                            tokens.Add(new Token(TokenType.PlusEq, "+=", line));
                            break;
                        case '+':
                            tokens.Add(new Token(TokenType.Increase, "++", line));
                            break;
                        default:
                            tokens.Add(new Token(TokenType.Plus, "+", line));
                            continue;
                    }
                else if (current == '-')
                    switch (Advance()) {
                        case '=':
                            tokens.Add(new Token(TokenType.MinusEq, "-=", line));
                            break;
                        case '+':
                            tokens.Add(new Token(TokenType.Decrease, "--", line));
                            break;
                        default:
                            tokens.Add(new Token(TokenType.Minus, "-", line));
                            continue;
                    }
                else if (current == '*')
                    if (Advance() == '=')
                        tokens.Add(new Token(TokenType.MulEq, "*=", line));
                    else {
                        tokens.Add(new Token(TokenType.Mul, "*", line));
                        continue;
                    }
                else if (current == '/') {
                    char next = Advance();
                    if (next == '=')
                        tokens.Add(new Token(TokenType.DivEq, "/=", line));
                    else if (next == '/') {
                        while (current != '\n') Advance();
                        continue;
                    }
                    else if (next == '*') {
                        while (true) {
                            Advance();

                            if (current == '*') {
                                Advance();

                                if (current == '/') break;
                            }

                            if (current == '\0') throw new Exception();
                        }
                    }
                    else {
                        tokens.Add(new Token(TokenType.Div, '/', line));
                        continue;
                    }
                }
                else if (current == '<')
                    if (Advance() == '=')
                        tokens.Add(new Token(TokenType.LessEq, "<=", line));
                    else {
                        tokens.Add(new Token(TokenType.Less, "<", line));
                        continue;
                    }
                else if (current == '>')
                    if (Advance() == '=')
                        tokens.Add(new Token(TokenType.GreaterEq, ">=", line));
                    else {
                        tokens.Add(new Token(TokenType.Greater, ">", line));
                        continue;
                    }
                else if (current == '=')
                    if (Advance() == '=')
                        tokens.Add(new Token(TokenType.CmpEquals, "==", line));
                    else {
                        tokens.Add(new Token(TokenType.Equals, "=", line));
                        continue;
                    }
                else if (current == ';')
                    tokens.Add(new Token(TokenType.Semicolon, ';', line));
                else if (current == '.')
                    tokens.Add(new Token(TokenType.Period, '.', line));
                else if (current == ',')
                    tokens.Add(new Token(TokenType.Comma, ',', line));
                else if (current == '\n')
                    line++;
                else if ((" \t\r\v").Contains(current)) { }
                else if (current == '\0') {
                    tokens.Add(new Token(TokenType.Eof, "Eof", line));
                    break;
                }
                else throw new Exception("Unexpected character '" + current + "' on line " + line);

                Advance();
            }
            return tokens.ToArray();
        }

        private string MakeIdentifier() {
            string ident = "";
            
            while ((LETTERS + DIGITS + "_").Contains(current)) {
                ident += current;
                Advance();
            }

            return ident;
        }

        private Token MakeNumber() {
            string numStr = "";
            bool hasDot = false;

            while ((DIGITS + ".").Contains(current)) {
                if (current == '.') {
                    if (!hasDot)
                        hasDot = true;
                    else throw new Exception("Unexpected: '.' on line " + line);
                }

                numStr += current;
                Advance();
            }

            if (hasDot) return new Token(TokenType.Float, float.Parse(numStr, CultureInfo.InvariantCulture.NumberFormat), line);
            else return new Token(TokenType.Int, int.Parse(numStr), line);
        }

        private string MakeString() {
            string str = "";

            Advance();
            while (current != '"') {
                if (current == '\n') throw new Exception("Exception on line " + line);

                if (current == '\\')
                    current = ResolveEscape();
                str += current;
                Advance();
            }

            return str;
        }

        private char MakeChar() {
            string str = "";

            Advance();
            while (current != '"') {
                if (current == '\n') throw new Exception("Exception on line " + line);

                if (current == '\\')
                    current = ResolveEscape();

                str += current;
                Advance();
            }

            if (str.Length > 1) throw new Exception("Exception on line " + line);

            return str[0];
        }

        private char ResolveEscape() {
            switch (current) {
                case 'n':
                    return '\n';
                case 'r':
                    return '\r';
                case 'v':
                    return '\v';
                case 't':
                    return '\t';
                case 'a':
                    return '\a';
                case 'f':
                    return '\f';
                case 'b':
                    return '\b';
                default:
                    return current;
            }
        }

        private char Advance() {
            current = code[++ptr];
            return current;
        }
    }
}
