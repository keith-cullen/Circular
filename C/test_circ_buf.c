/****************************
 *    Copyright (c) 2010    *
 *    Keith Cullen          *
 ****************************/

#include <stdio.h>
#include <string.h>
#include "circ_buf.h"

#define BUF_LEN  8

struct test_space_data
{
    unsigned num_write[2];
    unsigned num_read[2];
};

struct test_space_data test_space_data =
{
    .num_write = {6, 4},
    .num_read = {6, 4}
};

void test_space_func(const char *name, struct test_space_data *test_data)
{
    circ_buf_t cb = {0};
    unsigned space = 0;
    unsigned i = 0;
    unsigned j = 0;
    char buf[BUF_LEN] = {0};
    char val = 0;
    int pass = 1;

    printf("%-60s...", name);

    circ_buf_init(&cb, buf, sizeof(buf));
    for (i = 0; i < test_data->num_write[0]; i++)
    {
        val = i;
        circ_buf_write(&cb, &val, 1);
        space = circ_buf_space(&cb);
        if (space != BUF_LEN - i - 2)
        {
            pass = 0;
        }
    }
    j = space;
    for (i = 0; i < test_data->num_read[0]; i++)
    {
        circ_buf_read(&cb, &val, 1);
        space = circ_buf_space(&cb);
        if (space != j + i + 1)
        {
            pass = 0;
        }
    }
    j = space;
    for (i = 0; i < test_data->num_write[1]; i++)
    {
        val = i;
        circ_buf_write(&cb, &val, 1);
        space = circ_buf_space(&cb);
        if (space != j - i - 1)
        {
            pass = 0;
        }
    }
    j = space;
    for (i = 0; i < test_data->num_read[1]; i++)
    {
        circ_buf_read(&cb, &val, 1);
        space = circ_buf_space(&cb);
        if (space != j + i + 1)
        {
            pass = 0;
        }
    }
    printf("%s\n", pass ? "PASS" : "FAIL");
}

struct test_count_data
{
    const char *desc;
    unsigned num_write[2];
    unsigned num_read[2];
};

struct test_count_data test_count_data =
{
    .num_write = {6, 4},
    .num_read = {6, 4}
};

void test_count_func(const char *name, struct test_count_data *test_data)
{
    circ_buf_t cb = {0};
    unsigned count = 0;
    unsigned i = 0;
    unsigned j = 0;
    char buf[BUF_LEN] = {0};
    char val = 0;
    int pass = 1;

    printf("%-60s...", name);

    circ_buf_init(&cb, buf, sizeof(buf));
    for (i = 0; i < test_data->num_write[0]; i++)
    {
        val = i;
        circ_buf_write(&cb, &val, 1);
        count = circ_buf_count(&cb);
        if (count != i + 1)
        {
            pass = 0;
        }
    }
    j = count;
    for (i = 0; i < test_data->num_read[0]; i++)
    {
        circ_buf_read(&cb, &val, 1);
        count = circ_buf_count(&cb);
        if (count != j - i - 1)
        {
            pass = 0;
        }
    }
    j = count;
    for (i = 0; i < test_data->num_write[1]; i++)
    {
        val = i;
        circ_buf_write(&cb, &val, 1);
        count = circ_buf_count(&cb);
        if (count != j + i + 1)
        {
            pass = 0;
        }
    }
    j = count;
    for (i = 0; i < test_data->num_read[1]; i++)
    {
        circ_buf_read(&cb, &val, 1);
        count = circ_buf_count(&cb);
        if (count != j - i - 1)
        {
            pass = 0;
        }
    }
    printf("%s\n", pass ? "PASS" : "FAIL");
}

struct test_read_data
{
    const char *str;
    unsigned start;
    unsigned end;
    unsigned len;
    unsigned space_to_end;
    unsigned num_iter;
    int expected_ret[3];
    const char *expected[3];
};

struct test_read_data test_read_smaller_data =
{
    .str = "abcd123",
    .start = 0,
    .end = 7,
    .len = 6,
    .space_to_end = BUF_LEN - 1,
    .num_iter = 3,
    .expected_ret = {6, 1, 0},
    .expected = {"abcd12", "3", ""}
};

struct test_read_data test_read_larger_data =
{
    .str = "abcd123",
    .start = 0,
    .end = 7,
    .len = 12,
    .space_to_end = BUF_LEN - 1,
    .num_iter = 2,
    .expected_ret = {7, 0, 0},
    .expected = {"abcd123", "", ""}
};

struct test_read_data test_tail_gt_head_read_smaller_data =
{
    .str = "abcd123",
    .start = 5,
    .end = 4,
    .len = 6,
    .space_to_end = BUF_LEN - 5,
    .num_iter = 3,
    .expected_ret = {6, 1, 0},
    .expected = {"abcd12", "3", ""}
};

