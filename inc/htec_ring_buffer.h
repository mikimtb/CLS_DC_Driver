/*
 * htec_ring_buffer.h
 *
 *  Created on: Mar 1, 2018
 *      Author: Miroslav Bozic
 */

#ifndef HTEC_RING_BUFFER_H_
#define HTEC_RING_BUFFER_H_

#include "stm32f10x.h"
#include <stdbool.h>
#include "definitions.h"

//Type definitions
/** Ring buffer type definition */
typedef struct buffer
{
	uint8_t * const buffer; 	/*!< The pointer that pointing on the first element in the array */
	uint16_t next_in;			/*!< Ring buffer head, keeps the index where new element will be stored */
	uint16_t next_out;			/*!< Ring buffer tail, keeps the index of the byte that should be taken */
	const uint16_t max_length;	/*!< Ring buffer size */
} buffer_t;

/**
 * Macro definitions
 * The macro should be used each time new buffer should be created
 */
#define CIRCBUFF_DEF(x, y)				\
	uint8_t x##_dataSpace[y];			\
	buffer_t x = { 						\
		.buffer = x##_dataSpace,		\
		.next_in = 0,					\
		.next_out = 0,					\
		.max_length = y					\
	}

/**
 * Function return how many bytes are stored in the buffer
 * @param [in] buff pointer to the buffer
 * @return number of stored bytes
 */
uint32_t ring_buffer_get_count(buffer_t* buff);

/**
 * Function check whether the buffer is empty
 * @param [in] buff pointer to the buffer
 * @return buffer_empty if buffer is empty, buffer_no_error if buffer is not empty
 */
bool ring_buffer_isEmpty(buffer_t* buff);

/**
 * Function check whether the buffer is full
 * @param [in] buff pointer to the buffer
 * @return buffer_full if buffer is full, buffer_no_error if buffer is not full
 */
bool ring_buffer_isFull(buffer_t* buff);

/**
 * Function initialize the buffer
 * @param [in] buff pointer to the buffer that should be initialized
 */
void ring_buffer_init(buffer_t* buff);

/**
 * Function push byte to the ring buffer
 * @param [in] buff pointer to the buffer
 * @param [in] data byte that will be pushed to the buffer
 * @return	false if buffer is full and enqueue is failed, true if enqueue is successful
 */
bool ring_buffer_enQ(buffer_t* buff, uint8_t data);

/**
 * Function pop one byte from ring buffer
 * @param [in] buff pointer to the buffer
 * @param [out] data data that is dequeued from the buffer
 * @return false if buffer is empty and de-enqueue is failed, true if de-enqueue is successful
 */
bool ring_buffer_deQ(buffer_t* buff, uint8_t* data);

#endif /* HTEC_RING_BUFFER_H_ */
