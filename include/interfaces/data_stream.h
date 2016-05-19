#pragma once

#include "../sensor_data/sensor_data.h"

class Data_Stream{

    public:
        
        virtual void process_data ( Sensor_Data record ) = 0;

};
