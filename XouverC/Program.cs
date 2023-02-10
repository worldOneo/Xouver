using System;

using XouverC.Lexing;
using XouverC.Parsing;
using XouverC.Compiling;
using System.Xml.Linq;

using XouverC.Exceptions;
using System.Text;

namespace XouverC {
    internal class Program {
        private const string VERSION = "1.0";
        private const string LANG_VERSION = "1.0";
        private const string INSTRUCT_SET_VERSION = "1.0";

        public static void Main(string[] args) {
            if (args.Length > 0) {
                if (args[0] == "-version") {
                    Console.WriteLine("Xouver Compiler " + VERSION);
                    Console.WriteLine("Xouver " + LANG_VERSION);
                    Console.WriteLine("Instruction set " + INSTRUCT_SET_VERSION);
                }
                else if (args[0] == "-help" || args[0] == "?") {
                    PrintHelp();
                }
                else {
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
                            fname = Path.GetFileNameWithoutExtension(fname);

                            if (File.Exists(fname + ".xs")) {
                                if (firstFileName == "")
                                    firstFileName = fname;

                                string text = File.ReadAllText(Path.GetFullPath(fname + ".xs")) + '\0';
                                Lexer lexer = new Lexer(text);
                                Token[] tokens = lexer.Lex();

                                Parser parser = new Parser(tokens);
                                ASTExpr[] ast = parser.Parse();
                                compilers.Add(new Compiler("", fname, Path.GetFullPath(fname), outDir, "", ast));
                            }
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
            }
            else {
                PrintHelp();
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
            Console.WriteLine("Xouver Compiler " + VERSION);
            Console.WriteLine("Usage: xouverc [options] <sourceFiles>");
            Console.WriteLine("Where 'options' includes:");
            Console.WriteLine("-outdir <path>                  -- Specifies the directory to write the compiled files into.");
            Console.WriteLine("-outname <name>                 -- Specifies the name of the compiled output without extension.");
            Console.WriteLine("\nOther usage: xouverc [subcommand]");
            Console.WriteLine("Possible subcommands include:");
            Console.WriteLine("-version                         -- Displays the versionig of the compiler.");
            Console.WriteLine("-help || ?                       -- Shows this list.");
        }


        private static byte[] GetBytes(int i) {
            byte[] bytes = BitConverter.GetBytes(i);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            return bytes;
        }
    }
}