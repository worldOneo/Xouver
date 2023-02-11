using System;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Net.Http.Headers;
using System.Runtime.Intrinsics.Arm;
using System.Security.AccessControl;
using System.Text.RegularExpressions;
using System.Transactions;
using XouverC.Exceptions;
using XouverC.Lexing;
using XouverC.Parsing;

namespace XouverC.Parsing {
    public class Parser {
        private Token[] tokens;
        private Token current;
        private int pointer;

        private TokenType[] binaryOps = { TokenType.Plus, TokenType.Minus, TokenType.Mul, TokenType.Div };
        private TokenType[] compOps = { TokenType.CmpEquals, TokenType.Greater, TokenType.Less, TokenType.GreaterEq, TokenType.LessEq };
        private TokenType[] assignOps = { TokenType.Equals, TokenType.PlusEq, TokenType.MinusEq, TokenType.MulEq, TokenType.DivEq };

        private string[] MODIFIERS = { "public", "private", "pub", "priv", "native", "nat" };
        private string[] KEYWORDS = { "define", "def", "function", "func", "public", "pub", "private", "priv" };
        public Parser(Token[] tokens) {
            this.tokens = tokens;
            pointer = 0;
            current = tokens[pointer];
        }

        public ASTExpr[] Parse() {
            List<ASTExpr> expressions = new List<ASTExpr>();

            while (current.type != TokenType.Eof)
                expressions.Add(ParseExpr());
            
            return expressions.ToArray();
        }

        private Token PeekNextToken() {
            return tokens[pointer + 1];
        }

        private Token NextToken() {
            current = tokens[++pointer];
            return current;
        }

        private bool HasNextToken() {
            return pointer < tokens.Length;
        }

        private ASTExpr ParseFuncCall() {
            string name = current.value;
            int line = current.line;
            List<ASTExpr> args = new();

            NextToken();
            NextToken();

            int nline = current.line;
            while (current.type != TokenType.R_Paren) {
                if (current.type == TokenType.Eof)
                    throw new XException("Expected ')' to close '(' on line " + nline);

                args.Add(ParseExpr());

                if (current.type == TokenType.Comma) NextToken();
                else if (current.type == TokenType.R_Paren) break;
                else throw new Exception();
            }

            NextToken();

            ASTFuncCall call = new();
            call.args = args.ToArray();
            call.name = name;
            call.line = line;

            return call;
        }

        private ASTExpr ParseVar() {
            string type = current.value;
            string name = NextToken().value;

            if (NextToken().type != TokenType.Equals) {
                ASTVarDef def = new();

                def.type = type;
                def.name = name;

                return def;
            }
            NextToken();

            ASTVarDec dec = new();
            dec.type = type;
            dec.name = name;
            dec.line = current.line;
            dec.value = ParseExpr();

            return dec;
        }

        private ASTExpr ParseAssign() {
            ASTAssign assign = new();
            assign.identifier = current.value;
            NextToken();
            assign.op = current.value;
            assign.line = current.line;
            NextToken();

            assign.value = ParseExpr();

            return assign;
        }

        private ASTExpr ParseFac() {
            if (current.type == TokenType.Plus || current.type == TokenType.Minus) {
                int line = current.line;
                string op = current.value;
                NextToken();

                ASTExpr factor = ParseFac();


                ASTUnaryExpr uexpr = new();

                uexpr.op = op;
                uexpr.val = factor;
                uexpr.line = line;

                return uexpr;
            }
            else if (current.type == TokenType.Int || current.type == TokenType.Float) {
                if (current.type == TokenType.Float) {
                    ASTFloat f = new();

                    f.value = current.value;
                    f.line = current.line;
                    NextToken();

                    return f;
                }
                else {
                    ASTInt i = new();

                    i.value = current.value;
                    i.line = current.line;
                    NextToken();

                    return i;
                }
            }
            else if (current.type == TokenType.L_Paren) {
                NextToken();

                ASTExpr expr = ParseExpr();

                if (current.type != TokenType.R_Paren) throw new Exception("Expected ')' on line " + current.line + ", got " + current.value);


                NextToken();
                return expr;
            }
            else return ParseIdent();
        }

