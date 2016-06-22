/*
 *  File Name : linux_syscall_record.cpp
 *  
 *  Creation Date : 01-06-2016
 *
 *  Last Modified : Wed 22 Jun 2016 01:22:39 PM EDT
 *
 *  Created By : ronin-zero (浪人ー無)
 *
 */

#include "linux_syscall_record.h"
#include "utils/ascii_operations.h"


Linux_Syscall_Record::Linux_Syscall_Record( Sensor_Data data, uint_fast8_t settings_flags, string sep )
: Data_Record( data, settings_flags, sep ){

}

/*
string Linux_Syscall_Record::raw_string() const{

    return raw_data;
}
*/

// The basic pattern of the "get_<x>" methods is mostly the same:
// Find the start and/or end of the field using find or rfind as 
// appropriate searching from the index (explained in 
// linux_syscall_record.h, lines 27-74, particularly line 65, and 
// defined on lines 76-81) corresponding to the relevant field.
// Often, the index for a field will be either the first or last
// char of that field, but not always (eg. TASK_INDEX is the last 
// char of the TASK field, PID_INDEX is the first char of the PID 
// field, but TIMESTAMP_INDEX and FUNCTION_INDEX point into the 
// middle of the field). The difference in indices plus 1 will be 
// the number of characters to read when substringing, hence the 
// "+ 1" magic numbers.
//
// Please forgive those.

string Linux_Syscall_Record::get_task() const{

    uint_fast8_t field_start = raw_data.rfind(' ', TASK_INDEX ) + 1;

    uint_fast8_t field_size = ( TASK_INDEX - field_start ) + 1;

    string task = raw_data.substr( field_start, field_size );

    return task;
}

string Linux_Syscall_Record::get_pid() const{

    uint_fast8_t field_end = raw_data.find( ' ', PID_INDEX );

    uint_fast8_t field_size = field_end - PID_INDEX;

    string pid = raw_data.substr( PID_INDEX, field_size );

    return pid;

}

string Linux_Syscall_Record::get_cpu() const{

    uint_fast8_t field_start = raw_data.rfind( '[', CPU_INDEX ) + 1;

    uint_fast8_t field_end = raw_data.find( ']', CPU_INDEX );   // Really, it's one char before that,
    // but leaving it this way makes calculating
    // the size of the field more simple.
    uint_fast8_t field_size = field_end - field_start;

    string cpu = raw_data.substr( field_start, field_size );

    return cpu;
}

string Linux_Syscall_Record::get_trace_flags() const{

    string trace_flags = raw_data.substr( TFLAGS_INDEX, NUM_TFLAGS );

    return trace_flags;
}

string Linux_Syscall_Record::get_timestamp() const{

    uint_fast8_t field_start    = raw_data.rfind( ' ', TIMESTAMP_INDEX ) + 1;
    uint_fast8_t field_end      = raw_data.find( ':', TIMESTAMP_INDEX );    // The last char in this field is
    // actually one prior to this, but
    // as with get_cpu(), leaving it
    // like this makes the math for getting
    // the size of the field simpler
    uint_fast8_t field_size     = field_end - field_start;

    string timestamp = raw_data.substr( field_start, field_size );

    return timestamp;
}

// This one is trickier.  The system call number field is after the function name.
// The function name should always be sys_enter (for our purposes), and its index is 50.
// I am not comfortable assuming it's in the same place, so all I can tell is that there
// is a field between the function name and the system call number.
// This means that there are two spaces between the function name and the syscall number.
// This gets the first space after the function index, then gets the first space after
// that space's index.  The next char after that index will be the first char of the
// system call number, which will be followed by a space.
//
// CHECK:  I'm not super happy with how this is done, but it should work.  I am reticent
// to get the start of the field by finding the first index of the string "NR" after
// FUNCTION_INDEX, which seems like it would be simpler on the surface.  I can't
// seem to find any documentation that guarantees the format of a record produced by
// ftrace into trace_pipe.  If this causes problems and there's a solution, by all
// means: change it.

