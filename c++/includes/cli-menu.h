#ifndef __CLI_MENU_H__
#define __CLI_MENU_H__

#include "base.h"
#include "color.h"
#include "message.h"
#include "command.h"
#include "parameter.h"
#include "toggle.h"
#include "program.h"

namespace cm = cli_menu;

/**
 * These 3 parameters have the same size.
 * The index represents the input sequence.
 * Only one of the 3 parameters in one index has a meaningful value.
 *
 * Example:
 *   command = "set-spaces"
 *
 *   filename_flag = 0
 *   STR[filename_flag] == "foo.txt"   --> argument
 *   DBL[filename_flag] == {0}
 *   BOL[filename_flag] == false
 *
 *   count_flag = 1
 *   STR[count_flag] == ""
 *   DBL[count_flag] == {4}     --> argument
 *   BOL[count_flag] == false
 *
 *   copy_flag = 2
 *   STR[copy_flag] == ""
 *   DBL[copy_flag] == {0}
 *   BOL[copy_flag] == true   --> argument
 */

#ifndef CM_FUNCTION
#define CM_FUNCTION(NAME) void NAME(\
  mt::CR_VEC_STR TEXTS,\
  mt::CR_VEC2_DBL NUMBERS,\
  mt::CR_VEC_BOL CONDITIONS\
)
#endif

#ifdef CLI_MENU_FUNCTION
#undef CLI_MENU_FUNCTION
#endif

#ifndef CLI_MENU_FUNCTION
#define CLI_MENU_FUNCTION(NAME) void NAME(\
  mt::CR_VEC_STR TEXTS,\
  mt::CR_VEC2_DBL NUMBERS,\
  mt::CR_VEC_BOL CONDITIONS\
)
#endif

/** CALLBACK */

#ifndef CM_CALLBACK
#define CM_CALLBACK(FUNCTION) std::make_shared<cli_menu::CALLBACK>(FUNCTION)
#endif

#ifdef CLI_MENU_CALLBACK
#undef CLI_MENU_CALLBACK
#endif

#ifndef CLI_MENU_CALLBACK
#define CLI_MENU_CALLBACK(FUNCTION) std::make_shared<cli_menu::CALLBACK>(FUNCTION)
#endif

/** PLAIN CALLBACK */

#ifndef CM_PLAIN_CALLBACK
#define CM_PLAIN_CALLBACK(FUNCTION) std::make_shared<cli_menu::PLAIN_CALLBACK>(FUNCTION)
#endif

#ifdef CLI_MENU_PLAIN_CALLBACK
#undef CLI_MENU_PLAIN_CALLBACK
#endif

#ifndef CLI_MENU_PLAIN_CALLBACK
#define CLI_MENU_PLAIN_CALLBACK(FUNCTION) std::make_shared<cli_menu::PLAIN_CALLBACK>(FUNCTION)
#endif

#endif // __CLI_MENU_H__