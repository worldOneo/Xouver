using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Exceptions {
    internal class XException : Exception {
        public XException(string? message) : base(message) { }
    }
}