string Linux_Syscall_Record::get_syscall() const{

    uint_fast8_t tmp = raw_data.find( ' ', FUNCTION_INDEX );

    uint_fast8_t field_start    = raw_data.find( ' ', tmp + 1 ) + 1;
    uint_fast8_t field_end      = raw_data.find( ' ', field_start );    // Again, as with get_timestamp() and get_cpu(),
    // the last field is actually one char prior to
    // this index, but it simplifies getting the length
    // of the field.
    uint_fast8_t field_size     = field_end - field_start;

    string syscall = raw_data.substr( field_start, field_size );

    return syscall;
}

// For this method, it would seem easier to get the index of the '(' char
// from the right side and just call substring at that index.
// I am not sure if the '(' character could potentially appear within
// the arguments of the system call (not super-well documented), so
// I don't think that's a safe method of pulling the arguments.
// Furthermore, a malicious user could name a process such that it includes
// the a '(' character.  This would cause major problems.
//
// Basically, I've opted to find the index of the system call number
// like in get_syscall(), then find the index of the first space after that.
// the next character should be the '(' character that begins the arguments to
// the system call.  I am worried about the time this might take, however.
//
// CHECK: I am concerned that this may be a performance bottleneck.
// This is one of the places that I should check when optimizing or if
// the program is running too slowly.

string Linux_Syscall_Record::get_syscall_args() const{

    uint_fast8_t tmp = raw_data.find( ' ', FUNCTION_INDEX );

    uint_fast8_t syscall_start = raw_data.find( ' ', tmp + 1 ) + 1;

    uint_fast8_t field_start = raw_data.find( ' ', syscall_start + 1 ) + 1;

    // Because this is the last field, we can just grab everything after the
    // index of the start of the field.  A second argument to substr isn't needed.
    string syscall_args = raw_data.substr( field_start );

    return syscall_args;
}

//  At present, even on a 64 bit system, the maximum PID value 
//  is 2^22 and a PID will never be negative.  As such, I calculate
//  the PID value as a uint_fast32_t.

uint_fast32_t Linux_Syscall_Record::get_pid_num() const{

    return ASCII_Operations::to_uint( get_pid() );
}

uint_fast16_t Linux_Syscall_Record::get_syscall_num() const{

    return ASCII_Operations::to_uint( get_syscall() );
}

uint_fast16_t Linux_Syscall_Record::get_cpu_num() const{

    return ASCII_Operations::to_uint( get_cpu() );
}

double Linux_Syscall_Record::get_timestamp_num() const{

    return ASCII_Operations::to_floating_point( get_timestamp() );
}
/*
uint_fast8_t Linux_Syscall_Record::check_flags() const{

    return flags;
}
*/

// Protected methods

// This will print out the fields specified by the flags,
// separated by commas.

void Linux_Syscall_Record::print( ostream& s_out ) const{

    // We will keep delim as an empty character.
    // If there are no fields to be printed, we
    // will not need to separate them with anything.
    // Once a field appears, we will change it to
    // whatever our separator is (comma by default: ',').
    // This way, we won't need to check a nested 
    // if-statement every single time we print a new field.

    bool needs_sep = false;

    if ( flags & PROCESS_NAME )
    {
        s_out << get_task();

        needs_sep = true;
    }

    if ( flags & PID )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_pid();

        needs_sep = true;
    }

    if ( flags & CPU )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_cpu();

        needs_sep = true;
    }

    if ( flags & TRACE_FLAGS )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_trace_flags();

        needs_sep = true;
    }

    if ( flags & TIMESTAMP )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_timestamp();

        needs_sep = true;
    }

    if ( flags & SYSCALL_NUM )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }
        
        s_out << get_syscall();

        needs_sep = true;
    }

    if ( flags & SYSCALL_ARGS )
    {
        if ( needs_sep )
        {
            s_out << separator;
        }

        s_out << get_syscall_args();
    }
}
