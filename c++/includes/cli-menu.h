#ifndef __CLI_MENU_H__
#define __CLI_MENU_H__

#include "base.h"
#include "command.h"
#include "program.h"
#include "parameter.h"
#include "toggle.h"
#include "message.h"
#include "executor.h"

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
#define CLI_MENU_PARAMS (mt::CR_VEC_STR TEXTS, mt::CR_VEC2_DBL NUMBERS, mt::CR_VEC_BOL CONDITIONS)

namespace cm = cli_menu;

#endif // __CLI_MENU_H__