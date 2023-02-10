using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Globalization;
using System.Linq;
using System.Linq.Expressions;
using System.Net.Http.Headers;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using XouverC.Exceptions;
using XouverC.Parsing;

namespace XouverC.Compiling
{
    internal class Compiler {
        public static Dictionary<string, Compiler> compiledClasses = new();
        private static Dictionary<string, Compiler> classes = new();

        public byte[] code;

        private List<string> imports;
        private string path;
        private string filePath;
        private string className;
        private string outDir;
        private string outName;
        public string fullName;

        private ASTExpr[] exprs;

        private FunctionInfo currentInfo;

        public List<dynamic> constants;
        public List<FieldInfo> fieldInfos;
        private List<byte> content;
        private List<byte> instructions;
        public List<FunctionInfo> funcInfos;
        private Stack<List<VarInfo>> scopes;

        private List<FunctionInfo> constructors;

        private bool hasCompiled;
        public bool HasCompiled {
            get {
                return hasCompiled;
            }
        }

        public Compiler(string path, string className, string filePath, string outPath, string outName, ASTExpr[] exprs) {
            this.className = className;
            this.path = path;
            this.exprs = exprs;
            this.outDir = outPath;
            this.filePath = filePath;
            this.fullName = path + className;
            this.outName = outName;

            constants = new();
            content = new();
            instructions = new();
            funcInfos = new();
            fieldInfos = new();
            scopes = new();

            imports = new();

            classes.Add(fullName, this);

            hasCompiled = false;
        }

        public byte[] Compile() {
            content.AddRange(GetBytes(className.Length));
            for (int i = 0; i < className.Length; i++)
                content.Add(Convert.ToByte(className[i]));

            classes.Remove(fullName);
            compiledClasses.Add(fullName, this); // TODO: path + className

            foreach (ASTExpr expr in exprs)
                if (expr is ASTFuncDef)
                    CompileFuncASTExpr((ASTFuncDef)expr);
                else if (expr is ASTFieldDef)
                    CompileFieldASTExpr((ASTFieldDef)expr);
                else
                    throw new Exception();

            content.AddRange(GetBytes(fieldInfos.Count));
            content.AddRange(GetBytes(funcInfos.Count));
            foreach (FunctionInfo info in funcInfos) {
                content.AddRange(GetBytes(info.signature.Length));
                foreach (char c in info.signature)
                    content.Add(Convert.ToByte(c));

                info.pointer = instructions.Count;
                content.AddRange(GetBytes(info.pointer));

                currentInfo = info;
                scopes.Push(new());
                for (int i = 0; i < info.argNames.Length; i++) {
                    VarInfo vinfo = new();

                    vinfo.name = info.argNames[i];
                    vinfo.type = info.argTypes[i];
                    vinfo.pointer = scopes.Peek().Count;
                    scopes.Peek().Add(vinfo);
                }

                List<byte> bytes = new();
                foreach (ASTExpr e in info.exprs) {
                    bytes.AddRange(CompileExpr(e));
                }

                byte[] len = GetBytes(scopes.Pop().Count);
                instructions.AddRange(len);

                byte[] args = GetBytes(info.argNames.Length);
                instructions.AddRange(args);

                instructions.AddRange(bytes);
            }

            content.AddRange(GetBytes(constants.Count));
            foreach (dynamic constant in constants) {
                if (constant is int) {
                    content.Add(11);
                    content.AddRange(GetBytes((int)constant));
                }
            }

            content.AddRange(GetBytes(instructions.Count));

            foreach (byte i in instructions) {
                content.Add(i);
            }

            /*string outPath = filePath;
            if (outDir != "") outPath = outDir;

            if (outName != "") outPath += "\\" + outName;
            else outPath += "\\" + className + ".xcls";

            //File.WriteAllBytes(outPath, content.ToArray());
            //Console.WriteLine(outPath);*/
            hasCompiled = true;
            code = content.ToArray();

            return code;
        }

