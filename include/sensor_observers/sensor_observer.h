#pragma once

#include "sensor_data/sensor_data.h"

class Sensor_Observer{

    public:
        virtual void update() = 0;
        virtual void update( Sensor_Data ) = 0;

};
