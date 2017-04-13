/*
 *  File Name : sensor_data.h
 *  
 *  Creation Date : 05-09-2016
 *
 *  Last Modified : Wed 12 Apr 2017 08:13:02 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <string>

using std::string;

class Sensor_Data{
    public:

        /*
         * Constructor
         */
        
        // Default Constructor

        Sensor_Data(); // CHECK: This caused problems because there were multiple definitions.  It is only declared here, defined in sensor_data.cxx.

        // Alternate Constructor

        Sensor_Data( string operating_system_label,  // For Labeling the OS
                string data_label,              // What type of data are recorded
                string data,                    // The data
                string aux="" );                // Possibly used in future.  Defaults to an empty string.

        // Copy Constructor

        Sensor_Data( const Sensor_Data & other );

        /*
         *  Destructor
         */

        ~Sensor_Data() {}

        /*
         * Accessors/Inspectors
         */

        string get_operating_system_label() const;
        string get_data_label() const;
        string get_data() const;
        string get_aux() const;

    private:

        /*
         * Data Members
         */

        string operating_system_label_;
        string data_label_;
        string data_;
        string aux_;
};
