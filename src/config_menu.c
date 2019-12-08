/*
 * config_menu.c
 *
 *  Created on: Dec 8, 2019
 *      Author: Miroslav
 */
#include "config_menu.h"

void MENU_build_node(struct menu_node *current_node, char name[4], struct menu_node *prev_node, struct menu_node *next_node, struct menu_node *up_node, struct menu_node *down_node, void (*menu_item_handler)(uint8_t event))
{
	strcpy(current_node->name, name);
	current_node->prev = prev_node;
	current_node->next = next_node;
	current_node->up = up_node;
	current_node->down = down_node;
	current_node->menu_item_handler = menu_item_handler;
}

void MENU_up(struct menu_node **current_node)
{
	// Move to up node if there is any
	if ((*current_node)->up != NULL)
	{
		(*current_node) = (*current_node)->up;
	}
}

void MENU_down(struct menu_node **current_node)
{
	// Move to up node if there is any
	if ((*current_node)->down != NULL)
	{
		(*current_node) = (*current_node)->down;
	}
}

void MENU_enter(struct menu_node **current_node)
{
	// Move to next node if there is any
	if ((*current_node)->next != NULL)
	{
		(*current_node) = (*current_node)->next;
	}
}

void MENU_save_and_return(struct menu_node **current_node)
{
	// Move to up node if there is any
	if ((*current_node)->prev != NULL)
	{
		(*current_node) = (*current_node)->prev;
	}
}

