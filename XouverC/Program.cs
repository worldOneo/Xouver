using System;

using XouverC.Lexing;
using XouverC.Parsing;
using XouverC.Compiling;
using System.Xml.Linq;

using XouverC.Exceptions;
using System.Text;

namespace XouverC {
    internal class Program {
        private static string[] BUILD_OPTIONS = new string[] { "-outName", "-outDir", "-classPath" };
        
        public static void Main(string[] args) {
            if (args.Length > 0) {
                string outDir = Directory.GetCurrentDirectory();
                string outName = "";
                string firstFileName = "";
                List<Compiler> compilers = new();

                for (int i = 0; i < args.Length; i++) {
                    if (args[i] == "-outdir") {
                        outDir = args[++i];
                        i++;
                    }
                    else if (args[i] == "-outname") {
                        outName = args[++i];
                        i++;
                    }
                    else {
                        string fname = args[i];
                        if (firstFileName == "")
                            firstFileName = Path.GetFileNameWithoutExtension(fname);

                        string text = File.ReadAllText(Path.GetFullPath(fname)) + '\0';
                        Lexer lexer = new Lexer(text);
                        Token[] tokens = lexer.Lex();

                        Parser parser = new Parser(tokens);
                        ASTExpr[] ast = parser.Parse();
                        compilers.Add(new Compiler("", Path.GetFileNameWithoutExtension(fname), Path.GetFullPath(fname), outDir, "", ast));
                    }
                }

                if (outName == "") outName = firstFileName;
                List<byte> classMap = new List<byte>();

                classMap.AddRange(GetBytes(compilers.Count));
                foreach (Compiler cmp in compilers) {
                    classMap.AddRange(cmp.Compile());
                }

                FileStream fs = File.Create(outDir + '\\' + outName + ".xvr");
                fs.Write(classMap.ToArray());
                fs.Close();
            }
            
            //try {
            //}
            /*catch (XException e) {
                ConsoleColor col = Console.ForegroundColor;
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine("Error in class \'" + fileName + "\':");
                Console.WriteLine(e.Message);
                Console.ForegroundColor = col;
            }*/
        }

        private static void PrintHelp() {
            Console.WriteLine("Usage: xouverc [options] <sourceFiles>");
            Console.WriteLine("Where options includes:");
            Console.WriteLine("-outdir <path>                  -- Specifies the directory to write the compiled files into. ");
        }

        private static byte[] GetBytes(int i) {
            byte[] bytes = BitConverter.GetBytes(i);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            return bytes;
        }
    }
}