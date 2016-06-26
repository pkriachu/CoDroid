#ifndef __SMALI_CFG_H__
#define __SMALI_CFG_H__

#include "smalitype.h"

namespace smali {

void controlflow_scan_blocks(codelist_t &codes, methodlist_t &methods);
unsigned int controlflow_remove_blocks(codelist_t &codes, methodlist_t &methods);
void controlflow_optimize_blocks(codelist_t &codes, methodlist_t &methods);
unsigned controlflow_assign_blockid(codelist_t &codes, methodlist_t &methods, unsigned startid);
unsigned controlflow_assign_blockid(codelist_t &codes, methodlist_t &methods, const char **ordered_methods);

}

#endif