struct test_read_data test_tail_gt_head_read_larger_data =
{
    .str = "abcd123",
    .start = 5,
    .end = 4,
    .len = 12,
    .space_to_end = BUF_LEN - 5,
    .num_iter = 2,
    .expected_ret = {7, 0, 0},
    .expected = {"abcd123", "", ""}
};

void test_read_func(const char *name, struct test_read_data *test_data)
{
    circ_buf_t cb = {0};
    unsigned i = 0;
    size_t str_len = 0;
    char buf[BUF_LEN] = {0};
    char out[test_data->len];
    int pass = 1;
    int ret = 0;

    printf("%-60s...", name);

    circ_buf_init(&cb, buf, sizeof(buf));
    str_len = strlen(test_data->str);
    if (str_len <= test_data->space_to_end)
    {
        memcpy(cb.buf + test_data->start, test_data->str, str_len);
    }
    else
    {
        memcpy(cb.buf + test_data->start, test_data->str, test_data->space_to_end);
        str_len -= test_data->space_to_end;
        memcpy(cb.buf, test_data->str + test_data->space_to_end, str_len);
    }
    cb.head = test_data->end;
    cb.tail = test_data->start;
    for (i = 0; i < test_data->num_iter; i++)
    {
        memset(out, 0, test_data->len);
        ret = circ_buf_read(&cb, out, test_data->len);
        if (ret != test_data->expected_ret[i])
        {
            pass = 0;
        }
        if (strncmp(out, test_data->expected[i], test_data->len) != 0)
        {
            pass = 0;
        }
    }
    printf("%s\n", pass ? "PASS" : "FAIL");
}

struct test_write_data
{
    const char *str;
    unsigned start;
    unsigned end;
    unsigned num_iter;
    int expected_ret[4];
    const char expected[4][BUF_LEN];
};

struct test_write_data test_write_smaller_data =
{
    .str = "abc",
    .start = 0,
    .end = 0,
    .num_iter = 4,
    .expected_ret = {3, 3, 1, 0},
    .expected = {{'a', 'b', 'c',  0,   0,   0,   0,   0},
                 {'a', 'b', 'c', 'a', 'b', 'c',  0,   0},
                 {'a', 'b', 'c', 'a', 'b', 'c', 'a',  0},
                 {'a', 'b', 'c', 'a', 'b', 'c', 'a',  0}}
};

struct test_write_data test_write_larger_data =
{
    .str = "abcd1234",
    .start = 0,
    .end = 0,
    .num_iter = 2,
    .expected_ret = {7, 0, 0, 0},
    .expected = {{'a', 'b', 'c', 'd', '1', '2', '3',  0},
                 {'a', 'b', 'c', 'd', '1', '2', '3',  0},
                 { 0,   0,   0,   0,   0,   0,   0,   0},
                 { 0,   0,   0,   0,   0,   0,   0,   0}}
};

struct test_write_data test_head_tail_nz_write_smaller_data =
{
    .str = "abc",
    .start = 6,
    .end = 6,
    .num_iter = 4,
    .expected_ret = {3, 3, 1, 0},
    .expected = {{'c',  0,   0,   0,   0,   0, 'a', 'b'},
                 {'c', 'a', 'b', 'c',  0,   0, 'a', 'b'},
                 {'c', 'a', 'b', 'c', 'a',  0, 'a', 'b'},
                 {'c', 'a', 'b', 'c', 'a',  0, 'a', 'b'}}
};

struct test_write_data test_head_tail_nz_write_larger_data =
{
    .str = "abcd1234",
    .start = 6,
    .end = 6,
    .num_iter = 2,
    .expected_ret = {7, 0, 0, 0},
    .expected = {{'c', 'd', '1', '2', '3',  0, 'a', 'b'},
                 {'c', 'd', '1', '2', '3',  0, 'a', 'b'},
                 { 0,   0,   0,   0,   0,   0,   0,   0},
                 { 0,   0,   0,   0,   0,   0,   0,   0}}
};

void test_write_func(const char *name, struct test_write_data *test_data)
{
    circ_buf_t cb = {0};
    unsigned i = 0;
    char buf[BUF_LEN] = {0};
    int pass = 1;
    int ret = 0;

    printf("%-60s...", name);

    cb.head = test_data->end;
    cb.tail = test_data->start;
    cb.len = sizeof(buf);
    cb.buf = buf;
    for (i = 0; i < test_data->num_iter; i++)
    {
        ret = circ_buf_write(&cb, test_data->str, strlen(test_data->str));
        if (ret != test_data->expected_ret[i])
        {
            pass = 0;
        }
        if (memcmp(cb.buf, test_data->expected[i], BUF_LEN) != 0)
        {
            pass = 0;
        }
    }
    printf("%s\n", pass ? "PASS" : "FAIL");
}

