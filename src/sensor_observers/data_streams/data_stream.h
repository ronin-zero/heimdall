#pragma once

#include "sensor_observers/data_records/data_record.h"

class Data_Stream{

    public:
        
        virtual void process_data ( Data_Record* record )=0;
        virtual ~Data_Stream() {};
};
