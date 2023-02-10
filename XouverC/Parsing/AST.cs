using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Metadata.Ecma335;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Parsing {
    public class ASTExpr {
        public int line;
    }

    public class ASTValue<T> : ASTExpr {
        public T value;

        public override string ToString() {
            return typeof(T).Name + "(" + value + ")";
        }
    }

    public class ASTFloat : ASTValue<float> { }
    public class ASTInt : ASTValue<int> { }
    public class ASTString : ASTValue<string> { }
    public class ASTChar : ASTValue<char> { }
    public class ASTBool : ASTValue<bool> { }
    public class ASTIdentifier : ASTValue<string> {
        public override string ToString() {
            return "Identifier(" + value + ")";
        }
    }

    public class ASTAccess : ASTExpr {
        public ASTExpr left;
        public ASTExpr right;

        public override string ToString() {
            return left.ToString() + "." + right.ToString();
        }
    }

    public class ASTImport : ASTExpr {
        public string import;
    }

    public class ASTBinaryOperator : ASTExpr {
        public char value;
    }

    public class ASTUnaryExpr : ASTExpr {
        public ASTExpr val;
        public string op;

        public override string ToString() {
            return "UnaryExpr( " + op + val.ToString() + " )";
        }
    }

    public class ASTBinaryExpr : ASTExpr {
        public ASTExpr lval;
        public string op;
        public ASTExpr rval;

        public override string ToString() {
            return "BinaryExpr( " + lval.ToString() + ", BinaryOp(" + op + "), " + rval.ToString() + " )";
        }
    }

    public class ASTBlock : ASTExpr {
        public ASTExpr[] exprs;
    }

    public class ASTDef : ASTExpr {
        public string[] modifiers;
        public string name;
        public string type;
    }

    public class ASTReturn : ASTExpr { }

    public class ASTFieldDef : ASTDef { }

    public class ASTVarDef : ASTExpr {
        public string type;
        public string name;
    }

    public class ASTVarDec : ASTVarDef {
        public ASTExpr value;

        public override string ToString() {
            return type + " " + name + " = " + value.ToString();
        }
    }

    public class ASTAssign : ASTExpr {
        public string identifier;
        public string op;
        public ASTExpr value;
    }

    public class ASTFuncDef : ASTDef {
        public int argCount;
        public string[] argTypes;
        public string[] argNames;

        public ASTExpr[] exprs;

        public override string ToString() {
            string str = name + "():\n";
            
            foreach (ASTExpr e in exprs) {
                str += e.ToString() + "\n";
            }

            str.Remove(str.Length - 1);

            return str;
        }
    }

    public class ASTConstructor : ASTExpr {
        public int argCount;
        public string[] argTypes;
        public string[] argNames;
        public string[] modifiers;

        public ASTExpr[] exprs;
    }

    public class ASTNewExpr : ASTExpr {
        public string name;
    }

    public class ASTFuncCall : ASTExpr {
        public ASTExpr[] args;
        public string name;
    }

    public class ASTCondition : ASTExpr { }

    public class ASTComparison : ASTCondition {
        public ASTExpr lval;
        public string op;
        public ASTExpr rval;

        public override string ToString() {
            return lval.ToString() + " " + op + " " + rval;
        }
    }

    public class ASTIf : ASTExpr {
        public ASTExpr condition;
        public ASTExpr expr;
        public ASTExpr elseExpr;
        public bool not;
    }

    public class ASTWhile : ASTExpr {
        public ASTExpr condition;
        public ASTExpr expr;
        public bool not;
    }

    public class ASTIn : ASTExpr {
        public ASTExpr value;
    }

    public class ASTFor : ASTExpr {
        public ASTExpr def;
        public ASTExpr count;
        public ASTExpr expr;
    }
}
