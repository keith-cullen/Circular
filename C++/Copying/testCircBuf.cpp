// +--------------------------+
// |                          |
// |    Copyright (c) 2010    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

#include "CircBuf.h"
#include <gtest/gtest.h>
#include <array>
#include <thread>

using namespace Circular::Copying;

const std::size_t circBufLen = 8;
const std::size_t maxNumIter = (circBufLen + circBufLen / 2);

typedef int Elem;

struct TestCopyConstructorData
{
    std::size_t head;
    std::size_t tail;
};

TestCopyConstructorData testCopyConstructorData =
{
    .head = 2,
    .tail = 6
};

void testCopyConstructorFunc(TestCopyConstructorData* data)
{
    Elem i = 0;

    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        p1[i] = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2(cb1);

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2 = cb2.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2[i], i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has remained the same
    std::array<Elem, circBufLen>& p3 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3[i], i);
    }
    ASSERT_EQ(cb1.head(), data->head);
    ASSERT_EQ(cb1.tail(), data->tail);
}

struct TestMoveConstructorData
{
    std::size_t head;
    std::size_t tail;
};

TestMoveConstructorData testMoveConstructorData =
{
    .head = 2,
    .tail = 6
};

void testMoveConstructorFunc(TestMoveConstructorData* data)
{
    Elem i = 0;

    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        p1[i] = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2(std::move(cb1));

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2 = cb2.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2[i], i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has not remained the same
    std::array<Elem, circBufLen>& p3 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3[i], 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestCopyAssignmentData
{
    std::size_t head;
    std::size_t tail;
};

TestCopyAssignmentData testCopyAssignmentData =
{
    .head = 2,
    .tail = 6
};

void testCopyAssignmentFunc(TestCopyAssignmentData* data)
{
    Elem i = 0;

    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        p1[i] = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2;
    cb2 = cb1;

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2 = cb2.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2[i], i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has remained the same
    std::array<Elem, circBufLen>& p3 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3[i], i);
    }
    ASSERT_EQ(cb1.head(), data->head);
    ASSERT_EQ(cb1.tail(), data->tail);
}

struct TestMoveAssignmentData
{
    std::size_t head;
    std::size_t tail;
};

TestMoveAssignmentData testMoveAssignmentData =
{
    .head = 2,
    .tail = 6
};

void testMoveAssignmentFunc(TestMoveAssignmentData* data)
{
    Elem i = 0;

    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        p1[i] = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2;
    cb2 = std::move(cb1);

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2 = cb2.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2[i], i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb2 has not remained the same
    std::array<Elem, circBufLen>& p3 = cb1.buf();
    for (i = 0; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3[i], 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestCountSpaceData
{
    std::size_t numWrite[2];
    std::size_t numRead[2];
};

TestCountSpaceData testSpaceData =
{
    .numWrite = {6, 4},
    .numRead = {6, 4}
};

void testSpaceFunc(TestCountSpaceData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t space = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem val = 0;

    for (i = 0; i < data->numWrite[0]; i++)
    {
        val = i;
        cb.write(&val, 1);
        space = cb.space();
        ASSERT_EQ(space, circBufLen - i - 2);
    }
    j = space;
    for (i = 0; i < data->numRead[0]; i++)
    {
        cb.read(&val, 1);
        space = cb.space();
        ASSERT_EQ(space, j + i + 1);
    }
    j = space;
    for (i = 0; i < data->numWrite[1]; i++)
    {
        val = i;
        cb.write(&val, 1);
        space = cb.space();
        ASSERT_EQ(space, j - i - 1);
    }
    j = space;
    for (i = 0; i < data->numRead[1]; i++)
    {
        cb.read(&val, 1);
        space = cb.space();
        ASSERT_EQ(space, j + i + 1);
    }
}

TestCountSpaceData testCountData =
{
    .numWrite = {6, 4},
    .numRead = {6, 4}
};

void testCountFunc(TestCountSpaceData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t count = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem val = 0;

    for (i = 0; i < data->numWrite[0]; i++)
    {
        val = i;
        cb.write(&val, 1);
        count = cb.count();
        ASSERT_EQ(count, i + 1);
    }
    j = count;
    for (i = 0; i < data->numRead[0]; i++)
    {
        cb.read(&val, 1);
        count = cb.count();
        ASSERT_EQ(count, j - i - 1);
    }
    j = count;
    for (i = 0; i < data->numWrite[1]; i++)
    {
        val = i;
        cb.write(&val, 1);
        count = cb.count();
        ASSERT_EQ(count, j + i + 1);
    }
    j = count;
    for (i = 0; i < data->numRead[1]; i++)
    {
        cb.read(&val, 1);
        count = cb.count();
        ASSERT_EQ(count, j - i - 1);
    }
}

struct TestPopData
{
    const Elem str[circBufLen];
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t spaceToEnd;
    std::size_t numIter;
    std::size_t expectedNum[maxNumIter];
    const Elem expected[maxNumIter];
};

TestPopData testPopData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .spaceToEnd = circBufLen - 1,
    .numIter = maxNumIter,
    .expectedNum = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected = {1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0}
};

TestPopData testTailGtHeadPopData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .spaceToEnd = circBufLen - 5,
    .numIter = maxNumIter,
    .expectedNum = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected = {1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0}
};

