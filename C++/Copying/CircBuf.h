// +--------------------------+
// |                          |
// |    Copyright (c) 2010    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

#ifndef CIRC_BUF_H
#define CIRC_BUF_H

#include <iostream>
#include <array>

namespace Circular
{
namespace Copying
{

template<typename T, std::size_t N>
class CircBuf;

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream&, CircBuf<T, N>&);

template<typename T, std::size_t N>
class CircBuf
{
    static constexpr bool power_of_2(std::size_t i) {return (i > 0) && ((i & (i - 1)) == 0);}
    static_assert(power_of_2(N), "N must be an integer power of 2");
    friend std::ostream& operator<< <T, N>(std::ostream&, CircBuf&);
public:
    CircBuf() = default;
    CircBuf(const CircBuf&);
    CircBuf(CircBuf&&);
    virtual ~CircBuf() = default;
    CircBuf& operator=(const CircBuf&);
    CircBuf& operator=(CircBuf&&);
    std::size_t head() const;
    void head(std::size_t);
    std::size_t tail() const;
    void tail(std::size_t);
    std::size_t len() const;
    std::array<T, N>& buf();
    std::size_t countToEnd(std::size_t) const;
    std::size_t countToEnd() const;
    std::size_t spaceToEnd() const;
    std::size_t count() const;
    std::size_t space() const;
    std::size_t pop(T&);
    std::size_t push(const T&);
    std::size_t read(T*, std::size_t);
    std::size_t write(const T*, std::size_t);
    std::size_t peek(T*, std::size_t);
    std::size_t consume(std::size_t);
protected:
    std::size_t _head{0};
    std::size_t _tail{0};
    std::array<T, N> _buf{};
};

#include "CircBuf.hpp"

}  // namespace Copying
}  // namespace Circular

#endif
