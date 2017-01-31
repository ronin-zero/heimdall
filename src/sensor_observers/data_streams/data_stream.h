/*
 *  File Name : data_stream.h
 *  
 *  Creation Date : 05-20-2016
 *
 *  Last Modified : Tue 31 Jan 2017 03:28:17 PM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include "sensor_observers/data_records/data_record.h"

class Data_Stream{

    public:
        
        virtual void process_data ( Data_Record& record )=0;
        virtual ~Data_Stream() {};
};
