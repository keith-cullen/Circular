// +--------------------------+
// |                          |
// |    Copyright (c) 2019    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

#include "CircBuf.h"
#include <gtest/gtest.h>
#include <array>
#include <utility>

using namespace Circular::Moving;

const std::size_t circBufLen = 8;
const std::size_t maxNumIter = (circBufLen + circBufLen / 2);

struct Elem
{
    friend std::ostream& operator<<(std::ostream& ostr, Elem e) {ostr << e.i; return ostr;}
    Elem() = default;
    Elem(const Elem& e) : i{e.i} {}
    Elem(Elem&& e) {std::swap(i, e.i);}
    Elem(std::size_t sz) : i{sz} {}
    Elem& operator=(const Elem& e) {i = e.i; return *this;}
    Elem& operator=(Elem&& e) {std::swap(i, e.i); return *this;}
    std::size_t i{0};
};

struct TestConstructorData
{
    std::size_t head;
    std::size_t tail;
};

TestConstructorData testConstructorData =
{
    .head = 2,
    .tail = 6
};

void testConstructorFunc(TestConstructorData* data)
{
    std::size_t i = 0;

    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1 = cb1.buf();
    for (i = 0; i < circBufLen; i++)
    {
        p1[i].i = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2(std::move(cb1));

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2 = cb2.buf();
    for (i = 0; i < circBufLen; i++)
    {
        ASSERT_EQ(p2[i].i, i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has remained the same
    std::array<Elem, circBufLen>& p3 = cb1.buf();
    for (i = 0; i < circBufLen; i++)
    {
        ASSERT_EQ(p3[i].i, 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestAssignmentData
{
    std::size_t head;
    std::size_t tail;
};

TestAssignmentData testAssignmentData =
{
    .head = 2,
    .tail = 6
};

void testAssignmentFunc(TestAssignmentData* data)
{
    std::size_t i = 0;

    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1 = cb1.buf();
    for (i = 0; i < circBufLen; i++)
    {
        p1[i].i = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2;
    cb2 = std::move(cb1);

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2 = cb2.buf();
    for (i = 0; i < circBufLen; i++)
    {
        ASSERT_EQ(p2[i].i, i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has not remained the same
    std::array<Elem, circBufLen>& p3 = cb1.buf();
    for (i = 0; i < circBufLen; i++)
    {
        ASSERT_EQ(p3[i].i, 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestCountSpaceData
{
    std::size_t numPush[2];
    std::size_t numPop[2];
};

TestCountSpaceData testSpaceData =
{
    .numPush = {6, 4},
    .numPop = {6, 4}
};

void testSpaceFunc(TestCountSpaceData* data)
{
    CircBuf<Elem, circBufLen> circBuf;
    std::size_t space = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem val = 0;

    for (i = 0; i < data->numPush[0]; i++)
    {
        val.i = i;
        circBuf.push(std::move(val));
        space = circBuf.space();
        ASSERT_EQ(space, circBufLen - i - 2);
    }
    j = space;
    for (i = 0; i < data->numPop[0]; i++)
    {
        circBuf.pop(std::move(val));
        space = circBuf.space();
        ASSERT_EQ(space, j + i + 1);
    }
    j = space;
    for (i = 0; i < data->numPush[1]; i++)
    {
        val.i = i;
        circBuf.push(std::move(val));
        space = circBuf.space();
        ASSERT_EQ(space, j - i - 1);
    }
    j = space;
    for (i = 0; i < data->numPop[1]; i++)
    {
        circBuf.pop(std::move(val));
        space = circBuf.space();
        ASSERT_EQ(space, j + i + 1);
    }
}

TestCountSpaceData testCountData =
{
    .numPush = {6, 4},
    .numPop = {6, 4}
};

void testCountFunc(TestCountSpaceData* data)
{
    CircBuf<Elem, circBufLen> circBuf;
    std::size_t count = 0;
    std::size_t i = 0;
    std::size_t j = 0;
    Elem val = 0;

    for (i = 0; i < data->numPush[0]; i++)
    {
        val.i = i;
        circBuf.push(std::move(val));
        count = circBuf.count();
        ASSERT_EQ(count, i + 1);
    }
    j = count;
    for (i = 0; i < data->numPop[0]; i++)
    {
        circBuf.pop(std::move(val));
        count = circBuf.count();
        ASSERT_EQ(count, j - i - 1);
    }
    j = count;
    for (i = 0; i < data->numPush[1]; i++)
    {
        val.i = i;
        circBuf.push(std::move(val));
        count = circBuf.count();
        ASSERT_EQ(count, j + i + 1);
    }
    j = count;
    for (i = 0; i < data->numPop[1]; i++)
    {
        circBuf.pop(std::move(val));
        count = circBuf.count();
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
    Elem* prevVal = NULL;
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
        prevVal = &p[circBuf.tail()];
        val.i = 0;
        num = circBuf.pop(std::move(val));
        ASSERT_EQ(num, data->expectedNum[i]);
        ASSERT_EQ(val.i, data->expected[i].i);
        if (num == 1)
        {
            ASSERT_EQ(prevVal->i, 0);
        }
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
    Elem prevVal = 0;
    Elem val = 0;

    circBuf.head(data->end);
    circBuf.tail(data->start);
    for (i = 0; i < data->strLen; i++)
    {
        val = data->str[i];
        prevVal = val;
        num = circBuf.push(std::move(val));
        ASSERT_EQ(num, data->expectedNum[i]);
        std::array<Elem, circBufLen>& p = circBuf.buf();
        for (j = 0; j < circBufLen; j++)
        {
            ASSERT_EQ(p[j].i, data->expected[i][j].i);
        }
        if (num == 1)
        {
            ASSERT_EQ(val.i, 0);
        }
        else
        {
            ASSERT_EQ(val.i, prevVal.i);
        }
    }
}

struct testReadData
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

testReadData testReadIntoSmallerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .len = 6,
    .spaceToEnd = circBufLen - 1,
    .numIter = 3,
    .expectedNum = {6, 1, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}}
};

testReadData testReadIntoLargerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 0,
    .end = 7,
    .len = 12,
    .spaceToEnd = circBufLen - 1,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}}
};

testReadData testTailGtHeadReadIntoSmallerBufferData =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .len = 6,
    .spaceToEnd = circBufLen - 5,
    .numIter = 3,
    .expectedNum = {6, 1, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}}
};

testReadData testTailGtHeadReadIntoLargerBuffer =
{
    .str = {1, 2, 3, 4, 5, 6, 7},
    .strLen = 7,
    .start = 5,
    .end = 4,
    .len = 12,
    .spaceToEnd = circBufLen - 5,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}}
};

void testReadFunc(testReadData *data)
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
            ASSERT_EQ(buf[j].i, data->expected[i][j].i);
        }
    }
    // confirm that data has been moved out of the circular buffer
    for (i = 0; i < cb.len(); i++)
    {
        ASSERT_EQ(p[i].i, 0);
    }
}

struct testWriteData
{
    Elem str[maxNumIter][circBufLen];
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t numIter;
    std::size_t expectedNum[maxNumIter];
    const Elem expected[maxNumIter][circBufLen];
    Elem expectedStr[maxNumIter][circBufLen];
};

testWriteData testWriteFromSmallerBufferData =
{
    .str = {{1, 2, 3},
            {1, 2, 3},
            {1, 2, 3},
            {1, 2, 3}},
    .strLen = 3,
    .start = 0,
    .end = 0,
    .numIter = 4,
    .expectedNum = {3, 3, 1, 0},
    .expected = {{1, 2, 3, 0, 0, 0, 0, 0},
                 {1, 2, 3, 1, 2, 3, 0, 0},
                 {1, 2, 3, 1, 2, 3, 1, 0},
                 {1, 2, 3, 1, 2, 3, 1, 0}},
    .expectedStr = {{0, 0, 0},
                    {0, 0, 0},
                    {0, 2, 3},
                    {1, 2, 3}}
};

testWriteData testWriteFromLargerBufferData =
{
    .str = {{1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7}},
    .strLen = 7,
    .start = 0,
    .end = 0,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0},
    .expected = {{1, 2, 3, 4, 5, 6, 7, 0},
                 {1, 2, 3, 4, 5, 6, 7, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}},
    .expectedStr = {{0, 0, 0, 0, 0, 0, 0},
                    {1, 2, 3, 4, 5, 6, 7},
                    {1, 2, 3, 4, 5, 6, 7},
                    {1, 2, 3, 4, 5, 6, 7}}
};

