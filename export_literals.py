# This is a sample Python script.
import xml.sax

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
from clang.cindex import Config
import clang.cindex
import typing
import ast
import re
import json

from pathlib import Path

def fnv1a_32(string):
    hash = 2166136261
    for i in string:
        hash = hash ^ i
        hash = hash * 16777619 % 2**32
    return hash


def find_type_refs(node, action) -> typing.Iterable[clang.cindex.Cursor]:
    """ Find all references to the type named 'typename'
    """
    if node.kind is clang.cindex.CursorKind.COMPOUND_STMT:
        #action(node)
        for i in node.get_tokens():
           if i.kind is clang.cindex.TokenKind.LITERAL:
                action(i)
    # Recurse for children of this node
    for c in node.get_children():
        find_type_refs(c, action)


Config.set_library_path("/usr/local/lib/python3.11/site-packages/clang/native/")

index = clang.cindex.Index.create()
files = (p.resolve() for p in Path(".").glob("**/*") if
         p.suffix in {".c", ".cc", ".cpp", ".hxx", ".h", ".hpp", ".cpp"})

all_literals = set()

def get_hash(data):
    input_string = data.spelling
    result = re.findall('"([^"]*)"', input_string)
    if len(result) > 0:
        normalized_string  = input_string[input_string.find('"'):input_string.rfind('"')+1]
        string = ast.literal_eval(normalized_string)
        all_literals.add(( result[0], fnv1a_32(string.encode('utf-8'))))
    



for file in files:
    # try:
        translation_unit = index.parse(file)
        print("---------------------------" + file.name + "------------------\n")
        find_type_refs(translation_unit.cursor, get_hash)
        # except:
        #     print("fail: " + file.name)



resultDictionary = dict(all_literals)

json_object = json.dumps(resultDictionary, indent=4)
 
# Writing to sample.json
with open("literals.json", "w") as outfile:
    outfile.write(json_object)
    