/**
 *
 * @file    string_utils.h
 * @author  Daniele Bellavista (daniele.bellavista@studio.unibo.it)
 * @date    12/30/2013 02:24:44 PM
 * @brief   Utility functions for strings
 *
 * @details 
 *
 */

#ifndef  __STRING_UTILS_INC__
#define  __STRING_UTILS_INC__

#include <string>
#include <vector>


/**
 *  @brief          Split the string.
 *
 *  @param[in]      string  The string to split
 *  @param[in]     sequence  The splitter sequence
 *
 *  @return         A vector containing the splitted string
 *
 *  @details
 */
std::vector<std::string> split_string(const std::string& string, const
    std::string& sequence);

#endif   /* ----- #ifndef __STRING_UTILS_INC__  ----- */
