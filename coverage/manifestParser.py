#!/usr/bin/env python
#-*- coding: utf-8 -*-
import sys, os, zipfile, json
try:
    import xml.etree.cElementTree as ET
except ImportError:
    import xml.etree.ElementTree as ET



if __name__ == "__main__" :
    # sys.argv: [instrument.py, input folder, main activity]
    if len(sys.argv) < 2 :
        print "Usage: manifestParser.py ANDROID_MANIFEST.XML"
        sys.exit(0)

    ET.register_namespace("android", "http://schemas.android.com/apk/res/android")
    tree = ET.ElementTree(file=sys.argv[1])
    root = tree.getroot()
    up = ET.SubElement(root, "uses-permission", {"android:name":"android.permission.WRITE_EXTERNAL_STORAGE"})
    up = ET.SubElement(root, "uses-permission", {"android:name":"android.permission.INTERNET"})
    tree.write(sys.argv[1], encoding="utf-8", method="xml", xml_declaration=True)

