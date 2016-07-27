#!/usr/bin/env python
#-*- coding: utf-8 -*-


# Author: Kaze Mewno (C. H. Chiu)
# Last Modify: 2016.7.27
# 
# Parsing the CoDroid records passing through the internet.
#
# Usage: %(prog)s [-A | -D]  -m METAFILE(or folder) [-i COVERAGE_FILE(or folder)]"



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
    class __RecordUnit:
        def __init__(self, siteName="", apkId="", buildId="", submitId="") :
            self.__siteName = siteName
            self.__apkId = apkId
            self.__buildId = buildId
            self.__submitId = submitId
            self.__lastFile = ""
            self.__fileList = set()
            self.__submitTime = [0L, 0L]
            self.__receiveTime = [0L, 0L]

            self.__countingList = dict()
            self.__classes = set()
            self.__methods = set()
            self.__blocks = set()
            self.__lines = 0

            self.__meta = None

        def addFile(self, file) :
            if file in self.__fileList :
                return

            self.__fileList.add(file)

            # format of filename:  SITENAME_PACKAGENAME_BUILDID_RUNTIMEID_TIMESTAMP
            token = os.path.basename(file).split('_')

            submitTime = int(token[4])
            receiveTime = os.stat(file).st_mtime

            # only take the latest record file
            if receiveTime > self.__receiveTime[1] or submitTime > self.__submitTime[1] :
                self.__lastFile = file

            # update submitTime and receiveTime if need
            if self.__receiveTime[0] == 0L :
                self.__submitTime[0] = submitTime
                self.__submitTime[1] = submitTime
                self.__receiveTime[0] = receiveTime
                self.__receiveTime[1] = receiveTime
            else :
                self.__submitTime[0] = self.__submitTime[0] if self.__submitTime[0] < submitTime else submitTime
                self.__submitTime[1] = self.__submitTime[1] if self.__submitTime[1] > submitTime else submitTime
                self.__receiveTime[0] = self.__receiveTime[0] if self.__receiveTime[0] < receiveTime else receiveTime
                self.__receiveTime[1] = self.__receiveTime[1] if self.__receiveTime[1] > receiveTime else receiveTime

        def update(self, unit) :
            self.__fileList |= unit.__fileList

            if not self.__meta :
                self.__meta = unit.__meta
            elif unit.__meta and self.__meta != unit.__meta :
                raise Exception("Apps to be merged should have the same meta file.")

            if self.__receiveTime[0] == 0L :
                self.__submitTime[0] = unit.__submitTime[0]
                self.__submitTime[1] = unit.__submitTime[1]
                self.__receiveTime[0] = unit.__receiveTime[0]
                self.__receiveTime[1] = unit.__receiveTime[1]
            else :
                self.__submitTime[0] = self.__submitTime[0] if self.__submitTime[0] < unit.__submitTime[0] else unit.__submitTime[0]
                self.__submitTime[1] = self.__submitTime[1] if self.__submitTime[1] > unit.__submitTime[1] else unit.__submitTime[1]
                self.__receiveTime[0] = self.__receiveTime[0] if self.__receiveTime[0] < unit.__receiveTime[0] else unit.__receiveTime[0]
                self.__receiveTime[1] = self.__receiveTime[1] if self.__receiveTime[1] > unit.__receiveTime[1] else unit.__receiveTime[1]

            for k, v in unit.__countingList.iteritems() :
                if k in self.__countingList :
                    self.__countingList[k] += v
                else :
                    self.__countingList[k] = v

            self.__parseCountingList()


        def parseRecords(self, meta, modeAccumulative) :
            if modeAccumulative :
                self.__parseAccumulativeRecords(meta)
            else :
                self.__parseDiscreteRecords(meta)


        def __parseAccumulativeRecords(self, meta) :
            self.__meta = meta
            with open(self.__lastFile, "r") as infile :
                readbuffer = infile.read()
                if readbuffer :
                    self.__countingList = json.loads(readbuffer)

            if self.__countingList :
                self.__parseCountingList()

        def __parseDiscreteRecords(self, meta) :
            self.__meta = meta
            for file in self.__fileList :
                with open(file, "r") as infile :
                    readbuffer = infile.read()
                    if readbuffer :
                        countingList = json.loads(readbuffer)
                        for key in countingList :
                            blockId = int(key)
                            if blockId in self.__countingList :
                                self.__countingList[blockId] += countingList[key]
                            else :
                                self.__countingList[blockId] = countingList[key]

            if self.__countingList :
                self.__parseCountingList()

        def __parseCountingList(self, meta=None) :
            if not meta :
                meta = self.__meta
            if not meta or not self.__countingList :
                raise Exception("Meta file does not exist.")
            if not self.__countingList :
                raise Exception("Record does not exist.")

            for key in self.__countingList.keys() :
                blockId = int(key)
                # skip if blockId not in metaInfo
                if not blockId in meta.blocks :
                    continue

                block = meta.blocks[blockId]
                self.__classes.add(block["className"])
                self.__methods.add("%s->%s" % (block["className"], block["methodName"]))
                self.__blocks.add(blockId)
                self.__lines += block["lines"]

        def printCoverageReport(self) :
            if not self.__meta :
                raise Exception("Meta file does not exist.")

            meta = self.__meta
            print "  Build / Runtime Id: %s / %s (%d records)" % (self.__buildId, self.__submitId, len(self.__fileList))

            print "  Duration time (Sender): %d ms (%d~%d)" % ( self.__submitTime[1] - self.__submitTime[0] , self.__submitTime[0], self.__submitTime[1])
            print "  Duration time (Receiver): %d s (%d~%d)" % ( self.__receiveTime[1] - self.__receiveTime[0], self.__receiveTime[0], self.__receiveTime[1])

            print "  Class: %.4f (%d/%d)" % (float(len(self.__classes))/len(meta.classes), len(self.__classes), len(meta.classes))
            print "  Method: %.4f (%d/%d)" % (float(len(self.__methods))/len(meta.methods), len(self.__methods), len(meta.methods))
            print "  Block: %.4f (%d/%d)" % (float(len(self.__blocks))/len(meta.blocks), len(self.__blocks), len(meta.blocks))
            print "  Line: %.4f (%d/%d)" % (float(self.__lines)/meta.lines, self.__lines, meta.lines)


        def toString(self) :
            return "RecordUnit: (%s, %s, %s, %s, %d, %s, (%d~%d), (%d~%d))" % (self.__siteName, self.__apkId, self.__buildId, self.__submitId, len(self.__fileList), self.__lastFile, self.__submitTime[0], self.__submitTime[1], self.__receiveTime[0], self.__receiveTime[1])

        def getCoverageData(self) :
            if not self.__meta :
                raise Exception("Meta file does not exist.")

            meta = self.__meta
            coverageData = dict()

            coverageData["records"] = len(self.__fileList)
            coverageData["duration"] = dict()
            coverageData["duration"]["sender"] =  [self.__submitTime[1] - self.__submitTime[0] , self.__submitTime[0], self.__submitTime[1]]
            coverageData["duration"]["receiver"] = [self.__receiveTime[1] - self.__receiveTime[0], self.__receiveTime[0], self.__receiveTime[1]]

            coverageData["class"] = [float(len(self.__classes))/len(meta.classes), len(self.__classes), len(meta.classes)]
            coverageData["method"] = [float(len(self.__methods))/len(meta.methods), len(self.__methods), len(meta.methods)]
            coverageData["block"] = [float(len(self.__blocks))/len(meta.blocks), len(self.__blocks), len(meta.blocks)]
            coverageData["line"] = [float(self.__lines)/meta.lines, self.__lines, meta.lines]

            return (self.__buildId, self.__submitId, coverageData)



        def getRawData(self) :
            rawData = dict()

            rawData["class"] = self.__classes
            rawData["method"] = self.__methods
            rawData["block"] = self.__blocks
