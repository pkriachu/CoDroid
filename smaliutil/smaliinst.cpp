#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include "smaliparse.h"
#include "smaliinst.h"

using namespace std;
using namespace smali;

map<string, void (*)(code_t&)> parser;	/*< the parser callbacks */

/**
 * Handle the array-get instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_array_get(code_t &line) {
	// aget* vAA, vBB, vCC
	// - get vBB[vCC] and store in vAA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 3) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
		line.registers.push_back(tokens[3]);
	}
	return;
}

/**
 * Handle the array-put instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_array_put(code_t &line) {
	// aput* vAA, vBB, vCC
	// - store value of vAA into vBB[vCC]
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 3) {
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
		line.registers.push_back(tokens[3]);
	}
	return;
}

/**
 * Handle the cmp instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_cmp(code_t &line) {
	// cmp* vAA, vBB, vCC
	// - comopare vBB and vCC, and then store result in vAA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 3) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
		line.registers.push_back(tokens[3]);
	}
	return;
}

/**
 * Handle the const instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_const(code_t &line) {
	// const* vA, #+BBBBB
	// - store value in vA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 2) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.value = tokens[2];
	}
	return;
}

/**
 * Handle the string instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_const_string(code_t &line) {
	// const-string* vA, "string@BBBB"
	// - store string in vA
	vector<string> tokens;
	size_t begin, end;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 1) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		begin = line.raw.find('"');
		end = line.raw.rfind('"');
		if(end-begin-1 == 0) {
			line.value = "";
		} else {
			line.value = line.raw.substr(begin+1, end-begin-1);
		}
	}
	return;
}

/**
 * Handle the class-get instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_class_get(code_t &line) {
	// iget* vA, vB, field@CCCC
	// - read class member vB->field@CCCC to vA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 2) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
	}
}

/**
 * Handle the class-put instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_class_put(code_t &line) {
	// iput* vA, vB, field@CCCC
	// - write vA to class member vB->field@CCCC
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 2) {
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
	}
	return;
}

/**
 * Handle the if instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_if(code_t &line) {
	// if* vA, vB, +CCCC
	// - test vA and vV, and jump to +CCCC if true
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 2) {
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
	}
	return;
}

/**
 * Handle instructions that reads registers in { .... }. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_read_set(code_t &line) {
	// command {VA, vB, vC, ..., vZ}, something left ...
	unsigned int i;
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) < 3)
		return;
	for(i = 1; i < tokens.size(); i++) {
		if(tokens[i].size() <= 0)
			continue;
		if(tokens[i][0] == '{') {
			line.registers.push_back(tokens[i].substr(1));
		} else if(tokens[i].substr(tokens[i].size()-2) == "},") {
			line.registers.push_back(tokens[i].substr(0, tokens[i].size()-2));
			break;
		} else {
			line.registers.push_back(tokens[i]);
		}
	}
	return;
}

/**
 * Handle instructions that reads registers in a range { vA .. vB }. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_read_set_range(code_t &line) {
	// command {VA, vB, vC, ..., vZ}, something left ...
	unsigned int i;
	int left_num = -1;
	int right_num = -1;
	string left, right;
	vector<string> tokens;
	//
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) < 3)
		return;
	for(i = 1; i < tokens.size(); i++) {
		if(tokens[i].size() <= 0)
			continue;
		if(tokens[i][0] == '{') {
			left = tokens[i].substr(1);
			left_num = strtol(left.c_str()+1, NULL, 0);
		} else if(tokens[i].substr(tokens[i].size()-2) == "},") {
			right = tokens[i].substr(0, tokens[i].size()-2);
			right_num = strtol(right.c_str()+1, NULL, 0);
			break;
		}
	}
	if(left_num > 0 && right_num > 0) {
		int i;
		char regname[128];
		for(i = left_num; i <= right_num; i++) {
			snprintf(regname, sizeof(regname), "v%d", i);
			line.registers.push_back(regname);
		}
	}
	return;
}

/**
 * Handle instructions that read the first register. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_read_first(code_t &line) {
	//// instructions that only read the first register
	// return* vAA: return, return-wide, return-object
	//	- return something back to the caller
	// check-cast vAA, type@BBBB
	//	- check if vAA can be casted to type@BBBB
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 1) {
		line.registers.push_back(tokens[1]);
	}
}

/**
 * Handle instructions that write the first register. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_write_first(code_t &line) {
	//// instructions that only write to the first register
	// monitor-enter vAA
	//	- acquire the monitor for the indicated object
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 1) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
	}
	return;
}

/**
 * Handle unary instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_unary_op(code_t &line) {
	// instruction vA, vB
	// - read vB and then write to vA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 2) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
	}
	return;
}

/**
 * Handle binary instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_binary_op(code_t &line) {
	// instruction vA, vB, vC
	// - read vB and vC, and then write to vA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 3) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
		line.registers.push_back(tokens[3]);
	}
}

/**
 * Handle move instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_move(code_t &line) {
	// move* vA, vB
	// - move from vB to vA
	vector<string> tokens;
	if(tokenize(line.raw.c_str(), " \t\n\r,", tokens) > 2) {
		line.modified_registers.push_back(tokens[1]);
		line.registers.push_back(tokens[1]);
		line.registers.push_back(tokens[2]);
	}
	return;
}

/**
 * Handle move-result instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_move_result(code_t &line) {
	// move-* vAA
	// - save something into vAA
	parse_instruction_write_first(line);
	return;
}

/**
 * Handle array-length instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_array_length(code_t &line) {
	// array-length vA, vB
	// - store the array length of vB into vA
	parse_instruction_move(line);
	return;
}

/**
 * Handle instance-of instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_instance_of(code_t &line) {
	// instance-of vA, vB, type@CCCC
	// - store 1 in vA if vB is an instance of type@CCCC
	parse_instruction_move(line);
	return;
}

/**
 * Unhandeled instructions. This is a helper function stored in \a parser structure.
 *
 * @param line [in/out] the line to be processed
 */