        private ASTExpr ParseIdent() {
            if (current.type != TokenType.Identifier) throw new UnexpectedTokenException(current);

            if (PeekNextToken().type == TokenType.L_Paren)
                return ParseFuncCall();

            if (PeekNextToken().type == TokenType.Identifier && !KEYWORDS.Contains((string)PeekNextToken().value))
                return ParseVar();

            if (assignOps.Contains(PeekNextToken().type))
                return ParseAssign();

            ASTIdentifier identifier = new();
            identifier.value = current.value;
            identifier.line = current.line;

            NextToken();                                                            

            if (current.type == TokenType.Increase || current.type == TokenType.Decrease) {
                ASTBinaryExpr expr = new();

                expr.lval = identifier;

                ASTInt i = new();
                i.value = 1;

                expr.rval = i;
                expr.op = current.type == TokenType.Increase ? "+" : "-";
                NextToken();

                return expr;
            }


            return identifier;
        }

        private ASTExpr ParseTerm() {
            return ParseBin(ParseFac, new TokenType[] { TokenType.Mul, TokenType.Div });
        }

        private ASTExpr ParseExpr() {
            if (current.type == TokenType.Identifier) {
                if (current.value == "define" || current.value == "def") {
                    int line = current.line;
                    List<string> modifiers = new();
                    NextToken();

                    while (MODIFIERS.Contains((string) current.value)) {
                        modifiers.Add(current.value);
                        NextToken();
                    }

                    if (current.type != TokenType.Identifier) throw new Exception();

                    if (current.value == "var") {
                        ASTFieldDef def = new();
                        def.line = line;
                        NextToken();

                        if (current.type != TokenType.Identifier) throw new Exception();
                        if (KEYWORDS.Contains((string)current.value)) throw new UnexpectedTokenException(current);
                        def.type = current.value;
                        NextToken();

                        if (current.type != TokenType.Identifier) throw new Exception();
                        if (KEYWORDS.Contains((string)current.value)) throw new UnexpectedTokenException(current);
                        def.name = current.value;
                        NextToken();

                        return def;
                    }
                    else if (current.value == "function" || current.value == "func") {
                        ASTFuncDef def = new();
                        def.line = line;
                        NextToken();

                        if (current.type != TokenType.Identifier) throw new Exception();
                        def.type = current.value;
                        if (KEYWORDS.Contains((string)current.value)) throw new UnexpectedTokenException(current);
                        NextToken();

                        if (current.type != TokenType.Identifier) throw new Exception();
                        def.name = current.value;
                        if (KEYWORDS.Contains((string)current.value)) throw new UnexpectedTokenException(current);
                        NextToken();

                        if (current.type != TokenType.L_Paren) throw new Exception();

                        List<string> argTypes = new();
                        List<string> argNames = new();

                        int nline = 0;
                        if (PeekNextToken().type != TokenType.R_Paren) {
                            do {
                                if (current.type == TokenType.Eof)
                                    throw new XException("Expected ')' to close '(' on line " + nline);

                                NextToken();
                                if (current.type != TokenType.Identifier) throw new Exception();
                                if (KEYWORDS.Contains((string)current.value)) throw new UnexpectedTokenException(current);
                                argTypes.Add(current.value);
                                NextToken();

                                if (current.type != TokenType.Identifier) throw new Exception();
                                if (KEYWORDS.Contains((string)current.value)) throw new UnexpectedTokenException(current);
                                argNames.Add(current.value);
                                NextToken();
                            } while (current.type == TokenType.Comma && current.type != TokenType.R_Paren);
                        }
                        else NextToken();
                        def.argCount = argTypes.Count;
                        def.argTypes = argTypes.ToArray();
                        def.argNames = argNames.ToArray();

                        NextToken();

                        if (!modifiers.Contains("native") || !modifiers.Contains("nat")) {
                            List<ASTExpr> exprs = new();
                            exprs.Add(ParseExpr());
                            exprs.Add(new ASTReturn());
                            def.isNative = false;
                            def.exprs = exprs.ToArray();
                        }
                        else def.isNative = true;

                        Console.WriteLine(def.ToString() + "\n");
                        return def;
                    }
                }
                else if (current.value == "import") {
                    NextToken();

                    ASTImport imp = new();
                    imp.import = "";
                    while (current.type == TokenType.Identifier && Array.IndexOf(KEYWORDS, current.value) != -1) {
                        imp.import += current.value;
                        NextToken();

                        if (current.type == TokenType.Period && (PeekNextToken().type == TokenType.Identifier && Array.IndexOf(KEYWORDS, NextToken().value) != -1)) {
                            imp.import += '.';
                            NextToken();
                        }
                        else {
                            throw new NotImplementedException();
                        }
                    }
                }
                else if (current.value == "if") {
                    ASTIf ifExpr = new();
                    ifExpr.line = current.line;
                    NextToken();
                    if (current.type == TokenType.Identifier && current.value == "not")
                        ifExpr.not = true;
                    else ifExpr.not = false;

                    ifExpr.condition = ParseExpr();

                    ifExpr.expr = ParseExpr();

                    if (current.type == TokenType.Identifier && current.value == "else") {
                        NextToken();
                        ifExpr.elseExpr = ParseExpr();
                    }

                    return ifExpr;
                }
                else if (current.value == "while") {
                    ASTWhile we = new();
                    we.line = current.line;
                    NextToken();
                    if (current.type == TokenType.Identifier && current.value == "not")
                        we.not = true;
                    else we.not = false;

                    we.condition = ParseExpr();
                    we.expr = ParseExpr();

                    return we;
                }
                else if (current.value == "for") {
                    NextToken();

                    if (current.type != TokenType.Identifier) throw new UnexpectedTokenException(current);
                    ASTExpr def = ParseVar();

                    if (!(def is ASTVarDef)) throw new Exception();

                    if (current.type != TokenType.Identifier && current.value != "in") throw new UnexpectedTokenException(current);
                    ASTExpr count = ParseExpr();

                    ASTFor fExpr = new();

                    fExpr.def = def;
                    fExpr.count = count;
                    fExpr.expr = ParseExpr();

                    return fExpr;
                }
                else if (current.value == "in") {
                    int line = current.line;
                    NextToken();

                    return new ASTIn() {
                        line = line,
                        value = ParseExpr()
                    };
                }
                else if (current.value == "return") {
                    ASTReturn ret = new();
                    ret.line = current.line;

                    return ret;
                }
                else if (current.value == "new") {
                    NextToken();

                    if (current.type != TokenType.Identifier) throw new NotImplementedException();
                    
                    ASTNewExpr newExpr = new();
                    newExpr.name = current.value;
                    NextToken();

                    return newExpr;
                }
            }
            else if (current.type == TokenType.L_Brace) {
                int line = current.line;
                NextToken();
                ASTBlock block = new();

                List<ASTExpr> exprs = new();

                //if (current.type == TokenType.R_Brace) {
                    while (current.type != TokenType.R_Brace) {
                        if (current.type == TokenType.Eof)
                            throw new XException("Expected '}' to close '{' on line " + line);
                        exprs.Add(ParseExpr());
                    }
                //}
                //else NextToken();
                NextToken();


                block.exprs = exprs.ToArray();
                return block;
            }
            return ParseCom();
        }