        private byte[] CompileExpr(ASTExpr expr) {
            List<byte> instructs = new();

            if (expr is ASTBinaryExpr) {
                ASTBinaryExpr binExpr = (ASTBinaryExpr)expr;

                instructs.AddRange(CompileExpr(binExpr.lval));
                instructs.AddRange(CompileExpr(binExpr.rval));

                if (binExpr.op == "+")
                    instructs.Add(Instructions.Add);
                else if (binExpr.op == "-")
                    instructs.Add(Instructions.Sub);
                if (binExpr.op == "*")
                    instructs.Add(Instructions.Mul);
                else if (binExpr.op == "/")
                    instructs.Add(Instructions.Div);
            }
            else if (expr is ASTUnaryExpr) {
                ASTUnaryExpr uExpr = (ASTUnaryExpr)expr;

                instructs.AddRange(CompileExpr(new ASTInt() { value = 0 }));
                instructs.AddRange(CompileExpr(uExpr.val));
                if (uExpr.op == "+")
                    instructs.Add(Instructions.Add);
                else if (uExpr.op == "-")
                    instructs.Add(Instructions.Sub);
                else throw new Exception();
            }
            else if (expr is ASTBlock) {
                ASTBlock block = (ASTBlock)expr;

                foreach (ASTExpr e in block.exprs)
                    instructs.AddRange(CompileExpr(e));
            }
            else if (expr is ASTFuncCall) {
                ASTFuncCall astCall = (ASTFuncCall)expr;

                List<string> argTypes = new();

                for (int i = 0; i < astCall.args.Length; i++) {
                    argTypes.Add(ResolveType(astCall.args[i]));
                    instructs.AddRange(CompileExpr(astCall.args[i]));
                }

                int func = ResolveFunction(astCall.name, argTypes.ToArray());

                instructs.Add(Instructions.Call);
                instructs.AddRange(GetBytes(func));
            }
            else if (expr is ASTVarDec) {
                ASTVarDec dec = (ASTVarDec)expr;

                VarInfo info = new();
                info.type = dec.type;
                info.name = dec.name;
                info.pointer = scopes.Peek().Count;

                string valType = ResolveType(dec.value);
                if (info.type != valType) throw new InvalidTypeException(valType, info.type, dec.line);

                instructs.AddRange(CompileExpr(dec.value));
                instructs.Add(Instructions.Store);
                instructs.AddRange(GetBytes(info.pointer));

                scopes.Peek().Add(info);
            }
            else if (expr is ASTVarDef) {
                ASTVarDef def = (ASTVarDef)expr;

                VarInfo info = new();
                info.type = def.type;
                info.name = def.name;
                info.pointer = scopes.Peek().Count;

                scopes.Peek().Add(info);
            }
            else if (expr is ASTAssign) {
                ASTAssign assign = (ASTAssign)expr;

                foreach (VarInfo info in scopes.Peek())
                    if (info.name == assign.identifier) {
                        string valType = ResolveType(assign.value);
                        if (info.type != valType) throw new InvalidTypeException(valType, info.type, expr.line);

                        byte[] pointer = GetBytes(info.pointer);

                        if (assign.op == "=") {
                            instructs.AddRange(CompileExpr(assign.value));
                        }
                        else {
                            instructs.Add(Instructions.Load);
                            instructs.AddRange(pointer);
                            instructs.AddRange(CompileExpr(assign.value));
                            
                            if (assign.op == "+=")
                                instructs.Add(Instructions.Add);
                            if (assign.op == "-=")
                                instructs.Add(Instructions.Sub);
                            if (assign.op == "*=")
                                instructs.Add(Instructions.Mul);
                            if (assign.op == "/=")
                                instructs.Add(Instructions.Div);
                        }

                        instructs.Add(Instructions.Store);
                        instructs.AddRange(pointer);

                        return instructs.ToArray();
                    }

                foreach (FieldInfo info in fieldInfos)
                    if (info.name == assign.identifier) {
                        string valType = ResolveType(assign.value);
                        if (info.type != valType) throw new InvalidTypeException(valType, info.type, expr.line);

                        byte[] pointer = GetBytes(info.pointer);

                        if (assign.op == "=") {
                            instructs.AddRange(CompileExpr(assign.value));
                        }
                        else {
                            instructs.Add(Instructions.OLoad);
                            instructs.AddRange(pointer);

                            instructs.AddRange(CompileExpr(assign.value));
                            if (assign.op == "+=")
                                instructs.Add(Instructions.Add);
                            if (assign.op == "-=")
                                instructs.Add(Instructions.Sub);
                            if (assign.op == "*=")
                                instructs.Add(Instructions.Mul);
                            if (assign.op == "/=")
                                instructs.Add(Instructions.Div);
                        }

                        instructs.Add(Instructions.OStore);
                        instructs.AddRange(GetBytes(info.pointer));

                        return instructs.ToArray();
                    }

                throw new Exception();
            }
            else if (expr is ASTIf) {
                ASTIf ifExpr = (ASTIf)expr;

                instructs.AddRange(CompileExpr(ifExpr.condition));

                if (ifExpr.not)
                    if (instructs.Last() != Instructions.Not)
                        instructs.Add(Instructions.Not);
                    else instructs.RemoveAt(instructs.Count - 1);
                    

                List<byte> bytes = new();
                bytes.AddRange(CompileExpr(ifExpr.expr));

                instructs.Add(Instructions.JIf);
                instructs.AddRange(GetBytes(bytes.Count));
                instructs.AddRange(bytes);

                if (ifExpr.elseExpr != null) {
                    instructs.AddRange(CompileExpr(ifExpr.elseExpr));
                }
            }
            else if (expr is ASTWhile) {
                ASTWhile we = (ASTWhile)expr;

                instructs.AddRange(CompileExpr(we.condition));

                if (we.not)
                    if (instructs.Last() != Instructions.Not)
                        instructs.Add(Instructions.Not);
                    else instructs.RemoveAt(instructs.Count - 1);

                List<byte> bytes = new();
                bytes.AddRange(CompileExpr(we.expr));
                bytes.Add(Instructions.Jmp);
                bytes.AddRange(GetBytes(-bytes.Count - instructs.Count - 5));

                instructs.Add(Instructions.JIf);
                instructs.AddRange(GetBytes(bytes.Count));

                instructs.AddRange(bytes);
            }
            else if (expr is ASTFor) {
                ASTFor fe = (ASTFor)expr;

                if (!(fe.def is ASTVarDec)) throw new Exception();

                instructs.AddRange(CompileExpr(fe.def));

                ASTIdentifier ident = new() {
                    value = ((ASTVarDec)fe.def).name
                };

                int offset = instructs.Count;
                instructs.AddRange(ResolveIdent(ident));
                instructs.AddRange(CompileExpr(fe.count));

                List<byte> bytes = new();
                bytes.AddRange(CompileExpr(fe.expr));
                bytes.AddRange(ResolveIdent(ident));
                bytes.Add(Instructions.CLoad);
                bytes.AddRange(GetBytes(GetConstant(1)));
                bytes.Add(Instructions.Add);
                bytes.Add(Instructions.Store);
                bytes.AddRange(GetVar(ident.value));
                bytes.Add(Instructions.Jmp);
                bytes.AddRange(GetBytes(-bytes.Count - instructs.Count - 5 + offset));

                instructs.Add(Instructions.JIf);
                instructs.AddRange(GetBytes(bytes.Count));
                instructs.AddRange(bytes);
            }
            else if (expr is ASTIn) {
                ASTIn aIn = (ASTIn)expr;

                if (ResolveType(aIn.value) != "int") throw new Exception();

                instructs.AddRange(CompileExpr(aIn.value));
                instructs.Add(Instructions.IfEq);
            }
            else if (expr is ASTComparison) {
                ASTComparison cmp = (ASTComparison)expr;

                instructs.AddRange(CompileExpr(cmp.lval));
                instructs.AddRange(CompileExpr(cmp.rval));

                switch (cmp.op) {
                    case "==":
                        instructs.Add(Instructions.IfEq);
                        break;
                    case "<":
                        instructs.Add(Instructions.IfGq);
                        instructs.Add(Instructions.Not);
                        break;
                    case ">":
                        instructs.Add(Instructions.IfGt);
                        break;
                    case "<=":
                        instructs.Add(Instructions.IfGq);
                        instructs.Add(Instructions.Not);
                        break;
                    case ">=":
                        instructs.Add(Instructions.IfGq);
                        break;
                }
            }
            else if (expr is ASTReturn) {
                instructs.Add(Instructions.Return);
            }
            else if (expr is ASTInt) {
                instructs.Add(Instructions.CLoad);
                instructs.AddRange(GetBytes(GetConstant(((ASTInt)expr).value)));
            }
            else if (expr is ASTFloat) {
                instructs.Add(Instructions.CLoad);
                instructs.AddRange(GetBytes(GetConstant(((ASTFloat)expr).value)));
            }
            else if (expr is ASTChar) {
                instructs.Add(Instructions.CLoad);
                instructs.Add(Convert.ToByte(GetConstant(((ASTChar)expr).value)));
            }
            else if (expr is ASTIdentifier) {
                instructs.AddRange(ResolveIdent(((ASTIdentifier)expr)));
            }
            else if (expr is ASTAccess) {
                ASTAccess ac = (ASTAccess)expr;

                string t = ResolveType(ac.left);

                Compiler c = compiledClasses[t];

                instructs.Add(Instructions.Invoke);
                instructs.AddRange(GetBytes(Array.IndexOf(compiledClasses.Values.ToArray(), c)));
                instructs.AddRange(c.CompileExpr(ac.right));
                instructs.Add(Instructions.Invoke);
                instructs.AddRange(GetBytes(Array.IndexOf(compiledClasses.Values.ToArray(), this)));
            }
            else if (expr is ASTNewExpr) {
                string name = ((ASTNewExpr)expr).name;
                Compiler c = compiledClasses[name];

                instructs.Add(Instructions.New);
                instructs.AddRange(GetBytes(Array.IndexOf(compiledClasses.Values.ToArray(), c)));
            }
            else throw new NotImplementedException();
            return instructs.ToArray();
        }

