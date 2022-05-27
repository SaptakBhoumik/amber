import platform
import os
from os import path
import sys
lib_name="libamber"
if platform.system().lower()=="windows":
    lib_name+=".lib"
else:
    lib_name+=".a"
pagerank=["Amber/pagerank/pagerank.cpp"]
search=["Amber/search/search.cpp"]
tokenizer=[
    "Amber/tokenizer/special_char.cpp",
    "Amber/tokenizer/tokenizer.cpp",
]
files=[
    tokenizer,
    search,
    pagerank
]
cpp_compiler="clang++"
cargs="-I / -std=c++20 -fPIC"
objdir="obj"
buildir="buildir"

def system(cmd):
    print(cmd)
    if os.system(cmd)!=0:
        print("Command failed")
        exit(1)

def build():
    rebuild="rebuild" in sys.argv
    obj=[]
    system(f"mkdir -p {buildir}")
    system(f"mkdir -p {buildir}/{objdir}")
    modified=False
    for x in files:
        for y in x:
            obj_name=y.replace("/","_").replace(".cpp",".o")
            obj.append(obj_name)
            if rebuild:
                modified=True
                system(f"{cpp_compiler} -c {cargs} {y} -o {buildir}/{objdir}/{obj_name}")
            elif path.exists(f"{buildir}/{objdir}/{obj_name}"):
                if path.getmtime(y)>path.getmtime(f"{buildir}/{objdir}/{obj_name}"):
                    modified=True
                    system(f"{cpp_compiler} -c {cargs} {y} -o {buildir}/{objdir}/{obj_name}")
            else:
                modified=True
                system(f"{cpp_compiler} -c {cargs} {y} -o {buildir}/{objdir}/{obj_name}")
    if modified==True:
        cmd=f"ar rcs {buildir}/{lib_name} "
        for x in obj:
            cmd+=f" {buildir}/{objdir}/{x}"
        system(cmd)
    else:
        print("No changes detected, skipping build")

def install():
    if platform.system().lower()=="windows":
        print("Unfortunately I dont know how to install static library in so you have to install them manually or include them in your project directly")
        print("If you know how to do it please create a pull request")
        exit(1)
    header_dir="/usr/include"
    lib_dir="/usr/lib"
    if path.exists(f"{header_dir}/Amber"):
        print("Amber header already exists, deleting it")
        system(f"rm -rf {header_dir}/Amber")
    system(f"mkdir -p {header_dir}/Amber")
    system(f"cp -r  Amber/include {header_dir}/Amber/include")
    if path.exists(f"{lib_dir}/{lib_name}"):
        print("Amber library already exists, deleting it")
        system(f"rm {lib_dir}/{lib_name}")
    system(f"cp {buildir}/{lib_name} {lib_dir}")


    
if "help" in sys.argv:
    print("""
    Usage:
        make.py         - Build the library
        make.py rebuild - Build the library again
        make.py debug   - Build the library with debug symbols
        make.py release - Build the library with optimizations
        make.py clean   - Clean the build directory
    """)


elif "debug" in sys.argv:
    cargs+=" -ggdb -glldb "
    build()

elif "release" in sys.argv:
    cargs+=" -O2 "
    build()

elif "clean" in sys.argv:
    system(f"rm -rf {buildir}")

elif "install" in sys.argv:
    install()

else:
    build()
