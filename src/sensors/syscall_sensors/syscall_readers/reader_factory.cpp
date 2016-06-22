/*
 *  File Name : reader_factory.cpp
 *  
 *  Creation Date : 09-05-2016
 *
 *  Last Modified : Wed 22 Jun 2016 01:41:46 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "reader_factory.h"

Reader_Factory::Reader_Factory( uint_fast8_t flags ){

    status = flags;
}

uint_fast8_t Reader_Factory::set_status( uint_fast8_t flags ){

    status = flags;
    return status;
}

Reader * Reader_Factory::create_reader(){

    Reader * concrete_reader = Reader::get_instance( status );
    return concrete_reader;
}
