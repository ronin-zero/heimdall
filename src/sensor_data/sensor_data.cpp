/*
 *  File:       sensor_data.cpp
 *  Author:     ronin_zero (浪人ー無) 
 *
 *  Created:    Monday, May 9 2016, 12:55PM
 */

#include <string>

#include "sensor_data.h"

using std::string;

Sensor_Data::Sensor_Data( string operating_system_label,
                          string data_label,
                          string data,
                          string aux){

    operating_system_label_ = operating_system_label;
    data_label_ = data_label;
    data_ = data_;
    aux_ = aux;

}

Sensor_Data::Sensor_Data( Sensor_Data & other ){

    operating_system_label_ = other.get_operating_system_label();
    data_label_ = other.get_data_label();
    data_ = other.get_data();
    aux_ = other.get_aux();
}

string Sensor_Data::get_operating_system_label(){

    return operating_system_label_;
}

string Sensor_Data::get_data_label(){

    return data_label_;
}

string Sensor_Data::get_data(){

    return data_;
}

string Sensor_Data::get_aux(){

    return aux_;
}
