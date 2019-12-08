/*
 * config_menu.h
 *
 *  Created on: Dec 8, 2019
 *      Author: Miroslav
 */

#ifndef CONFIG_MENU_H_
#define CONFIG_MENU_H_

#include "definitions.h"
#include "tm1637.h"

typedef enum _menu_events_e
{
	menu_up_event = 2,
	menu_down_event,
	menu_enter_event,
	menu_save_and_return_event
} menu_events_e;

struct menu_node {
   char name[2];
   struct menu_node *next;
   struct menu_node *prev;
   struct menu_node *down;
   struct menu_node *up;
   void (*do_work_handler)(menu_events_e e);
};

void MENU_build_node(struct menu_node *current_node, char name[2], struct menu_node *prev_node, struct menu_node *next_node,
				struct menu_node *up_node, struct menu_node *down_node, void (*do_work_handler)(menu_events_e e));
void MENU_up(struct menu_node **current_node, menu_events_e e);
void MENU_down(struct menu_node **current_node, menu_events_e e);
void MENU_enter(struct menu_node **current_node, menu_events_e e);
void MENU_save_and_return(struct menu_node **current_node, menu_events_e e);

#endif /* CONFIG_MENU_H_ */
