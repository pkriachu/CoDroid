#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include "smaliutil.h"

using namespace std;
using namespace smali;

static const char *progname = "undefined";
static char *apkdir = NULL;
static FILE *fdump = NULL;
static char *filedump = NULL;
static char *filemeta = NULL;
static char *dirout = NULL;
static const char *_localwriter = "LocalWriter";
static const char *_networkwriter = "NetworkWriter";
static const char *writer = _localwriter;

int mylog(const char *fmt, ...) {
	int ret;
	char ts[64];
	va_list ap;
	snprintf(ts, sizeof(ts), "%s", timestamp(true).c_str());
	va_start(ap, fmt);
	fprintf(stderr, "# %s ", ts);
	ret = vfprintf(stderr, fmt, ap);
	va_end(ap);
	return ret; 
}

int
usage() {
	fprintf(stderr, "# usage: %s [options ...] apk-dir\n"
			"\t-d dumpfile: dump source codes, use '-' for writing to stdout\n"
			"\t-m metadata: output block metadata to the given filename\n"
			"\t-o directory: output directory\n"
			"\t-l: use LocalWriter, default is %s\n"
			"\t-n: use NetworkWriter, default is %s\n"
			"", progname, writer, writer
	       );
	return -1;
}

int
parse_opt(int argc, char *argv[]) {
	int ch;

	progname = strdup(argv[0]);

	while((ch = getopt(argc, argv, "d:lm:no:")) != -1) {
		switch(ch) {
		case 'd':
			filedump = strdup(optarg);
			if(strcmp(optarg, "-") == 0) {
				fdump = stdout;
				fprintf(stderr, "# enable source code dump to stdout\n");
			} else {
				if((fdump = fopen(optarg, "wt")) != NULL) {
					fprintf(stderr, "# enable source code dump to %s\n", optarg);
				} else {
					fprintf(stderr, "# enabling source code dump failed (%s)\n", strerror(errno));
				}
			}
			break;
		case 'l':
			writer = _localwriter;
			break;
		case 'm':
			filemeta = strdup(optarg);
			fprintf(stderr, "# enable metadata output to %s\n", filemeta);
			break;
		case 'n':
			writer = _networkwriter;
			break;
		case 'o':
			dirout = strdup(optarg);
			fprintf(stderr, "# write source codes to %s\n", dirout);
			break;
		default:
			return usage();
		}
	}

	argc -= optind;
	argv += optind;

	if(argc <= 0) {
		exit(usage());
		return -1;
	}

	apkdir = strdup(argv[0]);

	return 0;
}

static callgraph_t	caller;	// a calls b -> caller[a] = b]
static callgraph_t	callee;	// b is caled by a -> callee[b] = a

static int
comp_callee(const void *a, const void *b) {
	callgraph_t::iterator ci;
	unsigned sa = 0;
	unsigned sb = 0;
	const char *va = *((const char **) a);
	const char *vb = *((const char **) b);
	//
	if((ci = callee.find(va)) != callee.end())
		sa = ci->second.size();
	if((ci = callee.find(vb)) != callee.end())
		sb = ci->second.size();
	if(sa > sb)
		return -1;
	if(sa < sb)
		return 1;
	return 0;
}

static const char**
sort_methods(methodlist_t &methods) {
	methodlist_t::iterator mi;
	unsigned i;
	const char **ret = NULL;
	if((ret = (const char**) malloc(sizeof(const char*) * methods.size())) == NULL)
		return NULL;
	for(i = 0, mi = methods.begin(); mi != methods.end(); i++, mi++) {
		ret[i] = mi->first.c_str();
	}
	qsort(ret, methods.size(), sizeof(const char*), comp_callee);
	return ret;
}

void
dump_source(codelist_t &codelist, classlist_t &classlist, methodlist_t &methodlist) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	for(mi = methodlist.begin(); mi != methodlist.end(); mi++) {
		fprintf(fdump, "#### %s: locals %d {%d--%d}\n",
			mi->first.c_str(),
			mi->second.locals,
			mi->second.start, mi->second.end);
		if((ci = codelist.find(mi->second.classname)) == codelist.end()) {
			mylog("unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start; i < ci->second.size() && i <= mi->second.end; i++) {
			char blockstate[16] = "";
			if(ci->second[i].blockflags != 0) {
				fprintf(fdump, "[%c%c%c%c%c]",
					ci->second[i].blockflags & SMALI_BLOCK_FLAG_BLOCK_BEGIN ? 'S' : '.',
					ci->second[i].blockflags & SMALI_BLOCK_FLAG_BLOCK_END ? 'E' : '.',
					ci->second[i].blockflags & SMALI_BLOCK_FLAG_CLASS ? 'C' : '.',
					ci->second[i].blockflags & SMALI_BLOCK_FLAG_METHOD ? 'M' : '.',
					ci->second[i].blockflags & SMALI_BLOCK_FLAG_NATIVE ? 'N' : '.');
			} else {
				fprintf(fdump, "[.....]");
			}
			fprintf(fdump, "%s\t%s\n", blockstate, ci->second[i].raw.c_str());
		}
	}
	return;
}

