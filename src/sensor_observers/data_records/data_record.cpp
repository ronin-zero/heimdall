/*
 *  File Name : data_record.cpp
 *  
 *  Creation Date : 06-10-2016
 *
 *  Last Modified : Sun 25 Nov 2018 12:38:49 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "data_record.h"

Data_Record::Data_Record( const Sensor_Data& data, uint_fast8_t settings_flags, std::string sep ){

    raw_data = data.get_data();
    flags = settings_flags;
    separator = sep;
}

Data_Record::Data_Record( const std::smatch& matches, uint_fast8_t settings_flags, std::string sep ){

    raw_data = matches[0];
    flags = settings_flags;
    searator = sep;
}

string Data_Record::raw_string() const{

    return raw_data;
}

void Data_Record::set_flags( uint_fast8_t new_flags ){

    flags = new_flags;
}

uint_fast8_t Data_Record::get_flags(){

    return flags;
}

void Data_Record::set_separator( string sep ){

    separator = sep;
}

string Data_Record::get_separator(){

    return separator;
}

// This must be defined like this in order to have polymorphic behavior of <<

ostream& operator<<( ostream& s_out, Data_Record const& record ){

        record.print( s_out );
        return s_out;
}

// This is to give default behavior if there isn't a 
// type of record available for that type of data/os.
// It just prints out the raw data. 

void Data_Record::print( ostream& s_out ) const{

    s_out << raw_string();
}
