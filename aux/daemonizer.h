/*
 *  File Name : daemonizer.h
 *  
 *  Creation Date : 23-06-2016
 *
 *  Last Modified : Fri 08 Jul 2016 05:51:47 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <unistd.h>
#include <cstdint>

const uint_fast8_t  PIPE_ENDS   = 2;
const uint_fast32_t READ_END    = 0;
const uint_fast32_t WRITE_END   = 1;

class Daemonizer{

    // Based largely on Bander's linux_daemon.h
    // and the guide found here:
    // http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html

    public:

        static void launch_daemon( const std::string daemon_name, const std::string run_path="/var/run/" );
        static void write_log( const std::string file_name, const std::string contents );
        static pid_t get_daemon_pid( const std::string daemon_name, const std::string run_path="/var/run/");
        static int32_t remove_daemon_pid( const std::string daemon_name, const std::string run_path="/var/run/" );

        static bool get_fds( pid_t daemon_pid, int32_t fds[2], const std::string run_path="/proc/" );

        /*
         * May need functions to get the file descriptors and whatnot.
         */
};
