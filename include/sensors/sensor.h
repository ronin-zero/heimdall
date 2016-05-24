#pragma once

#include <stdint.h>
#include <string>
#include <unordered_set>

#include "sensor_data/sensor_data.h"
#include "sensor_observers/sensor_observer.h"
#include "concurrentqueue/blockingconcurrentqueue.h"


typedef moodycamel::BlockingConcurrentQueue<Sensor_Data> queue;

// Sensor constants

const uint_fast8_t SENSING_OFF = 0x00;


class Sensor {

    public:

        Sensor() {}
        virtual ~Sensor()=0;

        virtual uint_fast8_t sensing_status()=0;
        virtual bool is_sensing()=0;
        virtual Sensor_Data sense_data()=0;

        uint_fast32_t observer_count();

        virtual void sense()=0;

        virtual uint_fast8_t toggle_sensing()=0;
        virtual uint_fast8_t start_sensing()=0;
        virtual uint_fast8_t stop_sensing()=0;

        void attach_observer( Sensor_Observer * observer );
        void detatch_observer( Sensor_Observer * observer );


    protected:

        bool sensing = false;
        std::unordered_set<Sensor_Observer *> observers;
        uint_fast8_t status = SENSING_OFF;

        queue data_queue;
        string data_label = "";
        string operating_system_label = "";
        string aux = "";

        virtual void notify_observers( Sensor_Data data )=0;
        virtual void notify_observers()=0; 
        virtual uint_fast8_t set_sensing( bool on )=0;

};

Sensor::~Sensor() {}

void Sensor::attach_observer( Sensor_Observer * observer ){

    observers.insert( observer );
}


void Sensor::detatch_observer( Sensor_Observer * observer ){

    observers.erase( observer );
}

uint_fast32_t Sensor::observer_count(){

    observers.size();
}