testWriteData testTailHeadNzWriteFromSmallerBufferData =
{
    .str = {{1, 2, 3},
            {1, 2, 3},
            {1, 2, 3},
            {1, 2, 3}},
    .strLen = 3,
    .start = 6,
    .end = 6,
    .numIter = 4,
    .expectedNum = {3, 3, 1, 0},
    .expected = {{3, 0, 0, 0, 0, 0, 1, 2},
                 {3, 1, 2, 3, 0, 0, 1, 2},
                 {3, 1, 2, 3, 1, 0, 1, 2},
                 {3, 1, 2, 3, 1, 0, 1, 2}},
    .expectedStr = {{0, 0, 0},
                    {0, 0, 0},
                    {0, 2, 3},
                    {1, 2, 3}}
};

testWriteData testTailHeadNzWriteFromLargerBufferData =
{
    .str = {{1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7},
            {1, 2, 3, 4, 5, 6, 7}},
    .strLen = 7,
    .start = 6,
    .end = 6,
    .numIter = 2,
    .expectedNum = {7, 0, 0, 0},
    .expected = {{3, 4, 5, 6, 7, 0, 1, 2},
                 {3, 4, 5, 6, 7, 0, 1, 2},
                 {0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0}},
    .expectedStr = {{0, 0, 0, 0, 0, 0, 0},
                    {1, 2, 3, 4, 5, 6, 7},
                    {1, 2, 3, 4, 5, 6, 7},
                    {1, 2, 3, 4, 5, 6, 7}}
};

