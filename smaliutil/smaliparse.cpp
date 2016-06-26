#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>

#include <vector>
#include "smaliparse.h"

using namespace std;
using namespace smali;

/**
 * tokenize a string
 *
 * @param string [in] the string to be tokenized
 * @param delim [in] list of delimiters
 * @param tokens [out] obtained tokens
 * @returns number of tokens
 */
unsigned
smali::tokenize(const char *string, const char *delim, tokens_t &tokens) {
	char *token, *saveptr, buf[65536];

	tokens.clear();

	strncpy(buf, string, sizeof(buf));
	token = strtok_r(buf, delim, &saveptr);
	while(token != NULL) {
		tokens.push_back(token);
		token = strtok_r(NULL, delim, &saveptr);
	}

	return tokens.size();
}

/**
 * Return the current timestamp in second.microsecond format
 *
 * @return a second.microsecond string
 */
std::string
smali::timestamp(bool showdiff) {
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	static struct timeval lasttv = { 0, 0 };
	long long delta = 0LL;
	struct timeval tv;
	char buf[64];
	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&mutex);
	if(showdiff && lasttv.tv_sec != 0) {
		delta = (tv.tv_sec - lasttv.tv_sec) * 1000000;
		delta += tv.tv_usec;
		delta -= lasttv.tv_usec;
	}
	lasttv = tv;
	pthread_mutex_unlock(&mutex);
#ifdef __APPLE__
#define TSFORMAT	"%ld.%06d"
#else
#define TSFORMAT	"%ld.%06ld"
#endif
	if(showdiff) {
		snprintf(buf, sizeof(buf), TSFORMAT "(+%.3f)", tv.tv_sec, tv.tv_usec, 0.000001 * delta);
	} else {
		snprintf(buf, sizeof(buf), TSFORMAT, tv.tv_sec, tv.tv_usec);
	}
#undef	TSFORMAT
	return buf;
}

/**
 * Find files recursively.
 * This is an internal function called by find_files
 *
 * @param rootdir [in] path to the directory
 * @param suffix [in] filename extention, can be NULL
 * @param suffixlen [in] length of suffix, must be 0 if suffix is NULL
 * @param pathlist [out] reference to the list storing filenames (in string)
 * @return number of matched files
 */
static int
find_files_recursive(const char *rootdir, const char *suffix, int suffixlen, pathlist_t &pathlist) {
	DIR *dir;
	int name_max, entlen, fcount = 0;
	struct dirent *ent, *result = NULL;
	struct stat st;

	if((dir = opendir(rootdir)) == NULL) {
		return 0;
	}

	name_max = pathconf(rootdir, _PC_NAME_MAX);
	if (name_max == -1)         /* Limit not defined, or error */
		name_max = 255;         /* Take a guess */
	entlen = sizeof(struct dirent) + name_max + 1;
	if((ent = (struct dirent *) malloc(entlen)) == NULL) {
		perror("malloc(dirent)");
		closedir(dir);
		return 0;
	}
	bzero(ent, entlen);

	while(readdir_r(dir, ent, &result) == 0) {
		string fullname = rootdir;
		if(result == NULL)
			break;
		if(strcmp(ent->d_name, ".") == 0)
			continue;
		if(strcmp(ent->d_name, "..") == 0)
			continue;
		fullname += "/";
		fullname += ent->d_name;
		if(stat(fullname.c_str(), &st) != 0)
			continue;
		if(S_ISDIR(st.st_mode)) {
			fcount += find_files_recursive(fullname.c_str(), suffix, suffixlen, pathlist);
			continue;
		}
		if(!S_ISREG(st.st_mode)) {
			continue;
		}
		if(suffixlen > 0) {
			if(strcmp(suffix, fullname.c_str()+fullname.size()-suffixlen) != 0)
				continue;
		}
		//fprintf(stderr, "# file: %s\n", fullname.c_str());
		pathlist.push_back(fullname);
		fcount++;
	}

	free(ent);
	closedir(dir);
	return fcount;
}

/**
 * Find files recursively.
 *
 * @param rootdir [in] path to the directory
 * @param suffix [in] filename extention, can be NULL
 * @param pathlist [out] reference to the list storing filenames (in string)
 * @return number of matched files
 */
int
smali::find_files(const char *rootdir, const char *suffix, std::list<std::string> &pathlist) {
	pathlist.clear();
	return find_files_recursive(rootdir, suffix, suffix == NULL ? 0 : strlen(suffix), pathlist);
}

