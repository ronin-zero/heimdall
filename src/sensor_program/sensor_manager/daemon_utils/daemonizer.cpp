/*
 *  File Name : Daemonizer.cpp
 *  
 *  Creation Date : 24-06-2016
 *
 *  Last Modified : Mon 11 Jul 2016 08:50:40 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "daemonizer.h"

void Daemonizer::launch_daemon( const std::string daemon_name, const std::string run_path ){

    // Source: http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html

    pid_t pid, sid;

    std::string aux;

    /* Fork off of the parent process */

    pid = fork();

    if ( pid < 0 )
    {
        exit( EXIT_FAILURE );
    }

    /* If the PID is greater than 0,
     * we successfully created a child process
     * and can exit the parent successfully.
     */

    if ( pid > 0 )
    {   
        std::cout << "pid is " << pid << std::endl;
        std::cout << "We are the parent process because the pid is " << pid << std::endl;
        std::cout << "Except when we call getpid(), it's " << getpid() << std::endl;
        exit( EXIT_SUCCESS );
    }
    else
    {
        std::cout << "Allegedly, this is the child." << std::endl;
        std::cout << "and the pid is " << pid << std::endl;
        std::cout << "Except when we call getpid() of course then it's: " << getpid() << std::endl;
    }

    /* Change the file mode mask */

    umask( 0 );

    /* Write the daemon's PID to <run_path><daemon_name>.pid */

    write_log( run_path + daemon_name + ".pid", std::to_string( getpid() ) );

    /* Open any logs here */

    /* Create a new SID for the child process */
    sid = setsid();

    if ( sid < 0 )
    {
        /* Log any failures here */
        exit( EXIT_FAILURE );
    }

    /* Change the current working directory to root */
    if ( ( chdir( "/" ) ) < 0 )
    {
        /* Log any failures here */
        exit( EXIT_FAILURE );
    }

    /* Close out the standard file descriptors.
     *
     * Alex's note: I'm still not clear on why 
     * I need to do this.
     */
    /*
    close( STDIN_FILENO );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );
    */
}

void Daemonizer::write_log( const std::string file_name, const std::string contents ){

    std::cout << "Logging info " << contents << " to file " << file_name << std::endl;

    std::ofstream process_file( file_name );

    try
    {
        process_file << contents << std::endl;;
    }
    catch ( std::ofstream::failure &write_err )
    {
        std::cerr << "Exception occurred: Could not write to file." << std::endl;
        std::cerr << "Filename: " << file_name << std::endl << std::endl;
        std::cerr << write_err.what() << std::endl << std::endl;
        std::cerr.flush();
    }

    process_file.close();
}

pid_t Daemonizer::get_daemon_pid( const std::string daemon_name, const std::string run_path ){

    pid_t pid = -1;

    std::ifstream daemon_file( run_path + daemon_name + ".pid" );

    if ( daemon_file.good() )
    {
        daemon_file >> pid;
        daemon_file.close();
    }

    std::cout << "PID is : " << pid << std::endl;

    return pid;
}

int32_t Daemonizer::remove_daemon_pid( const std::string daemon_name, const std::string run_path ){

    return std::remove( (run_path + daemon_name + ".pid").c_str() );
}

bool Daemonizer::get_fds( pid_t daemon_pid, int32_t fds[2], const std::string run_path ){

    std::string command = "ls " + run_path + std::to_string( daemon_pid ) + "/fd/";
    std::string mode = "r";

    // std::string result = "";

    FILE* pipe = popen( command.c_str(), mode.c_str() );

    std::string tmp_num = "";
    uint_fast8_t tmp_char = 0;

    while ( !feof( pipe ) )
    {
        tmp_char = fgetc ( pipe );

        if ( tmp_char == '\n' && tmp_num.length() > 0 )
        {
            std::cout << "tmp_num is " << tmp_num << std::endl;
            fds[0] = fds[1];
            fds[1] = stoi( tmp_num );
            tmp_num = "";
            
        }
        else if ( tmp_char > '0' && tmp_char < '9' )
        {
            tmp_num += tmp_char;
        }
    }

    return true;
}