void testWriteFunc(testWriteData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t num = 0;
    std::size_t i = 0;
    std::size_t j = 0;

    cb.head(data->end);
    cb.tail(data->start);
    for (i = 0; i < data->numIter; i++)
    {
        num = cb.write(data->str[i], data->strLen);
        ASSERT_EQ(num, data->expectedNum[i]);
        std::array<Elem, circBufLen>& p = cb.buf();
        for (j = 0; j < circBufLen; j++)
        {
            ASSERT_EQ(p[j].i, data->expected[i][j].i);
        }
        // confirm that data has been moved out of the source buffer
        for (j = 0; j < data->strLen; j++)
        {
            ASSERT_EQ(data->str[i][j].i, data->expectedStr[i][j].i);
        }
    }
}

TEST(testCircBuf, constructor) {testConstructorFunc(&testConstructorData);}
TEST(testCircBuf, assignment) {testAssignmentFunc(&testAssignmentData);}
TEST(testCircBuf, space) {testSpaceFunc(&testSpaceData);}
TEST(testCircBuf, count) {testCountFunc(&testCountData);}
TEST(testCircBuf, pop) {testPopFunc(&testPopData);}
TEST(testCircBuf, tailGtHeadPop) {testPopFunc(&testTailGtHeadPopData);}
TEST(testCircBuf, push) {testPushFunc(&testPushData);}
TEST(testCircBuf, tailHeadNzPush) {testPushFunc(&testTailHeadNzData);}
TEST(testCircBuf, readIntoSmallerBuffer) {testReadFunc(&testReadIntoSmallerBufferData);}
TEST(testCircBuf, readIntoLargerBuffer) {testReadFunc(&testReadIntoLargerBufferData);}
TEST(testCircBuf, tailGtHeadReadIntoSmallerBuffer) {testReadFunc(&testTailGtHeadReadIntoSmallerBufferData);}
TEST(testCircBuf, tailGtHeadReadIntoLargerBuffer) {testReadFunc(&testTailGtHeadReadIntoLargerBuffer);}
TEST(testCircBuf, writeFromSmallerBuffer) {testWriteFunc(&testWriteFromSmallerBufferData);}
TEST(testCircBuf, writeFromLargerBuffer) {testWriteFunc(&testWriteFromLargerBufferData);}
TEST(testCircBuf, tailHeadNzWriteFromSmallerBuffer) {testWriteFunc(&testTailHeadNzWriteFromSmallerBufferData);}
TEST(testCircBuf, tailHeadNzWriteFromLargerBuffer) {testWriteFunc(&testTailHeadNzWriteFromLargerBufferData);}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
