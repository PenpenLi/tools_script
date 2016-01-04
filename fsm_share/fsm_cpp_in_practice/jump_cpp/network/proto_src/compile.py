#!/usr/bin/env python2

import os

def getDictionaryFile(dir, search):
    files = os.listdir(dir)
    dictionary_file = []
    for f in files:
        if(f.find(search) > -1):
            dictionary_file.append(f)
    return dictionary_file


if __name__ == '__main__':
    if(not os.path.exists("../protobuf/")):
        os.mkdir("../protobuf/")

    os.system("protoc -I=./ --cpp_out=../protobuf/ *.proto")
    heads = getDictionaryFile('..\protobuf', "pb.h");

    optput_str = """// Do not edit this file!! this file created by python script!!
#ifndef HiFly_ProtobufMsg_h
#define HiFly_ProtobufMsg_h
"""

    for h in heads:
        optput_str += '#include "' + h + '"' + "\n"
    
    optput_str += "using namespace protobuf;\n"
    optput_str += "#endif\n"

    export_header_file = open("../protobuf/ProtobufMsg.h", "w")
    export_header_file.write(optput_str)
    export_header_file.close()

    print "Done!"
