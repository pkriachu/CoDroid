#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include "smaliutil.h"

using namespace std;
using namespace smali;

int
main(int argc, char *argv[]) {
	metadatalist_t meta;
	metadatalist_t::iterator mi;
	unsigned lines = 0;
	unsigned methods = 0;
	unsigned classes = 0;
	unsigned natives = 0;
	char *progname = strdup(argv[0]);
	FILE *fdump = NULL;

	int ch;

	while((ch = getopt(argc, argv, "d:h")) != -1) {
		switch(ch) {
		case 'd':
			fdump = fopen(optarg, "wt");
			if(fdump == NULL) {
				fprintf(stderr, "# open dumpfile failed: %s\n", strerror(errno));
			}
			break;
		case 'h':
		default:
usage:
			fprintf(stderr, "*** dump meta data file\n"
					"usage: %s filename.meta\n"
					"\t-d dumpfile: dump info to the given file\n"
					"", progname);
			return -1;
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if(argc <= 0)
		goto usage;

	fprintf(stderr, "# %u metadata loaded.\n", load_block_metadata(argv[0], meta));

	for(mi = meta.begin(); mi != meta.end(); mi++) {
		lines += mi->second.raw.lines;
		if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_CLASS)	classes++;
		if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_METHOD)	methods++;
		if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_NATIVE)	natives++;
		//
		if(fdump == NULL)
			continue;
		fprintf(fdump, "[%c%c%c%c%c]	%x	%s	%s\n",
			mi->second.raw.blockflags & SMALI_BLOCK_FLAG_BLOCK_BEGIN ? 'S' : '.',
			mi->second.raw.blockflags & SMALI_BLOCK_FLAG_BLOCK_END ? 'E' : '.',
			mi->second.raw.blockflags & SMALI_BLOCK_FLAG_CLASS ? 'C' : '.',
			mi->second.raw.blockflags & SMALI_BLOCK_FLAG_METHOD ? 'M' : '.',
			mi->second.raw.blockflags & SMALI_BLOCK_FLAG_NATIVE ? 'N' : '.',
			mi->second.raw.blockid,
			mi->second.classname.c_str(),
			mi->second.methodname.c_str());
	}

	fprintf(stderr, "# %lu blocks, %u lines, %u methods, %u natives, %u classes\n",
		meta.size(), lines, methods, natives, classes);

	return 0;
}

