/**
* This header file provides function protoypes for working with command arguments from the user.
* Relies on the model component.
* @file command.h
* @author Matthew Kierski
*/

#include <stdbool.h>


/**
* This function reads user command from standard input and performs that command
* updating or using the given contest instance as necessary. If the user enters
* an invalid command, this function returns false, true otherwise.
* @param *contest pointer to contest to update if necessary
* @return true if valid command, false otherwise
*/
bool processCommand( Contest *contest );
