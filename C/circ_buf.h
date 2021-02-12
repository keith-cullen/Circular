/****************************
 *    Copyright (c) 2010    *
 *    Keith Cullen          *
 ****************************/

#ifndef CIRC_BUF_H
#define CIRC_BUF_H

/* total space available in the circular buffer */
#define circ_buf_space(cb) (((cb)->tail - (cb)->head - 1) & ((cb)->len - 1))

/* total number of bytes present in the circular buffer */
#define circ_buf_count(cb) (((cb)->head - (cb)->tail) & ((cb)->len - 1))

/* space available to the end of the linear buffer */
#define circ_buf_space_to_end(cb)                                                            \
({                                                                                           \
    unsigned space_end_linear_buf = (cb)->len - (cb)->head;                                  \
    unsigned space_end_circ_buf = ((cb)->tail + space_end_linear_buf - 1) & ((cb)->len - 1); \
    space_end_linear_buf < space_end_circ_buf ? space_end_linear_buf : space_end_circ_buf;   \
})

/* number of bytes present up to the end of the linear buffer */
/* this special version is required by circ_buf_peek */
#define circ_buf_count_to_end_(cb, tail)                                                   \
({                                                                                         \
    unsigned count_end_linear_buf = (cb)->len - (tail);                                    \
    unsigned count_end_circ_buf = ((cb)->head + count_end_linear_buf) & ((cb)->len - 1);   \
    count_end_circ_buf < count_end_linear_buf ? count_end_circ_buf : count_end_linear_buf; \
})

/* number of bytes present up to the end of the linear buffer */
#define circ_buf_count_to_end(cb)  circ_buf_count_to_end_(cb, (cb)->tail)

/* determine if the buffer is empty or not */
#define circ_buf_is_empty(cb)  ((cb)->tail == (cb)->head)

/* wrap an index past the end of the linear buffer back to the start */
#define circ_buf_wrap_index(cb, i)  ((i) & ((cb)->len - 1))

/* conert a read index into the circular buffer to an index into the linear buffer */
#define circ_buf_read_index(cb, i)  (circ_buf_wrap_index((cb)->tail + (i)))

/* get the value at a position in the circular buffer */
#define circ_buf_get_val(cb, i)  ((cb)->buf[circ_buf_read_index((cb), (i)])

typedef struct
{
    unsigned head; /* in index */
    unsigned tail; /* out index */
    unsigned len;  /* must be an integer power of 2 */
    char *buf;
}
circ_buf_t;

void circ_buf_init(circ_buf_t *cb, char *buf, unsigned len);
int circ_buf_peek(circ_buf_t *cb, char *buf, unsigned len);
int circ_buf_consume(circ_buf_t *cb, unsigned len);
int circ_buf_read(circ_buf_t *cb, char *buf, unsigned len);
int circ_buf_write(circ_buf_t *cb, const char *buf, unsigned len);

#endif
