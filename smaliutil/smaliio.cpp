#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "smaliutil.h"

using namespace std;

namespace smali {

/**
 * Create a directory
 *
 * @param dirname [in] the name of the directory
 * @returns 0 on success, or -1 on error
 * 
 * This function create directories in 0755 mode, and return success even if 
 * the target directory already exists.
 */
static int my_mkdir(const char *dirname) {
	if(mkdir(dirname, 0755) < 0) {
		if(errno == EEXIST)
			return 0;
		return -1;
	}
	return 0;
}

/**
 * Create a directory string based on given tokens.
 *
 * @param v [in] directory tokens.
 * @returns the directory string.
 *
 * For example, given tokens 'a', 'b', 'c', it returns the string "a/b/c".
 */
static string makepath(const vector<string> &v) {
	string s = "";
	for(unsigned i = 0; i < v.size(); i++) {
		if(v[i] != "/") {
			s += v[i] + "/";
		} else {
			s += "/";
		}
	}
	return s;
}

/**
 * Create directories recursively.
 *
 * @param dirname [in] the directory name to be created.
 * @returns 0 on success, or -1 on error
 *
 * This function creates directories recursively.
 * Like what 'mkdir -p' does, which make parent directories as needed.
 */
int
recursive_mkdir(const char *dirname) {
	vector<string> tokens, temp;
	// fast path
	if(my_mkdir(dirname) == 0)
		return 0;
	// slow path
	tokenize(dirname, "/", tokens);
	if(dirname[0] == '/')
		tokens.insert(tokens.begin(), "/");
	// backward test
	temp = tokens;
	while(temp.size() > 0) {
		if(my_mkdir(makepath(temp).c_str()) == 0)
			break;
		temp.pop_back();
	}
	if(temp.size() == 0) {
		return -1;
	}
	// forward creation
	for(unsigned i = temp.size(); i < tokens.size(); i++) {
		temp.push_back(tokens[i]);
		if(my_mkdir(makepath(temp).c_str()) != 0)
			return -1;
	}
	//
	return 0;
}

/**
 * Output block metadata to a file.
 *
 * @param codes [in] Codes with block info.
 * @param methods [in] Methods with block info.
 * @param filename [in] Output filename.
 * @returns Metadata file size in bytes.
 *
 * The metadata file contains block id, block flags, block size (by lines), class name, and method name.
 * The header format is defined by block_metadata_t structure.
 */
long
write_block_metadata(codelist_t &codes, methodlist_t &methods, const char *filename) {
	methodlist_t::iterator mi;
	codelist_t::iterator ci;
	FILE *fout = NULL;
	long size = 0;
	//
	if((fout = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "# smali/write_metadata: unable to open %s (%s)\n",
			filename, strerror(errno));
		return -1;
	}
	//
	for(mi = methods.begin(); mi != methods.end(); mi++) {
		if((ci = codes.find(mi->second.classname)) == codes.end()) {
			fprintf(stderr, "# smaliIO/write_block_metadata: unable to find class %s\n", mi->second.classname.c_str());
			exit(1);
		}
		for(unsigned i = mi->second.start+1; i < ci->second.size() && i <= mi->second.end; i++) {
			block_metadata_t dat;
			unsigned lines = 0;
			//
			if(ci->second[i].blockid == 0)
				continue;
			if((ci->second[i].blockflags & (SMALI_BLOCK_FLAG_BLOCK_BEGIN|SMALI_BLOCK_FLAG_NATIVE)) == 0)
				continue;
			// detect block size (in the unit of lines)
			for(unsigned j = i+1; j < ci->second.size() && j <= mi->second.end; j++) {
				if(ci->second[j].instruction != "") {
					if(ci->second[j].instruction[0] != ':')
						lines++;
				}
				if(ci->second[j].blockflags & SMALI_BLOCK_FLAG_BLOCK_END)
					break;
			}
			//
			dat.blockid = htonl(ci->second[i].blockid);
			dat.blockflags = htonl(ci->second[i].blockflags);
			dat.lines = htonl(lines);
			dat.classlen = htonl(mi->second.classname.size() + 1);
			dat.methodlen = htonl(mi->second.methodname.size() + 1);
			//
			if(fwrite(&dat, sizeof(dat), 1, fout) != 1
			|| fwrite(mi->second.classname.c_str(), mi->second.classname.size() + 1, 1, fout) != 1
			|| fwrite(mi->second.methodname.c_str(), mi->second.methodname.size() + 1, 1, fout) != 1) {
				fprintf(stderr, "# smaliIO/write_block_metadata: write failed (%s)\n",
					strerror(errno));
				exit(1);
			}
		}
	}
	//
	size = ftell(fout);
	fclose(fout);
	//
	return size;
}

/**
 * Load meta data from metadata file
 *
 * @param filename [in] the metadata filename
 * @param meta [in/out] store the metadata
 * @returns number of metadata loaded
 */
unsigned
load_block_metadata(const char *filename, metadatalist_t& meta) {
	FILE *fin;
	char buf[10240];
	block_metadata_t block;
	//
	if((fin = fopen(filename, "rb")) == NULL)
		return 0;
	//
	while(fread(&block, sizeof(block), 1, fin) == 1) {
		metadata_t m;
		m.raw.blockid = ntohl(block.blockid);
		m.raw.blockflags = ntohl(block.blockflags);
		m.raw.lines = ntohl(block.lines);
		m.raw.classlen = ntohl(block.classlen);
		m.raw.methodlen = ntohl(block.methodlen);
		//
		if(m.raw.classlen <= sizeof(buf)) {
			fread(buf, m.raw.classlen, 1, fin);
			m.classname = buf;
		} else {
			fprintf(stderr, "# class name truncated for block id %x (%u)\n",
				m.raw.blockid, m.raw.classlen);
			fread(buf, sizeof(buf)-1, 1, fin);
			fseek(fin, m.raw.methodlen - sizeof(buf) + 1, SEEK_CUR);
			buf[sizeof(buf)] = '\0';
			m.classname = buf;
		}
		//
		if(m.raw.methodlen <= sizeof(buf)) {
			fread(buf, m.raw.methodlen, 1, fin);
			m.methodname = buf;
		} else {
			fprintf(stderr, "# method name truncated for block id %x (%u)\n",
				m.raw.blockid, m.raw.methodlen);
			fread(buf, sizeof(buf)-1, 1, fin);
			fseek(fin, m.raw.methodlen - sizeof(buf) + 1, SEEK_CUR);
			buf[sizeof(buf)] = '\0';
			m.methodname = buf;
		}
		//
		meta[m.raw.blockid] = m;
	}
	//
	fclose(fin);
	return meta.size();
}

/**
 * Write smali files to a given directory.
 *
 * @param codes [in] Codes to be written.
 * @param dirname [in] Output directory.
 * @param pre [in] Callback function. Called before output a single line. Pass NULL if no callback is required.
 * @param mid [in] Callback function. Called when writing the line. Pass NULL if no callback is required.
 * @param post [in] Callback function. Called after output a single line. Pass NULL if no callback is required.
 * @returns Number of files written, or -1 on error.
 */
int
write_smali_hooked(codelist_t &codes, const char *dirname,
		void (*pre)(FILE*, code_t&), void (*mid)(FILE *, code_t&), void (*post)(FILE*, code_t&)) {
	codelist_t::iterator ci;
	FILE *fout;
	int count = 0;
	for(ci = codes.begin(); ci != codes.end(); ci++) {
		string classname = ci->first;
		string filename = dirname;
		string dirname;
		char mydir[1024];
		//
		if(filename[filename.size()-1] != '/')
			filename += "/";
		filename += classname.substr(1, classname.size()-2) + ".smali";
		//
		snprintf(mydir, sizeof(mydir), "%s", filename.c_str());
		dirname = ::dirname(mydir);
		//
		if(recursive_mkdir(dirname.c_str()) < 0) {
			fprintf(stderr, "# smaliIO/write_smali: mkdir failed.\n");
			return -1;
		}
		//
		if((fout = fopen(filename.c_str(), "wt")) == NULL) {
			fprintf(stderr, "# smaliIO/write_smali: open %s failed.\n", filename.c_str());
			return -1;
		}
		for(unsigned i = 0; i < ci->second.size(); i++) {
			if(pre)		pre(fout, ci->second[i]);
			if(mid == NULL) {
					fprintf(fout, "%s\n", ci->second[i].raw.c_str());
			} else {
					mid(fout, ci->second[i]);
			}
			if(post)	post(fout, ci->second[i]);
		}
		fclose(fout);
		count++;
	}
	return count;
}

/**
 * Write smali files to a given directory.
 *
 * @param codes [in] Codes to be written.
 * @param dirname [in] Output directory.
 * @return Number of files written, or -1 on error.
 */
int
write_smali(codelist_t &codes, const char *dirname) {
	return write_smali_hooked(codes, dirname, NULL, NULL, NULL);
}

}

