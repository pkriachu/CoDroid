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


codroid_conf = '../codroid.conf'

codroid_root = '/home/pkriachu/codroid'

remote_lifetime = 259200
patched_lifetime = 86400
submit_url = ""

#lock_file = '/tmp/codroid_cron_lock'
#sleep_interval = 60
#lock_interval = 2 * sleep_interval



def init() :
    config = ConfigParser.ConfigParser()
    config.read(codroid_conf)

    codroid_root = config.get('global', 'codroid_root')
    patched_lifetime = config.get('global', 'patched_lifetime')

    remote_lifetime = config.get('remote', 'remote_lifetime')
    submit_url = config.get('remote', 'submit_url')



def parseRemoteRecords(file_id) : 
    records_root = "%s/records/%d" % (cordroid_root, file_id)



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






if __name__ == "__main__" :
    #    if is_executed() :
#        sys.exit("* CoDroid is already executed.")

    parseRemoteRecords()

