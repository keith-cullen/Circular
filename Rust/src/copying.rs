// +--------------------------+
// |                          |
// |    Copyright (c) 2020    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

///  A queue implemented using a contiguous buffer with separate indices for reading and writing.
///
///  Elements are copied in to and out from the buffer.
//
///  The head index is the next location to be written. It is incremented when items
///  are added and wraps to zero when it reaches the end of the linear buffer.
///
///  The tail index is the next location to be read. It is incremented when items
///  are removed and wraps to zero when it reaches the end of the linear buffer.
///
///  When the head index is equal to the tail index, the circular buffer is empty.
///  When the head index is one less than the tail index, the circular buffer is full.
pub struct CircBuf<T: Copy + Default> {
    /// The next location in the linear buffer to be written
    head: usize,
    /// The next location in the linear buffer to be read
    tail: usize,
    /// The linear buffer
    buf: Vec<T>,
}

impl<T: Copy + Default> CircBuf<T> {
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

    /// Calculate the count until the end of the linear buffer given a specific tail value
    fn count_to_end_(&self, tail: usize) -> usize {
        let count_end_linear_buf = self.buf.len() - tail;
        let count_end_circ_buf = (self.head + count_end_linear_buf) & (self.buf.len() - 1);
        if count_end_circ_buf < count_end_linear_buf {
            return count_end_circ_buf;
        }
        count_end_linear_buf
    }

    /// Calculate the count until the end of the linear buffer
    fn count_to_end(&self) -> usize {
        self.count_to_end_(self.tail)
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
        let mut vec: Vec<T> = Vec::new();
        for _ in 0..l {
            vec.push(Default::default());
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
        let val = self.buf[self.tail];
        self.tail = (self.tail + 1) & (self.buf.len() - 1);
        return Some(val);
    }

    /// Put an element at the head of the circular buffer
    pub fn push(&mut self, val: T) -> bool {
        if self.space() == 0 {
            return false;
        }
        self.buf[self.head] = val;
        self.head = (self.head + 1) & (self.buf.len() - 1);
        return true;
    }

    /// Get contiguous elements from the tail of the circular buffer without changing the buffer
    pub fn peek(&mut self, data: &mut [T]) -> usize {
        let mut index: usize = 0;
        let mut tail = self.tail;
        let mut len = data.len();
        let mut num: usize;
        let mut ret: usize = 0;
        loop {
            num = self.count_to_end_(tail);
            if len < num {
                num = len
            }
            if num <= 0 {
                return ret;
            }
            for i in 0..num {
                data[i + index] = self.buf[i + tail];
            }
            tail = (tail + num) & (self.buf.len() - 1);
            index += num;
            len -= num;
            ret += num;
        }
    }

    /// Remove contiguous elements from tail of the circular buffer
    pub fn consume(&mut self, data: &[T]) -> usize {
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
            self.tail = (self.tail + num) & (self.buf.len() - 1);
            len -= num;
            ret += num;
        }
    }

    /// Get contiguous elements from the tail of the circular buffer
    pub fn read(&mut self, data: &mut [T]) -> usize {
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
                data[i + index] = self.buf[i + self.tail];
            }
            self.tail = (self.tail + num) & (self.buf.len() - 1);
            index += num;
            len -= num;
            ret += num;
        }
    }

    /// Put contiguous elements at the head of the circular buffer
    pub fn write(&mut self, data: &[T]) -> usize {
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
                self.buf[i + self.head] = data[i + index];
            }
            self.head = (self.head + num) & (self.buf.len() - 1);
            index += num;
            len -= num;
            ret += num;
        }
    }
}
