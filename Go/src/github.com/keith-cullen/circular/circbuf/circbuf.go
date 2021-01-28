// +--------------------------+
// |                          |
// |    Copyright (c) 2016    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

// Package circbuf provides primitives for implementing circular buffers.
package circbuf

import "fmt"

// A queue implemented using a contiguous buffer with separate indices for reading and writing.
//
// Bytes are copied in to and out from the buffer.
//
// The head index is the next location to be written. It is incremented when items
// are added and wraps to zero when it reaches the end of the linear buffer.
//
// The tail index is the next location to be read. It is incremented when items
// are removed and wraps to zero when it reaches the end of the linear buffer.
//
// When the head index is equal to the tail index, the circular buffer is empty.
// When the head index is one less than the tail index, the circular buffer is full.
type CircBuf struct {
	head int		// head is the index of the next location in buf to be written.
	tail int		// tail is the index of the next location in buf to be read.
	buf []byte		// buf is the linear buffer underlying the circular buffer.
}

// powerOf2 determines if its argument is an integer power of 2.
func powerOf2(x int) bool {
	var i int = 1

	for i > 0 {
		if i == x {
			return true
		}
		i <<= 1
	}
	return false
}

// New allocates and initialises a circular buffer and returns a pointer to it.
func New(len int) (*CircBuf, error) {
	if len <= 0 {
		return nil, fmt.Errorf("len argument, %v, is not positive", len)
	}
	if !powerOf2(len) {
		return nil, fmt.Errorf("len argument, %v, is not an integer power of 2", len)
	}
	c := new(CircBuf)
	c.buf = make([]byte, len)
	return c, nil
}

// spaceToEnd counts the number of unused bytes in a circular buffer
// to the end of the linear buffer or the end of the circular buffer
// whichever is smaller.
func (c *CircBuf) spaceToEnd() int {
	spaceEndLinearBuf := len(c.buf) - c.head
	spaceEndCircBuf := (c.tail + spaceEndLinearBuf - 1) & (len(c.buf) - 1)
	if spaceEndLinearBuf < spaceEndCircBuf {
		return spaceEndLinearBuf
	}
	return spaceEndCircBuf
}

// countToEndArg counts the number of used bytes in a circular buffer
// to the end of the liner buffer or the end of the circular buffer
// whichever is smaller. The tail parameter allows the Peek function
// to count used bytes without having to update the tail index in the
// circular buffer.
func (c *CircBuf) countToEndArg(tail int) int {
	countEndLinearBuf := len(c.buf) - tail
	countEndCircBuf := (c.head + countEndLinearBuf) & (len(c.buf) - 1)
	if countEndCircBuf < countEndLinearBuf {
		return countEndCircBuf
	}
	return countEndLinearBuf
}

// countToEnd counts the number of used bytes in a circular buffer
// to the end of the liner buffer or the end of the circular buffer
// whichever is smaller.
func (c *CircBuf) countToEnd() int {
	return c.countToEndArg(c.tail)
}

// Space returns the number of unused bytes in a circular buffer.
func (c *CircBuf) Space() int {
	return (c.tail - c.head - 1) & (len(c.buf) - 1)
}

// Count returns the number of used bytes in a circular buffer.
func (c *CircBuf) Count() int {
	return (c.head - c.tail) & (len(c.buf) - 1)
}

// Read reads data from a circular buffer.
// The number of bytes read is returned.
func (c *CircBuf) Read(buf []byte) int {
	var count int
	var num int
	for {
		count = c.countToEnd()
		if len(buf) - num < count {
			count = len(buf) - num
		}
		if count <= 0 {
			break
		}
		copy(buf[num : num + count], c.buf[c.tail : c.tail + count])
		c.tail = (c.tail + count) & (len(c.buf) - 1)
		num += count
	}
	return num
}

// Write writes data to a circular buffer.
// The number of bytes written is returned.
func (c *CircBuf) Write(buf []byte) int {
	var space int
	var num int
	for {
		space = c.spaceToEnd()
		if len(buf) - num < space {
			space = len(buf) - num
		}
		if space <= 0 {
			break
		}
		copy(c.buf[c.head : c.head + space], buf[num : num + space])
		c.head = (c.head + space) & (len(c.buf) - 1)
		num += space
	}
	return num
}

// Peek reads data from a circular buffer but does not update the tail index.
// Subsequent calls to Peek will produce the same results.
// The number of bytes read is returned.
func (c *CircBuf) Peek(buf []byte) int {
	var count int
	var tail int = c.tail  // Use a local tail variable
	var num int
	for {
		count = c.countToEndArg(tail)
		if len(buf) - num < count {
			count = len(buf) - num
		}
		if count <= 0 {
			break
		}
		copy(buf[num : num + count], c.buf[tail : tail + count])
		tail = (tail + count) & (len(c.buf) - 1)
		num += count
	}
	return num
}

// Consume advances the tail index to remove data from a circular buffer.
// A call to Consume usually follows a call to Peek.
// The number of bytes consumed is returned.
func (c *CircBuf) Consume(nbytes int) int {
	var count int
	var num int
	for {
		count = c.countToEnd()
		if nbytes - num < count {
			count = nbytes - num
		}
		if count <= 0 {
			break
		}
		c.tail = (c.tail + count) & (len(c.buf) - 1)
		num += count
	}
	return num
}

// String converts a circular buffer to a string representation.
func (c *CircBuf) String() string {
	return fmt.Sprintf("CircBuf{len: %v, head: %v, tail: %v, space: %v, count: %v, buf: %v}", len(c.buf), c.head, c.tail, c.Space(), c.Count(), c.buf)
}
