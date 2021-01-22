// +--------------------------+
// |                          |
// |    Copyright (c) 2019    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

//  A queue implemented using a contiguous buffer with separate indices for reading and writing.
//
//  Elements are moved in to and out from the buffer.
//
//  The head index is the next location to be written. It is incremented when items
//  are added and wraps to zero when it reaches the end of the linear buffer.
//
//  The tail index is the next location to be read. It is incremented when items
//  are removed and wraps to zero when it reaches the end of the linear buffer.
//
//  When the head index is equal to the tail index, the circular buffer is empty.
//  When the head index is one less than the tail index, the circular buffer is full.

#include <utility>

template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& ostr, CircBuf<T, N>& cb)
{
    std::size_t i{cb._tail};

    ostr << "{";
    while (i != cb._head)
    {
        ostr << ' ' <<  cb._buf[i];
        if (++i >= cb.len())
        {
            i = 0;
        }
    }
    ostr << " }";
    return ostr;
}

template<typename T, std::size_t N>
CircBuf<T, N>::CircBuf(CircBuf&& cb) : _head(0), _tail(0), _buf()
{
    std::swap(_head, cb._head);
    std::swap(_tail, cb._tail);
    std::swap(_buf, cb._buf);
}

template<typename T, std::size_t N>
CircBuf<T, N>& CircBuf<T, N>::operator=(CircBuf&& cb)
{
    std::swap(_head, cb._head);
    std::swap(_tail, cb._tail);
    std::swap(_buf, cb._buf);
    return *this;
}

template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::head() const
{
    return _head;
}

template<typename T, std::size_t N>
void CircBuf<T, N>::head(std::size_t i)
{
    _head = i;
}

template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::tail() const
{
    return _tail;
}

template<typename T, std::size_t N>
void CircBuf<T, N>::tail(std::size_t i)
{
    _tail = i;
}

template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::len() const
{
    return N;
}

template<typename T, std::size_t N>
std::array<T, N>& CircBuf<T, N>::buf()
{
    return _buf;
}

// number of bytes present up to the end of the linear buffer or
// the end of the circular buffer, whichever is smaller
// this special version (with an argument) is required by CircBuf::peek()
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::countToEnd() const
{
    std::size_t countEndLinearBuf{N - _tail};
    std::size_t countEndCircBuf{(_head + countEndLinearBuf) & (N - 1)};
    return countEndCircBuf < countEndLinearBuf ? countEndCircBuf : countEndLinearBuf;
}

// space available to the end of the linear buffer or
// the end of the circular buffer, whichever is smaller
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::spaceToEnd() const
{
    std::size_t spaceEndLinearBuf{N - _head};
    std::size_t spaceEndCircBuf{(_tail + spaceEndLinearBuf - 1) & (N - 1)};
    return spaceEndLinearBuf < spaceEndCircBuf ? spaceEndLinearBuf : spaceEndCircBuf;
}

// total number of items present in the circular buffer
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::count() const
{
    return (_head - _tail) & (N - 1);
}

// total space available for items in the circular buffer
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::space() const
{
    return (_tail - _head - 1) & (N - 1);
}

// returns number of items popped
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::pop(T&& val)
{
    if (count() == 0)
    {
        return 0;
    }
    val = std::move(_buf[_tail]);
    _tail = (_tail + 1) & (N - 1);
    return 1;
}

// returns number of items pushed
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::push(T&& val)
{
    if (space() == 0)
    {
        return 0;
    }
    _buf[_head] = std::move(val);
    _head = (_head + 1) & (N - 1);
    return 1;
}

// returns number of items read
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::read(T* buf, std::size_t len)
{
    std::size_t ret{0};

    while (1)
    {
        std::size_t num{countToEnd()};
        if (len < num)
        {
            num = len;
        }
        if (num <= 0)
        {
            break;
        }
        for (std::size_t i = 0; i < num; i++)
        {
            *buf++ = std::move(_buf[i + _tail]);
        }
        _tail = (_tail + num) & (N - 1);
        len -= num;
        ret += num;
    }
    return ret;
}

// returns number of items written
template<typename T, std::size_t N>
std::size_t CircBuf<T, N>::write(T* buf, std::size_t len)
{
    std::size_t ret{0};

    while (1)
    {
        std::size_t num{spaceToEnd()};
        if (len < num)
        {
            num = len;
        }
        if (num <= 0)
        {
            break;
        }
        for (std::size_t i = 0; i < num; i++)
        {
            _buf[i + _head] = std::move(*buf++);
        }
        _head = (_head + num) & (N - 1);
        len -= num;
        ret += num;
    }
    return ret;
}
