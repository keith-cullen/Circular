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

constexpr std::size_t circBufLen{8};
constexpr std::size_t maxNumIter{(circBufLen + circBufLen / 2)};

typedef int Elem;

struct TestCopyConstructorData
{
    std::size_t head;
    std::size_t tail;
};

TestCopyConstructorData testCopyConstructorData
{
    .head{2},
    .tail{6}
};

void testCopyConstructorFunc(TestCopyConstructorData* data)
{
    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        p1.at(i) = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2(cb1);

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2{cb2.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2.at(i), i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has remained the same
    std::array<Elem, circBufLen>& p3{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3.at(i), i);
    }
    ASSERT_EQ(cb1.head(), data->head);
    ASSERT_EQ(cb1.tail(), data->tail);
}

struct TestMoveConstructorData
{
    std::size_t head;
    std::size_t tail;
};

TestMoveConstructorData testMoveConstructorData
{
    .head{2},
    .tail{6}
};

void testMoveConstructorFunc(TestMoveConstructorData* data)
{
    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        p1.at(i) = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2(std::move(cb1));

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2{cb2.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2.at(i), i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has not remained the same
    std::array<Elem, circBufLen>& p3{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3.at(i), 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestCopyAssignmentData
{
    std::size_t head;
    std::size_t tail;
};

TestCopyAssignmentData testCopyAssignmentData
{
    .head{2},
    .tail{6}
};

void testCopyAssignmentFunc(TestCopyAssignmentData* data)
{
    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        p1.at(i) = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2;
    cb2 = cb1;

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2{cb2.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2.at(i), i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has remained the same
    std::array<Elem, circBufLen>& p3{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3.at(i), i);
    }
    ASSERT_EQ(cb1.head(), data->head);
    ASSERT_EQ(cb1.tail(), data->tail);
}

struct TestMoveAssignmentData
{
    std::size_t head;
    std::size_t tail;
};

TestMoveAssignmentData testMoveAssignmentData
{
    .head{2},
    .tail{6}
};

void testMoveAssignmentFunc(TestMoveAssignmentData* data)
{
    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        p1.at(i) = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2;
    cb2 = std::move(cb1);

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2{cb2.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p2.at(i), i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb2 has not remained the same
    std::array<Elem, circBufLen>& p3{cb1.buf()};
    for (Elem i{0}; i < Elem(circBufLen); i++)
    {
        ASSERT_EQ(p3.at(i), 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestSpaceData
{
    std::array<std::size_t, 2> numWrite;
    std::array<std::size_t, 2> numRead;
};

TestSpaceData testSpaceData
{
    .numWrite{6, 4},
    .numRead{6, 4}
};

void testSpaceFunc(TestSpaceData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t n{0};

    for (std::size_t j{0}; j < data->numWrite.size(); j++)
    {
        n = cb.space();
        for (std::size_t i{0}; i < data->numWrite.at(j); i++)
        {
            Elem val{Elem(i)};
            cb.write(&val, 1);
            ASSERT_EQ(cb.space(), n - i - 1);
        }
        n = cb.space();
        for (std::size_t i{0}; i < data->numRead.at(j); i++)
        {
            Elem val{0};
            cb.read(&val, 1);
            ASSERT_EQ(cb.space(), n + i + 1);
        }
    }
}

struct TestCountData
{
    std::array<std::size_t, 2> numWrite;
    std::array<std::size_t, 2> numRead;
};

TestCountData testCountData
{
    .numWrite{6, 4},
    .numRead{6, 4}
};

void testCountFunc(TestCountData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t n{0};

    for (std::size_t j{0}; j < data->numWrite.size(); j++)
    {
        n = cb.count();
        for (std::size_t i{0}; i < data->numWrite.at(j); i++)
        {
            Elem val{Elem(i)};
            cb.write(&val, 1);
            ASSERT_EQ(cb.count(), n + i + 1);
        }
        n = cb.count();
        for (std::size_t i{0}; i < data->numRead.at(j); i++)
        {
            Elem val{0};
            cb.read(&val, 1);
            ASSERT_EQ(cb.count(), n - i - 1);
        }
    }
}

struct TestPopData
{
    std::array<const Elem, circBufLen> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<const Elem, maxNumIter> expected;
};

TestPopData testPopData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .numIter{maxNumIter},
    .expectedNum{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected{1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0}
};

TestPopData testTailGtHeadPopData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .numIter{maxNumIter},
    .expectedNum{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected{1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0}
};

void testPopFunc(TestPopData* data)
{
    CircBuf<Elem, circBufLen> circBuf;

    std::array<Elem, circBufLen>& p{circBuf.buf()};
    std::size_t n{data->start};
    for (std::size_t i{0}; i < data->strLen; i++)
    {
        p.at(n) = data->str.at(i);
        if (++n >= circBuf.len())
        {
            n = 0;
        }
    }
    circBuf.head(data->end);
    circBuf.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        Elem val{0};
        std::size_t num{circBuf.pop(val)};
        ASSERT_EQ(num, data->expectedNum.at(i));
        ASSERT_EQ(val, data->expected.at(i));
    }
}

struct TestPushData
{
    std::array<const Elem, maxNumIter> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<std::array<const Elem, circBufLen>, maxNumIter> expected;
};

TestPushData testPushData
{
    .str{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    .strLen{maxNumIter},
    .start{0},
    .end{0},
    .numIter{maxNumIter},
    .expectedNum{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected{{{1, 0, 0, 0, 0, 0, 0, 0},
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
               {1, 2, 3, 4, 5, 6, 7, 0}}}
};

TestPushData testTailHeadNzData
{
    .str{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    .strLen{maxNumIter},
    .start{6},
    .end{6},
    .numIter{maxNumIter},
    .expectedNum{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    .expected{{{0, 0, 0, 0, 0, 0, 1, 0},
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
               {3, 4, 5, 6, 7, 0, 1, 2}}}
};

void testPushFunc(TestPushData* data)
{
    CircBuf<Elem, circBufLen> circBuf;

    circBuf.head(data->end);
    circBuf.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        Elem val{data->str.at(i)};
        std::size_t num{circBuf.push(val)};
        ASSERT_EQ(num, data->expectedNum.at(i));
        std::array<Elem, circBufLen>& p{circBuf.buf()};
        for (std::size_t j{0}; j < circBufLen; j++)
        {
            ASSERT_EQ(p.at(j), data->expected.at(i).at(j));
        }
    }
}

struct TestReadData
{
    std::array<const Elem, circBufLen> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t len;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<std::array<const Elem, 2 * circBufLen>, maxNumIter> expected;
};

TestReadData testReadIntoSmallerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .len{6},
    .numIter{3},
    .expectedNum{6, 1, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

TestReadData testReadIntoLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .len{12},
    .numIter{2},
    .expectedNum{7, 0, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

TestReadData testTailGtHeadReadIntoSmallerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .len{6},
    .numIter{3},
    .expectedNum{6, 1, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

TestReadData testTailGtHeadReadIntoLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .len{12},
    .numIter{2},
    .expectedNum{7, 0, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

void testReadFunc(TestReadData* data)
{
    CircBuf<Elem, circBufLen> cb;

    std::array<Elem, circBufLen>& p{cb.buf()};
    std::size_t n{data->start};
    for (std::size_t i{0}; i < data->strLen; i++)
    {
        p.at(n) = data->str.at(i);
        if (++n >= cb.len())
        {
            n = 0;
        }
    }
    cb.head(data->end);
    cb.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        Elem buf[data->len];
        for (std::size_t j{0}; j < data->len; j++)
        {
            buf[j] = 0;
        }
        std::size_t num{cb.read(buf, data->len)};
        ASSERT_EQ(num, data->expectedNum.at(i));
        for (std::size_t j{0}; j < data->len; j++)
        {
            ASSERT_EQ(buf[j], data->expected.at(i).at(j));
        }
    }
}

struct TestWriteData
{
    std::array<const Elem, circBufLen> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<std::array<const Elem, circBufLen>, maxNumIter> expected;
};

TestWriteData testWriteFromSmallerBufferData
{
    .str{1, 2, 3},
    .strLen{3},
    .start{0},
    .end{0},
    .numIter{4},
    .expectedNum{3, 3, 1, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 0, 0, 0, 0, 0},
               {1, 2, 3, 1, 2, 3, 0, 0},
               {1, 2, 3, 1, 2, 3, 1, 0},
               {1, 2, 3, 1, 2, 3, 1, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0}}}
};

TestWriteData testWriteFromLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{0},
    .numIter{2},
    .expectedNum{7, 0, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7, 0},
               {1, 2, 3, 4, 5, 6, 7, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0}}}
};

TestWriteData testTailHeadNzWriteFromSmallerBufferData
{
    .str{1, 2, 3},
    .strLen{3},
    .start{6},
    .end{6},
    .numIter{4},
    .expectedNum{3, 3, 1, 0, 0, 0, 0, 0},
    .expected{{{3, 0, 0, 0, 0, 0, 1, 2},
               {3, 1, 2, 3, 0, 0, 1, 2},
               {3, 1, 2, 3, 1, 0, 1, 2},
               {3, 1, 2, 3, 1, 0, 1, 2},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0}}}
};

TestWriteData testTailHeadNzWriteFromLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{6},
    .end{6},
    .numIter{2},
    .expectedNum{7, 0, 0, 0, 0, 0, 0, 0},
    .expected{{{3, 4, 5, 6, 7, 0, 1, 2},
               {3, 4, 5, 6, 7, 0, 1, 2},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0}}}
};

void testWriteFunc(TestWriteData* data)
{
    CircBuf<Elem, circBufLen> cb;

    cb.head(data->end);
    cb.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        std::size_t num{cb.write(data->str.data(), data->strLen)};
        ASSERT_EQ(num, data->expectedNum.at(i));
        std::array<Elem, circBufLen>& p{cb.buf()};
        for (std::size_t j{0}; j < circBufLen; j++)
        {
            ASSERT_EQ(p.at(j), data->expected.at(i).at(j));
        }
    }
}

struct TestPeekConsumeData
{
    std::array<const Elem, circBufLen> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t len;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<std::array<const Elem, 2 * circBufLen>, maxNumIter> expected;
};

TestPeekConsumeData testPeekConsumeIntoSmallerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .len{6},
    .numIter{3},
    .expectedNum{6, 1, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

TestPeekConsumeData testPeekConsumeIntoLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .len{12},
    .numIter{2},
    .expectedNum{7, 0, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

TestPeekConsumeData testTailGtHeadPeekConsumeIntoSmallerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .len{6},
    .numIter{3},
    .expectedNum{6, 1, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

TestPeekConsumeData testTailGtHeadPeekConsumeIntoLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .len{12},
    .numIter{2},
    .expectedNum{7,  0, 0, 0, 0, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}, {0}, {0}, {0}, {0}}}
};

void testPeekConsumeFunc(TestPeekConsumeData* data)
{
    CircBuf<Elem, circBufLen> cb;

    std::array<Elem, circBufLen>& p{cb.buf()};
    std::size_t n{data->start};
    for (std::size_t i{0}; i < data->strLen; i++)
    {
        p.at(n) = data->str.at(i);
        if (++n >= cb.len())
        {
            n = 0;
        }
    }
    cb.head(data->end);
    cb.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        for (std::size_t j{0}; j < 2; j++)
        {
            Elem buf[data->len];
            for (std::size_t k{0}; k < data->len; k++)
            {
                buf[k] = 0;
            }
            std::size_t num{cb.peek(buf, data->len)};
            ASSERT_EQ(num, data->expectedNum.at(i));
            for (std::size_t k{0}; k < data->len; k++)
            {
                ASSERT_EQ(buf[k], data->expected.at(i).at(k));
            }
        }
        std::size_t num{cb.consume(data->len)};
        ASSERT_EQ(num, data->expectedNum.at(i));
    }
}

struct TestMultithreadedData
{
    std::size_t numIter;
    std::array<const Elem, maxNumIter> elem;
    std::array<std::size_t, maxNumIter> num;
};

TestMultithreadedData testMultithreadedData
{
    .numIter{2},
    .elem{1, 2, 0, 0, 0, 0, 0, 0},
    .num{1, 1, 0, 0, 0, 0, 0, 0}
};

void testMultithreadedFunc(TestMultithreadedData* data)
{
    CircBuf<Elem, circBufLen> cb;

    for (std::size_t i{0}; i < data->numIter; i++)
    {
        std::size_t num{cb.push(data->elem.at(i))};
        ASSERT_EQ(num, data->num.at(i));
    }
    std::thread t([&cb, data]()
                    {
                        for (std::size_t i{0}; i < data->numIter + 1; i++)
                        {
                            Elem val{0};
                            std::size_t num{cb.pop(val)};
                            ASSERT_EQ(num, data->num.at(i));
                            ASSERT_EQ(val, data->elem.at(i));
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