        private void CompileFieldASTExpr(ASTFieldDef expr) {
            FieldInfo info = new();

            info.name = expr.name;
            info.type = expr.type;
            info.modifiers = expr.modifiers;
            info.pointer = fieldInfos.Count;

            fieldInfos.Add(info);
        }
        private void CompileFuncASTExpr(ASTFuncDef expr) {
            FunctionInfo info = new();
            info.signature = "className:" + expr.name + "(";

            if (expr.argCount > 0)
                info.signature += expr.argTypes[0];
            for (int i = 1; i < expr.argCount; i++) {
                info.signature += ", " + expr.argTypes[i];
            }

            info.signature += ")#" + expr.type;

            info.name = expr.name;
            info.argTypes = expr.argTypes;
            info.argNames = expr.argNames;
            info.exprs = expr.exprs;
            info.modifiers = expr.modifiers;
            info.type = expr.type;

            funcInfos.Add(info);
        }

        private void CompileConstructorASTExpr(ASTConstructor expr) {
            FunctionInfo info = new();
            info.signature = "className:" + className + "(";

            if (expr.argCount > 0)
                info.signature += expr.argTypes[0];
            for (int i = 1; i < expr.argCount; i++) {
                info.signature += ", " + expr.argTypes[i];
            }

            info.signature += ")#" + className;

            info.argTypes = expr.argTypes;
            info.argNames = expr.argNames;
            info.exprs = expr.exprs;
            info.modifiers = expr.modifiers;
            info.type = className;

            constructors.Add(info);
        }