/**
 * Load a file into memory.
 *
 * @parm smali [in] path to the smali filename
 * @param codes [out] store all loaded source codes
 * @param classlist [out] output identified classes
 * @param methodlist [out] output identified methods
 * @return always returns 0, or terminate program on error.
 *
 * The key to 'codes' and 'classlist' is the class name.
 * The key to 'methodlist' is 'classname->methodname', include method parameters and return value.
 * Note that each smali file must only contain one class.
 */
int
smali::load_a_file(const char *smali, codelist_t &codes, classlist_t &classlist, methodlist_t &methodlist) {
	FILE *fin;
	char buf[65536];
	string classname = "";
	string methodname = "";
	class_t myclass;
	//vector<string> lines;
	vector<code_t> lines;
	unsigned lineno = 0;
	unsigned method_locals = 0;
	unsigned method_flags = 0;
	unsigned method_start = 0;

	if((fin = fopen(smali, "rt")) == NULL)
		return 0;
	while(fgets(buf, sizeof(buf), fin) != NULL) {
		char *begin, *end;
		char *token, *saveptr;
		vector<string> tokens;
		string lasttoken = "";
		code_t code;
		// remove leading spaces
		for(begin = buf; *begin && isspace(*begin); begin++)
			;
		// move to end
		for(end = begin; *end; end++)
			;
		// remove tailing spaces
		for(--end; end >= begin; end--) {
			if(isspace(*end))
				*end = '\0';
			else
				break;
		}
		//
		code.raw = begin;
		lines.push_back(code);
		lineno++;
#define	DELIM		" \t\n\r\f\v"
		if((token = strtok_r(begin, DELIM, &saveptr)) == NULL)
			continue;
		lines.back().instruction = token;
		//
		if(strcmp(token, ".method") != 0
		&& strcmp(token, ".end") != 0
		&& strcmp(token, ".locals") != 0
		&& strcmp(token, ".field") != 0
		&& strcmp(token, ".super") != 0
		&& strcmp(token, ".class") != 0)
			continue;
		do {
			tokens.push_back(token);
		} while((token = strtok_r(NULL, DELIM, &saveptr)) != NULL);
#undef	DELIM
		if(tokens[0] == ".class") {
			if(classname != "") {
				fprintf(stderr, "# smali/load_a_file: duplicated class name definition @ %s:%u\n",
					classname.c_str(), lineno);
				exit(-1);
			}
			classname = tokens.back();
			myclass.classname = classname;
		} else if(tokens[0] == ".super") {
			myclass.super = tokens[1];
		} else if(tokens[0] == ".locals") {
			method_locals = strtol(tokens[1].c_str(), NULL, 0);
		} else if(tokens[0] == ".field") {
			string tmpname = "";
			field_t field;
			std::size_t pos;
			unsigned i;
			field.line = lineno-1;
			// identify the fieldname:type
			for(i = 1; i < tokens.size(); i++) {
				if(tokens[i] == "=") {
					tmpname = tokens[i-1];
					field.value = tokens[i+1];
					break;
				}
			}
			if(i == tokens.size())
				tmpname = tokens.back();
			// separate fieldname and type
			if((pos = tmpname.find(':')) != string::npos) {
				field.name = tmpname.substr(0, pos);
#define min(x, y)	((x) < (y) ? (x) : (y))
				field.type = tmpname.substr(min(pos+1, tmpname.size()));
#undef	min
			} else {
				field.name = tmpname;
				field.type = "___UNKNOWN___";
			}
			myclass.fields[field.name] = field;
		} else if(tokens[0] == ".method") {
			methodname = tokens.back();
			method_locals = 0;
			method_flags = 0;
			method_start = lineno-1;
			if(tokens.size() > 2) {
				for(unsigned i = 1; i < tokens.size()-1; i++) {
					if(tokens[i] == "abstract")		{ method_flags |= SMALI_METHOD_FLAG_ABSTRACT; }
					else if(tokens[i] == "bridge")		{ method_flags |= SMALI_METHOD_FLAG_BRIDGE; }
					else if(tokens[i] == "constructor")	{ method_flags |= SMALI_METHOD_FLAG_CONSTRUCTOR; }
					else if(tokens[i] == "declared-synchronized")	{ method_flags |= SMALI_METHOD_FLAG_DECLAREDSYNC; }
					else if(tokens[i] == "final")		{ method_flags |= SMALI_METHOD_FLAG_FINAL; }
					else if(tokens[i] == "native")		{ method_flags |= SMALI_METHOD_FLAG_NATIVE; }
					else if(tokens[i] == "private")		{ method_flags |= SMALI_METHOD_FLAG_PRIVATE; }
					else if(tokens[i] == "protected")	{ method_flags |= SMALI_METHOD_FLAG_PROTECTED; }
					else if(tokens[i] == "public")		{ method_flags |= SMALI_METHOD_FLAG_PUBLIC; }
					else if(tokens[i] == "static")		{ method_flags |= SMALI_METHOD_FLAG_STATIC; }
					else if(tokens[i] == "synthetic")	{ method_flags |= SMALI_METHOD_FLAG_SYNTHETIC; }
					else if(tokens[i] == "varargs")		{ method_flags |= SMALI_METHOD_FLAG_VARARGS; }
					else if(tokens[i] == "synchronized")	{ method_flags |= SMALI_METHOD_FLAG_SYNCHRONIZED; }
					else {
						fprintf(stderr, "smali/load_a_file[FATAL]: unknown .method specifiers (%s), PLEASE CHECK!\n", tokens[i].c_str());
						exit(1);
					}
				}
			}
		} else if(tokens[0] == ".end" && tokens.size() > 1 && tokens.back() == "method") {
			method_t m;
			m.classname = classname;
			m.methodname = methodname;
			m.flags = method_flags;
			m.locals = method_locals;
			m.start = method_start;
			m.end = lineno-1;
			methodlist[classname + "->" + methodname] = m;
			myclass.methods[methodname] = m;
		}
	}
	fclose(fin);
	//
	codes[classname] = lines;
	classlist[classname] = myclass;
	//
	return 0;
}

