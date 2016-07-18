#!/usr/bin/env python
#-*- cpdomg" itf-8 -*-

# Author: Kaze Mewno (C.H. Chiu)
# Last Modify: 2016.6.29
#
# This is a cron program that doing following jobs:
#   1. check the upload list and process files in it.
#   2. check the expire time of the files that has been modified.



import sys, os
import subprocess
import time
import struct
import sqlite3
import hashlib
import ConfigParser

sys.path.append("/home/pkriachu/androguard/")
sys.path.append("/home/pkriachu/androguard/elsim/")
from androguard.core.bytecodes.apk import *


codroid_conf = '../codroid.conf'

codroid_root = '/home/pkriachu/codroid'

patched_life_time = 0

#lock_file = '/tmp/codroid_cron_lock'
#sleep_interval = 60
#lock_interval = 2 * sleep_interval



def init() :
    config = ConfigParser.ConfigParser()
    config.read(codroid_conf)

    try :
        codroid_root = config.get('global', 'codroid_root')
        file_life_time = config.get('global', 'patched_life_time')



def process_uploads() :
    codroid_db = codroid_root + '/codroid.db'
    conn = sqlite3.connect(codroid_db)
    conn.row_factory = sqlite3.Row
    query = conn.cursor()
    insert = conn.cursor()
    delete = conn.cursor()

    # check file exists
    query.execute('SELECT * FROM uploads')
    for row in query :
        if row['modify_type'] == 'local' :
            # do patch with local mode
            print row
        elif row['modify_type'] == 'remote' :
            # do patch with remote mode
            print row

        # Modify and rebuild apk files
        patch_script = "%s/tools/patch.sh" % (codroid_root)
        patch_file = row['file_name']
        patch_mode = row['modify_type']
        out = subprocess.call([patch_script, patch_file, patch_mode])

        # append apk infomation to database
        apk_path = "%s/uploads/%s" % (codroid_root, patch_file)
        apk = APK(apk_path)

        apk_id = apk.get_package()
        apk_version = "%s/%s" % (apk.get_androidversion_code(), apk.get_androidversion_name())
        apk_hash = hash_file(apk_path)
        uploader = row['uploader']
        modify_type = patch_mode

        insert.execute("INSERT INTO apks(apk_id, apk_version, apk_hash, uploader, type, process_time) VALUES (?, ?, ?, ?, ?, datetime('now', 'localtime'))", 
                (apk_id, apk_version, apk_hash, uploader, modify_type ))

        file_id = insert.lastrowid
        file_path = "%s/downloads/%s" % (codroid_root, patch_file)

        insert.execute("INSERT INTO files VALUES(?, ?)", (file_id, file_path))

        # remove file from uploads
        delete.execute("DELETE FROM uploads WHERE file_name = '%s'" % (row['file_name']))
        os.remove("%s/uploads/%s" % (codroid_root, row['file_name']))

    conn.commit()

    conn.close()
    return



def process_expire() :
    return



def is_executed() :
    status = False
    try :
        #sys.stderr.write('* Check CoDroid executeion status...\n')

        # This command will check how much codroid_cron.py is running, include itself.
        out = subprocess.check_output('ps ax | grep codroid_cron.py | grep python | grep -cv grep', shell=True)

        #sys.stderr.write('* out=%s\n' % (out))

        if int(out) == 1 :
            status = False
        else :
            status = True

    except :
        #sys.stderr.write('* Exception: %s\n' % (sys.exc_info()[0]))
        status = False

    return status



def hash_file(file_path) :
    # read stuff in 64kb chunks
    BUF_SIZE = 65536

    hasher = hashlib.md5()

    with open(file_path) as fp :
        while True:
            data = fp.read(BUF_SIZE)
            if not data :
                break
            hasher.update(data)

    return hasher.hexdigest()





if __name__ == "__main__" :
    if is_executed() :
        sys.exit("* CoDroid is already executed.")

    process_uploads()


