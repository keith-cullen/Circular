// +--------------------------+
// |                          |
// |    Copyright (c) 2019    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

#include "CircBuf.h"
#include <gtest/gtest.h>
#include <array>
#include <thread>
#include <utility>

using namespace Circular::Moving;

constexpr std::size_t circBufLen{8};
constexpr std::size_t maxNumIter{(circBufLen + circBufLen / 2)};

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

TestConstructorData testConstructorData
{
    .head{2},
    .tail{6}
};

void testConstructorFunc(TestConstructorData* data)
{
    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1{cb1.buf()};
    for (std::size_t i{0}; i < circBufLen; i++)
    {
        p1.at(i).i = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2(std::move(cb1));

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2{cb2.buf()};
    for (std::size_t i{0}; i < circBufLen; i++)
    {
        ASSERT_EQ(p2.at(i).i, i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has remained the same
    std::array<Elem, circBufLen>& p3{cb1.buf()};
    for (std::size_t i{0}; i < circBufLen; i++)
    {
        ASSERT_EQ(p3.at(i).i, 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestAssignmentData
{
    std::size_t head;
    std::size_t tail;
};

TestAssignmentData testAssignmentData
{
    .head{2},
    .tail{6}
};

void testAssignmentFunc(TestAssignmentData* data)
{
    // construct cb1
    CircBuf<Elem, circBufLen> cb1;
    std::array<Elem, circBufLen>& p1{cb1.buf()};
    for (std::size_t i{0}; i < circBufLen; i++)
    {
        p1.at(i).i = i;
    }
    cb1.head(data->head);
    cb1.tail(data->tail);

    // construct cb2
    CircBuf<Elem, circBufLen> cb2;
    cb2 = std::move(cb1);

    // check that cb2 is now a copy of cb1
    std::array<Elem, circBufLen>& p2{cb2.buf()};
    for (std::size_t i{0}; i < circBufLen; i++)
    {
        ASSERT_EQ(p2.at(i).i, i);
    }
    ASSERT_EQ(cb2.head(), data->head);
    ASSERT_EQ(cb2.tail(), data->tail);

    // check that cb1 has not remained the same
    std::array<Elem, circBufLen>& p3{cb1.buf()};
    for (std::size_t i{0}; i < circBufLen; i++)
    {
        ASSERT_EQ(p3.at(i).i, 0);
    }
    ASSERT_EQ(cb1.head(), 0);
    ASSERT_EQ(cb1.tail(), 0);
}

struct TestSpaceData
{
    std::array<std::size_t, 2> numPush;
    std::array<std::size_t, 2> numPop;
};

TestSpaceData testSpaceData
{
    .numPush{6, 4},
    .numPop{6, 4}
};

void testSpaceFunc(TestSpaceData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t n{0};

    for (std::size_t j{0}; j < 2; j++)
    {
        n = cb.space();
        for (std::size_t i{0}; i < data->numPush.at(0); i++)
        {
            Elem val{Elem(i)};
            cb.push(std::move(val));
            ASSERT_EQ(cb.space(), n - i - 1);
        }
        n = cb.space();
        for (std::size_t i{0}; i < data->numPop.at(0); i++)
        {
            Elem val{0};
            cb.pop(std::move(val));
            ASSERT_EQ(cb.space(), n + i + 1);
        }
    }
}

struct TestCountData
{
    std::array<std::size_t, 2> numPush;
    std::array<std::size_t, 2> numPop;
};

TestCountData testCountData
{
    .numPush{6, 4},
    .numPop{6, 4}
};

void testCountFunc(TestCountData* data)
{
    CircBuf<Elem, circBufLen> cb;
    std::size_t n{0};

    for (std::size_t j{0}; j < 2; j++)
    {
        n = cb.count();
        for (std::size_t i{0}; i < data->numPush.at(0); i++)
        {
            Elem val{Elem(i)};
            cb.push(std::move(val));
            ASSERT_EQ(cb.count(), n + i + 1);
        }
        n = cb.count();
        for (std::size_t i{0}; i < data->numPop.at(0); i++)
        {
            Elem val{0};
            cb.pop(std::move(val));
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
        Elem* prevVal{&p.at(cb.tail())};
        Elem val{0};
        std::size_t num{cb.pop(std::move(val))};
        ASSERT_EQ(num, data->expectedNum.at(i));
        ASSERT_EQ(val.i, data->expected.at(i).i);
        if (num == 1)
        {
            ASSERT_EQ(prevVal->i, 0);
        }
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
    .numIter = 12,
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
    .numIter = 12,
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
    CircBuf<Elem, circBufLen> cb;

    cb.head(data->end);
    cb.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        Elem val{data->str.at(i)};
        Elem prevVal{val};
        std::size_t num{cb.push(std::move(val))};
        ASSERT_EQ(num, data->expectedNum.at(i));
        std::array<Elem, circBufLen>& p{cb.buf()};
        for (std::size_t j{0}; j < circBufLen; j++)
        {
            ASSERT_EQ(p.at(j).i, data->expected.at(i).at(j).i);
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
    std::array<const Elem, circBufLen> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t len;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<std::array<const Elem, 2 * circBufLen>, maxNumIter> expected;
};

testReadData testReadIntoSmallerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .len{6},
    .numIter{3},
    .expectedNum{6, 1, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}}}
};

testReadData testReadIntoLargerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{0},
    .end{7},
    .len{12},
    .numIter{2},
    .expectedNum{7, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}}}
};

testReadData testTailGtHeadReadIntoSmallerBufferData
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .len{6},
    .numIter{3},
    .expectedNum{6, 1, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6}, {7}, {0}, {0}}}
};

