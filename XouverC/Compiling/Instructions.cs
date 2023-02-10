using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace XouverC.Compiling {
    internal static class Instructions {
        public static byte Add = 1;
        public static byte Sub = 2;
        public static byte Mul = 3;
        public static byte Div = 4;

        public static byte Jmp = 5;

        public static byte IfEq = 6;
        public static byte IfGt = 7;
        public static byte IfGq = 9;
        public static byte Not = 11;

        public static byte JIf = 12; 

        public static byte Call = 13;
        public static byte Return = 14;
        public static byte CLoad = 17;
        public static byte Load = 18;
        public static byte Store = 19;
        public static byte OLoad = 21;
        public static byte OStore = 22;

        public static byte Invoke = 23;
    }
}
