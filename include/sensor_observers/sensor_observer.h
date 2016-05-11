#pragma once

#include "sensor_data/sensor_data.h"

class Sensor_Observer{

    public:
    
        void update();
        void update( Sensor_Data );

};