static void
parse_instruction_not_handled(code_t &line) {
	fprintf(stderr, "FATAL: instruction '%s' not handled, please check.\n",
		line.instruction.c_str());
	return;
}

/**
 * Initialize the \a parser strucure: map from instructions to helpers.
 */
static void
parse_instruction_init() {
	static int initialized = false;
	if(initialized)
		return;
	parser.clear();
	parser["aget"]			= parse_instruction_array_get;
	parser["aget-wide"]		= parse_instruction_array_get; // _wide
	parser["aget-object"]		= parse_instruction_array_get;
	parser["aget-boolean"]		= parse_instruction_array_get;
	parser["aget-byte"]		= parse_instruction_array_get;
	parser["aget-char"]		= parse_instruction_array_get;
	parser["aget-short"]		= parse_instruction_array_get;
	parser["aput"]			= parse_instruction_array_put;
	parser["aput-wide"]		= parse_instruction_array_put; // _wide
	parser["aput-object"]		= parse_instruction_array_put;
	parser["aput-boolean"]		= parse_instruction_array_put;
	parser["aput-byte"]		= parse_instruction_array_put;
	parser["aput-char"]		= parse_instruction_array_put;
	parser["aput-short"]		= parse_instruction_array_put;
	parser["array-length"]		= parse_instruction_array_length;
	//
	parser["check-cast"]		= parse_instruction_read_first;
	parser["cmpl-float"]		= parse_instruction_cmp;
	parser["cmpg-float"]		= parse_instruction_cmp;
	parser["cmpl-double"]		= parse_instruction_cmp;
	parser["cmpg-double"]		= parse_instruction_cmp;
	parser["const"]			= parse_instruction_const;
	parser["const/4"]		= parse_instruction_const;
	parser["const/16"]		= parse_instruction_const;
	parser["const/high16"]		= parse_instruction_const;
	parser["const-wide"]		= parse_instruction_const; // _wide
	parser["const-wide/16"]		= parse_instruction_const; // _wide
	parser["const-wide/high16"]	= parse_instruction_const; // _wide
	parser["const-wide/32"]		= parse_instruction_const; // _wide
	parser["const-class"]		= parse_instruction_const;
	parser["const-string"]		= parse_instruction_const_string;
	parser["const-string/jumbo"]	= parse_instruction_const_string;
	parser["fill-array-data"]	= parse_instruction_write_first;
	parser["filled-new-array"]	= parse_instruction_not_handled;
	parser["filled-new-array/range"]= parse_instruction_not_handled;
	//parser["goto"];
	//parser["goto/16"];
	//parser["goto/32"];
	parser["iget"]			= parse_instruction_class_get;
	parser["iget-wide"]		= parse_instruction_class_get; // _wide
	parser["iget-object"]		= parse_instruction_class_get;
	parser["iget-boolean"]		= parse_instruction_class_get;
	parser["iget-byte"]		= parse_instruction_class_get;
	parser["iget-char"]		= parse_instruction_class_get;
	parser["iget-short"]		= parse_instruction_class_get;
	parser["iput"]			= parse_instruction_class_put;
	parser["iput-wide"]		= parse_instruction_class_put; // _wide
	parser["iput-object"]		= parse_instruction_class_put;
	parser["iput-boolean"]		= parse_instruction_class_put;
	parser["iput-byte"]		= parse_instruction_class_put;
	parser["iput-char"]		= parse_instruction_class_put;
	parser["iput-short"]		= parse_instruction_class_put;
	parser["if-eq"]			= parse_instruction_if;
	parser["if-ne"]			= parse_instruction_if;
	parser["if-lt"]			= parse_instruction_if;
	parser["if-ge"]			= parse_instruction_if;
	parser["if-gt"]			= parse_instruction_if;
	parser["if-le"]			= parse_instruction_if;
	parser["if-eqz"]		= parse_instruction_read_first;
	parser["if-nez"]		= parse_instruction_read_first;
	parser["if-ltz"]		= parse_instruction_read_first;
	parser["if-gez"]		= parse_instruction_read_first;
	parser["if-gtz"]		= parse_instruction_read_first;
	parser["if-lez"]		= parse_instruction_read_first;
	parser["instance-of"]		= parse_instruction_instance_of;
	parser["invoke-virtual"]	= parse_instruction_read_set;
	parser["invoke-super"]		= parse_instruction_read_set;
	parser["invoke-direct"]		= parse_instruction_read_set;
	parser["invoke-static"]		= parse_instruction_read_set;
	parser["invoke-interface"]	= parse_instruction_read_set;
	parser["invoke-virtual/range"]	= parse_instruction_read_set_range;
	parser["invoke-super/range"]	= parse_instruction_read_set_range;
	parser["invoke-direct/range"]	= parse_instruction_read_set_range;
	parser["invoke-static/range"]	= parse_instruction_read_set_range;
	parser["invoke-interface/range"]= parse_instruction_read_set_range;
	//
	parser["monitor-enter"]		= parse_instruction_write_first;
	parser["monitor-exit"]		= parse_instruction_read_first;
	//
	parser["move"]			= parse_instruction_move;
	parser["move/from16"]		= parse_instruction_move;
	parser["move/16"]		= parse_instruction_move;
	parser["move-wide"]		= parse_instruction_move; // _wide
	parser["move-wide/from16"]	= parse_instruction_move; // _wide
	parser["move-wide/16"]		= parse_instruction_move; // _wide
	parser["move-object"]		= parse_instruction_move;
	parser["move-object/from16"]	= parse_instruction_move;
	parser["move-object/16"]	= parse_instruction_move;
	//
	parser["move-result"]		= parse_instruction_move_result;
	parser["move-result-wide"]	= parse_instruction_move_result; // _wide
	parser["move-result-object"]	= parse_instruction_move_result;
	parser["move-exception"]	= parse_instruction_move_result;
	//
	parser["new-array"]		= parse_instruction_move;
	parser["new-instance"]		= parse_instruction_write_first;
	//parser["nop"];
	parser["packed-switch"]		= parse_instruction_read_first;
	//parser["return-void"];
	parser["return"]		= parse_instruction_read_first;
	parser["return-wide"]		= parse_instruction_read_first;	// _wide
	parser["return-object"]		= parse_instruction_read_first;
	parser["sget"]			= parse_instruction_write_first;
	parser["sget-wide"]		= parse_instruction_write_first; // _wide
	parser["sget-object"]		= parse_instruction_write_first;
	parser["sget-boolean"]		= parse_instruction_write_first;
	parser["sget-byte"]		= parse_instruction_write_first;
	parser["sget-char"]		= parse_instruction_write_first;
	parser["sget-short"]		= parse_instruction_write_first;
	parser["sput"]			= parse_instruction_read_first;
	parser["sput-wide"]		= parse_instruction_read_first; // _wide
	parser["sput-object"]		= parse_instruction_read_first;
	parser["sput-boolean"]		= parse_instruction_read_first;
	parser["sput-byte"]		= parse_instruction_read_first;
	parser["sput-char"]		= parse_instruction_read_first;
	parser["sput-short"]		= parse_instruction_read_first;
	parser["sparse-switch"]		= parse_instruction_read_first;
	parser["throw"]			= parse_instruction_read_first;
	// unary operations
	parser["neg-int"]		= parse_instruction_unary_op;
	parser["not-int"]		= parse_instruction_unary_op;
	parser["neg-long"]		= parse_instruction_unary_op;
	parser["not-long"]		= parse_instruction_unary_op;
	parser["neg-float"]		= parse_instruction_unary_op;
	parser["neg-double"]		= parse_instruction_unary_op;
	parser["int-to-long"]		= parse_instruction_unary_op;
	parser["int-to-float"]		= parse_instruction_unary_op;
	parser["int-to-double"]		= parse_instruction_unary_op;
	parser["long-to-int"]		= parse_instruction_unary_op;
	parser["long-to-float"]		= parse_instruction_unary_op;
	parser["long-to-double"]	= parse_instruction_unary_op;
	parser["float-to-int"]		= parse_instruction_unary_op;
	parser["float-to-long"]		= parse_instruction_unary_op;
	parser["float-to-double"]	= parse_instruction_unary_op;
	parser["double-to-int"]		= parse_instruction_unary_op;
	parser["double-to-long"]	= parse_instruction_unary_op;
	parser["double-to-float"]	= parse_instruction_unary_op;
	parser["int-to-byte"]		= parse_instruction_unary_op;
	parser["int-to-char"]		= parse_instruction_unary_op;
	parser["int-to-short"]		= parse_instruction_unary_op;
	// binary operations
	parser["add-int"]		= parse_instruction_binary_op;
	parser["sub-int"]		= parse_instruction_binary_op;
	parser["mul-int"]		= parse_instruction_binary_op;
	parser["div-int"]		= parse_instruction_binary_op;
	parser["rem-int"]		= parse_instruction_binary_op;
	parser["and-int"]		= parse_instruction_binary_op;
	parser["or-int"]		= parse_instruction_binary_op;
	parser["xor-int"]		= parse_instruction_binary_op;
	parser["shl-int"]		= parse_instruction_binary_op;
	parser["shr-int"]		= parse_instruction_binary_op;
	parser["ushr-int"]		= parse_instruction_binary_op;
	parser["add-long"]		= parse_instruction_binary_op;
	parser["sub-long"]		= parse_instruction_binary_op;
	parser["mul-long"]		= parse_instruction_binary_op;
	parser["div-long"]		= parse_instruction_binary_op;
	parser["rem-long"]		= parse_instruction_binary_op;
	parser["and-long"]		= parse_instruction_binary_op;
	parser["or-long"]		= parse_instruction_binary_op;
	parser["xor-long"]		= parse_instruction_binary_op;
	parser["shl-long"]		= parse_instruction_binary_op;
	parser["shr-long"]		= parse_instruction_binary_op;
	parser["ushr-long"]		= parse_instruction_binary_op;
	parser["add-float"]		= parse_instruction_binary_op;
	parser["sub-float"]		= parse_instruction_binary_op;
	parser["mul-float"]		= parse_instruction_binary_op;
	parser["div-float"]		= parse_instruction_binary_op;
	parser["rem-float"]		= parse_instruction_binary_op;
	parser["add-double"]		= parse_instruction_binary_op;
	parser["sub-double"]		= parse_instruction_binary_op;
	parser["mul-double"]		= parse_instruction_binary_op;
	parser["div-double"]		= parse_instruction_binary_op;
	parser["rem-double"]		= parse_instruction_binary_op;
	// binary operations / 2addr
	parser["add-int/2addr"]		= parse_instruction_unary_op;
	parser["sub-int/2addr"]		= parse_instruction_unary_op;
	parser["mul-int/2addr"]		= parse_instruction_unary_op;
	parser["div-int/2addr"]		= parse_instruction_unary_op;
	parser["rem-int/2addr"]		= parse_instruction_unary_op;
	parser["and-int/2addr"]		= parse_instruction_unary_op;
	parser["or-int/2addr"]		= parse_instruction_unary_op;
	parser["xor-int/2addr"]		= parse_instruction_unary_op;
	parser["shl-int/2addr"]		= parse_instruction_unary_op;
	parser["shr-int/2addr"]		= parse_instruction_unary_op;
	parser["ushr-int/2addr"]	= parse_instruction_unary_op;
	parser["add-long/2addr"]	= parse_instruction_unary_op;
	parser["sub-long/2addr"]	= parse_instruction_unary_op;
	parser["mul-long/2addr"]	= parse_instruction_unary_op;
	parser["div-long/2addr"]	= parse_instruction_unary_op;
	parser["rem-long/2addr"]	= parse_instruction_unary_op;
	parser["and-long/2addr"]	= parse_instruction_unary_op;
	parser["or-long/2addr"]		= parse_instruction_unary_op;
	parser["xor-long/2addr"]	= parse_instruction_unary_op;
	parser["shl-long/2addr"]	= parse_instruction_unary_op;
	parser["shr-long/2addr"]	= parse_instruction_unary_op;
	parser["ushr-long/2addr"]	= parse_instruction_unary_op;
	parser["add-float/2addr"]	= parse_instruction_unary_op;
	parser["sub-float/2addr"]	= parse_instruction_unary_op;
	parser["mul-float/2addr"]	= parse_instruction_unary_op;
	parser["div-float/2addr"]	= parse_instruction_unary_op;
	parser["rem-float/2addr"]	= parse_instruction_unary_op;
	parser["add-double/2addr"]	= parse_instruction_unary_op;
	parser["sub-double/2addr"]	= parse_instruction_unary_op;
	parser["mul-double/2addr"]	= parse_instruction_unary_op;
	parser["div-double/2addr"]	= parse_instruction_unary_op;
	parser["rem-double/2addr"]	= parse_instruction_unary_op;
	// binary operation / literal
	parser["add-int/lit16"]		= parse_instruction_unary_op;
	parser["rsub-int/lit16"]	= parse_instruction_unary_op;
	parser["mul-int/lit16"]		= parse_instruction_unary_op;
	parser["div-int/lit16"]		= parse_instruction_unary_op;
	parser["rem-int/lit16"]		= parse_instruction_unary_op;
	parser["and-int/lit16"]		= parse_instruction_unary_op;
	parser["or-int/lit16"]		= parse_instruction_unary_op;
	parser["xor-int/lit16"]		= parse_instruction_unary_op;
	//
	parser["add-int/lit8"]		= parse_instruction_unary_op;
	parser["rsub-int/lit8"]		= parse_instruction_unary_op;
	parser["mul-int/lit8"]		= parse_instruction_unary_op;
	parser["div-int/lit8"]		= parse_instruction_unary_op;
	parser["rem-int/lit8"]		= parse_instruction_unary_op;
	parser["and-int/lit8"]		= parse_instruction_unary_op;
	parser["or-int/lit8"]		= parse_instruction_unary_op;
	parser["xor-int/lit8"]		= parse_instruction_unary_op;
	parser["shl-int/lit8"]		= parse_instruction_unary_op;
	parser["shr-int/lit8"]		= parse_instruction_unary_op;
	parser["ushr-int/lit8"]		= parse_instruction_unary_op;
	//
	initialized = true;
}

/**
 * parse a single line
 *
 * @param line [in/out] a code_t structure, with at least 'raw' and 'instruction' field filled
 */
void
smali::parse_instruction_line(code_t &line) {
	map<string, void (*)(code_t&)>::iterator mi;

	parse_instruction_init();

	if((mi = parser.find(line.instruction)) != parser.end()) {
		mi->second(line);
	}

	return;
}

/**
 * parse each line of codes and store additional information into code_t
 *
 * @param codes [in/out] the codelist_t structure
 */
void
smali::parse_instruction(codelist_t &codes) {
	codelist_t::iterator ci;
	vector<code_t>::iterator vi;
	for(ci = codes.begin(); ci != codes.end(); ci++) {
		for(vi = ci->second.begin(); vi != ci->second.end(); vi++) {
			parse_instruction_line(*vi);
		}
	}
	return;
}