void
test_goto(codelist_t &codelist, classlist_t &classlist, methodlist_t &methodlist) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	for(mi = methodlist.begin(); mi != methodlist.end(); mi++) {
		//// from methodlist, we have
		// full method name: mi->first.c_str()
		// class name:	mi->second.classname
		// method name:	mi->second.methodname
		// method flags: mi->second.flags
		// method ranges: [mi->second.start, mi->second.end]
		//// from codelist, we have
		// each line of codes: ci->second[i], index starts from zero
		// each line: 
		if((ci = codelist.find(mi->second.classname)) == codelist.end()) {
			mylog("unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start; i < ci->second.size() && i <= mi->second.end; i++) {
			if(ci->second[i].instruction == "goto"
			|| ci->second[i].instruction == "goto/16"
			|| ci->second[i].instruction == "goto/32") {
				for(unsigned j = i+1; j < ci->second.size() && j <= mi->second.end; j++) {
					if(ci->second[j].instruction[0] == ':')
						break;
					if(ci->second[j].instruction == ".end")
						break;
					if(ci->second[j].instruction != "") {
						printf("%s\n", ci->second[j].raw.c_str());
						break;
					}
				}
			}
		}
	}
	return;
}

static inline unsigned
encoding_mask(unsigned blockid, unsigned &mask, unsigned &shift) {
	mask = 0xf0000000;
	shift = 28;
	while((blockid & mask) == 0) {
		mask >>= 4;
		shift -= 4;
	}
	return mask;
}

static void
line_pre(FILE *fout, code_t &code) {
	unsigned oldmask, mask, shift;
	if(code.blockflags & SMALI_BLOCK_FLAG_NATIVE) {
		oldmask = encoding_mask(code.blockid, mask, shift);
		fprintf(fout, "# native block id 0x%x\n", code.blockid);
#if 1
		while(mask != 0) {
			fprintf(fout, "invoke-static {}, Lorg/codroid/utility/%s;->value%s%X%s()V\n",
				writer,
				mask == oldmask ? "I" : "",
				(code.blockid & mask)>>shift,
				mask == 0x0f? "C" : "");
			mask >>= 4;
			shift -= 4;
		}
#endif
	}
	return;
}

static void
line_mid(FILE *fout, code_t &code) {
	if(code.instruction == "goto") {
		vector<string> tokens;
		if(tokenize(code.raw.c_str(), " \t\n\r,", tokens) > 1) {
			code.instruction = "goto/16";
			code.raw = "goto/16 ";
			code.raw += tokens[1];
		}
	}
	fprintf(fout, "%s\n", code.raw.c_str());
	return;
}

static void
line_post(FILE *fout, code_t &code) {
	unsigned oldmask, mask, shift;
	if(code.blockflags & SMALI_BLOCK_FLAG_BLOCK_BEGIN) {
		oldmask = encoding_mask(code.blockid, mask, shift);
		fprintf(fout, "# block id 0x%x\n", code.blockid);
#if 1
		while(mask != 0) {
			fprintf(fout, "invoke-static {}, Lorg/codroid/utility/%s;->value%s%X%s()V\n",
				writer,
				mask == oldmask ? "I" : "",
				(code.blockid & mask)>>shift,
				mask == 0x0f? "C" : "");
			mask >>= 4;
			shift -= 4;
		}
#endif
	}
	return;
}

int
main(int argc, char *argv[]) {
	int n;
	pathlist_t	pathlist;
	codelist_t	codelist;
	classlist_t	classlist;
	methodlist_t	methodlist;

	const char	**ordered_methods = NULL;
	unsigned	maxid;

	if(parse_opt(argc, argv) < 0)
		return -1;
	// find files
	mylog("scanning directory [%s]\n", apkdir);
	if((n = find_files(apkdir, ".smali", pathlist)) <= 0) {
		fprintf(stderr, "# no valid file found\n");
		return -1;
	}
	mylog("%d file(s) found\n", n);
	// load all files
	load_files(pathlist, codelist, classlist, methodlist);
	mylog("%lu/%lu classes; %lu methods loaded\n",
		codelist.size(), classlist.size(), methodlist.size());
	// build call graph
	callgraph_build(codelist, methodlist, caller, callee);
	mylog("callgraph built\n");
	//
	controlflow_scan_blocks(codelist, methodlist);
	mylog("blocks scanned\n");
	//
	int rr;
	rr = controlflow_remove_blocks(codelist, methodlist);
	mylog("%u blocks removed\n", rr);
	//
	controlflow_optimize_blocks(codelist, methodlist);
	mylog("blocks optimized\n");
	//
	if((ordered_methods = sort_methods(methodlist)) == NULL) {
		fprintf(stderr, "sort methods failed.\n");
		return -1;
	}
#if 0
	for(unsigned i = 0; i < methodlist.size(); i++) {
		callgraph_t::iterator ci;
		if((ci = callee.find(ordered_methods[i])) == callee.end())
			printf("%s (0)\n", ordered_methods[i]);
		else
			printf("%s (%lu)\n", ordered_methods[i], ci->second.size());
	}
#endif
	maxid = controlflow_assign_blockid(codelist, methodlist, ordered_methods);
	mylog("block id assigned (max = %u)\n", maxid - 1);
	// dump methods 
	if(fdump) {
		dump_source(codelist, classlist, methodlist);
		mylog("source code dumped (%s)\n", filedump);
	}
	// test cases
	//test_goto(codelist, classlist, methodlist);
	//test_move_result(codelist, classlist, methodlist);

	if(fdump && fdump != stdout)
		fclose(fdump);
	//
	if(filemeta) {
		long sz;
		if((sz = write_block_metadata(codelist, methodlist, filemeta)) < 0) {
			mylog("write block metadata failed\n");
		} else {
			mylog("block meta data written (%s, %ld bytes)\n", filemeta, sz);
		}
	}
	//
	if(dirout) {
		int count;
		if((count = write_smali_hooked(codelist, dirout, line_pre, line_mid, line_post)) < 0) {
			mylog("write smali files failed\n");
		} else {
			mylog("%d smali files written\n", count);
		}
	}

	mylog("done.\n");

	return 0;
}

