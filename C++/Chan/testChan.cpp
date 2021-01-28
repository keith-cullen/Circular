// +--------------------------+
// |                          |
// |    Copyright (c) 2019    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

#include "Chan.h"
#include <gtest/gtest.h>
#include <thread>
#include <chrono>
#include <utility>

using namespace Circular;

constexpr std::size_t circBufLen{8};
constexpr std::size_t maxNumIter{128};
constexpr std::size_t sleepMsec{10};

struct Elem
{
    friend std::ostream &operator<<(std::ostream &ostr, Elem c) {ostr << c.i; return ostr;}
    Elem() = default;
    Elem(const Elem &c) : i{c.i} {}
    Elem(Elem &&c) {std::swap(i, c.i);}
    Elem(std::size_t sz) : i{sz} {}
    Elem &operator=(const Elem &c) {i = c.i; return *this;}
    Elem &operator=(Elem &&c) {std::swap(i, c.i); return *this;}
    std::size_t i{0};
};

TEST(testChan, multithreaded)
{
    Chan<Elem, circBufLen> chan;

    std::thread t1([&chan]()
                    {
                        // i cannot be zero for the move to be confirmed
                        for (std::size_t i{1}; i <= maxNumIter; i++)
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(sleepMsec));
                            Elem val{0};
                            std::size_t num{chan.pop(std::move(val))};
                            ASSERT_EQ(num, 1);
                            ASSERT_EQ(val.i, int(i));
                        }
                    });
    std::thread t2([&chan]()
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(sleepMsec));
                        // i cannot be zero for the move to be confirmed
                        for (std::size_t i{1}; i <= maxNumIter; i++)
                        {
                            std::this_thread::sleep_for(std::chrono::milliseconds(sleepMsec));
                            Elem val{i};
                            std::size_t num{chan.push(std::move(val))};
                            ASSERT_EQ(num, 1);
                            ASSERT_NE(val.i, int(i));
                        }
                    });
    t2.join();
    t1.join();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
