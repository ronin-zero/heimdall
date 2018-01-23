/*
 *  File Name : detector_launcher.cpp
 *  
 *  Creation Date : 03-07-2017
 *
 *  Last Modified : Tue 23 Jan 2018 02:58:47 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sstream>

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensor_observers/syscall_logger.h"
#include "sensor_observers/data_streams/output_stream.h"
#include "sensor_observers/data_records/linux/linux_syscall_record.h"
#include "sensor_observers/detectors/syscall_detector.h"

#include "utils/ascii_operations.h"

#include "detector_command_line_utils/command_line_parser.h"
#include "detector_manager/detector_manager.h"

const std::string pipe_name = "/var/run/detector.pipe";

std::string flag_string( uint_fast8_t flags );
//std::string get_program_name( char* arg );

void offline( Command_Line_Parser & parser );
void start( Command_Line_Parser & parser );
void stop( Command_Line_Parser & parser );
void status( Command_Line_Parser & parser );
bool is_running( std::string program_name );


int main( int argc, char** argv ){

    Command_Line_Parser command_parser( argc, argv );

    if ( command_parser.contains_arg( "-h" ) || command_parser.contains_option( "--help" ) )
    {
        command_parser.print_help();
    }
    else if ( command_parser.check_args() )
    {
        if ( command_parser.contains_arg( "offline" ) )
        {
            offline( command_parser );
        }
        else if ( command_parser.contains_arg( "start" ) )
        {
            start( command_parser );
        }
        else if ( command_parser.contains_arg( "status" ) )
        {
            status( command_parser );
        }
        else if ( command_parser.contains_arg( "stop" ) )
        {
            stop( command_parser );
        }
    }

    else
    {
        return -1;
    }

    return 0;
}

std::string flag_string( uint_fast8_t flags ){

    std::string string_flag = "";

    uint_fast32_t flag_count = 0;

    if ( flags & PROCESS_NAME )
    {
        string_flag += "PROCESS_NAME";

        flag_count++;
    }

    if ( flags & PID )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "PID";

        flag_count++;
    }

    if ( flags & CPU )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "CPU";

        flag_count++;
    }

    if ( flags & TRACE_FLAGS )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "TRACE_FLAGS";

        flag_count++;
    }

    if ( flags & TIMESTAMP )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "TIMESTAMP";

        flag_count++;
    }

    if ( flags & SYSCALL_NUM )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "SYSCALL_NUM";

        flag_count++;
    }

    if ( flags & SYSCALL_ARGS )
    {
        if ( flag_count > 0 )
        {
            string_flag += " | ";
        }

        string_flag += "SYSCALL_ARGS";

        flag_count++;
    }

    return string_flag;
}

std::string get_program_name( char* arg ){

    std::string argument( arg );

    std::string leading_slash = "./";

    size_t slash_pos = argument.find( leading_slash );

    if ( slash_pos == std::string::npos )
    {
        return argument;
    }
    else
    {
        return argument.substr( slash_pos + leading_slash.length() );
    }
}

void offline( Command_Line_Parser & parser ){

    std::vector<std::string> clean_files;
    std::vector<std::string> infected_files;

    uint_fast32_t correct = 0;
    uint_fast32_t incorrect = 0;

    uint_fast32_t true_positives = 0;
    uint_fast32_t true_negatives = 0;
    uint_fast32_t false_positives = 0;
    uint_fast32_t false_negatives = 0;

    double sum_error_clean = 0.0;
    double sum_error_infected = 0.0;

    double sum_confidence_clean = 0.0;
    double sum_confidence_infected = 0.0;

    double accuracy;
    double precision;
    double recall;
    double f1_score;

    bool used_model = false;

    double threshhold = 0.5;
    size_t window_len = 100;
    uint_fast32_t ngram_len = 2;
    std::string out_file_name = "new_trace.log";
    std::string detection_log = "syscall_detector.log";


    if ( parser.contains_arg( "-d" ) )
    {
        std::cout << "Contains flag -d " << std::endl;
        detection_log = parser.arg_at( parser.arg_index( "-d" ) + 1 );
    }

    if ( parser.contains_arg( "-l" ) )
    {
        std::cout << "Contains flag -l" << std::endl;
        std::string tmp_arg = parser.arg_at( parser.arg_index( "-l" ) + 1 );

        if ( ASCII_Operations::is_number( tmp_arg ) )
        {
            window_len = ASCII_Operations::to_uint( tmp_arg );
        }
        else
        {
            std::cout << "Argument given to -l: " << tmp_arg << " is not a number." << std::endl;
        }
    }

    if ( parser.contains_arg( "-n" ) )
    {
        std::cout << "Contains flag -n" << std::endl;
        std::string tmp_arg = parser.arg_at( parser.arg_index( "-n" ) + 1 );

        if ( ASCII_Operations::is_number( tmp_arg ) )
        {
            ngram_len = ASCII_Operations::to_uint( tmp_arg );
        }
        else
        {
            std::cout << "Argument given to -n: " << tmp_arg << " is not a number." << std::endl;
        }
    }

    if ( parser.contains_arg( "-o" ) )// && parser.arg_index( "-o" ) < parser.num_args() - 1 )
    {
        std::cout << "Contains flag -o " << std::endl;
        out_file_name = parser.arg_at( parser.arg_index( "-o" ) + 1 );
    }


    std::string prog_name = parser.get_program_name();
    uint_fast8_t trace_arch = ARCH_DEFAULT;

    if ( parser.contains_option( "--arch=" ) )
    {

        std::string tmp_arch = parser.get_option_string( parser.arg_at ( parser.option_index ( "--arch=" ) ) );

        if ( tmp_arch.find( "ARM" ) != std::string::npos || tmp_arch.find( "arm" ) != std::string::npos )
        {
            trace_arch = ARCH_ARM;
        }
        else if ( tmp_arch.find( "MIPS" ) != std::string::npos || tmp_arch.find("mips") != std::string::npos )
        {
            trace_arch = ARCH_MIPS;
        }
        else
        {
            trace_arch = ARCH_DEFAULT;
        }
    }

    Detector_Manager manager( window_len, ngram_len, detection_log, out_file_name, prog_name, trace_arch );

    if ( parser.contains_option( "--threshhold=" ) )
    {
        std::string tmp_arg = parser.get_option_string( parser.arg_at ( parser.option_index ( "--threshhold=" ) ) );
        std::cout << "Contains option --threshold=" << tmp_arg << std::endl;

        if ( ASCII_Operations::is_floating_point ( tmp_arg ) )
        {
            threshhold = ASCII_Operations::to_floating_point( tmp_arg );
        }
        else
        {
            std::cout << "ERROR: " << tmp_arg << " is not a valid option for threshold." << std::endl;
        }

    }

    if ( parser.contains_option( "--loadmodel=" ) )
    {
        std::cout << "Contains flag -m" << std::endl;

        std::string tmp_arg = parser.get_option_string(parser.arg_at( parser.arg_index( "--loadmodel=" ) ) );

        std::cout << "Attempting to load model from file " << tmp_arg << "..." << std::endl;

        if ( manager.train_on_model( tmp_arg ) )
        {
            used_model = true;
            std::cout << "Model loaded!" << std::endl;
        }
        else
        {
            std::cout << "Failed to load model." << std::endl;
        }


    }
    else if ( parser.contains_option( "--training=" ) )
    {
        std::string tmp_arg = parser.get_option_string( parser.arg_at( parser.arg_index( "--training=" ) ) );

        std::cout << "Attempting to train from the files listed in " << tmp_arg << "..." << std::endl;

        std::ifstream training_list( tmp_arg );

        std::string training_file;

        uint_fast32_t training_attempts=0;
        uint_fast32_t training_failures=0;

        while ( training_list >> training_file )
        {
            training_attempts++;
            if ( !manager.train_on_trace( training_file ) )
            {
                training_failures++;
            }
        }

        std::cout << "Finished reading the training list." << std::endl;
        std::cout << "Attempted to train from " << training_attempts << " files." << std::endl;

        if ( training_failures == 0 )
        {
            std::cout << "There were " << training_failures << " failed training files (see detection log)." << std::endl;
        }

        std::cout << "Attempting to train a model..." << std::endl;

        if ( manager.train_model () )
        {
            std::cout << "Training succeeded." << std::endl;
        }
        else
        {
            std::cout << "Training failed.  See detection log." << std::endl;
        }
    }

    if ( parser.contains_option( "--clean=" ) )
    {
        std::string tmp_arg = parser.get_option_string( parser.arg_at( parser.arg_index( "--clean=" ) ) );

        std::ifstream clean_list( tmp_arg );

        std::string clean_file;

        while ( clean_list >> clean_file )
        {
            clean_files.push_back( clean_file );
        }
    }

    if ( parser.contains_option( "--infected=" ) )
    {
        std::string tmp_arg = parser.get_option_string( parser.arg_at( parser.arg_index( "--infected=" ) ) );

        std::ifstream infected_list( tmp_arg );

        std::string infected_file;

        while ( infected_list >> infected_file )
        {
            infected_files.push_back( infected_file );
        }
    }

    for( uint_fast32_t i = 0; i < clean_files.size(); i++ )
    {
        std::cout << "Test File: " << clean_files[i] << std::endl;
        std::cout << "Actual: Clean" << std::endl;

        double percent_anomaly = manager.test_file( clean_files[i] );

        if ( percent_anomaly < 0.0 )
        {
            std::cout << "ERROR.  Anomaly rating was : " << percent_anomaly << " -- see log." << std::endl;
        }
        else
        {
            std::cout << "Predicted: "; 

            if ( percent_anomaly >= threshhold )
            {
                std::cout << "Infected (" << percent_anomaly << ")" << std::endl;
                std::cout << "Error:    " << ( percent_anomaly - threshhold ) << std::endl;

                sum_error_clean += ( percent_anomaly - threshhold );

                false_positives++;
                incorrect++;
            }
            else
            {
                std::cout << "Clean (" << percent_anomaly << ")" << std::endl;
                std::cout << "Confidence:   " << ( threshhold - percent_anomaly ) << std::endl;

                sum_confidence_clean += ( threshhold - percent_anomaly );

                true_negatives++;
                correct++;
            }
        }
    }

    std::cout << std::endl << "End of clean test files." << std::endl << std::endl;

    for( uint_fast32_t i = 0; i < infected_files.size(); i++ )
    {
        std::cout << "Test File: " << infected_files[i] << std::endl;
        std::cout << "Actual: Infected" << std::endl;

        double percent_anomaly = manager.test_file( infected_files[i] );

        if ( percent_anomaly < 0.0 )
        {
            std::cout << "ERROR.  Anomaly rating was : " << percent_anomaly << " -- see log." << std::endl;
        }
        else
        {
            std::cout << "Predicted: "; 

            if ( percent_anomaly >= threshhold )
            {
                std::cout << "Infected (" << percent_anomaly << ")" << std::endl;
                std::cout << "Confidence:    " << ( percent_anomaly - threshhold ) << std::endl;

                sum_confidence_infected += ( percent_anomaly - threshhold );

                true_positives++;
                correct++;
            }
            else
            {
                std::cout << "Clean (" << percent_anomaly << ")" << std::endl;
                std::cout << "Error:   " << ( threshhold - percent_anomaly ) << std::endl;

                sum_error_infected += ( threshhold - percent_anomaly );

                false_negatives++;
                incorrect++;
            }
        }
    }

    std::cout << std::endl << "End of infected test files." << std::endl << std::endl;

    if ( incorrect + correct > 0 )
    {
        std::cout << "RESULTS" << std::endl;
        std::cout << "Clean Test Files: " << clean_files.size() << std::endl;
        std::cout << "Infected Test Files: " << infected_files.size() << std::endl;
        std::cout << "Total Classifications: " << correct + incorrect << std::endl;
        std::cout << "Correct Classifications: " << correct << std::endl;
        std::cout << "Incorrect Classifications: " << incorrect << std::endl;

        std::cout << "True Positives: " << true_positives << std::endl;
        std::cout << "True Negatives: " << true_negatives << std::endl;
        std::cout << "False Positives: " << false_positives << std::endl;
        std::cout << "False Negatives: " << false_negatives << std::endl;

        if ( incorrect + correct > 0 )
        {
            accuracy = (double) correct / (double) ( correct + incorrect );
        }
        else
        {
            accuracy = 0.0;
        }

        if ( true_positives + false_positives > 0 )
        {
            precision = (double) true_positives / (double) ( true_positives + false_positives );
        }
        else
        {
            precision = 0.0;
        }

        if ( true_positives + false_negatives > 0 )
        {
            recall = (double) true_positives / (double) ( true_positives + false_negatives );
        }
        else
        {
            recall = 0.0;
        }

        if ( 2 * true_positives + false_positives + false_negatives  > 0 )
        {
            f1_score = (double) ( 2.0 * true_positives ) / (double) ( 2.0 * true_positives + false_positives + false_negatives );
        }
        else
        {
            f1_score = 0.0;
        }


        std::cout << "Accuracy: " << accuracy << std::endl;

        std::cout << "Precision: " << precision << std::endl;
        std::cout << "Recall: " << recall << std::endl;
        std::cout << "F1_Score: " << f1_score << std::endl;

        std::cout << "Sum confidence (clean): " << sum_confidence_clean << std::endl;
        std::cout << "Sum Error (clean): "  << sum_error_clean << std::endl;
        std::cout << "Sum confidence (infected): " << sum_confidence_infected << std::endl;
        std::cout << "Sum Error (infected): "   << sum_error_infected << std::endl;
        std::cout << "Threshhold: " << threshhold << std::endl;

        if ( used_model )
        {
            std::cout << "Model loaded from file: " << parser.get_option_string(parser.arg_at( parser.arg_index( "--loadmodel=" ) ) ) << std::endl;
        }
        else
        {
            std::cout << "ngram size: " << ngram_len << std::endl;
            std::cout << "window size: " << window_len << std::endl;
        }

        std::string save_file_name;

        if ( parser.contains_option( "--savefile=" ) )
        {
            save_file_name = parser.get_option_string( parser.arg_at( parser.arg_index( "--savefile=" ) ) );
        }
        else
        {
            std::ostringstream oss;
            oss << "n_" << ngram_len << "_w_" << window_len << ".model";
            save_file_name = oss.str();
        }

        if ( manager.save_model ( save_file_name ) )
        {
            std::cout << "Model successfully saved to file " << save_file_name << std::endl;
        }
        else
        {
            std::cout << "Couldn't save model to file " << save_file_name << std::endl;
        }


    }
}

void start ( Command_Line_Parser & parser ){

    std::string prog_name = parser.get_program_name();

    if ( is_running ( prog_name ) )
    {

        pid_t pid = Daemonizer::get_daemon_pid( prog_name );

        std::cout << prog_name << " is already running with PID " << pid << std::endl;

    }
    else
    {
        mkfifo ( pipe_name.c_str(), 0666 );

        size_t window_len = 100;
        uint_fast32_t ngram_len = 2;
        std::string out_file_name = "new_trace.log";
        std::string detection_log = "syscall_detector.log";
        bool run_daemon = true;

        if ( parser.contains_arg( "-d" ) )
        {
            std::cout << "Contains flag -d " << std::endl;
            detection_log = parser.arg_at( parser.arg_index( "-d" ) + 1 );
        }

        if ( parser.contains_arg( "-l" ) )
        {
            std::cout << "Contains flag -l" << std::endl;
            std::string tmp_arg = parser.arg_at( parser.arg_index( "-l" ) + 1 );

            if ( ASCII_Operations::is_number( tmp_arg ) )
            {
                window_len = ASCII_Operations::to_uint( tmp_arg );
            }
            else
            {
                std::cout << "Argument given to -l: " << tmp_arg << " is not a number." << std::endl;
            }
        }

        if ( parser.contains_arg( "-n" ) )
        {
            std::cout << "Contains flag -n" << std::endl;
            std::string tmp_arg = parser.arg_at( parser.arg_index( "-n" ) + 1 );

            if ( ASCII_Operations::is_number( tmp_arg ) )
            {
                ngram_len = ASCII_Operations::to_uint( tmp_arg );
            }
            else
            {
                std::cout << "Argument given to -n: " << tmp_arg << " is not a number." << std::endl;
            }
        }

        if ( parser.contains_arg( "-o" ) )// && parser.arg_index( "-o" ) < parser.num_args() - 1 )
        {
            std::cout << "Contains flag -o " << std::endl;
            out_file_name = parser.arg_at( parser.arg_index( "-o" ) + 1 );
        }

        if ( parser.contains_option( "--daemon=" ) )
        {
            std::string daemon_option = parser.get_option_string( parser.arg_at ( parser.option_index ( "--daemon=" ) ) );

            // TODO: This should be cleaned up to do a better case-insensitive comparison.
            // In the interest of time, I just wrote this quickly.

            if ( daemon_option == "0" || daemon_option == "OFF" || daemon_option == "off" 
                    || daemon_option == "Off" || daemon_option == "oFf" || daemon_option == "ofF"
                    || daemon_option == "oFF" || daemon_option == "OFf" || daemon_option == "OfF" )
            {
                run_daemon = false;
            }
            else if ( daemon_option == "1" || daemon_option == "ON" || daemon_option == "on" || daemon_option == "oN" || daemon_option == "On" )
            {
                run_daemon = true;
            }
            else
            {
                std::cerr << "WARNING: Argument " << daemon_option << " passed to option --daemon is invalid." << std::endl;
                std::cerr << "Option/argument must be of the form: --daemon=[ON|1] to run as daemon or --daemon=[OFF|0] to run" << std::endl;
                std::cerr << "as a normal application (see help: run with -h or --help)." << std::endl;
            }
        }




        std::cout << "You chose to print to file: " << out_file_name << std::endl;

        if ( run_daemon )
        {
            std::cout << "Detector will be run as a daemon." << std::endl;
        }
        else
        {
            std::cout << "Daemonization disabled.  Detector will be run as a regular application." << std::endl;
        }

        if ( parser.contains_option( "--arch=" ) )
        {
            std::string tmp_arch = parser.get_option_string( parser.arg_at ( parser.option_index ( "--arch=" ) ) );

            uint_fast8_t trace_arch;

            if ( tmp_arch.find( "ARM" ) != std::string::npos || tmp_arch.find( "arm" ) != std::string::npos )
            {
                trace_arch = ARCH_ARM;
            }
            else if ( tmp_arch.find( "MIPS" ) != std::string::npos || tmp_arch.find("mips") != std::string::npos )
            {
                trace_arch = ARCH_MIPS;
            }
            else
            {
                trace_arch = ARCH_DEFAULT;
            }

            Detector_Manager manager( window_len, ngram_len, detection_log, out_file_name, prog_name, trace_arch );

        }
        else
        {
            Detector_Manager manager( window_len, ngram_len, detection_log, out_file_name, prog_name );
        }

        Detector_Manager manager( window_len, ngram_len, detection_log, out_file_name, prog_name );

        // Having a model supercedes having a trace file.

        if ( parser.contains_arg( "-m" ) )
        {
            std::cout << "Contains flag -m" << std::endl;

            std::string tmp_arg = parser.arg_at( parser.arg_index( "-m" ) + 1 );

            std::cout << "Attempting to load model from file " << tmp_arg << "..." << std::endl;

            if ( manager.train_on_model( tmp_arg ) )
            {
                std::cout << "Model loaded!" << std::endl;
            }
            else
            {
                std::cout << "Failed to load model." << std::endl;
            }
        }
        else if ( parser.contains_arg( "-t" ) )
        {
            std::string tmp_arg = parser.arg_at( parser.arg_index( "-t" ) + 1 );

            std::cout << "Attempting to train from trace data in file " << tmp_arg << "..." << std::endl;

            if ( manager.train_on_trace( tmp_arg ) )
            {
                std::cout << "Model trained!" << std::endl;
            }
            else
            {
                std::cout << "Could not train from trace in file " << tmp_arg << std::endl;
            }
        }

        manager.run_detector( run_daemon );

        std::cout << "From launcher, manager has finished run_detector." << std::endl;
    }
}

void status ( Command_Line_Parser & parser ){

    if ( is_running( parser.get_program_name() ) )
    {
        std::cout << "The detector is running with PID " << Daemonizer::get_daemon_pid( parser.get_program_name() ) << std::endl;
    }
    else
    {
        std::cout << "The detector is not running." << std::endl;
    }
}

void stop ( Command_Line_Parser & parser ){

    if ( !is_running( parser.get_program_name() ) )
    {
        std::cout << "The detector is not running." << std::endl;
    }
    else
    {
        int32_t fd;

        fd = open( pipe_name.c_str(), O_WRONLY );

        std::string stop = "STOP";

        const char* c_stop = stop.c_str();

        if ( !write( fd, c_stop, sizeof( c_stop ) ) )
        {
            std::cerr << "ERROR: Couldn't write to " << pipe_name << " -- perhaps you need root/sudo?" << std::endl;
        }
        else
        {
            // FIXME: This error checking was hacked in QUICKLY, hence the bad variable names
            // like "tmp_result" and the magic numbers and things.

            int32_t tmp_result = close( fd );


            if ( tmp_result != 0 )
            {
                std::cerr << "ERROR: Could not close the file descriptor for pipe: " << pipe_name << std::endl;
            }
            else
            {
                std::cout << "File descriptor for " << pipe_name << " closed successfully." << std::endl;
            }

            tmp_result = unlink( pipe_name.c_str() );

            if ( tmp_result != 0 )
            {
                std::cerr << "ERROR: Could not unlink file " << pipe_name << std::endl;
            }
            else
            {
                std::cout << "File " << pipe_name << " unlinked successfully." << std::endl;
            }

            tmp_result = Daemonizer::remove_daemon_pid( parser.get_program_name() );

            if ( tmp_result != 0 )
            {
                std::cerr << "ERROR: Could not remove pid for program " << parser.get_program_name() << std::endl;
            }
            else
            {
                std::cout << "Program " << parser.get_program_name() << " pid removed." << std::endl;
            }
        }
    }
}

bool is_running( std::string program_name ){

    pid_t pid = Daemonizer::get_daemon_pid( program_name );

    return pid > 0;
}
