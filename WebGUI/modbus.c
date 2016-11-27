/*
 * PLC GUI
 *
 * 2016 Andrew Palardy
 *
 * Reads data from a CLICK PLC and publish the results on a web interface
 *
 * modbus.c
 * Modbus functions
 */

/* Includes */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "modbus.h"


/* webui_init
 * Inputs: None
 * Outputs: None
 * Initializes webui
 */
void webui_init()
{
    //Setup the webserver
    server = mg_create_server(NULL, webui_ev_handler);
    
    //Configure the webserver
    //NOTE: This will fail if we are not running privelaged
    mg_set_option(server, "listening_port", "80");  // Open port 80
    mg_set_option(server, "enable_directory_listing","no");//No directory listing!
}

//Process task
void webui_task()
{
    //Poll the server
    mg_poll_server(server, 50);
}
