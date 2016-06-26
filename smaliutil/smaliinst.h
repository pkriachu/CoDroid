#ifndef __SMALI_INST_H__
#define	__SMALI_INST_H__

#include "smalitype.h"

namespace smali {

void	parse_instruction(codelist_t &codes);
void	parse_instruction_line(code_t &line);

}

#endif	/* __SMALI_INST_H__ */
