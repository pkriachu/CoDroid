#!/usr/bin/env python
#-*- cpdomg" itf-8 -*-

# Author: Kaze Mewno (C.H. Chiu)
# Last Modify: 2016.6.29
#
# This is a cron program that doing following jobs:
#   1. check the upload list and process files in it.
#   2. check the expire time of the files that has been modified.



import sys, os
import shutil
import subprocess
import time
import struct
import sqlite3
import hashlib
import ConfigParser
from random import Random
import re

sys.path.append("/home/pkriachu/androguard/")
sys.path.append("/home/pkriachu/androguard/elsim/")
from androguard.core.bytecodes.apk import *


codroid_root = '/home/pkriachu/codroid'
codroid_conf = codroid_root + '/codroid.conf'

remote_lifetime = 259200
patched_lifetime = 86400
submit_url = ""

#lock_file = '/tmp/codroid_cron_lock'
#sleep_interval = 60
#lock_interval = 2 * sleep_interval



def init() :
    # reading common settings
    config = ConfigParser.ConfigParser()

    with open(codroid_conf, 'r') as conf :
        config.readfp(conf)

    global codroid_root, patched_lifetime, remote_lifetime, submit_url

    #codroid_root = config.get('global', 'codroid_root')
    patched_lifetime = int(config.get('global', 'patched_lifetime'))
    remote_lifetime = int(config.get('remote', 'remote_lifetime'))
    submit_url = config.get('remote', 'submit_url')



def process_uploads() : 
    codroid_db = codroid_root + '/codroid.db'
    conn = sqlite3.connect(codroid_db)
    conn.row_factory = sqlite3.Row
    query = conn.cursor()
    insert = conn.cursor()
    delete = conn.cursor()

    # check uploading files
    query.execute('SELECT * FROM uploads')
    for row in query :
        # common process
        # pre-processing: collect required infomation
        patch_file = row['file_name']
        patch_mode = row['modify_type']
        apk_path = "%s/uploads/%s" % (codroid_root, patch_file)
        apk = APK(apk_path)

        apk_id = apk.get_package()
        apk_version = "%s/%s" % (apk.get_androidversion_code(), apk.get_androidversion_name())
        apk_hash = row['file_hash']
        uploader = row['uploader']
        upload_time = row['upload_time']
        modify_type = patch_mode

        now = time.time()

        # append apk infomation to database
        insert.execute("INSERT INTO apks(apk_id, apk_version, apk_hash, uploader, type, upload_time, process_time) VALUES (?, ?, ?, ?, ?, ?, datetime(?, 'unixepoch', 'localtime'))",
                (apk_id, apk_version, apk_hash, uploader, modify_type, upload_time, now ))

        file_id = insert.lastrowid
        insert.execute("INSERT INTO files VALUES(?, ?)", (file_id, now+patched_lifetime))


        # local patching
        if patch_mode == 'local' :
            # patch apk files
            patch_script = "%s/tools/patch.sh" % (codroid_root)
            out = subprocess.call([patch_script, patch_file, patch_mode, str(file_id)])

        # remote patching
        elif patch_mode == 'remote' :
            # generating authentication code
            auth_code = ""
            cur = conn.cursor()
            # generate an unique auth code
            while auth_code == "" or cur.fetchone() != None :
                (auth_code, salt) = generate_auth_code(apk_id, uploader, now+remote_lifetime)
                cur.execute("SELECT fid FROM remote_auth WHERE authcode='%s'" % (auth_code))
            # save the auth info into database
            insert.execute("INSERT INTO remote_auth VALUES (?, ?, ?, ?)", (file_id, auth_code, salt, now+remote_lifetime))

            # pre-processing (smali): setting the transfer infomation
            with open(codroid_root + '/coverage/org_template/codroid/utility/NetworkWriterTask.smali', 'r') as source, open(codroid_root + '/coverage/org/codroid/utility/NetworkWriterTask.smali', 'w+') as target :
                content = source.read()
                content = content.replace('@SUBMIT_URL@', submit_url)
                content = content.replace('@KEY@', auth_code)
                target.write(content)
            with open(codroid_root + '/coverage/org_template/codroid/utility/Statistics.smali', 'r') as source, open(codroid_root + '/coverage/org/codroid/utility/Statistics.smali', 'w+') as target :
                content = source.read()
                target.write(content.replace('@PACKAGE_NAME@', apk_id))

            # patch apk files
            patch_script = "%s/tools/patch.sh" % (codroid_root)
            out = subprocess.call([patch_script, patch_file, "network", str(file_id)])

            # post-processing: generate the dir for records, and copy the metafile to it.
            records_dir = "%s/records/%s" % (codroid_root, file_id)
            metafile = "%s/downloads/%s.meta" % (codroid_root, file_id)
            os.mkdir(records_dir)
            if os.path.isfile(metafile) :
                shutil.copy(metafile, "%s/metafile" % (records_dir))

            # post-processing: generate the info file for DC-module
            config = ConfigParser.RawConfigParser()
            config.add_section('basic')
            config.set('basic', 'package', apk_id)
            config.set('basic', 'key', auth_code)
            with open(records_dir+'/info', 'w+') as info_file :
                config.write(info_file)

        # post_processing: remove file from uploads
        delete.execute("DELETE FROM uploads WHERE file_name = '%s'" % (row['file_name']))
        os.remove("%s/uploads/%s" % (codroid_root, row['file_name']))

    conn.commit()

    conn.close()
    return


def hash_file(file_path) :
    # read stuff in 64kb chunks
    BUF_SIZE = 65536
    hasher = hashlib.md5()
    with open(file_path, 'rb') as f:
        while True:
            data = f.read(BUF_SIZE)
            if not data :
                break
            hasher.update(data)
    return hasher.hexdigest()


def generate_auth_code(apk_id, uploader, timestamp) :
    hasher = hashlib.md5()
    salt = create_salt()
    hasher.update("%s%s%s%s" % (apk_id, uploader, timestamp , salt))
    return (hasher.hexdigest(), salt)


def create_salt(length = 4) :
    salt = ''
    chars = 'AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789'
    len_chars = len(chars) - 1
    random = Random()
    for i in xrange(length) :
        # choose a random character from chars
        salt += chars[random.randint(0, len_chars)] 
    return salt





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





def check_expire() :
    codroid_db = codroid_root + '/codroid.db'
    conn = sqlite3.connect(codroid_db)
    conn.row_factory = sqlite3.Row
    query = conn.cursor()
    insert = conn.cursor()
    delete = conn.cursor()

    # check files
    now = time.time()
    query.execute('SELECT * FROM files')
    for row in query :
        file_id = row['id']
        exp_time = row['exp_time']

        # if expired
        if exp_time <= now :
            pattern = "^%d\..+$" % file_id
            for file in os.listdir("%s/downloads"%(codroid_root)) :
                if re.match(pattern, file) :
                    os.remove("%s/downloads/%s"%(codroid_root, file))
            delete.execute("DELETE FROM files WHERE id = '%d'" % (file_id))
        conn.commit()
    conn.close()
    return





if __name__ == "__main__" :
    if is_executed() :
        sys.exit("* CoDroid is already executed.")

    init()
    process_uploads()
    check_expire()

