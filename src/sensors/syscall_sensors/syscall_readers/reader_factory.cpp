/*
 *  File Name : reader_factory.cpp
 *  
 *  Creation Date : 05-09-2016
 *
 *  Last Modified : Tue 31 Jan 2017 03:35:11 PM EST
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