void testPopFunc(TestPopData* data)
{
    CircBuf<Elem, circBufLen> circBuf;
    std::size_t num = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem val = 0;

    std::array<Elem, circBufLen>& p = circBuf.buf();
    j = data->start;
    for (i = 0; i < data->strLen; i++)
    {
        p[j] = data->str[i];
        if (++j >= circBuf.len())
        {
            j = 0;
        }
    }
    circBuf.head(data->end);
    circBuf.tail(data->start);
    for (i = 0; i < data->numIter; i++)
    {
        val = 0;
        num = circBuf.pop(val);
        ASSERT_EQ(num, data->expectedNum[i]);
        ASSERT_EQ(val, data->expected[i]);
    }
}

struct TestPushData
{
    const Elem str[maxNumIter];
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t expectedNum[maxNumIter];
    const Elem expected[maxNumIter][circBufLen];
};

TestPushData testPushData =
{
    .str = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    .strLen = maxNumIter,
    .start = 0,
    .end = 0,
    .expectedNum = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected = {{1, 0, 0, 0, 0, 0, 0, 0},
                 {1, 2, 0, 0, 0, 0, 0, 0},
                 {1, 2, 3, 0, 0, 0, 0, 0},
                 {1, 2, 3, 4, 0, 0, 0, 0},
                 {1, 2, 3, 4, 5, 0, 0, 0},
                 {1, 2, 3, 4, 5, 6, 0, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0}}
};

TestPushData testTailHeadNzData =
{
    .str = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    .strLen = maxNumIter,
    .start = 6,
    .end = 6,
    .expectedNum = {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected = {{0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 0, 0, 0, 0, 0, 1, 2},
                 {3, 0, 0, 0, 0, 0, 1, 2},
                 {3, 4, 0, 0, 0, 0, 1, 2},
                 {3, 4, 5, 0, 0, 0, 1, 2},
                 {3, 4, 5, 6, 0, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2}}
};

void testPushFunc(TestPushData* data)
{
    CircBuf<Elem, circBufLen> circBuf;
    std::size_t num = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem val = 0;

    circBuf.head(data->end);
    circBuf.tail(data->start);
    for (i = 0; i < data->strLen; i++)
    {
        val = data->str[i];
        num = circBuf.push(val);
        ASSERT_EQ(num, data->expectedNum[i]);
        std::array<Elem, circBufLen>& p = circBuf.buf();
        for (j = 0; j < circBufLen; j++)
        {
            ASSERT_EQ(p[j], data->expected[i][j]);
        }
    }
}

struct TestReadData
{
    const Elem str[circBufLen];
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t len;
    std::size_t spaceToEnd;
    std::size_t numIter;
    std::size_t expectedNum[maxNumIter];
    const Elem expected[maxNumIter][circBufLen];
};

TestReadData testReadIntoSmallerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .len = 6,
    .spaceToEnd = circBufLen - 1,
    .numIter = 3,
    .expectedNum = {6, 1, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}
};

TestReadData testReadIntoLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .len = 12,
    .spaceToEnd = circBufLen - 1,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
};

TestReadData testTailGtHeadReadIntoSmallerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .len = 6,
    .spaceToEnd = circBufLen - 5,
    .numIter = 3,
    .expectedNum = {6, 1, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}
};

TestReadData testTailGtHeadReadIntoLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .len = 12,
    .spaceToEnd = circBufLen - 5,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
};

