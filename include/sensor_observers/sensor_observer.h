#pragma once

#include <cstdint>

#include "sensor_data/sensor_data.h"
#include "queues/readerwriterqueue.h"

using std::thread;
using std::string;

typedef moodycamel::ReaderWriterQueue<Sensor_Data> queue;

const uint_fast32_t OBSERVER_QUEUE_SIZE = 100; // CHECK: As with include/sensors/sensor.h, this figure is arbitrary for now.

class Sensor_Observer{

    public:

        Sensor_Observer() : data_queue( OBSERVER_QUEUE_SIZE ) {}

        virtual void update()=0;
        virtual void update( Sensor_Data data )=0;

        // This will be similar to the methods that set, toggle, and
        // check the sensing and reading status of the sensors and
        // readers, respectively.
        //
        // They will at first be simple getters and setters, but there
        // may come a time when a method to set/toggle/check observing
        // and/or processing is more complicated than flipping a boolean.

        virtual uint_fast8_t set_observing( bool observe )=0;
        virtual uint_fast8_t set_processing( bool process )=0;

        virtual uint_fast8_t observing_status()=0;
        virtual uint_fast8_t processing_status()=0;
        
        virtual uint_fast8_t toggle_observing()=0;
        virtual uint_fast8_t toggle_processing()=0;

        virtual uint_fast8_t start_observing()=0;
        virtual uint_fast8_t start_processing()=0;

        virtual uint_fast8_t stop_observing()=0;
        virtual uint_fast8_t stop_processing()=0;

    protected:

        queue data_queue;

        thread observing_thread;
        thread processing_thread;

        bool observing;
        bool processing;
};
