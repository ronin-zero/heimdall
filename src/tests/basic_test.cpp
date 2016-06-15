/*
 *  File Name : basic_test.cpp
 *  
 *  Creation Date :
 *
 *  Last Modified : Wed 15 Jun 2016 02:19:49 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

// The purpose of this file is just to test that the thing
// can actually run with a simple Syscall_Sensor and
// Syscall_Logger. 

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "sensors/syscall_sensors/syscall_sensor.h"
#include "sensor_observers/syscall_logger.h"
#include "sensor_observers/data_streams/output_stream.h"

using std::cout;
using std::cin;
using std::string;
using std::endl;

/*
uint_fast8_t PROCESS_NAME   =   0x80;
uint_fast8_t PID            =   0x40;
uint_fast8_t CPU            =   0x20;
uint_fast8_t TRACE_FLAGS    =   0x10;
uint_fast8_t TIMESTAMP      =   0x08;
uint_fast8_t SYSCALL_NUM    =   0X04;
uint_fast8_t SYSCALL_ARGS   =   0x02;
*/
uint_fast8_t FLAGS =    TIMESTAMP | PROCESS_NAME | PID | SYSCALL_NUM ;

int main(){

//    Syscall_Sensor *sensor = Syscall_Sensor::get_instance();

    Syscall_Sensor * sensor;//->get_instance();
    sensor = Syscall_Sensor::get_instance();

    Syscall_Logger logger;

    string filename = "test.txt";
    cout << filename << endl;
//    string filename;

//    cout << "Please enter a filename: ";

//    cin >> filename;

    Output_Stream s_out( filename, FLAGS, "\t" );

    logger.add_stream( &s_out );

    sensor->attach_observer( &logger );

    sensor->start_sensing();

    cout << "Enter anything to exit." << endl;

    string tmp;

    cin >> tmp;

    return 0;
}
