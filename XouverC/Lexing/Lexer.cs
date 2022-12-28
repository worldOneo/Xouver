using System;
using System.Collections.Generic;
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
            ptr = -1;
        }

        public Token[] Lex() {
            tokens = new List<Token>();

            while (ptr < code.Length) {
                if ((LETTERS + '_').Contains(current)) {
                    tokens.Add(new Token(TokenType.Identifier, MakeIdentifier(), line));
                }
                else if (DIGITS.Contains(current)) {
                    dynamic num = MakeNumber();

                    TokenType type = num is int ? TokenType.Int : TokenType.Float;
                    tokens.Add(new Token(type, num, line));
                }
                else if (current == '"') {
                    tokens.Add(new Token(TokenType.String, MakeString(), line));
                }
                else if (current == '\'') {
                    tokens.Add(new Token(TokenType.Char, MakeChar(), line));
                }
                else if (current == '(') {
                    tokens.Add(new Token(TokenType.L_Paren, null, line));
                    Advance();
                }
                else if (current == ')') {
                    tokens.Add(new Token(TokenType.R_Paren, null, line));
                    Advance();
                }
                else if (current == '[') {
                    tokens.Add(new Token(TokenType.L_Brack, null, line));
                    Advance();
                }
                else if (current == ']') {
                    tokens.Add(new Token(TokenType.R_Brack, null, line));
                    Advance();
                }
                else if (current == '{') {
                    tokens.Add(new Token(TokenType.L_Brace, null, line));
                    Advance();
                }
                else if (current == '}') {
                    tokens.Add(new Token(TokenType.R_Brace, null, line));
                    Advance();
                }
                else if (current == '+') {
                    tokens.Add(new Token(TokenType.Plus, null, line));
                    Advance();
                }
                else if (current == '-') {
                    tokens.Add(new Token(TokenType.Minus, null, line));
                    Advance();
                }
                else if (current == '*') {
                    tokens.Add(new Token(TokenType.Mul, null, line));
                    Advance();
                }
                else if (current == '/') {
                    tokens.Add(new Token(TokenType.Div, null, line));
                    Advance();
                }
                else if (current == '<') {
                    tokens.Add(new Token(TokenType.Less, null, line));
                    Advance();
                }
                else if (current == '>') {
                    tokens.Add(new Token(TokenType.Greater, null, line));
                    Advance();
                }
                else if (current == '=') {
                    tokens.Add(new Token(TokenType.Equals, null, line));
                    Advance();
                }
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

        private dynamic MakeNumber() {
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

            if (hasDot) return float.Parse(numStr);
            else return int.Parse(numStr);
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

        private void Advance() {
            current = code[++ptr];
        }
    }
}
