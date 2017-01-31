#pragma once

#include <cstdint>
#include <string>
#include <thread>

#include "sensor_data/sensor_data.h"
#include "queues/readerwriterqueue.h"

using std::thread;
using std::string;

typedef moodycamel::ReaderWriterQueue<Sensor_Data> queue;

const uint_fast32_t OBSERVER_QUEUE_SIZE = 100; // CHECK: As with include/sensors/sensor.h, this figure is arbitrary for now.

class Sensor_Observer{

    public:

        Sensor_Observer() : data_queue() {}
        virtual ~Sensor_Observer() {}

        virtual void update() = 0;
        virtual void update( Sensor_Data data ) = 0;

        // This will be similar to the methods that set, toggle, and
        // check the sensing and reading status of the sensors and
        // readers, respectively.
        //
        // They will at first be simple getters and setters, but there
        // may come a time when a method to set/toggle/check observing
        // and/or processing is more complicated than flipping a boolean.

        virtual void set_observing( bool observe ) = 0;
        virtual void set_processing( bool process ) = 0;

        virtual bool observing_status() = 0;
        virtual bool processing_status() = 0;
        
        virtual void start_observing() = 0;
        virtual void start_processing() = 0;

        virtual void stop_observing() = 0;
        virtual void stop_processing() = 0;


    protected:

        virtual void process() = 0;

        queue data_queue;

        thread processing_thread;

        bool observing;
        bool processing;
};
