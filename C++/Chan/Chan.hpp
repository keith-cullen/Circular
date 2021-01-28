// +--------------------------+
// |                          |
// |    Copyright (c) 2019    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

// A queue implemented using an underlying moving circular buffer and
// semaphores to block read operations when the queue is empty and write
// operations when the queue is full.

#include <utility>
#include <cerrno>

template<typename T, std::size_t N>
std::ostream &operator<<(std::ostream &ostr, Chan<T, N> &ch)
{
    return ostr << ch._circBuf;
}

template<typename T, std::size_t N>
Chan<T, N>::Chan() : _circBuf()
{
    int ret{0};

    ret = sem_init(&_rdSem, 0, 0);
    if (ret < 0)
    {
        throw errno;
    }
    ret = sem_init(&_wrSem, 0, N - 1);
    if (ret < 0)
    {
        sem_destroy(&_rdSem);
        throw errno;
    }
}

template<typename T, std::size_t N>
Chan<T, N>::~Chan()
{
    sem_destroy(&_wrSem);
    sem_destroy(&_rdSem);
}

// total number of items present in the channel
template<typename T, std::size_t N>
std::size_t Chan<T, N>::count() const
{
    return _circBuf.count();
}

// total space available for items in the channel
template<typename T, std::size_t N>
std::size_t Chan<T, N>::space() const
{
    return _circBuf.space();
}

// returns number of items popped
template<typename T, std::size_t N>
std::size_t Chan<T, N>::pop(T &&val)
{
    std::size_t num{0};
    int ret{0};

    for (;;)
    {
        ret = sem_wait(&_rdSem);
        if (ret == 0)
        {
            break;
        }
        if (errno != EINTR)
        {
            return 0;
        }
    }
    num = _circBuf.pop(std::forward<T>(val));
    ret = sem_post(&_wrSem);
    if (ret < 0)
    {
        return 0;
    }
    return num;
}

// returns number of items pushed
template<typename T, std::size_t N>
std::size_t Chan<T, N>::push(T &&val)
{
    std::size_t num{0};
    int ret{0};

    for (;;)
    {
        ret = sem_wait(&_wrSem);
        if (ret == 0)
        {
            break;
        }
        if (errno != EINTR)
        {
            return 0;
        }
    }
    num = _circBuf.push(std::forward<T>(val));
    ret = sem_post(&_rdSem);
    if (ret < 0)
    {
        return 0;
    }
    return num;
}
