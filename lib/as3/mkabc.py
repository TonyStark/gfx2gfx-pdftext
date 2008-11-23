#!/usr/bin/python

import re

fi = open("code.c", "rb")
foc = open("opcodes.c", "wb")
foh = open("opcodes.h", "wb")

foh.write("#ifndef __opcodes_h__\n")
foh.write("#define __opcodes_h__\n")
foh.write("#include \"abc.h\"\n")
foh.write("#include \"pool.h\"\n")
foh.write("#include \"code.h\"\n")

foc.write("#include \"opcodes.h\"\n")

R = re.compile('{(0x..),\s*"([^"]*)"\s*,\s*"([^"]*)"[^}]*}\s*')

for line in fi.readlines():
    line = line.strip()
    m = R.match(line)
    if m:
        op,name,params = m.group(1),m.group(2),m.group(3)
        params = params.strip()
        paramstr = ""
        seen = {}
        names = []

        for c in params:
            paramstr += ", "
            if c == "2":
                type,pname="char*","name"
            elif c == "s":
                type,pname="char*","s"
            elif c in "nubs":
                type,pname="int","v"
            elif c == "m":
                type,pname="abc_method_body_t*","m"
            elif c == "i":
                type,pname="abc_method_t*","m"
            elif c == "c":
                type,pname="abc_class_t*","m"
            elif c == "j":
                type,pname="abc_code_t*","label"
            elif c == "S":
                type,pname="void*","labels"
            elif c == "D":
                type,pname="void*","debuginfo"
            elif c == "r":
                type,pname="int","reg"
            else:
                raise "Unknown type "+c
            paramstr += type
            paramstr += " "
            if pname in seen:
                seen[pname]+=1
                pname += str(seen[pname])
            else:
                seen[pname]=1
            paramstr += pname
            names += [pname]

        foh.write("abc_code_t* abc_%s(abc_code_t*prev%s);\n" % (name, paramstr))

        foc.write("abc_code_t* abc_%s(abc_code_t*prev%s)\n" % (name, paramstr))
        foc.write("{\n")
        foc.write("    abc_code_t*self = add_opcode(prev, %s);\n" % op)
        i = 0
        for pname,c in zip(names,params):
            if(c == "2"):
                foc.write("    self->data[%d] = %s;\n" % (i,pname));
            elif(c in "nur"):
                foc.write("    self->data[%d] = (void*)(ptroff_t)%s;\n" % (i,pname))
            elif(c == "b"):
                foc.write("    self->data[%d] = (void*)(ptroff_t)%s;\n" % (i,pname))
            elif(c == "s"):
                foc.write("    self->data[%d] = strdup(%s);\n" % (i,pname))
            elif(c == "m"):
                foc.write("    self->data[%d] = %s;\n" % (i,pname))
            elif(c == "c"):
                foc.write("    self->data[%d] = %s;\n" % (i,pname))
            elif(c == "i"):
                foc.write("    self->data[%d] = %s;\n" % (i,pname))
            elif(c == "j"):
                foc.write("    self->data[%d] = %s;\n" % (i,pname))
            elif(c == "S"):
                foc.write("    /* FIXME: write labels %s */\n" % pname)
            elif(c == "D"):
                foc.write("    /* FIXME: write debuginfo %s */\n" % pname)
            else:
                raise "Unknown type "+c
            i = i+1
        foc.write("    return self;\n")
        foc.write("}\n")

        foh.write("#define "+name+"(")
        foh.write(",".join(["method"]+names))
        foh.write(") {method->code = abc_"+name+"(")
        foh.write(",".join(["method->code"]+names))
        foh.write(");}\n")

foh.write("#endif\n")

foh.close()
foc.close()
fi.close()
#{0x75, "convert_d", ""},

