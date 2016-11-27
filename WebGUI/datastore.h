/*
 * PLC GUI
 * 
 * 2016 Andrew Palardy
 *
 * datastore.h
 * Header for the Data Store
 */

#ifndef _WEBUI_H
#define _WEBUI_H

extern struct mg_server *server;
void webui_init();
void webui_task();
static int webui_ev_handler(struct mg_connection *conn, enum mg_event ev);

#endif