void testReadFunc(TestReadData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t num = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem buf[data->len];

    std::array<Elem, circBufLen>& p = cb.buf();
    j = data->start;
    for (i = 0; i < data->strLen; i++)
    {
        p[j] = data->str[i];
        if (++j >= cb.len())
        {
            j = 0;
        }
    }
    cb.head(data->end);
    cb.tail(data->start);
    for (i = 0; i < data->numIter; i++)
    {
        for (j = 0; j < data->len; j++)
        {
            buf[j] = 0;
        }
        num = cb.read(buf, data->len);
        ASSERT_EQ(num, data->expectedNum[i]);
        for (j = 0; j < data->len; j++)
        {
            ASSERT_EQ(buf[j], data->expected[i][j]);
        }
    }
}

struct TestWriteData
{
    const Elem str[circBufLen];
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t numIter;
    std::size_t expectedNum[maxNumIter];
    const Elem expected[maxNumIter][circBufLen];
};

TestWriteData testWriteFromSmallerBufferData =
{
    .str = {1, 2, 3},
    .strLen = 3,
    .start = 0,
    .end = 0,
    .numIter = 4,
    .expectedNum = {3, 3, 1, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 0, 0, 0, 0, 0},
                 {1, 2, 3, 1, 2, 3, 0, 0},
                 {1, 2, 3, 1, 2, 3, 1, 0},
                 {1, 2, 3, 1, 2, 3, 1, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}}
};

TestWriteData testWriteFromLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 0,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}}
};

TestWriteData testTailHeadNzWriteFromSmallerBufferData =
{
    .str = {1, 2, 3},
    .strLen = 3,
    .start = 6,
    .end = 6,
    .numIter = 4,
    .expectedNum = {3, 3, 1, 0, 0, 0, 0, 0},
    .expected = {{3, 0, 0, 0, 0, 0, 1, 2},
                 {3, 1, 2, 3, 0, 0, 1, 2},
                 {3, 1, 2, 3, 1, 0, 1, 2},
                 {3, 1, 2, 3, 1, 0, 1, 2},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}}
};

TestWriteData testTailHeadNzWriteFromLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 6,
    .end = 6,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0, 0, 0, 0, 0},
    .expected = {{3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}}
};

void testWriteFunc(TestWriteData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t num = 0;
    std::size_t i = 0;
    std::size_t j = 0;

    cb.head(data->end);
    cb.tail(data->start);
    for (i = 0; i < data->numIter; i++)
    {
        num = cb.write(data->str, data->strLen);
        ASSERT_EQ(num, data->expectedNum[i]);
        std::array<Elem, circBufLen>& p = cb.buf();
        for (j = 0; j < circBufLen; j++)
        {
            ASSERT_EQ(p[j], data->expected[i][j]);
        }
    }
}

struct TestPeekConsumeData
{
    const Elem str[circBufLen];
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t len;
    std::size_t spaceToEnd;
    std::size_t numIter;
    std::size_t expectedNum[maxNumIter];
    const Elem expected[maxNumIter][circBufLen];
};

TestPeekConsumeData testPeekConsumeIntoSmallerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .len = 6,
    .spaceToEnd = 7,
    .numIter = 3,
    .expectedNum = {6, 1, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}
};

TestPeekConsumeData testPeekConsumeIntoLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .len = 12,
    .spaceToEnd = 7,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
};

TestPeekConsumeData testTailGtHeadPeekConsumeIntoSmallerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .len = 6,
    .spaceToEnd = 3,
    .numIter = 3,
    .expectedNum = {6, 1, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}
};

TestPeekConsumeData testTailGtHeadPeekConsumeIntoLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .len = 12,
    .spaceToEnd = 3,
    .numIter = 2,
    .expectedNum = {7,  0, 0, 0, 0, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}
};

void testPeekConsumeFunc(TestPeekConsumeData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t i = 0;
    std::size_t j = 0;
    std::size_t k = 0;
    std::size_t num = 0;
    Elem buf[data->len];

    std::array<Elem, circBufLen>& p = cb.buf();
    j = data->start;
    for (i = 0; i < data->strLen; i++)
    {
        p[j] = data->str[i];
        if (++j >= cb.len())
        {
            j = 0;
        }
    }
    cb.head(data->end);
    cb.tail(data->start);
    for (i = 0; i < data->numIter; i++)
    {
        for (j = 0; j < 2; j++)
        {
            for (k = 0; k < data->len; k++)
            {
                buf[k] = 0;
            }
            num = cb.peek(buf, data->len);
            ASSERT_EQ(num, data->expectedNum[i]);
            for (k = 0; k < data->len; k++)
            {
                ASSERT_EQ(buf[k], data->expected[i][k]);
            }
        }
        num = cb.consume(data->len);
        ASSERT_EQ(num, data->expectedNum[i]);
    }
}

