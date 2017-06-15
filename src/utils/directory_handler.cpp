/*
 *  File Name : directory_handler.cpp
 *  
 *  Creation Date : 06-15-2017
 *
 *  Last Modified : Thu 15 Jun 2017 05:52:40 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "directory_handler.h"

std::vector<std::string> Directory_Handler::get_file_list( const std::string dirname ){

    return Directory_Handler::get_file_list( dirname.c_str() );
}

std::vector<std::string> Directory_Handler::get_file_list( const char* dirname ){

    std::vector<std::string> file_list;

    DIR* directory = opendir( dirname );

    if ( directory != NULL )
    {
        dirent* dir_entry;

        while ( dir_entry = readdir( directory ) )
        {
            if ( dir_entry->d_type == DT_REG )
            {
                file_list.push_back( std::string( dir_entry->d_name ) );
            }
        }
    }

    return file_list;
}
