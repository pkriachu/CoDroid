#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>

#include "smaliutil.h"

using namespace std;
using namespace smali;

#define	MAGIC1	0xefcdab9078563412LL
#define	MAGIC2	0xffffffffffffffffLL

unsigned long long
ntohll(unsigned long long x) {
	unsigned long long y;
	unsigned char *px = (unsigned char *) &x;
	unsigned char *py = (unsigned char *) &y;
	for(unsigned i = 0; i < sizeof(x); i++) {
		py[sizeof(x)-i-1] = px[i];
	}
	return y;
}

int
main(int argc, char *argv[]) {
	metadatalist_t meta;
	metadatalist_t::iterator mi;
	unsigned lines = 0;
	map<string,int> methods;
	map<string,int> classes;
	map<string,int> natives;
	char *progname = strdup(argv[0]);
	FILE *fdump = NULL;
	FILE *fresult = NULL;
	unsigned long long record[2];

	int ch;

	if(sizeof(record) != 16) {
		fprintf(stderr, "# FATAL: 2*sizeof(long long) is not equal to 16 bytes\n");
		return -1;
	}

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
			fprintf(stderr, "*** dump and count local report ratio\n"
					"usage: %s filename.meta codroid.report.out\n"
					"\t-d dumpfile: dump info to the given file\n"
					"", progname);
			return -1;
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if(argc <= 1)
		goto usage;

	fprintf(stderr, "# %u metadata loaded.\n", load_block_metadata(argv[0], meta));

	for(mi = meta.begin(); mi != meta.end(); mi++) {
		lines += mi->second.raw.lines;
		if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_CLASS)	classes[mi->second.classname]++;
		if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_METHOD)	methods[mi->second.classname + "->" + mi->second.methodname]++;
		if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_NATIVE)	natives[mi->second.classname + "->" + mi->second.methodname]++;
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

	fprintf(stderr, "# identified: %lu blocks, %u lines, %lu methods, %lu natives, %lu classes\n",
		meta.size(), lines, methods.size(), natives.size(), classes.size());

	if((fresult = fopen(argv[1], "rb")) == NULL) {
		fprintf(stderr, "# unable to open %s: %s\n", argv[1], strerror(errno));
		return -2;
	}

#define	ST_READ_HEADER	0
#define	ST_READ_NAME	1
#define	ST_READ_REST	2
#define	ST_ILLEGAL	3

	int state = ST_READ_HEADER;
	unsigned long long minTimestamp = -1LL, maxTimestamp = 0;
	string packageId;
	map<unsigned long long, unsigned long long> touched;
	// covered statistics
	int clines = 0;
	map<string,int> cmethods;
	map<string,int> cnatives;
	map<string,int> cclasses;

	while(fread(record, sizeof(record), 1, fresult) == 1 && state != ST_ILLEGAL) {
		unsigned long long blockId, blockVal;
		switch(state) {
		case ST_READ_HEADER:
			if(ntohll(record[0]) != MAGIC1) {
				fprintf(stderr, "# bad magic number %llx\n", record[0]);
				state = ST_ILLEGAL;
				break;
			}
			fprintf(stderr, "# codroidId = 0x%llx\n", ntohll(record[1]));
			state = ST_READ_NAME;
			break;
		case ST_READ_NAME:
			if(ntohll(record[0]) == MAGIC1) {
				int i;
				char *ptr = (char*) &record[1];
				for(i = 0; i < 8 && ptr[i]; i++) {
					packageId += ptr[i];
				}
			} else {
				fprintf(stderr, "# package = %s\n", packageId.c_str());
				state = ST_READ_REST;
				goto read_rest;
			}
			break;
		case ST_READ_REST:
read_rest:
			blockId = ntohll(record[0]);
			blockVal = ntohll(record[1]);
			if(blockId == (unsigned long long) -1LL) {
				if(blockVal < minTimestamp)	minTimestamp = blockVal;
				if(blockVal > maxTimestamp)	maxTimestamp = blockVal;
			} else {
				if(touched.find(blockId) == touched.end()) {
					touched[blockId] = blockVal;
					clines += meta[blockId].raw.lines;
					if(meta[blockId].raw.blockflags & SMALI_BLOCK_FLAG_CLASS)	
						cclasses[meta[blockId].classname]++;
					if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_METHOD)
						cmethods[meta[blockId].classname + "->" + meta[blockId].methodname]++;
					if(mi->second.raw.blockflags & SMALI_BLOCK_FLAG_NATIVE)
						cnatives[meta[blockId].classname + "->" + meta[blockId].methodname]++;
				} else {
					touched[blockId] += blockVal;
				}
			}
			break;
		}
	}

	fclose(fresult);

	fprintf(stderr, "# covered: %lu blocks, %u lines, %lu methods, %lu natives, %lu classes\n",
		touched.size(), lines, cmethods.size(), cnatives.size(), cclasses.size());

	return 0;
}

