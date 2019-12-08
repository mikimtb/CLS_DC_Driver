/*
 * config_menu.c
 *
 *  Created on: Dec 8, 2019
 *      Author: Miroslav
 */
#include "config_menu.h"

void MENU_build_node(struct menu_node *current_node, char name[2], struct menu_node *prev_node, struct menu_node *next_node,
				struct menu_node *up_node, struct menu_node *down_node, void (*do_work_handler)(menu_events_e e))
{
	strcpy(current_node->name, name);
	current_node->prev = prev_node;
	current_node->next = next_node;
	current_node->up = up_node;
	current_node->down = down_node;
	current_node->do_work_handler = do_work_handler;
}

void MENU_up(struct menu_node **current_node, menu_events_e e)
{
	// Check if there is do_work_handler callback for current node
	if ((*current_node)->do_work_handler != NULL)
	{
		(*current_node)->do_work_handler(e);
	}
	// Move to up node if there is any
	if ((*current_node)->up != NULL)
	{
		(*current_node) = (*current_node)->up;
	}
}

void MENU_down(struct menu_node **current_node, menu_events_e e)
{
	// Check if there is do_work_handler callback for current node
	if ((*current_node)->do_work_handler != NULL)
	{
		(*current_node)->do_work_handler(e);
	}
	// Move to up node if there is any
	if ((*current_node)->down != NULL)
	{
		(*current_node) = (*current_node)->down;
	}
}

void MENU_enter(struct menu_node **current_node, menu_events_e e)
{
	// Check if there is do_work_handler callback for current node
	if ((*current_node)->do_work_handler != NULL)
	{
		(*current_node)->do_work_handler(e);
	}
	// Move to up node if there is any
	if ((*current_node)->next != NULL)
	{
		(*current_node) = (*current_node)->next;
	}
}

void MENU_save_and_return(struct menu_node **current_node, menu_events_e e)
{
	// Check if there is do_work_handler callback for current node
	if ((*current_node)->do_work_handler != NULL)
	{
		(*current_node)->do_work_handler(e);
	}
	// Move to up node if there is any
	if ((*current_node)->prev != NULL)
	{
		(*current_node) = (*current_node)->prev;
	}
}

