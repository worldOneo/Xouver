using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using XouverC.Parsing;

namespace XouverC.Compiling {
    internal class FunctionInfo {
        public string signature;
        public string name;
        public string[] argTypes;
        public string[] argNames;
        public int pointer;
        public string[] modifiers;
        public ASTExpr[] exprs;
        public string type;
    }
}
