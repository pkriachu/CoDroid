#!/usr/bin/env python
#-*- cpdomg" itf-8 -*-

# Author: Kaze Mewno (C.H. Chiu)
# Last Modify: 2016.6.29
#
# This is a cron program that doing following jobs:
#   1. check the upload list and process files in it.
#   2. check the expire time of the files that has been modified.



import sys, os
import struct
import sqlite3

codroid_root = '/home/pkriachu/codroid'



def process_uploads() :
    codroid_db = codroid_root + '/codroid.db'
    conn = sqlite3.connect(codroid_db)
    conn.row_factory = sqlite3.Row
    query = conn.cursor()
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

        delete.execute("DELETE FROM uploads WHERE file_name = '%s'" % (row['file_name']))

    conn.commit()

    conn.close()
    return



def process_expire() :
    return



if __name__ == "__main__" :
    process_uploads()