/**
 * Load files into memory. This function basically calls smali::load_files(filename, ...)
 *
 * @param pathlist [in] list of file paths to be loaded
 * @param codes [out] store all loaded source codes
 * @param classlist [out] output identified classes
 * @param methodlist [out] output identified methods
 * @return always returns 0
 */
int
smali::load_files(pathlist_t &pathlist, codelist_t &codes, classlist_t &classlist, methodlist_t &methodlist) {
	pathlist_t::iterator pi;
	for(pi = pathlist.begin(); pi != pathlist.end(); pi++) {
		smali::load_a_file((*pi).c_str(), codes, classlist, methodlist);
	}
	return 0;
}

/**
 * Add one line for caller/callee
 *
 * @param graph [in] The graph used to store the result
 * @param a [in] origin function
 * @param b [in] destination function
 *
 * This function can be used to build both forward and reverse call graph.
 * For example,
 * do callgraph_build_connect(caller_graph, a, b) and then
 * do callgraph_build_connect(callee_graph, b, a)
 */
static void
callgraph_build_connect(callgraph_t &graph, string a, string b) {
	callgraph_t::iterator ci;
	map<string,int>::iterator cj;

	if((ci = graph.find(a)) == graph.end()) {
		map<string,int> m;
		m[b] = 1;
		graph[a] = m;
		return;
	}

	if((cj = ci->second.find(b)) == ci->second.end()) {
		ci->second[b] = 1;
		return;
	}

	ci->second[b] = ci->second[b] + 1;
	return;
}

/**
 * Build callgraph from smali source codes
 *
 * @param smali [in] path to the smali file
 * @param methodlist [in/out] list of all methods in the parsed smali
 * @param caller [in/out] caller relationships
 * @param callee [in/out] callee relationships
 * @return 0 on success, of -1 on error
 *
 * If function A calls function B, the relationship is created as follows:
 * - caller[A] = { [B,counter] }, where counter is the number of times that A calls B
 * - callee[B] = { [A,counter] }, where counter is the number of times that B is called by A
 */
int
smali::callgraph_build(const char *smali, methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee) {
	FILE *fin;
	char buf[65536];
	string classname = "";
	string methodname = "";
	unsigned lineno = 0;
	unsigned method_start = 0;

	if((fin = fopen(smali, "rt")) == NULL)
		return 0;
#define	DELIM		" \t\n\r\f\v"
#define TYPE_UNKNOWN	0
#define TYPE_CLASS	1
#define TYPE_METHOD	2
#define TYPE_INVOKE	3
#define	TYPE_END	4
	while(fgets(buf, sizeof(buf), fin) != NULL) {
		char *saveptr;
		char *token = strtok_r(buf, DELIM, &saveptr);
		int type = TYPE_UNKNOWN;
		string lasttoken = "";
		method_t l;
		//
		lineno++;
		if(token == NULL) {
			continue;
		} else if(strncmp(token, "invoke-", 7) == 0) {
			type = TYPE_INVOKE;
		} else if(strcmp(token, ".method") == 0) {
			type = TYPE_METHOD;
		} else if(strcmp(token, ".end") == 0) {
			type = TYPE_END;
		} else if(strcmp(token, ".class") == 0) {
			type = TYPE_CLASS;
		} else {
			continue;
		}
		// get the last token
		while((token = strtok_r(NULL, DELIM, &saveptr)) != NULL) {
			lasttoken = token;
		}
		if(type == TYPE_CLASS)		{ classname  = lasttoken; }
		else if(type == TYPE_METHOD) {
			methodname = lasttoken;
			method_start = lineno-1;
		} else if(type == TYPE_END && lasttoken == "method") {
			l.classname = classname;
			l.start = method_start;
			l.end = lineno-1;
			methodlist[classname + "->" + methodname] = l;
		} else if(type == TYPE_INVOKE) {
			string a = classname + "->" + methodname;
			string b = lasttoken;
			// a calls b
			callgraph_build_connect(caller, a, b);
			callgraph_build_connect(callee, b, a);
		}
	}
#undef	DELIM
#undef	TYPE_UNKNOWN
#undef	TYPE_CLASS
#undef	TYPE_METHOD
#undef	TYPE_INVOKE
#undef	TYPE_END
	fclose(fin);
	return 0;
}

