#!/usr/bin/env python
#-*- cpdomg" itf-8 -*-

# Author: Kaze Mewno (C.H. Chiu)
# Last Modify: 2016.6.29
#
# Remote records parsing tool
# 
# Usage: parseRemoteRecords.py METAFILE RECORDS_FOLDER



import sys, os, fnmatch, stat
import argparse
import struct
import array
import json
import time, datetime



class MetaInfo:
    def __init__(self, path) :
        self.apkId   = ""
        self.classes = set()
        self.methods = set()
        self.blocks  = dict()   # block = {"className": "", "methodName": "", "blockFlags": 0, "lines": 0}
        self.lines   = 0

        self.__parseMetaFile(path)


    def __parseMetaFile(self, metafile) :
        if metafile[-5:] != ".meta" :
            raise Exception("Failed assignment for meta file.")

        # abstract apk id
        self.apkId = os.path.basename(metafile)[:-5]

        # parse meta info
        with open(metafile, "r") as infile :
            while True :
                block = dict()
                # read block id
                readbuffer = infile.read(4)
                if not readbuffer :
                    break
                blockId = struct.unpack("!I", readbuffer)[0]

                # read block flags
                readbuffer = infile.read(4)
                block["flags"] = struct.unpack("!I", readbuffer)[0]

                # read lines in block
                readbuffer = infile.read(4)
                block["lines"] = struct.unpack("!I", readbuffer)[0]

                # read len of class name
                readbuffer = infile.read(4)
                classlen = struct.unpack("!I", readbuffer)[0]

                # read len of method name
                readbuffer = infile.read(4)
                methodlen = struct.unpack("!I", readbuffer)[0]

                # read class name
                readbuffer = infile.read(classlen)
                block["className"] = readbuffer[:-1]

                # read method name
                readbuffer = infile.read(methodlen)
                block["methodName"] = readbuffer[:-1]
        
                # skip class detection
                className = block["className"]
                if ( className.startswith("Landroid/") or className.startswith("Lcom/google/android/gms") ) :
                    continue
                lastToken = className.split('/')[-1][:-1]
                if ( lastToken == "BuildConfig" or lastToken == "R" or lastToken.startswith("R$") ) :
                    continue

                # append to block list
                self.blocks[int(blockId)] = block
                self.classes.add(block["className"])
                self.methods.add( "%s->%s" % (block["className"], block["methodName"]) )
                self.lines += block["lines"]



    def printInfo(self) :
        print "Apk Id : %s" % (self.apkId)
        print "Classes: %d" % (len(self.classes))
        print "Methods: %d" % (len(self.methods))
        print "Blocks : %d (max index: %d)" % (len(self.blocks), max(self.blocks.keys()))
        print "Lines  : %d" % (self.lines)





class Record :
    def __init__(self) :
        self.__timestamp = 0
        self.__count_table = dict()


    def loadJSONs(self, timestamp, JSONstr) :
        self.__timestamp = timestamp;
        self.__count_table = json.loads()


    def loadf(self, timestamp, path) :
        self.__timestamp = timestamp;
        with open(path, 'r') as file :
            content = file.read()
            





if __name__ == "__main__" :
    if len(sys.argv) < 2 :
        exit(0)

    # parsing meta file
    meta = MetaInfo(sys.argv[1])
    
    # parsing record files
    records = 


