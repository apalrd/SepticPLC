//
//  webui.h
//
//
//  Copyright 2014 Andrew Palardy
//
//  HVAC controller system
//  Web User Interface
//

#ifndef _WEBUI_H
#define _WEBUI_H

extern struct mg_server *server;
void webui_init();
void webui_task();
static int webui_ev_handler(struct mg_connection *conn, enum mg_event ev);

#endif