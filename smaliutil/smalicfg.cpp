#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "smaliutil.h"

#define	STATE_FIND_FIRST	0
#define	STATE_FIND_NEXT		1
#define	STATE_SKIP_SWITCH	2
#define	STATE_SKIP_ANNOTATION	3

namespace smali {

/**
 * Scan source codes and generate block flags, which include BLOCK_START, BLOCK_END, 
 * CLASS, METHOD, NATIVE
 *
 * @param codse [in/out] list of codes to be optimized
 * @param methods [in/out] list of methods to be optmized
 */
void
controlflow_scan_blocks(codelist_t &codes, methodlist_t &methods) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	for(mi = methods.begin(); mi != methods.end(); mi++) {
		int state = STATE_FIND_FIRST;
		//// from methodlist, we have
		// full method name: mi->first.c_str()
		// class name:	mi->second.classname
		// method name:	mi->second.methodname
		// method flags: mi->second.flags
		// method ranges: [mi->second.start, mi->second.end]
		//// from codelist, we have
		// each line of codes: ci->second[i], index starts from zero
		// each line: 
		if((ci = codes.find(mi->second.classname)) == codes.end()) {
			fprintf(stderr, "# control-flow/scan_blocks: unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start + 1; i < ci->second.size() && i < mi->second.end; i++) {
			switch(state) {
			case STATE_FIND_NEXT:
				if(ci->second[i].instruction == "") {
					// do nothing
					break;
				} else if(ci->second[i].instruction[0] == ':') {
					// is a label
					// cannot insert codes between :label and the following:
					// .packed-switch .sparse-switch .catch .catchall move-exception
					if(ci->second[i+1].instruction != ".packed-switch"
					&& ci->second[i+1].instruction != ".sparse-switch"
					&& ci->second[i+1].instruction != ".array-data"
					&& ci->second[i+1].instruction != ".catch"
					&& ci->second[i+1].instruction != ".catchall"
					&& ci->second[i+1].instruction != "move-exception") {
						ci->second[i].blockflags |= SMALI_BLOCK_FLAG_BLOCK_BEGIN;
						ci->second[i].blockflags |= SMALI_BLOCK_FLAG_BLOCK_END;
					} else if( ci->second[i+1].instruction == ".packed-switch"
						|| ci->second[i+1].instruction == ".sparse-switch"
						|| ci->second[i+1].instruction == ".array-data") {
						state = STATE_SKIP_SWITCH;
					} else if( ci->second[i+1].instruction == ".catch"
						|| ci->second[i+1].instruction == ".catchall") {
						// simply skip this one
					} else if(ci->second[i+1].instruction == "move-exception") {
						ci->second[i+1].blockflags |= SMALI_BLOCK_FLAG_BLOCK_BEGIN;
					}
				} else if(strncmp("if-", ci->second[i].instruction.c_str(), 3) == 0) {
					// if-
					ci->second[i].blockflags |= SMALI_BLOCK_FLAG_BLOCK_END;
					ci->second[i].blockflags |= SMALI_BLOCK_FLAG_BLOCK_BEGIN;
				} else if( strncmp("return", ci->second[i].instruction.c_str(), 6) == 0
					|| strncmp("goto", ci->second[i].instruction.c_str(), 4) == 0) {
					// goto and return
					ci->second[i].blockflags |= SMALI_BLOCK_FLAG_BLOCK_END;
				} else if(ci->second[i].instruction.substr(0, 7) == "invoke-") {
					// handle JNI native methods
					tokens_t tokens;
					methodlist_t::iterator mj;
					//
					tokenize(ci->second[i].raw.c_str(), " \t\r\n", tokens);
					if((mj = methods.find(tokens.back())) != methods.end()) {
						if(mj->second.flags & SMALI_METHOD_FLAG_NATIVE)
							ci->second[i].blockflags |= SMALI_BLOCK_FLAG_NATIVE;
					}
				}
				break;
			case STATE_SKIP_SWITCH:
				if(ci->second[i].instruction == ".end") {
					tokens_t tokens;
				       	if(tokenize(ci->second[i].raw.c_str(), " \t\r\n", tokens) > 1) {
						if(tokens[1] == "packed-switch"
						|| tokens[1] == "sparse-switch"
						|| tokens[1] == "array-data"
						|| tokens[1] == "method") {
							state = STATE_FIND_NEXT;
						}
					}
				}
				break;
			case STATE_SKIP_ANNOTATION:
				if(ci->second[i].instruction == ".end") {
					tokens_t tokens;
				       	if(tokenize(ci->second[i].raw.c_str(), " \t\r\n", tokens) > 1) {
						if(tokens[1] == "annotation") {
							state = STATE_FIND_FIRST;
						}
					}
				}
				break;
			case STATE_FIND_FIRST:
				if(ci->second[i].instruction == "")
					break;
				if(ci->second[i].instruction == ".annotation") {
					state = STATE_SKIP_ANNOTATION;
					break;
				}
				if(ci->second[i].instruction[0] == '.')
					break;
				ci->second[i-1].blockflags |= SMALI_BLOCK_FLAG_BLOCK_BEGIN;
				ci->second[i-1].blockflags |= SMALI_BLOCK_FLAG_METHOD;
				if(mi->second.flags & SMALI_METHOD_FLAG_CONSTRUCTOR)
					ci->second[i-1].blockflags |= SMALI_BLOCK_FLAG_CLASS;
				state = STATE_FIND_NEXT;
				// the first is a label
				if(ci->second[i].instruction[0] == ':') {
					// is a label
					if(ci->second[i+1].instruction != ".packed-switch"
					&& ci->second[i+1].instruction != ".sparse-switch") {
						ci->second[i].blockflags |= SMALI_BLOCK_FLAG_BLOCK_BEGIN;
					} else {
						state = STATE_SKIP_SWITCH;
					}
				}
				//
				break;
			}
		}
		ci->second[mi->second.end].blockflags |= SMALI_BLOCK_FLAG_BLOCK_END;
	}
	return;
}

/**
 * Remove blocks that could cause Java VerifyError
 *
 * @param codes [in/out] list of codes to be processed.
 * @param methods [in/out] list of methods to be processed.
 * @returns number of removed blocks
 *
 * Usually we call this function right after controlflow_scan_blocks....
 */
unsigned int
controlflow_remove_blocks(codelist_t &codes, methodlist_t &methods) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	unsigned int removed = 0;
	for(mi = methods.begin(); mi != methods.end(); mi++) {
		if((ci = codes.find(mi->second.classname)) == codes.end()) {
			fprintf(stderr, "# control-flow/remove_blocks: unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start+1; i < ci->second.size() && i <= mi->second.end; i++) {
			unsigned j;
			if(ci->second[i].instruction == "monitor-exit") {
				// XXX: not sure why, but it looks like we always get a VerifyError
				// if we use invoke-static right after monitor-exit instruction
				for(j = i+1; j < ci->second.size() && j <= mi->second.end; j++) {
					if(ci->second[j].instruction == "")
						continue;
					if((ci->second[j].blockflags & SMALI_BLOCK_FLAG_BLOCK_BEGIN) == 0)
						break;
					ci->second[j].blockflags &= ~SMALI_BLOCK_FLAG_BLOCK_BEGIN;
					removed++;
				}
			}
			if(strncmp(ci->second[i].instruction.c_str(), "return", 6) == 0) {
				// XXX: not sure why, but it looks like we may get a VerifyError
				// for monitor-exit instruction if we insert invoke-static codes between
				// a label and a followed return* statements
				for(j = i-1; j > 0 && j < ci->second.size(); j--) {
					if(ci->second[j].instruction == "")
						continue;
					if(ci->second[j].instruction[0] != ':')
						break;
					ci->second[j].blockflags &= ~SMALI_BLOCK_FLAG_BLOCK_BEGIN;
					removed++;
				}
			}
		}
	}
	return removed;
}

/**
 * Remove redundant BLOCK_START and BLOCK_END
 *
 * @param codse [in/out] list of codes to be optimized
 * @param methods [in/out] list of methods to be optmized
 *
 * Usually we call this function right after controlflow_scan_blocks and controlflow_remove_blocks.
 */
void
controlflow_optimize_blocks(codelist_t &codes, methodlist_t &methods) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	for(mi = methods.begin(); mi != methods.end(); mi++) {
		if((ci = codes.find(mi->second.classname)) == codes.end()) {
			fprintf(stderr, "# control-flow/optimize_blocks: unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start+1; i < ci->second.size() && i <= mi->second.end; i++) {
			unsigned j = i+1;
			if((ci->second[i].blockflags & SMALI_BLOCK_FLAG_BLOCK_BEGIN) == 0)
				continue;
			if(ci->second[i].instruction[0] != ':')
				continue;
			while(ci->second[j].instruction == "" && j <= mi->second.end && j < ci->second.size())
				j++;
			if((ci->second[j].blockflags & SMALI_BLOCK_FLAG_BLOCK_BEGIN) == 0)
				continue;
			if(ci->second[j].instruction[0] != ':')
				continue;
			// remove redundant BLOCK_BEGIN
			ci->second[i].blockflags &= ~SMALI_BLOCK_FLAG_BLOCK_BEGIN;
			ci->second[j].blockflags &= ~SMALI_BLOCK_FLAG_BLOCK_END;
		}
		// TBD
	}
	return;
}

/**
 * Sequentially assign blockid to each identified blocks.
 *
 * @param codes [in/out] list of codes to scan
 * @param methdos [in/out] list of methdos to scan
 * @param startid [in] id for the first unassigned control flow block
 * @returns the next block id
 *
 * This function searches for BLOCK_BEGIN and NATIVE and assign block id
 * to blocks without block ids. The order of assignment is based on the order
 * listed in methods, which is usually alphabetically.
 * Block id is incremented by one after each assignment.
 */
unsigned
controlflow_assign_blockid(codelist_t &codes, methodlist_t &methods, unsigned startid) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	for(mi = methods.begin(); mi != methods.end(); mi++) {
		if((ci = codes.find(mi->second.classname)) == codes.end()) {
			fprintf(stderr, "# control-flow/assign_blockid: unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start + 1; i < ci->second.size() && i < mi->second.end; i++) {
			// already assigned?
			if(ci->second[i].blockid != 0)
				continue;
			// need to assign?
			if((ci->second[i].blockflags & (SMALI_BLOCK_FLAG_BLOCK_BEGIN|SMALI_BLOCK_FLAG_NATIVE)) == 0)
				continue;
			ci->second[i].blockid = startid++;
		}
	}
	return startid;
}

/**
 * Sequentially assign blockid to each identified blocks.
 *
 * @param codes [in/out] list of codes to scan
 * @param methdos [in/out] list of methdos to scan
 * @param ordered_metods [in] order of assignment
 * @returns the next block id
 *
 * This function searches for BLOCK_BEGIN and NATIVE and assign block id
 * to blocks without block ids.
 * The order of assignment is based on the the method names listed in \a ordered_methods.
 * Block id is always starts from 1. Usually we call this function and then call the
 * sequential version of controlflow_assign_blockid.
 */
unsigned
controlflow_assign_blockid(codelist_t &codes, methodlist_t &methods, const char **ordered_methods) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	unsigned blockid = 1;	// initial id must be greater than zero
	if(ordered_methods == NULL) {
		return controlflow_assign_blockid(codes, methods, blockid);
	}
	for(unsigned i = 0; i < methods.size(); i++) {
		if((mi = methods.find(ordered_methods[i])) == methods.end()) {
			fprintf(stderr, "# control-flow/assign_blockid: method %s not found\n", ordered_methods[i]);
			exit(1);
		}
		if((ci = codes.find(mi->second.classname)) == codes.end()) {
			fprintf(stderr, "# control-flow/assign_blockid: unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned j = mi->second.start + 1; j < ci->second.size() && j < mi->second.end; j++) {
			// already assigned?
			if(ci->second[j].blockid != 0)
				continue;
			// need to assign?
			if((ci->second[j].blockflags & (SMALI_BLOCK_FLAG_BLOCK_BEGIN|SMALI_BLOCK_FLAG_NATIVE)) == 0)
				continue;
			ci->second[j].blockid = blockid++;
		}
	}
	return controlflow_assign_blockid(codes, methods, blockid);
}

}

