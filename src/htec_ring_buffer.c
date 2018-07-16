/*
 * htec_ring_buffer.c
 *
 *  Created on: Mar 1, 2018
 *      Author: Miroslav Bozic
 */

#include "htec_ring_buffer.h"

uint32_t ring_buffer_get_count(buffer_t* buff)
{
	return (buff->next_in - buff->next_out);
}

bool ring_buffer_isEmpty(buffer_t* buff)
{
	if (ring_buffer_get_count(buff) == 0)
		return true;
	else
		return false;
}

bool ring_buffer_isFull(buffer_t* buff)
{
	if (ring_buffer_get_count(buff) == buff->max_length)
		return true;
	else
		return false;
}

void ring_buffer_init(buffer_t* buff)
{
	buff->next_in = 0;
	buff->next_out = 0;
}

bool ring_buffer_enQ(buffer_t* buff, uint8_t data)
{

	bool result = ring_buffer_isFull(buff);
	if (!result)
	{
		buff->buffer[buff->next_in++ % buff->max_length] = data;
	}

	return !result;
}

bool ring_buffer_deQ(buffer_t* buff, uint8_t* data)
{
	bool result = ring_buffer_isEmpty(buff);
	if (!result)
	{
		*data = buff->buffer[buff->next_out++ % buff->max_length];
	}

	return !result;
}