#            rawData["line"] = self.__lines
            rawData["records"] = self.__fileList

            return (self.__buildId, self.__submitId, rawData)



    def __init__(self, meta, filepath, modeAccumulative=True) :
        self.__metas = meta
        self.__records = dict()
#        self.__modeAccumulative = modeAccumulative
        self.__modeAccumulative = False

        if os.path.isfile(filepath) :
            self.__preparse(filepath)
        elif os.path.isdir(filepath) :
            dirname = os.path.abspath(filepath)
            for file in os.listdir(filepath) :
                fullpath = "%s/%s" % (dirname, file)
                self.__preparse(fullpath)
        self.__parse()


    def __preparse(self, file) :
        # format of filename:  SITENAME_PACKAGENAME_BUILDID_RUNTIMEID_TIMESTAMP
        token = os.path.basename(file).split('_')

        siteName = token[0]
        apkId = token[1]
        buildId = token[2]    # generated on building
        submitId = token[3]   # generated on runtime

        recordIndex = "_".join((siteName, apkId, buildId, submitId))
        if recordIndex not in self.__records :
            self.__records[recordIndex] = self.__RecordUnit(siteName, apkId, buildId, submitId)
        self.__records[recordIndex].addFile(file)

        # extra record item: summary of sample
        recordIndex = "_".join((siteName, apkId, buildId, "summary"))
        if recordIndex not in self.__records :
            self.__records[recordIndex] = self.__RecordUnit(siteName, apkId, buildId, "summary")
        self.__records[recordIndex].addFile(file)

    def __parse(self) :
        for index, record in  self.__records.iteritems() :
            apkId = index.split("_")[1]
            record.parseRecords(self.__metas[apkId], self.__modeAccumulative)

    def printRecord(self) :
        for i, record in  self.__records.iteritems() :
            print i
            print record.toString()

    def printCoverageReport(self, site=None) :
        for index in sorted(self.__records.keys()) :
            (siteName, apkId) = index.split("_")[0:2]
            if site and siteName != site :
                continue
            print "--"
            print "Site: ", siteName
            print "Apk Id: ", apkId
            self.__records[index].printCoverageReport()

    def printCoverageTable(self, site=None) :
        coverageTable = dict()

        for index in sorted(self.__records.keys()) :
            (siteName, apkId) = index.split("_")[0:2]
            if site and siteName != site :
                continue
            (bid, sid, coverageData) = self.__records[index].getCoverageData()

            if siteName not in coverageTable :
                coverageTable[siteName] = dict()
            if apkId not in coverageTable[siteName] :
                coverageTable[siteName][apkId] = dict()
            if bid not in coverageTable[siteName][apkId] :
                coverageTable[siteName][apkId][bid] = dict()
            if sid not in coverageTable[siteName][apkId][bid] :
                coverageTable[siteName][apkId][bid][sid] = coverageData

        timestamp = lambda t : (t - t % 1000) / 1000
        for siteName in sorted(coverageTable.keys()) :
            print "Remote Tool (Site): %s" % (siteName)
            for apkId in sorted(coverageTable[siteName].keys()) :
                print "- Test apk: %s" % (apkId)
                for bid in sorted(coverageTable[siteName][apkId].keys()) :
                    print "  - Apk build Id: %s" % (bid)