        private int GetConstant(dynamic value) {
            for (int i = 0; i < constants.Count; i++)
                if (constants[i] == value)
                    return i;

            constants.Add(value);
            return constants.Count - 1;
        }

        private int ResolveFunction(string name, string[] argTypes) {
            for (int i = 0; i < funcInfos.Count; i++) {
                if (funcInfos[i].name == name) {
                    if (argTypes.SequenceEqual(funcInfos[i].argTypes)) {
                        return i;
                    }
                }
            }

            throw new Exception();
        }


        private string ResolveType(ASTExpr expr) {
            if (expr is ASTInt) return "int";
            else if (expr is ASTFloat) return "float";
            else if (expr is ASTBool) return "bool";
            else if (expr is ASTChar) return "char";
            else if (expr is ASTFuncCall) {
                ASTFuncCall call = (ASTFuncCall)expr;

                List<string> argTypes = new();

                foreach (ASTExpr arg in call.args)
                    argTypes.Add(ResolveType(arg));

                int i = ResolveFunction(call.name, argTypes.ToArray());

                return funcInfos[i].type;
            }
            else if (expr is ASTIdentifier) {
                ASTIdentifier ident = (ASTIdentifier)expr;

                if (scopes.Count > 0) {
                    foreach (VarInfo info in scopes.Peek()) {
                        if (info.name == ident.value) {
                            return info.type;
                        }
                    }
                }

                foreach (FieldInfo info in fieldInfos)
                    if (info.name == ident.value) {
                        return info.type;
                    }

                foreach (Compiler c in compiledClasses.Values) {
                    if (c.className == ident.value) {
                        return c.className;
                    }
                }
            }
            else if (expr is ASTBinaryExpr) {
                return ResolveType(((ASTBinaryExpr)expr).lval);
            }
            else if (expr is ASTUnaryExpr) {
                return ResolveType(((ASTUnaryExpr)expr).val);
            }
            else if (expr is ASTAccess) {
                string t = ResolveType(((ASTAccess)expr).left);

                foreach (Compiler c in compiledClasses.Values) {
                    if (c.className == t) return c.ResolveType(((ASTAccess)expr).right);
                }
            }
            else if (expr is ASTNewExpr) {
                string t = ((ASTNewExpr)expr).name;

                foreach (Compiler c in compiledClasses.Values) {
                    if (c.className == t) return c.className;
                }
            }
            throw new Exception();
        }