/**
 * Build callgraph from preloaded source codes
 *
 * @param codes [in] preloaded source codes
 * @param codes [in] preloaded function list
 * @param caller [in/out] caller relationships
 * @param callee [in/out] callee relationships
 * @return 0 on success, of -1 on error
 *
 * If function A calls function B, the relationship is created as follows:
 * - caller[A] = { [B,counter] }, where counter is the number of times that A calls B
 * - callee[B] = { [A,counter] }, where counter is the number of times that B is called by A
 */
int
smali::callgraph_build(codelist_t &codes, methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee) {
	codelist_t::iterator ci;
	methodlist_t::iterator fi;
	string classname, methodname, lasttoken;
	unsigned i, start, end;
	size_t pos;
	// for each function
	for(fi = methodlist.begin(); fi != methodlist.end(); fi++) {
		classname = fi->second.classname;
		methodname = fi->first;
		start = fi->second.start;
		end = fi->second.end;
		//
		ci = codes.find(fi->second.classname);
		// scan each line of the function
		for(i = start+1; i < end; i++) {
			if(strncmp("invoke-", ci->second[i].raw.c_str(), 7) != 0)
				continue;
			if((pos = ci->second[i].raw.rfind(' ')) == string::npos)
				continue;
			lasttoken = ci->second[i].raw.substr(pos+1);
			// methodname calls lasttoken
			callgraph_build_connect(caller, methodname, lasttoken);
			callgraph_build_connect(callee, lasttoken, methodname);
		}
	}
	return 0;
}

/**
 * Remove methods not provided in the package
 *
 * @param methodlist [in] The methodlist contains all the methods in the package
 * @param caller [in/out] The caller graph to be removed
 * @param callee [in/out] The callee graph to be removed
 */
void
smali::callgraph_remove_external(methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee) {
	bool stable;
	callgraph_t::iterator ci, ci_next, ci_remove;
	map<string,int>::iterator cj, cj_next, cj_remove;

	do {
		stable = true;
		for(ci = caller.begin(); ci != caller.end(); ci = ci_next) {
			if(ci->second.size() == 0) {
				ci_remove = ci;
				ci_next = ++ci;
				caller.erase(ci_remove);
				stable = false;
				continue;
			}
			for(cj = ci->second.begin(); cj != ci->second.end(); cj = cj_next) {
				if(methodlist.find(cj->first) == methodlist.end()) {
					callee.erase(cj->first);
					cj_remove = cj;
					cj_next = ++cj;
					ci->second.erase(cj_remove);
					stable = false;
					continue;
				}
				cj_next = ++cj;
			}
			ci_next = ++ci;
		}
	} while(!stable);

	return;
}

/**
 * Identify methods used by caller/callee.
 * Functions made no calls or funcions not called are ignored.
 *
 * @param methodused [out] The resulted function list
 * @param methodlist [in] List of all methods
 * @param caller [in] The caller graph
 * @param callee [in] The callee graph
 *
 * The output methodlist contains the key in either caller graph or the callee graph
 */
void
smali::callgraph_methods_used(methodlist_t &methodused, methodlist_t &methodlist, callgraph_t &caller, callgraph_t &callee) {
	callgraph_t::iterator ci;
	methodused.clear();
	for(ci = caller.begin(); ci != caller.end(); ci++) {
		methodused[ci->first] = methodlist[ci->first];
	}
	for(ci = callee.begin(); ci != callee.end(); ci++) {
		methodused[ci->first] = methodlist[ci->first];
	}
	return;
}

