#pragma once

#include <stdint.h>
#include <string>
#include <set>

#include "sensor_data.h"
#include "concurrentqueue/blockingconcurrentqueue.h"

typedef blockingconcurrentqueue queue;

// Sensor constants

const fast_uint8_t SENSING_OFF = 0x00;


class Sensor {

public:     // Public

    /*
     *  Constructor(s)
     */

    Sensor() {}

    /*
     *  Destructor
     */

    virtual ~Sensor()=0;

    /*
     *  Accessors/Inspectors
     */
    
    virtual fast_uint8_t sensing_status();
    virtual bool sensing();
    virtual Sensor_Data sense_data()=0;

    fast_uint32_t observer_count();

    /*
     *  Facilitators
     */

    virtual void sense()=0;

    /*
     *  Mutators
     */

    virtual fast_uint8_t toggle_sensing()=0;

    void attach_observer( Sensor_Observer & );
    void detatch_observer( Sensor_Observer & );
    
private:    // Private

    /*
     * Variables
     */

    bool sensing = false;
    set<Sensor_Observer &> observers;
    status fast_uint8_t = SENSING_OFF;

protected:  // Protected

    /*
     * Variables
     */

    queue<Sensor_Data> data_queue;
    string data_label = "";
    string operating_system_label = "";
    string aux = "";

    /*
     *  Facilitators
     */

    virtual void notify_observers( Sensor_Data sensor )=0;
    virtual void notify_observers()=0;

};

Sensor::~Sensor() {}
