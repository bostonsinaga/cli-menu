#ifndef __MINI_TOOLS__TYPES_H__
#define __MINI_TOOLS__TYPES_H__

#include <type_traits>
#include <string>
#include <vector>

namespace mini_tools {

  /** Flags for direction of sorting process */

  enum SortOrderFlag {DESCENDING, ASCENDING};
  enum SortPriorityFlag {LETTER_LOWER, LETTER_UPPER};

  /** INSPECTOR */

  namespace CheckType {

    template <typename T>
    constexpr bool isLetter() {
      return
        std::is_same<T, char>::value ||
        std::is_same<T, wchar_t>::value ||
        std::is_same<T, std::string>::value;
    }

    template <typename T>
    constexpr bool isNumber() {
      if (!isLetter<T>()) {
        return
          std::is_integral<T>::value ||
          std::is_floating_point<T>::value;
      }
      return false;
    }
  }

  /** SHORTENED TYPES */
  
  typedef short int SI;
  typedef long int LI;
  typedef long long int LLI;
  typedef unsigned short int USI;
  typedef unsigned int UI;
  typedef unsigned long int ULI;
  typedef unsigned long long int ULLI;
  typedef long double LD;

  /** SHORTENED VALUE VECTORS */

  typedef std::vector<bool> VEC_BOL;
  typedef std::vector<std::string> VEC_STR;
  typedef std::vector<char> VEC_CHR;
  typedef std::vector<wchar_t> VEC_WCH;
  typedef std::vector<int> VEC_INT;
  typedef std::vector<SI> VEC_SI;
  typedef std::vector<LI> VEC_LI;
  typedef std::vector<LLI> VEC_LLI;
  typedef std::vector<USI> VEC_USI;
  typedef std::vector<UI> VEC_UI;
  typedef std::vector<ULI> VEC_ULI;
  typedef std::vector<ULLI> VEC_ULLI;
  typedef std::vector<size_t> VEC_SZ;
  typedef std::vector<time_t> VEC_TM;
  typedef std::vector<float> VEC_FLT;
  typedef std::vector<double> VEC_DBL;
  typedef std::vector<LD> VEC_LD;

  typedef std::vector<VEC_BOL> VEC2_BOL;
  typedef std::vector<VEC_STR> VEC2_STR;
  typedef std::vector<VEC_CHR> VEC2_CHR;
  typedef std::vector<VEC_WCH> VEC2_WCH;
  typedef std::vector<VEC_INT> VEC2_INT;
  typedef std::vector<VEC_SI> VEC2_SI;
  typedef std::vector<VEC_LI> VEC2_LI;
  typedef std::vector<VEC_LLI> VEC2_LLI;
  typedef std::vector<VEC_USI> VEC2_USI;
  typedef std::vector<VEC_UI> VEC2_UI;
  typedef std::vector<VEC_ULI> VEC2_ULI;
  typedef std::vector<VEC_ULLI> VEC2_ULLI;
  typedef std::vector<VEC_SZ> VEC2_SZ;
  typedef std::vector<VEC_TM> VEC2_TM;
  typedef std::vector<VEC_FLT> VEC2_FLT;
  typedef std::vector<VEC_DBL> VEC2_DBL;
  typedef std::vector<VEC_LD> VEC2_LD;

  /** SHORTENED POINTER VECTORS */

  typedef std::vector<bool*> VEC_BOL_P;
  typedef std::vector<std::string*> VEC_STR_P;
  typedef std::vector<char*> VEC_CHR_P;
  typedef std::vector<wchar_t*> VEC_WCH_P;
  typedef std::vector<int*> VEC_INT_P;
  typedef std::vector<SI*> VEC_SI_P;
  typedef std::vector<LI*> VEC_LI_P;
  typedef std::vector<LLI*> VEC_LLI_P;
  typedef std::vector<USI*> VEC_USI_P;
  typedef std::vector<UI*> VEC_UI_P;
  typedef std::vector<ULI*> VEC_ULI_P;
  typedef std::vector<ULLI*> VEC_ULLI_P;
  typedef std::vector<size_t*> VEC_SZ_P;
  typedef std::vector<time_t*> VEC_TM_P;
  typedef std::vector<float*> VEC_FLT_P;
  typedef std::vector<double*> VEC_DBL_P;
  typedef std::vector<LD*> VEC_LD_P;

  typedef std::vector<VEC_BOL_P> VEC2_BOL_P;
  typedef std::vector<VEC_STR_P> VEC2_STR_P;
  typedef std::vector<VEC_CHR_P> VEC2_CHR_P;
  typedef std::vector<VEC_WCH_P> VEC2_WCH_P;
  typedef std::vector<VEC_INT_P> VEC2_INT_P;
  typedef std::vector<VEC_SI_P> VEC2_SI_P;
  typedef std::vector<VEC_LI_P> VEC2_LI_P;
  typedef std::vector<VEC_LLI_P> VEC2_LLI_P;
  typedef std::vector<VEC_USI_P> VEC2_USI_P;
  typedef std::vector<VEC_UI_P> VEC2_UI_P;
  typedef std::vector<VEC_ULI_P> VEC2_ULI_P;
  typedef std::vector<VEC_ULLI_P> VEC2_ULLI_P;
  typedef std::vector<VEC_SZ_P> VEC2_SZ_P;
  typedef std::vector<VEC_TM_P> VEC2_TM_P;
  typedef std::vector<VEC_FLT_P> VEC2_FLT_P;
  typedef std::vector<VEC_DBL_P> VEC2_DBL_P;
  typedef std::vector<VEC_LD_P> VEC2_LD_P;

