#ifndef __SMALI_IO_H__
#define __SMALI_IO_H__

#include <stdio.h>
#include "smalitype.h"

namespace smali {

int recursive_mkdir(const char *dirname);
long write_block_metadata(codelist_t &codes, methodlist_t &methods, const char *filename);
unsigned load_block_metadata(const char *filename, metadatalist_t& meta);
int write_smali_hooked(codelist_t &codes, const char *dirname,
	void (*pre)(FILE*, code_t&), void (*mid)(FILE *, code_t&), void (*post)(FILE*, code_t&)); 
int write_smali(codelist_t &codes, const char *dirname);

}

#endif	/* __SMALI_IO_H__ */