testReadData testTailGtHeadReadIntoLargerBuffer
{
    .str{1, 2, 3, 4, 5, 6, 7},
    .strLen{7},
    .start{5},
    .end{4},
    .len{12},
    .numIter{2},
    .expectedNum{7, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7}, {0}, {0}, {0}}}
};

void testReadFunc(testReadData *data)
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
            ASSERT_EQ(buf[j].i, data->expected.at(i).at(j).i);
        }
    }
    // confirm that data has been moved out of the circular buffer
    for (std::size_t i{0}; i < cb.len(); i++)
    {
        ASSERT_EQ(p.at(i).i, 0);
    }
}

struct testWriteData
{
    std::array<std::array<Elem, circBufLen>, maxNumIter> str;
    std::size_t strLen;
    std::size_t start;
    std::size_t end;
    std::size_t numIter;
    std::array<std::size_t, maxNumIter> expectedNum;
    std::array<std::array<const Elem, circBufLen>, maxNumIter> expected;
    std::array<std::array<Elem, circBufLen>, maxNumIter> expectedStr;
};

testWriteData testWriteFromSmallerBufferData
{
    .str{{{1, 2, 3},
          {1, 2, 3},
          {1, 2, 3},
          {1, 2, 3}}},
    .strLen{3},
    .start{0},
    .end{0},
    .numIter{4},
    .expectedNum{3, 3, 1, 0},
    .expected{{{1, 2, 3, 0, 0, 0, 0, 0},
               {1, 2, 3, 1, 2, 3, 0, 0},
               {1, 2, 3, 1, 2, 3, 1, 0},
               {1, 2, 3, 1, 2, 3, 1, 0}}},
    .expectedStr{{{0, 0, 0},
                  {0, 0, 0},
                  {0, 2, 3},
                  {1, 2, 3}}}
};

testWriteData testWriteFromLargerBufferData
{
    .str{{{1, 2, 3, 4, 5, 6, 7},
          {1, 2, 3, 4, 5, 6, 7},
          {1, 2, 3, 4, 5, 6, 7},
          {1, 2, 3, 4, 5, 6, 7}}},
    .strLen{7},
    .start{0},
    .end{0},
    .numIter{2},
    .expectedNum{7, 0, 0, 0},
    .expected{{{1, 2, 3, 4, 5, 6, 7, 0},
               {1, 2, 3, 4, 5, 6, 7, 0},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0}}},
    .expectedStr{{{0, 0, 0, 0, 0, 0, 0},
                  {1, 2, 3, 4, 5, 6, 7},
                  {1, 2, 3, 4, 5, 6, 7},
                  {1, 2, 3, 4, 5, 6, 7}}}
};

testWriteData testTailHeadNzWriteFromSmallerBufferData
{
    .str{{{1, 2, 3},
          {1, 2, 3},
          {1, 2, 3},
          {1, 2, 3}}},
    .strLen{3},
    .start{6},
    .end{6},
    .numIter{4},
    .expectedNum{3, 3, 1, 0},
    .expected{{{3, 0, 0, 0, 0, 0, 1, 2},
               {3, 1, 2, 3, 0, 0, 1, 2},
               {3, 1, 2, 3, 1, 0, 1, 2},
               {3, 1, 2, 3, 1, 0, 1, 2}}},
    .expectedStr{{{0, 0, 0},
                  {0, 0, 0},
                  {0, 2, 3},
                  {1, 2, 3}}}
};

testWriteData testTailHeadNzWriteFromLargerBufferData
{
    .str{{{1, 2, 3, 4, 5, 6, 7},
          {1, 2, 3, 4, 5, 6, 7},
          {1, 2, 3, 4, 5, 6, 7},
          {1, 2, 3, 4, 5, 6, 7}}},
    .strLen{7},
    .start{6},
    .end{6},
    .numIter{2},
    .expectedNum{7, 0, 0, 0},
    .expected{{{3, 4, 5, 6, 7, 0, 1, 2},
               {3, 4, 5, 6, 7, 0, 1, 2},
               {0, 0, 0, 0, 0, 0, 0, 0},
               {0, 0, 0, 0, 0, 0, 0, 0}}},
    .expectedStr{{{0, 0, 0, 0, 0, 0, 0},
                  {1, 2, 3, 4, 5, 6, 7},
                  {1, 2, 3, 4, 5, 6, 7},
                  {1, 2, 3, 4, 5, 6, 7}}}
};

void testWriteFunc(testWriteData* data)
{
    CircBuf<Elem, circBufLen> cb;

    cb.head(data->end);
    cb.tail(data->start);
    for (std::size_t i{0}; i < data->numIter; i++)
    {
        std::size_t num{cb.write(data->str.at(i).data(), data->strLen)};
        ASSERT_EQ(num, data->expectedNum.at(i));
        std::array<Elem, circBufLen>& p{cb.buf()};
        for (std::size_t j{0}; j < circBufLen; j++)
        {
            ASSERT_EQ(p.at(j).i, data->expected.at(i).at(j).i);
        }
        // confirm that data has been moved out of the source buffer
        for (std::size_t j{0}; j < data->strLen; j++)
        {
            ASSERT_EQ(data->str.at(i).at(j).i, data->expectedStr.at(i).at(j).i);
        }
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
        Elem val{data->elem.at(i)};
        std::size_t num{cb.push(std::move(val))};
        ASSERT_EQ(num, data->num.at(i));
    }
    std::thread t([&cb, data]()
                    {
                        for (std::size_t i{0}; i < data->numIter + 1; i++)
                        {
                            Elem val{0};
                            std::size_t num{cb.pop(std::move(val))};
                            ASSERT_EQ(num, data->num.at(i));
                            ASSERT_EQ(val.i, data->elem.at(i).i);
                        }
                    });
    t.join();
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
TEST(testCircBuf, multithreaded) {testMultithreadedFunc(&testMultithreadedData);}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