        private byte[] ResolveIdent(ASTIdentifier ident) {
            List<byte> instructs = new();

            foreach (VarInfo info in scopes.Peek()) {
                if (info.name == ident.value) {
                    instructs.Add(Instructions.Load);
                    instructs.AddRange(GetBytes(info.pointer));
                    return instructs.ToArray();
                }
            }

            foreach (FieldInfo info in fieldInfos)
                if (info.name == ident.value) {
                    instructs.Add(Instructions.OLoad);
                    instructs.AddRange(GetBytes(info.pointer));
                    return instructs.ToArray();
                }

            foreach (string imp in imports) {
                string[] str = imp.Split('.');

                if (str.Last() == ident.value) {
                    instructs.Add(Instructions.Invoke);
                    if (compiledClasses.ContainsKey(ident.value)) {
                        int index = Array.IndexOf(compiledClasses.Keys.ToArray(), ident.value);

                        instructs.Add(Instructions.Invoke);
                        instructs.AddRange(GetBytes(index));
                        return instructs.ToArray();
                    }
                }
            }

            throw new Exception();
        }

        private byte[] GetVar(string name) {
            foreach (VarInfo info in scopes.Peek()) {
                if (info.name == name) {
                    return GetBytes(info.pointer);
                }
            }

            throw new Exception();
        }

        private byte[] GetBytes(int i) {
            byte[] bytes = BitConverter.GetBytes(i);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            return bytes;
        }

        private byte[] GetBytes(float f) {
            byte[] bytes = BitConverter.GetBytes(f);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            return bytes;
        }
    }
}
