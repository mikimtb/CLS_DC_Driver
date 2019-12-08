/*
 * config_menu.h
 *
 *  Created on: Dec 8, 2019
 *      Author: Miroslav
 */

#ifndef CONFIG_MENU_H_
#define CONFIG_MENU_H_

#include "definitions.h"

struct menu_node {
   char name[4];
   struct menu_node *next;
   struct menu_node *prev;
   struct menu_node *down;
   struct menu_node *up;
   void (*menu_item_handler)(uint8_t event);
};

void MENU_build_node(struct menu_node *current_node, char name[4], struct menu_node *prev_node, struct menu_node *next_node, struct menu_node *up_node, struct menu_node *down_node, void (*menu_item_handler)(uint8_t event));
void MENU_up(struct menu_node **current_node);
void MENU_down(struct menu_node **current_node);
void MENU_enter(struct menu_node **current_node);
void MENU_save_and_return(struct menu_node **current_node);

#endif /* CONFIG_MENU_H_ */