#                    for sid in sorted(coverageTable[siteName][apkId][bid]) :
#                        if sid == "summary" :
#                            continue
#                        record = coverageTable[siteName][apkId][bid][sid]
#                        print "    - Submit Id: %s, %d records, duration(sender): %d ms, duration(receiver): %d s" % (
#                                sid, record["records"], record["duration"]["sender"][0], record["duration"]["receiver"][0] )
#                        outstring = "    "
#                        for name in ("class", "method", "block", "line") :
#                            outstring += "    %s: %.4f(%d/%d)" % (name, record[name][0], record[name][1], record[name][2])
#                        print outstring

                    # Summary of sample
                    record = coverageTable[siteName][apkId][bid]["summary"]
                    print "    - Summary :"
                    timestr = "%d" % (record["duration"]["sender"][0])
                    s, ms = divmod(record["duration"]["sender"][0], 1000)
                    m, s = divmod(s, 60)
                    h, m = divmod(m, 60)
                    #timestr = "%d:%02d:%02d.%03d" % (h, m, s, ms)
                    print "      - Analysis duration (Remote Time): %s (%s ~ %s)" % ( timestr,
                            datetime.datetime.fromtimestamp(timestamp(record["duration"]["sender"][1])), 
                            datetime.datetime.fromtimestamp(timestamp(record["duration"]["sender"][2])))
                    timestr = "%d" % (record["duration"]["receiver"][0])
                    s, ms = (record["duration"]["receiver"][0], 0)
                    m, s = divmod(s, 60)
                    h, m = divmod(m, 60)
                    #timestr = "%d:%02d:%02d.%03d" % (h, m, s, ms)
                    print "      - Analysis duration (Server Time): %s (%s ~ %s)" % ( timestr,
                            datetime.datetime.fromtimestamp(record["duration"]["receiver"][1]), 
                            datetime.datetime.fromtimestamp(record["duration"]["receiver"][2]))
                    outstring = "    "
                    for name in ("class", "method", "block", "line") :
                        outstring += "    %s: %.4f(%d/%d)" % (name, record[name][0], record[name][1], record[name][2])
                    print outstring






if __name__ == "__main__" :
    usage = "usage: %(prog)s [-A | -D]  -m METAFILE(or folder) [-i COVERAGE_FILE(or folder)]"
    parser = argparse.ArgumentParser(usage=usage)
    group = parser.add_mutually_exclusive_group()
    group.add_argument('-A', dest="modeA", action="store_true", default=True,
            help="Records are accumulative data, this is the DEFAULT option.")
    group.add_argument('-D', dest="modeA", action="store_false",
            help="Records are discrete data.")
    parser.add_argument("-m", "--meta", dest = "meta", required = True,
                        help = "a METAFILE or a folder contains METAFILEs.")
    parser.add_argument("-i", "--input", dest="input",
                        help="a COVERAGE RESULT FILE or a folder contains COVERAGE RESULT FILEs.")
    parser.add_argument("-s", "--site", dest="site",
                        help="name string of the analysis website.")

    args = parser.parse_args()

    metas = dict()
    if args.meta :
        if os.path.isfile(args.meta) :
            meta = MetaInfo(args.meta)

            if not meta.apkId in metas :
                metas[meta.apkId] = meta

        elif os.path.isdir(args.meta) :
            dirname = os.path.abspath(args.meta)

            for file in os.listdir(args.meta) :
                fullpath = "%s/%s" % (dirname, file)
                meta = MetaInfo(fullpath)

                if not meta.apkId in metas :
                    metas[meta.apkId] = meta

    if not args.input :
        for i,meta in metas.iteritems() :
            meta.printInfo()
            print ""
    else :
        record = Record(metas, args.input, modeAccumulative=args.modeA)
#        record.printRecord()
        if args.site :
            record.printCoverageTable(site=args.site)
        else :
            record.printCoverageTable()
