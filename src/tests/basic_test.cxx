/*
 *  File Name : basic_test.cxx
 *  
 *  Creation Date :
 *
 *  Last Modified :
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

int main(){

    Syscall_Sensor * sensor;
    sensor = Syscall_Sensor::get_instance();

    Syscall_Logger logger;

    string filename;

    cout << "Please enter a filename: ";

    cin >> filename;

    Output_Stream s_out( filename, TIMESTAMP | PROCESS_NAME | PID | SYSCALL_NUM, "\t" );

    logger.add_stream( &s_out );

    sensor->Sensor::attach_observer( &logger );

    sensor->Sensor::start_sensing();

    cout << "Enter anything to exit." << endl;

    string tmp;

    cin >> tmp;

    return 0;
}
