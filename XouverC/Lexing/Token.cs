using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Lexing {
    public struct Token {
        TokenType type;
        dynamic value;
        int line;

        public Token(TokenType type, dynamic value, int line) {
            this.type = type;
            this.value = value;
            this.line = line;
        }
    }
}
