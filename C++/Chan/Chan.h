// +--------------------------+
// |                          |
// |    Copyright (c) 2019    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

#ifndef CHAN_H
#define CHAN_H

#include "CircBuf.h"
#include <semaphore.h>
#include <iostream>

namespace Circular
{

template<typename T, std::size_t N>
class Chan;

template<typename T, std::size_t N>
std::ostream &operator<<(std::ostream &, Chan<T, N> &);

template<typename T, std::size_t N>
class Chan
{
    friend std::ostream &operator<< <T, N>(std::ostream &, Chan &);
public:
    Chan();
    Chan(const Chan &) = delete;
    Chan(Chan &&) = delete;
    virtual ~Chan();
    Chan &operator=(const Chan &) = delete;
    Chan &operator=(Chan &&) = delete;
    std::size_t count() const;
    std::size_t space() const;
    std::size_t pop(T &&);
    std::size_t push(T &&);
protected:
    Circular::Moving::CircBuf<T, N> _circBuf;
    sem_t _rdSem;
    sem_t _wrSem;
};

#include "Chan.hpp"

}  // namespace Circular

#endif