  /** SHORTENED CONSTANT REFERENCES */

  typedef const bool& CR_BOL;
  typedef const std::string& CR_STR;
  typedef const char& CR_CHR;
  typedef const wchar_t& CR_WCH;
  typedef const int& CR_INT;
  typedef const SI& CR_SI;
  typedef const LI& CR_LI;
  typedef const LLI& CR_LLI;
  typedef const USI& CR_USI;
  typedef const UI& CR_UI;
  typedef const ULI& CR_ULI;
  typedef const ULLI& CR_ULLI;
  typedef const size_t& CR_SZ;
  typedef const time_t& CR_TM;
  typedef const float& CR_FLT;
  typedef const double& CR_DBL;
  typedef const LD& CR_LD;

  /** SHORTENED CONSTANT REFERENCE VECTORS */

  typedef const VEC_BOL& CR_VEC_BOL;
  typedef const VEC_STR& CR_VEC_STR;
  typedef const VEC_CHR& CR_VEC_CHR;
  typedef const VEC_WCH& CR_VEC_WCH;
  typedef const VEC_INT& CR_VEC_INT;
  typedef const VEC_SI& CR_VEC_SI;
  typedef const VEC_LI& CR_VEC_LI;
  typedef const VEC_LLI& CR_VEC_LLI;
  typedef const VEC_USI& CR_VEC_USI;
  typedef const VEC_UI& CR_VEC_UI;
  typedef const VEC_ULI& CR_VEC_ULI;
  typedef const VEC_ULLI& CR_VEC_ULLI;
  typedef const VEC_SZ& CR_VEC_SZ;
  typedef const VEC_TM& CR_VEC_TM;
  typedef const VEC_FLT& CR_VEC_FLT;
  typedef const VEC_DBL& CR_VEC_DBL;
  typedef const VEC_LD& CR_VEC_LD;

  typedef const VEC2_BOL& CR_VEC2_BOL;
  typedef const VEC2_STR& CR_VEC2_STR;
  typedef const VEC2_CHR& CR_VEC2_CHR;
  typedef const VEC2_WCH& CR_VEC2_WCH;
  typedef const VEC2_INT& CR_VEC2_INT;
  typedef const VEC2_SI& CR_VEC2_SI;
  typedef const VEC2_LI& CR_VEC2_LI;
  typedef const VEC2_LLI& CR_VEC2_LLI;
  typedef const VEC2_USI& CR_VEC2_USI;
  typedef const VEC2_UI& CR_VEC2_UI;
  typedef const VEC2_ULI& CR_VEC2_ULI;
  typedef const VEC2_ULLI& CR_VEC2_ULLI;
  typedef const VEC2_SZ& CR_VEC2_SZ;
  typedef const VEC2_TM& CR_VEC2_TM;
  typedef const VEC2_FLT& CR_VEC2_FLT;
  typedef const VEC2_DBL& CR_VEC2_DBL;
  typedef const VEC2_LD& CR_VEC2_LD;

  /** SHORTENED CONSTANT REFERENCE POINTER VECTORS */

  typedef const VEC_BOL_P& CR_VEC_BOL_P;
  typedef const VEC_STR_P& CR_VEC_STR_P;
  typedef const VEC_CHR_P& CR_VEC_CHR_P;
  typedef const VEC_WCH_P& CR_VEC_WCH_P;
  typedef const VEC_INT_P& CR_VEC_INT_P;
  typedef const VEC_SI_P& CR_VEC_SI_P;
  typedef const VEC_LI_P& CR_VEC_LI_P;
  typedef const VEC_LLI_P& CR_VEC_LLI_P;
  typedef const VEC_USI_P& CR_VEC_USI_P;
  typedef const VEC_UI_P& CR_VEC_UI_P;
  typedef const VEC_ULI_P& CR_VEC_ULI_P;
  typedef const VEC_ULLI_P& CR_VEC_ULLI_P;
  typedef const VEC_SZ_P& CR_VEC_SZ_P;
  typedef const VEC_TM_P& CR_VEC_TM_P;
  typedef const VEC_FLT_P& CR_VEC_FLT_P;
  typedef const VEC_DBL_P& CR_VEC_DBL_P;
  typedef const VEC_LD_P& CR_VEC_LD_P;

  typedef const VEC2_BOL_P& CR_VEC2_BOL_P;
  typedef const VEC2_STR_P& CR_VEC2_STR_P;
  typedef const VEC2_CHR_P& CR_VEC2_CHR_P;
  typedef const VEC2_WCH_P& CR_VEC2_WCH_P;
  typedef const VEC2_INT_P& CR_VEC2_INT_P;
  typedef const VEC2_SI_P& CR_VEC2_SI_P;
  typedef const VEC2_LI_P& CR_VEC2_LI_P;
  typedef const VEC2_LLI_P& CR_VEC2_LLI_P;
  typedef const VEC2_USI_P& CR_VEC2_USI_P;
  typedef const VEC2_UI_P& CR_VEC2_UI_P;
  typedef const VEC2_ULI_P& CR_VEC2_ULI_P;
  typedef const VEC2_ULLI_P& CR_VEC2_ULLI_P;
  typedef const VEC2_SZ_P& CR_VEC2_SZ_P;
  typedef const VEC2_TM_P& CR_VEC2_TM_P;
  typedef const VEC2_FLT_P& CR_VEC2_FLT_P;
  typedef const VEC2_DBL_P& CR_VEC2_DBL_P;
  typedef const VEC2_LD_P& CR_VEC2_LD_P;
}

#endif // __MINI_TOOLS__TYPES_H__