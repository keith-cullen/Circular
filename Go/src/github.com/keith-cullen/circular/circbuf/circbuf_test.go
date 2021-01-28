// +--------------------------+
// |                          |
// |    Copyright (c) 2016    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

package circbuf

import (
	"bytes"
	"fmt"
	"testing"
)

const (
	writeOp = iota
	readOp
	peekOp
	consumeOp
)

type testData struct {
	src []byte
	op int
	num int
	space int
	count int
	exp []byte
}

func run(t *testing.T, size int, data []testData) {
	c, err := New(size)
	if err != nil {
		t.Fatalf("%v", err)
	}

	for i := 0; i < len(data); i++ {
		var num int
		dst := make([]byte, size)
		fmt.Println("before:")
		fmt.Printf("src: %v\n", data[i].src)
		fmt.Printf("dst: %v\n", dst)
		fmt.Printf("%v\n", c)
		switch data[i].op {
		case writeOp:
			fmt.Printf("num = c.Write(src[0:%v])\n", len(data[i].src))
			num = c.Write(data[i].src)
			if num != data[i].num {
				t.Errorf("Write: num: %v, want %v", num, data[i].num)
			}
			if bytes.Compare(c.buf, data[i].exp) != 0 {
				t.Errorf("Write: c.buf: %v, want %v", c.buf, data[i].exp)
			}
			if c.Space() != data[i].space {
				t.Errorf("Write: c.Space(): %v, want: %v\n", c.Space(), data[i].space)
			}
			if c.Count() != data[i].count {
				t.Errorf("Write: c.Count(): %v, want: %v\n", c.Count(), data[i].count)
			}
		case readOp:
			fmt.Printf("num = c.Read(dst[0:%v])\n", len(dst))
			num = c.Read(dst)
			if num != data[i].num {
				t.Errorf("Read: num: %v, want %v", num, data[i].num)
			}
			if bytes.Compare(dst, data[i].exp) != 0 {
				t.Errorf("Read: c.buf: %v, want %v", dst, data[i].exp)
			}
			if c.Space() != data[i].space {
				t.Errorf("Write: c.Space(): %v, want: %v\n", c.Space(), data[i].space)
			}
			if c.Count() != data[i].count {
				t.Errorf("Write: c.Count(): %v, want: %v\n", c.Count(), data[i].count)
			}
		case peekOp:
			fmt.Printf("num = c.Peek(dst[0:%v])\n", len(dst))
			num = c.Peek(dst)
			if num != data[i].num {
				t.Errorf("Peek: num: %v, want %v", num, data[i].num)
			}
			if bytes.Compare(dst, data[i].exp) != 0 {
				t.Errorf("Peek: c.buf: %v, want %v", dst, data[i].exp)
			}
			if c.Space() != data[i].space {
				t.Errorf("Write: c.Space(): %v, want: %v\n", c.Space(), data[i].space)
			}
			if c.Count() != data[i].count {
				t.Errorf("Write: c.Count(): %v, want: %v\n", c.Count(), data[i].count)
			}
		case consumeOp:
			fmt.Printf("num = c.Consume(%v)\n", data[i].num)
			num = c.Consume(data[i].num)
			if num != data[i].num {
				t.Errorf("Consume: num: %v, want %v", num, data[i].num)
			}
			if c.Space() != data[i].space {
				t.Errorf("Write: c.Space(): %v, want: %v\n", c.Space(), data[i].space)
			}
			if c.Count() != data[i].count {
				t.Errorf("Write: c.Count(): %v, want: %v\n", c.Count(), data[i].count)
			}
		}
		fmt.Println("after:")
		fmt.Printf("num: %v\n", num)
		fmt.Printf("src: %v\n", data[i].src)
		fmt.Printf("dst: %v\n", dst)
		fmt.Printf("%v\n\n", c)
	}
}

func TestWriteRead(t *testing.T) {
	const size = 8
	data := []testData{{
			src: []byte{1, 2, 3, 4, 5},
			op: writeOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{1, 2, 3, 4, 5, 0, 0, 0},
		},{
			src: []byte{},
			op: readOp,
			num: 5,
			space: 7,
			count: 0,
			exp: []byte{1, 2, 3, 4, 5, 0, 0, 0},
		},{
			src: []byte{},
			op: readOp,
			num: 0,
			space: 7,
			count: 0,
			exp: []byte{0, 0, 0, 0, 0, 0, 0, 0},
		},{
			src: []byte{6, 7, 8, 9, 10},
			op: writeOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{9, 10, 3, 4, 5, 6, 7, 8},
		},{
			src: []byte{},
			op: readOp,
			num: 5,
			space: 7,
			count: 0,
			exp: []byte{6, 7, 8, 9, 10, 0, 0, 0},
		},{
			src: []byte{},
			op: readOp,
			num: 0,
			space: 7,
			count: 0,
			exp: []byte{0, 0, 0, 0, 0, 0, 0, 0},
		},{
			src: []byte{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			op: writeOp,
			num: 7,
			space: 0,
			count: 7,
			exp: []byte{7, 10, 1, 2, 3, 4, 5, 6},
		},{
			src: []byte{},
			op: readOp,
			num: 7,
			space: 7,
			count: 0,
			exp: []byte{1, 2, 3, 4, 5, 6, 7, 0},
		},{
			src: []byte{},
			op: readOp,
			num: 0,
			space: 7,
			count: 0,
			exp: []byte{0, 0, 0, 0, 0, 0, 0, 0},
		},
	}
	fmt.Println("--------< TestWriteRead >--------")
	run(t, size, data)
}

func TestWritePeekConsume(t *testing.T) {
	const size = 8
	data := []testData{{
			src: []byte{1, 2, 3, 4, 5},
			op: writeOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{1, 2, 3, 4, 5, 0, 0, 0},
		},{
			src: []byte{},
			op: peekOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{1, 2, 3, 4, 5, 0, 0, 0},
		},{
			src: []byte{},
			op: peekOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{1, 2, 3, 4, 5, 0, 0, 0},
		},{
			src: []byte{},
			op: consumeOp,
			num: 5,
			space: 7,
			count: 0,
			exp: []byte{},
		},{
			src: []byte{6, 7, 8, 9, 10},
			op: writeOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{9, 10, 3, 4, 5, 6, 7, 8},
		},{
			src: []byte{},
			op: peekOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{6, 7, 8, 9, 10, 0, 0, 0},
		},{
			src: []byte{},
			op: peekOp,
			num: 5,
			space: 2,
			count: 5,
			exp: []byte{6, 7, 8, 9, 10, 0, 0, 0},
		},{
			src: []byte{},
			op: consumeOp,
			num: 5,
			space: 7,
			count: 0,
			exp: []byte{},
		},{
			src: []byte{1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
			op: writeOp,
			num: 7,
			space: 0,
			count: 7,
			exp: []byte{7, 10, 1, 2, 3, 4, 5, 6},
		},{
			src: []byte{},
			op: peekOp,
			num: 7,
			space: 0,
			count: 7,
			exp: []byte{1, 2, 3, 4, 5, 6, 7, 0},
		},{
			src: []byte{},
			op: peekOp,
			num: 7,
			space: 0,
			count: 7,
			exp: []byte{1, 2, 3, 4, 5, 6, 7, 0},
		},{
			src: []byte{},
			op: consumeOp,
			num: 7,
			space: 7,
			count: 0,
			exp: []byte{},
		},
	}
	fmt.Println("--------< TestWritePeekConsume >--------")
	run(t, size, data)
}