        private ASTExpr ParseBin(Func<ASTExpr> func, TokenType[] ops) {
            ASTExpr left = func();

            while (ops.Contains(current.type)) {
                Token opTok = current;
                string op = current.value;
                NextToken();

                ASTExpr right = func();
                ASTBinaryExpr expr = new();

                expr.lval = left;
                expr.op = op;
                expr.rval = right;

                left = expr;
            }

            return left;
        }

        private ASTExpr ParseAccess() {
            ASTExpr left = ParseBin(ParseTerm, new TokenType[] { TokenType.Plus, TokenType.Minus });

            if (current.type == TokenType.Period) {
                NextToken();
                int line = current.line;

                ASTExpr right = ParseBin(ParseTerm, new TokenType[] { TokenType.Plus, TokenType.Minus });
                ASTAccess acc = new();
                acc.left = left;
                acc.right = right;
                acc.line = line;
                left = acc;
            }

            return left;
        }

        private ASTExpr ParseCom() {
            ASTExpr left = ParseAccess();

            if (compOps.Contains(current.type)) {
                int line = current.line;
                string op = current.value;
                NextToken();

                ASTExpr right = ParseAccess();
                ASTComparison comp = new();

                comp.lval = left;
                comp.op = op;
                comp.rval = right;

                comp.line = line;

                left = comp;
            }

            return left;
        }
    }
}