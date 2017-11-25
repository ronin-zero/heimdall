/*
 *  File Name : syscall_detector.h
 *  
 *  Creation Date : 10-04-2016
 *
 *  Last Modified : Sat 25 Nov 2017 01:12:00 AM EST
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#pragma once

#include <string>
#include <thread>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

#include "utils/ascii_operations.h"
#include "sensor_data/sensor_data.h"
#include "sensor_observers/sensor_observer.h"
#include "sensor_observers/data_records/data_record.h"
#include "sensor_observers/data_records/system_call_record.h"
#include "sensor_observers/detectors/trace_window.h"
#include "sensor_observers/detectors/trace_reader.h"
#include "sensor_observers/detectors/data_point_generator.h"
#include "sensor_observers/detectors/ngram_generator.h"
#include "sensor_observers/detectors/svm_modules/svm_module.h"
#include "sensor_observers/detectors/svm_modules/one_class_svm.h"
#include "sensor_observers/detectors/support_vector_generator.h"
#include "sensor_observers/data_records/linux/linux_syscall_record.h"
#include "sensor_observers/detectors/linux/syscall_formatters/arch/arm/arm_syscall_formatter.h"
#include "sensor_observers/detectors/linux/syscall_formatters/syscall_formatter.h"
#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_syscall_formatter.h"
#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_o32_syscall_formatter.h"
#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_n32_syscall_formatter.h"
#include "sensor_observers/detectors/linux/syscall_formatters/arch/mips/mips_n64_syscall_formatter.h"

// Architecture Constants
//
// NOTE: There is probably a better way to do this, 
// but after spending hours trying to come up with
// a more clever way and looking into precompiler
// macro definitions, I decided to go with what I
// declared to be "good enough" and move on.
//
// The idea is that the rightmost 4 bits indicate
// basic instruction set families with 0000 being
// a generic or unknown instruction set.
//
// The next bit to the left indicates endiannness
// where a value of 0 indicates little endianness
// and a value of 1 indicates big endianness.
//
// The next bit to the left after that indicates
// whether the architecture is 32-bit or 64-bit
// where 0 indicates 32-bit architecture and 1
// indicates 64-bit architecture.
//
// The leftmost two bits are left to provide
// information that is specific to architecture
// families.
//
// Largely arbitrarily, little endianness and
// 32-bit architecture is treated as the default
// with a value of 0 in their respective bits.
//
// Not all instruction sets are supported.
// 
// See the definitions to be sure.

static const uint_fast8_t   ARCH_DEFAULT    =   0x00;
static const uint_fast8_t   ARCH_MASK       =   0x0F;
static const uint_fast8_t   OPTS_MASK       =   0xF0;

// Basic architecture families  (x86, ARM, MIPS...)

static const uint_fast8_t   ARCH_x86        =   0x01;
static const uint_fast8_t   ARCH_ARM        =   0x02;
static const uint_fast8_t   ARCH_MIPS       =   0x04;

// MIPS ABI specific constants...

static const uint_fast8_t   OABI_32        =   0x10;
static const uint_fast8_t   NABI_32        =   0x50;
static const uint_fast8_t   NABI_64        =   0x60;

// CHECK: I'm not sure the MIPS defines used are universal.

#ifdef __arm__
static const uint_fast8_t HOST_ARCH = ARCH_ARM;
#elif __mips__
#if ( _MIPS_SIM == _MIPS_SIM_ABI32 )
static const uint_fast8_t HOST_ARCH = ( ARCH_MIPS | OABI_32 );
#elif ( _MIPS_SIM == _MIPS_SIM_NABI32 )
static const uint_fast8_t HOST_ARCH = ( ARCH_MIPS | NABI_32 );
#elif ( _MIPS_SIM == _MIPS_SIM_ABI64 )
static const uint_fast8_t HOST_ARCH = ( ARCH_MIPS | NABI_64 );
#else
static const uint_fast8_t HOST_ARCH = ARCH_MIPS;
#endif
#else
static const uint_fast8_t HOST_ARCH = ARCH_DEFAULT;
#endif
/*  
*  For now, only Linux is supported.  If other operating systems are added later, handle 
*  them similarly.
*/

#ifdef __linux__
typedef Linux_Syscall_Record Syscall_Record;
#else
typedef System_Call_Record Syscall_Record;
#endif

class Syscall_Detector : public Sensor_Observer{

    public:

        Syscall_Detector( size_t window_size, uint_fast32_t ngram_length, std::string detection_log_file_name, uint_fast8_t arch = HOST_ARCH );

        ~Syscall_Detector();

        bool train_from_trace( const std::string file_name, double class_label = 0.0, uint_fast8_t sep = ',' );
        double test_trace_file( const std::string file_name, uint_fast8_t sep = ',' );

        bool train_from_saved_model( const std::string file_name );
        
        bool train_model();

        void update();
        void update( Sensor_Data data );

        void set_observing( bool on );
        void set_processing( bool on );

        bool observing_status();
        bool processing_status();

        void start_observing();
        void start_processing();

        void stop_observing();
        void stop_processing();

        //bool start_training();
        //bool stop_training();

        bool save_model( const std::string file_name );

        //void set_trace_window( Trace_Window * window );
        //void set_generator( Data_Point_Generator * genenerator );

    private:

        std::ofstream detection_log;

        char * current_time();

        void process();

        void send_data( Syscall_Record & record );

        void process_data_point( uint_fast32_t data_point );
        void process_data_point( uint_fast32_t data_point, Trace_Window & window, Support_Vector_Generator & sv_generator );
        void process_data_vector( struct svm_node * node );

        std::vector<struct svm_node *> get_trace_vectors( Trace_Reader & trace_reader );

        bool fill_generator( Trace_Window & window, Support_Vector_Generator & sv_generator );
        struct svm_node * get_current_vector();
        struct svm_node * get_current_vector( Trace_Window window );


        uint_fast8_t _arch;

        // bool update_window();
        
        // bool generate_data();

        Syscall_Formatter * _call_formatter;
        NGram_Generator * _ngram_generator;
        Trace_Window _window;
        Support_Vector_Generator _sv_generator;
        One_Class_SVM _svm_module;
};
