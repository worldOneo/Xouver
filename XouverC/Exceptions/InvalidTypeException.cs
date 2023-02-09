using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Exceptions {
    internal class InvalidTypeException : XException {
        public InvalidTypeException(string srcType, string destType, int line) :
            base((string)("Cannot cast object of type '" + srcType + "' to '" + destType + "' on line " + line)) { }
    }
}
