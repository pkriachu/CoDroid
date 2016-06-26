#ifndef __SMALI_TYPE_H__
#define	__SMALI_TYPE_H__

#include <map>
#include <list>
#include <vector>
#include <string>

namespace smali {

#define	SMALI_METHOD_FLAG_NONE		0x000000
#define	SMALI_METHOD_FLAG_ABSTRACT	0x000001
#define	SMALI_METHOD_FLAG_BRIDGE	0x000002
#define	SMALI_METHOD_FLAG_CONSTRUCTOR	0x000004
#define	SMALI_METHOD_FLAG_DECLAREDSYNC	0x000008
#define	SMALI_METHOD_FLAG_FINAL		0x000010
#define	SMALI_METHOD_FLAG_NATIVE	0x000020
#define	SMALI_METHOD_FLAG_PRIVATE	0x000040
#define	SMALI_METHOD_FLAG_PROTECTED	0x000080
#define	SMALI_METHOD_FLAG_PUBLIC	0x000100
#define	SMALI_METHOD_FLAG_STATIC	0x000200
#define	SMALI_METHOD_FLAG_SYNTHETIC	0x000400
#define	SMALI_METHOD_FLAG_VARARGS	0x000800
#define SMALI_METHOD_FLAG_SYNCHRONIZED	0x001000

#define	SMALI_BLOCK_FLAG_NONE		0x000000
#define	SMALI_BLOCK_FLAG_BLOCK_BEGIN	0x000001	/*< begin of a contorl flow block */
#define	SMALI_BLOCK_FLAG_BLOCK_END	0x000002	/*< end of a control flow block */
#define	SMALI_BLOCK_FLAG_CLASS		0x000004	/*< increase class counters */
#define	SMALI_BLOCK_FLAG_METHOD		0x000008	/*< increase method counters */
#define	SMALI_BLOCK_FLAG_NATIVE		0x000010	/*< invoke a native method */

/**
 * class to represent a line of code
 */
class code_t {
public:
	code_t();
	std::string	raw;		/*< raw string of the line */
	std::string	instruction;	/*< the instruction, if any */
	std::vector<std::string>	modified_registers;	/*< modified register */
	std::vector<std::string>	registers;		/*< passed registers */
	std::string	value;		/*< value, if any */
	unsigned	blockflags;	/*< block flag, see SMALI_BLOCK_FLAG_XXX */
	unsigned	blockid;	/*< block id */
};

/**
 * class to represent a method
 */
class method_t {
public:
	method_t();
	std::string	classname;	/*< classname, or file name without .smali suffix */
	std::string	methodname;	/*< methodname */
	unsigned	flags;		/*< flags about the method, see SMALI_METHOD_FLAGS_XXX  */
	unsigned	locals;		/*< number of local variable */
	unsigned	start;		/*< start line number, line number starts from 0 */
	unsigned	end;		/*< end line number */
};

/**
 * class to represent a class field
 */
class field_t {
public:
	field_t();
	std::string 	name;		/*< field name */
	std::string	type;		/*< field type */
	std::string	value;		/*< field (initial) value */
	unsigned	line;		/*< line number, line number starts from 0 */
};

class class_t;
//typedef std::map<std::string,location_t> methodlist_t;

typedef std::list<std::string>					pathlist_t;	/*< list of file name strings */
//typedef std::map<std::string, std::vector<std::string> >	codelist_t;	/*< map from classname -> line of codes */
typedef std::map<std::string, std::vector<code_t> >		codelist_t;	/*< map from classname -> line of codes */
typedef std::map<std::string, class_t>				classlist_t;	/*< map from classname -> class_t */
typedef std::map<std::string, field_t>				fieldlist_t;	/*< map from class->field -> field_t */
typedef std::map<std::string, method_t> 			methodlist_t;	/*< map from class->method -> method_t */
typedef std::map<std::string, std::map<std::string,int> >	callgraph_t;	/*< map from class->method -> class->method:counter */

typedef std::vector<std::string>				tokens_t;	/*< string tokens */
/**
 * class to represent a class
 */
class class_t {
public:
	class_t();
	std::string	classname;	/*< class name */
	std::string	super;		/*< super class name */
	fieldlist_t	fields;		/*< list of all fields */
	methodlist_t	methods;	/*< list of all methods */
};

/**
 * struct used to store block metadata (header only)
 * the actual data is block metaddata header plus
 * class name string (with '\0') and method name string (with '\0')
 * note that all the values are stored in big-endian (network byte order)
 */
struct block_metadata_s {
	unsigned blockid;		/*< block id; copied from code_t->blockid*/
	unsigned blockflags;		/*< block flags; copied from code_t->blockflags */
	unsigned lines;			/*< number of lines in this block (empty lines are ignored) */
	unsigned classlen;		/*< class name length */
	unsigned methodlen;		/*< method name length */
}	__attribute__((packed));
typedef struct block_metadata_s block_metadata_t;

/**
 * struct used to load metadata
 */
class metadata_t {
public:
	metadata_t();
	block_metadata_t raw;		/*< metadata from file: endian has been coverted to host endian */
	std::string	classname;	/*< class name */
	std::string	methodname;	/*< method name */
};

typedef std::map<unsigned, metadata_t>	metadatalist_t;		/*< map from blockid to metadata */

}	/* namespae smali */

#endif	/* __SMALI_TYPE_H__ */
