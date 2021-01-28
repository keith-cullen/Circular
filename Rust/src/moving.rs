// +--------------------------+
// |                          |
// |    Copyright (c) 2020    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

/// A queue implemented using a contiguous buffer with separate indices for reading and writing.
///
/// Elements are moved in to and out of the buffer.
///
/// The head index is the next location to be written. It is incremented when items
/// are added and wraps to zero when it reaches the end of the linear buffer.
///
/// The tail index is the next location to be read. It is incremented when items
/// are removed and wraps to zero when it reaches the end of the linear buffer.
///
/// When the head index is equal to the tail index, the circular buffer is empty.
/// When the head index is one less than the tail index, the circular buffer is full.
pub struct CircBuf<T> {
    /// The next location in the linear buffer to be written
    head: usize,
    /// The next location in the linear buffer to be read
    tail: usize,
    /// The linear buffer
    buf: Vec<Option<T>>,
}

impl<T: Default> CircBuf<T> {
    /// Check if the size of the linear buffer is an integer power of 2
    fn power_of_2(len: usize) {
        if len < 2 {
            panic!("len must be greater than or equal to 2");
        }
        let mut i = 1;
        while i > 0 {
            if i == len {
                return;
            }
            i <<= 1;
        }
        panic!("len must be an integer power of 2")
    }

    /// Calculate the space until the end of the linear buffer
    fn space_to_end(&self) -> usize {
        let space_end_linear_buf = self.buf.len() - self.head;
        let space_end_circ_buf = (self.tail + space_end_linear_buf - 1) & (self.buf.len() - 1);
        if space_end_linear_buf < space_end_circ_buf {
            return space_end_linear_buf;
        }
        space_end_circ_buf
    }

    /// Calculate the count until the end of the linear buffer
    fn count_to_end(&self) -> usize {
        let count_end_linear_buf = self.buf.len() - self.tail;
        let count_end_circ_buf = (self.head + count_end_linear_buf) & (self.buf.len() - 1);
        if count_end_circ_buf < count_end_linear_buf {
            return count_end_circ_buf;
        }
        count_end_linear_buf
    }

    /// Calculate the space in the circular buffer
    pub fn space(&self) -> usize {
        let tmp: usize = ((self.tail as isize) - (self.head as isize) - 1) as usize;
        tmp & (self.buf.len() - 1)
    }

    /// Calculate the count of the circular buffer
    pub fn count(&self) -> usize {
        let tmp: usize = ((self.head as isize) - (self.tail as isize)) as usize;
        tmp & (self.buf.len() - 1)
    }

    /// Initialise a new circular buffer
    pub fn new(l: usize) -> CircBuf<T> {
        CircBuf::<T>::power_of_2(l);
        let mut vec: Vec<Option<T>> = Vec::new();
        for _ in 0..l {
            vec.push(None);
        }
        CircBuf { head: 0, tail: 0, buf: vec }
    }

    /// Get the head index
    pub fn head(&self) -> usize {
        self.head
    }

    /// Get the tail index
    pub fn tail(&self) -> usize {
        self.tail
    }

    /// Get the length of the linear buffer
    pub fn len(&self) -> usize {
        self.buf.len()
    }

    /// Get an element from the tail of the circular buffer
    pub fn pop(&mut self) -> Option<T> {
        if self.count() == 0 {
            return None;
        }
        let val = self.buf[self.tail].take();
        self.tail = (self.tail + 1) & (self.buf.len() - 1);
        return val;
    }

    /// Put an element at the head of the circular buffer
    pub fn push(&mut self, val: &mut Option<T>) -> bool {
        if self.space() == 0 {
            return false;
        }
        self.buf[self.head] = val.take();
        self.head = (self.head + 1) & (self.buf.len() - 1);
        return true;
    }

    /// Get contiguous elements from the tail of the circular buffer
    pub fn read(&mut self, data: &mut [Option<T>]) -> usize {
        let mut index: usize = 0;
        let mut len = data.len();
        let mut num: usize;
        let mut ret: usize = 0;
        loop {
            num = self.count_to_end();
            if len < num {
                num = len;
            }
            if num <= 0 {
                return ret;
            }
            for i in 0..num {
                data[i + index] = self.buf[i + self.tail].take();
            }
            self.tail = (self.tail + num) & (self.buf.len() - 1);
            index += num;
            len -= num;
            ret += num;
        }
    }

    /// Put contiguous elements at the head of the circular buffer
    pub fn write(&mut self, data: &mut [Option<T>]) -> usize {
        let mut index: usize = 0;
        let mut len = data.len();
        let mut num: usize;
        let mut ret: usize = 0;
        loop {
            num = self.space_to_end();
            if len < num {
                num = len;
            }
            if num <= 0 {
                return ret;
            }
            for i in 0..num {
                self.buf[i + self.head] = data[i + index].take();
            }
            self.head = (self.head + num) & (self.buf.len() - 1);
            index += num;
            len -= num;
            ret += num;
        }
    }
}
