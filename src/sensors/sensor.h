/*
 *  File Name : sensor.h
 *  
 *  Creation Date : 05-09-2016
 *
 *  Last Modified : Fri 05 Oct 2018 04:28:46 AM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <cstdint>
#include <unordered_set>
#include <thread>

#include "sensor_data/sensor_data.h"
#include "sensor_observers/sensor_observer.h"
#include "queues/readerwriterqueue.h"

typedef moodycamel::ReaderWriterQueue<Sensor_Data> queue;

using std::thread;

// Sensor constants
static const uint_fast8_t SENSING_OFF   = 0x00;
static const uint_fast8_t SENSING_ON    = 0x01;
//static const uint_fast32_t SENSOR_QUEUE_SIZE = 100; // CHECK: This is entirely arbitrary right now. If something works better, it will be changed. CURRENTLY UNUSED.

class Sensor {

    public:

        // As of 5/26/2016, this compiles and *should* work.
        // The sensor abstract class merely
        // initializes a queue. All sensors
        // should have a queue like this.

        Sensor() : data_queue () {} // CHECK: I gave "SENSOR_QUEUE_SIZE" as an argument at one point while debugging, but I don't believe it's necessary.
        virtual ~Sensor() {} // CHECK: I had to define this one in the header.

       
        // These methods will vary from sensor to sensor.
        // As such, they are pure virtual and derived classes
        // must provide a specific implementation.
        
        virtual bool is_sensing() = 0;

        virtual uint_fast8_t set_sensing( bool on ) = 0;
        virtual uint_fast8_t sensing_status() = 0;

        virtual uint_fast8_t start_sensing() = 0;
        virtual uint_fast8_t stop_sensing() = 0;
        virtual uint_fast8_t configure( uint_fast8_t flags ) = 0;

        // These methods should be the same for all sensors.

        void attach_observer( Sensor_Observer * observer ) { observers.insert( observer ); }
        void detach_observer( Sensor_Observer * observer ) { observers.erase( observer ); }
        uint_fast32_t observer_count() { return observers.size(); }

    protected:

        // All sensors will have these data members
        // in common.

        thread sense_thread;
        thread notify_thread;

        std::unordered_set<Sensor_Observer *> observers;

        queue data_queue;

        uint_fast8_t status;

        // These methods will vary from sensor to sensor.
        // Thus, they are pure virtual and must be
        // defined in all derived classes.

        virtual Sensor_Data * sense_data() = 0;
        virtual void sense() = 0;
        virtual void notify_observers() = 0;

        // This is so we detatch and delete all of the observers.

        void clear_observers() { observers.clear(); }
};