struct TestMultithreadedData
{
    std::size_t numIter;
    const Elem elem[maxNumIter];
    std::size_t num[maxNumIter];
};

TestMultithreadedData testMultithreadedData =
{
    .numIter = 2,
    .elem = {1, 2, 0, 0, 0, 0, 0, 0},
    .num = {1, 1, 0, 0, 0, 0, 0, 0}
};

void testMultithreadedFunc(TestMultithreadedData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t num = 0;
    std::size_t i = 0;

    for (i = 0; i < data->numIter; i++)
    {
        num = cb.push(data->elem[i]);
        ASSERT_EQ(num, data->num[i]);
    }
    std::thread t([&cb, data]()
                    {
                        std::size_t num = 0;
                        std::size_t i = 0;
                        Elem val = 0;

                        for (i = 0; i < data->numIter + 1; i++)
                        {
                            val = 0;
                            num = cb.pop(val);
                            ASSERT_EQ(num, data->num[i]);
                            ASSERT_EQ(val, data->elem[i]);
                        }
                    });
    t.join();
}

TEST(testCircBuf, copyConstructor) {testCopyConstructorFunc(&testCopyConstructorData);}
TEST(testCircBuf, moveConstructor) {testMoveConstructorFunc(&testMoveConstructorData);}
TEST(testCircBuf, copyAssignment) {testCopyAssignmentFunc(&testCopyAssignmentData);}
TEST(testCircBuf, moveAssignment) {testMoveAssignmentFunc(&testMoveAssignmentData);}
TEST(testCircBuf, space) {testSpaceFunc(&testSpaceData);}
TEST(testCircBuf, count) {testCountFunc(&testCountData);}
TEST(testCircBuf, pop) {testPopFunc(&testPopData);}
TEST(testCircBuf, tailGtHeadPop) {testPopFunc(&testTailGtHeadPopData);}
TEST(testCircBuf, push) {testPushFunc(&testPushData);}
TEST(testCircBuf, tailHeadNzPush) {testPushFunc(&testTailHeadNzData);}
TEST(testCircBuf, readIntoSmallerBuffer) {testReadFunc(&testReadIntoSmallerBufferData);}
TEST(testCircBuf, readIntoLargerBuffer) {testReadFunc(&testReadIntoLargerBufferData);}
TEST(testCircBuf, tailGtHeadReadIntoSmallerBuffer) {testReadFunc(&testTailGtHeadReadIntoSmallerBufferData);}
TEST(testCircBuf, tailGtHeadReadIntoLargerBuffer) {testReadFunc(&testTailGtHeadReadIntoLargerBufferData);}
TEST(testCircBuf, writeFromSmallerBuffer) {testWriteFunc(&testWriteFromSmallerBufferData);}
TEST(testCircBuf, writeFromLargerBuffer) {testWriteFunc(&testWriteFromLargerBufferData);}
TEST(testCircBuf, tailHeadNzWriteFromSmallerBuffer) {testWriteFunc(&testTailHeadNzWriteFromSmallerBufferData);}
TEST(testCircBuf, tailHeadNzWriteFromLargerBuffer) {testWriteFunc(&testTailHeadNzWriteFromLargerBufferData);}
TEST(testCircBuf, peekConsumeIntoSmallerBuffer) {testPeekConsumeFunc(&testPeekConsumeIntoSmallerBufferData);}
TEST(testCircBuf, peekConsumeIntoLargerBuffer) {testPeekConsumeFunc(&testPeekConsumeIntoLargerBufferData);}
TEST(testCircBuf, tailGtHeadPeekConsumeIntoSmallerBuffer) {testPeekConsumeFunc(&testTailGtHeadPeekConsumeIntoSmallerBufferData);}
TEST(testCircBuf, tailGtHedPeekConsumeIntoLargerBuffer) {testPeekConsumeFunc(&testTailGtHeadPeekConsumeIntoLargerBufferData);}
TEST(testCircBuf, multithreaded) {testMultithreadedFunc(&testMultithreadedData);}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
