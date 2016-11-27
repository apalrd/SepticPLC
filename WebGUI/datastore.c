/*
 * PLC GUI
 *
 * 2016 Andrew Palardy
 *
 * datastore.c
 * Data Store functions
 */


/* Includes */
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "mongoose.h"
#include "webui.h"
#include "modbus.h"

/* Global Variables */
struct mg_server *server;

/* webui_init
 * Inputs: None
 * Outputs: None
 * Initializes webui
 */
void webui_init()
{
    /* Setup webserver */
    server = mg_create_server(NULL, webui_ev_handler);
    
    /* Confiure the webserver
     * This is where the port is set
     * Must be privelaged to use port 80!
     */
    mg_set_option(server, "listening_port", "8080");
    mg_set_option(server, "enable_directory_listing","no");
}

/* webui_task
 * Inputs: None
 * Outputs: None
 * Processes webui
 */
void webui_task()
{
    /* Mongoose must be polled */
    mg_poll_server(server, 50);
}

/* webui_ev_handler
 * Inputs:
 *      (struct mg_connection*) Mongoose connection struct
 *      (enum mg_event)         Mongoose event
 * Outputs: (static int) Mongoose return value
 * Handle a page request from Mongoose
 */
static int webui_ev_handler(struct mg_connection *conn, enum mg_event ev)
{
    if (ev == MG_AUTH)
    {
        //Received an AUTH request. Authorize it.
        return MG_TRUE;   // Authorize all requests
    }
    else if (ev == MG_REQUEST)
    {
        //Got a request. Check to see the page being requested
        
        //Check to see if this is the hello page to indicate the server is up
        if(!strcmp(conn->uri, "/hello"))
        {
            //This is a hello request, print a H1
            mg_printf_data(conn,"<h1>Hello World</h1>\n");
            return MG_TRUE;
        }
        else if(!strcmp(conn->uri, "/page"))
        {
            //This is a page request, submit the page (from pages.c)
            mg_send_file(conn,"page.html",NULL);
            //Request was processed but there is more
            return MG_MORE;
        }
        else if(!strcmp(conn->uri, "/data"))
        {
            //This is a data request, submit the sensor data to sensors
            
            //Variables we will need
            char tempchar[32];
            int id = -1;
            float val = 0;
            int rtn;
            int valid = 1;
            
            //Get an argument 'id'
            mg_get_var(conn, "id", tempchar, sizeof(tempchar));
            
            //Get the ID out into an int
            rtn = sscanf(&tempchar,"%d",&id);
            
            //Check to see if we got enough numbers
            if(rtn != 1)
            {
                //We didn't get a number, we are invalid
                valid = 0;
            }
            
            //Now do the same for the argument 'val'
            mg_get_var(conn, "val", tempchar, sizeof(tempchar));
            
            //Get the ID out into an int
            rtn = sscanf(&tempchar,"%f",&val);
            
            //Check to see if we got enough numbers
            if(rtn != 1)
            {
                //We didn't get a number, we are invalid
                valid = 0;
            }
            
            //If we got enough numbers
            if(valid)
            {
                //The request is valid, work with it
                
                //Submit the data to sensors
                valid = sensors_web_update(id,val);
            }
            
            if(valid)
            {
                //Data was valid, send a GOOD INPUT back to the client
                mg_send_header(conn, "Content-Type", "text/plain");
                mg_printf_data(conn,"GOOD INPUT\n");
            }
            else
            {
                //Data was not valid, send a BAD INPUT back to the client
                mg_send_header(conn, "Content-Type", "text/plain");
                mg_printf_data(conn,"BAD INPUT\n");
            }
            //Finished processing this request
            return MG_TRUE;
        }
        else if(!strcmp(conn->uri, "/ajax"))
        {
            //This is an AJAX request. Print out the AJAX reply.
            mg_send_header(conn, "Content-Type", "text/plain");
            
            //First send the temperatures of each zone
            mg_printf_data(conn,"Zone1 Temp: %2.1f degC<br>\n",sensors_get_val(SENS_ZONE_1));
            mg_printf_data(conn,"Zone2 Temp: %2.1f degC<br>\n",sensors_get_val(SENS_ZONE_2));
            mg_printf_data(conn,"Onboard Exterior Temp: %2.1f degC<br>\n",sensors_get_val(SENS_EXT_OB));
            mg_printf_data(conn,"RF Exterior Temp: %2.1f degC<br>\n",sensors_get_val(SENS_RF));
            mg_printf_data(conn,"RF Humidity: %3.0f \%<br><br>\n",sensors_get_val(SENS_RF_HUMID));
            
            //Next send the zone setpoints
            mg_printf_data(conn,"Zone1 Setpoint: %2.0f degC<br>\n",zone_setpt[ZONE_1]);
            mg_printf_data(conn,"Zone2 Setpoint: %2.0f degC<br>\n",zone_setpt[ZONE_2]);
            
            //Next send the actuator commands??
            //TODO send the actuator commands
            
            //Return that we have finished the page
            return MG_TRUE;
            
        }
        else
        {
            //No page to process, send a 404 page
            mg_printf_data(conn,"<h1>404 page error!</h1>\n");
            return MG_TRUE;
        }
        return MG_TRUE;   // Mark as processed
    }
    else if(ev == MG_HTTP_ERROR)
    {
        //Error is an HTTP error, send it as a 404
        mg_printf_data(conn,"<h1>404 page error!</h1>\n");
        return MG_TRUE;
    }
    else
    {
        return MG_FALSE;  // Rest of the events are not processed
    }
}