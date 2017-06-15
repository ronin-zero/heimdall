/*
 *  File Name : directory_handler.h
 *  
 *  Creation Date : 06-15-2017
 *
 *  Last Modified : Thu 15 Jun 2017 05:52:37 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sys/types.h>
#include <dirent.h>

class Directory_Handler {

    public:

        static std::vector<std::string> get_file_list( const std::string dirname );
        static std::vector<std::string> get_file_list( const char* dirname );

};
