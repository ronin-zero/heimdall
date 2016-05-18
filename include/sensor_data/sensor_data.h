/*
 *  File:       sensor_data.h
 *  Author:     ronin_zero (浪人ー無) 
 *
 *  Created:    Monday, May 9 2016, 12:53PM
 *
 */

#pragma once

#include <stdint.h>
#include <string>

using std::string;

class Sensor_Data{
    public:

        /*
         * Constructor
         */

        Sensor_Data( string operating_system_label,  // For Labeling the OS
                string data_label,              // What type of data are recorded
                string data,                    // The data
                string aux="" );                // Possibly used in future.  Defaults to an empty string.

        // Copy Constructor

        Sensor_Data( Sensor_Data & other );

        /*
         *  Destructor
         */

        ~Sensor_Data();

        /*
         * Accessors/Inspectors
         */

        string get_operating_system_label();
        string get_data_label();
        string get_data();
        string get_aux();

    private:

        /*
         * Data Members
         */

        string operating_system_label_;
        string data_label_;
        string data_;
        string aux_;
};
