/****************************
 *    Copyright (c) 2010    *
 *    Keith Cullen          *
 ****************************/

/*
 *  A queue implemented using a contiguous buffer with separate indices for reading and writing.
 *
 *  Bytes are copied in to and out from the buffer.
 *
 *  The head index is the next location to be written. It is incremented when items
 *  are added and wraps to zero when it reaches the end of the linear buffer.
 *
 *  The tail index is the next location to be read. It is incremented when items
 *  are removed and wraps to zero when it reaches the end of the linear buffer.
 *
 *  When the head index is equal to the tail index, the circular buffer is empty.
 *  When the head index is one less than the tail index, the circular buffer is full.
 */

#include <string.h>
#include "circ_buf.h"

void circ_buf_init(circ_buf_t *cb, char *buf, unsigned len)
{
    memset(buf, 0, len);
    cb->head = 0;
    cb->tail = 0;
    cb->buf = buf;
    cb->len = len;
}

/*  read data but don't update tail
 *  (2 consecutive peek operations with the same arguments will produce the same result)
 *  returns number of bytes read
 */
int circ_buf_peek(circ_buf_t *cb, char *buf, unsigned len)
{
    unsigned tail = cb->tail;
    unsigned num = 0;
    int ret = 0;

    while (1)
    {
        num = circ_buf_count_to_end_(cb, tail);
        if (len < num)
            num = len;
        if (num <= 0)
            break;
        memcpy(buf, cb->buf + tail, num);
        tail = circ_buf_wrap_index(cb, tail + num);
        buf += num;
        len -= num;
        ret += num;
    }
    return ret;
}

/*  returns number of bytes read
 */
int circ_buf_consume(circ_buf_t *cb, unsigned len)
{
    unsigned num = 0;
    int ret = 0;

    while (1)
    {
        num = circ_buf_count_to_end(cb);
        if (len < num)
            num = len;
        if (num <= 0)
            break;
        cb->tail = circ_buf_wrap_index(cb, cb->tail + num);
        len -= num;
        ret += num;
    }
    return ret;
}

/*  returns number of bytes read
 */
int circ_buf_read(circ_buf_t *cb, char *buf, unsigned len)
{
    unsigned num = 0;
    int ret = 0;

    while (1)
    {
        num = circ_buf_count_to_end(cb);
        if (len < num)
            num = len;
        if (num <= 0)
            break;
        memcpy(buf, cb->buf + cb->tail, num);
        cb->tail = circ_buf_wrap_index(cb, cb->tail + num);
        buf += num;
        len -= num;
        ret += num;
    }
    return ret;
}

/*  returns number of bytes written
 */
int circ_buf_write(circ_buf_t *cb, const char *buf, unsigned len)
{
    unsigned num = 0;
    int ret = 0;

    while (1)
    {
        num = circ_buf_space_to_end(cb);
        if (len < num)
            num = len;
        if (num <= 0)
            break;
        memcpy(cb->buf + cb->head, buf, num);
        cb->head = circ_buf_wrap_index(cb, cb->head + num);
        buf += num;
        len -= num;
        ret += num;
    }
    return ret;
}