struct test_peek_consume_data
{
    const char *desc;
    const char *str;
    unsigned start;
    unsigned end;
    unsigned len;
    unsigned space_to_end;
    unsigned num_iter;
    int expected_ret[3];
    const char *expected[3];
};

struct test_peek_consume_data test_peek_consume_smaller_data =
{
    .str = "abcd123",
    .start = 0,
    .end = 7,
    .len = 6,
    .space_to_end = 7,
    .num_iter = 3,
    .expected_ret = {6, 1, 0},
    .expected = {"abcd12", "3", ""}
};

struct test_peek_consume_data test_peek_consume_larger_data =
{
    .str = "abcd123",
    .start = 0,
    .end = 7,
    .len = 12,
    .space_to_end = 7,
    .num_iter = 2,
    .expected_ret = {7, 0, 0},
    .expected = {"abcd123", ""}
};

struct test_peek_consume_data test_tail_gt_head_peek_consume_smaller_buffer =
{
    .str = "abcd123",
    .start = 5,
    .end = 4,
    .len = 6,
    .space_to_end = 3,
    .num_iter = 3,
    .expected_ret = {6, 1, 0},
    .expected = {"abcd12", "3", ""}
};

struct test_peek_consume_data test_tail_gt_head_peek_consume_larger_data =
{
    .str = "abcd123",
    .start = 5,
    .end = 4,
    .len = 12,
    .space_to_end = 3,
    .num_iter = 2,
    .expected_ret = {7,  0},
    .expected = {"abcd123", ""}
};

void test_peek_consume_func(const char *name, struct test_peek_consume_data *test_data)
{
    circ_buf_t cb = {0};
    unsigned i = 0;
    unsigned j = 0;
    size_t str_len = 0;
    char buf[BUF_LEN] = {0};
    char out[test_data->len];
    int pass = 1;
    int ret = 0;

    printf("%-60s...", name);

    circ_buf_init(&cb, buf, sizeof(buf));
    str_len = strlen(test_data->str);
    if (str_len <= test_data->space_to_end)
    {
        memcpy(cb.buf + test_data->start, test_data->str, str_len);
    }
    else
    {
        memcpy(cb.buf + test_data->start, test_data->str, test_data->space_to_end);
        str_len -= test_data->space_to_end;
        memcpy(cb.buf, test_data->str + test_data->space_to_end, str_len);
    }
    cb.head = test_data->end;
    cb.tail = test_data->start;
    for (i = 0; i < test_data->num_iter; i++)
    {
        for (j = 0; j < 2; j++)
        {
            memset(out, 0, test_data->len);
            ret = circ_buf_peek(&cb, out, test_data->len);
            if (ret != test_data->expected_ret[i])
            {
                pass = 0;
            }
            if (strncmp(out, test_data->expected[i], test_data->len) != 0)
            {
                pass = 0;
            }
        }
        ret = circ_buf_consume(&cb, test_data->len);
        if (ret != test_data->expected_ret[i])
        {
            pass = 0;
        }
    }
    printf("%s\n", pass ? "PASS" : "FAIL");
}

int main()
{
    test_space_func("count", &test_space_data);
    test_count_func("space", &test_count_data);
    test_read_func("read data into smaller buffer", &test_read_smaller_data);
    test_read_func("read data into larger buffer", &test_read_larger_data);
    test_read_func("tail > head, read data into smaller buffer", &test_tail_gt_head_read_smaller_data);
    test_read_func("tail > head, read data into larger buffer", &test_tail_gt_head_read_larger_data);
    test_write_func("write data from smaller buffer", &test_write_smaller_data);
    test_write_func("write data from larger buffer", &test_write_larger_data);
    test_write_func("tail and head > 0, write data from smaller buffer", &test_head_tail_nz_write_smaller_data);
    test_write_func("tail and head > 0, write data from larger buffer", &test_head_tail_nz_write_larger_data);
    test_peek_consume_func("peek/consume data into smaller buffer", &test_peek_consume_smaller_data);
    test_peek_consume_func("peek/consume data into larger buffer", &test_peek_consume_larger_data);
    test_peek_consume_func("tail > head, peek/consume data into smaller buffer", &test_tail_gt_head_peek_consume_smaller_buffer);
    test_peek_consume_func("tail > head, peek/consume data into larger buffer", &test_tail_gt_head_peek_consume_larger_data);

    return 0;
}
