/*
 *  File Name : thread_manager.h
 *  
 *  Creation Date : 06-28-2016
 *
 *  Last Modified : Tue 31 Jan 2017 03:40:47 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>


/* This class will probably not be necessary.
 * The idea would be that everything that 
 * launches a thread updates this object
 * when a thread is launched in order to guarantee
 * that the thread id is added to the filter,
 * but I just realized that I think I'm already
 * handling that.
 */

class Thread_Manager{

    public:

        void add_thread( pid_t thread_id );


    private:

        vector<string> filter_files;
        string filter;
        bool filter_self;

};
