#ifndef __SMALI_PARSE_H__
#define __SMALI_PARSE_H__

#include "smalitype.h"

namespace smali {

std::string timestamp(bool showdiff = false);

unsigned tokenize(const char *string, const char *delim, tokens_t &tokens);

int	find_files(const char *rootdir, const char *suffix, pathlist_t &pathlist);
int	load_a_file(const char *smali, codelist_t &codes, classlist_t &classlist, methodlist_t &methodlist);
int	load_files(pathlist_t &pathlist, codelist_t &codes, classlist_t &classlist, methodlist_t &methodlist);

int	callgraph_build(const char *smali, methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee);
int	callgraph_build(codelist_t &codes, methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee);
void	callgraph_remove_external(methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee);
void	callgraph_methods_used(methodlist_t &methodused, methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee);

}

#endif /* __SMALI_PARSE_H__ */
