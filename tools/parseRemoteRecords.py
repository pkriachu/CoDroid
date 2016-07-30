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
import base64
import zlib
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


    def getClassSize(self) :
        return len(self.classes)


    def getMethodSize(self) :
        return len(self.methods)


    def getBlockInfo(self, blockId) :
        return self.blocks[blockId]


    def getBlockSize(self) :
        return len(self.blocks)


    def getLineSize(self, blockId = -1) :
        if blockId == -1 :
            return self.lines
        return self.blocks[blockId]


    def printInfo(self) :
        print "Apk Id : %s" % (self.apkId)
        print "Classes: %d" % (len(self.classes))
        print "Methods: %d" % (len(self.methods))
        print "Blocks : %d (max index: %d)" % (len(self.blocks), max(self.blocks.keys()))
        print "Lines  : %d" % (self.lines)





class Record :
    def __init__(self) :
        self.__timestamp = 0
        self.__countTable = dict()


    def loadJSONs(self, JSONstr, timestamp = 0) :
        self.__timestamp = timestamp;
        self.__count_table = json.loads()


    def loadRecordFile(self, path, timestamp = 0) :
        self.__timestamp = timestamp;
        with open(path, 'r') as file :
            content = file.read()
            self.__countTable = json.loads(content)


    # update table from a newer record
    # update timestamp, and merge two tables
    def update(self, newer) :
        # update timestamp
        self.timestamp = newer.timestamp
        
        # merge two tables
        for key in newer.__countTable :
            if key in self.__countTable :
                self.__countTable[key] += newer.__countTable[key]
            else :
                self.__countTable[key] = newer.__countTable[key]


    def printTable(self) :
        print self.__countTable


    def calculateCoverage(self, meta) :
        coverage = dict("class":0.0, "method":0.0, "block":0.0, "line":0.0)
        
        # calculate coverage from block list (count table) and meta
        classes = set()
        methods = set()
        lines = 0
        
        for blockId in self.__countTable :
            # block = {"className": "", "methodName": "", "blockFlags": 0, "lines": 0}
            block = meta.getBlockInfo(blockId)
            classes.add(block["className"])
            methods.add( "%s->%s" % (block["className"], block["methodName"]) )




if __name__ == "__main__" :
    if len(sys.argv) < 2 :
        exit(0)
    metafile = sys.argv[1]
    recordsDir = sys.argv[2]


    # parsing meta file
    meta = MetaInfo(metafile)
    
    # parsing record files
    # loading the timestamp of last modified file
    last = 0
    lastfile = os.path.join(recordsDir, 'last')
    if os.path.isfile(lastfile) :
        with open(lastfile, 'r') as file :
            last = int(file.read())

    for file in sorted(os.listdir(recordsDir)) :
        if file.endswith(".out") :
            timestamp = int(file.split('.')[0])
            # skip the files thar already parsed
            if timestamp <= last :
                break

            print "Loading %s" % (os.path.join(recordsDir, file))
            records = Record()
            records.loadRecordFile(os.path.join(recordsDir, file), timestamp)
            records.printTable()

            last = timestamp

    with open(lastfile, 'w+') as file :
        file.write(str(last))


