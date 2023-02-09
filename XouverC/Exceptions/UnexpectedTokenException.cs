using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using XouverC.Lexing;

namespace XouverC.Exceptions {
    internal class UnexpectedTokenException : XException {
        public UnexpectedTokenException(Token token) :
            base((string) ("Unexpected token \'" + token.value + "\' on line " + token.line)) { }
    }
}
