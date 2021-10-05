Circular
========

Circular buffers implemented in Rust, Go, C++ and C with copy and move semantics

Rust circular::moving::CircBuf
------------------------------
Suitable for moving single elements or sequences of elements

$ cd Rust

$ cargo test

Rust circular::copying::CircBuf
-------------------------------
Suitable for copying single elements or sequences of elements

$ cd Rust

$ cargo test

C++ Circular::Moving::CircBuf
-----------------------------
Suitable for moving single elements or sequences of elements

$ cd C++/moving

$ make

$ ./testCircBuf

C++ Circular::Copying::CircBuf
------------------------------
Suitable for copying single elements or sequences of elements

$ cd C++/copying

$ make

$ ./testCircBuf

C++ Circular::Chan
------------------
Suitable for moving single elements using blocking operations

$ cd C++/Chan

$ make

$ ./testChan

Go github.com/keith-cullen/circular/circbuf
-------------------------------------------
Suitable for copying sequences of bytes

$ cd Go/src/github.com/keith-cullen/circular/circbuf

$ go test

C circ_buf
----------
Suitable for copying sequences of bytes

$ cd C

$ make

$ ./test_circ_buf

C# Circular.CircBuf
-------------------
Suitable for copying single elements or sequences of elements

$ cd C#

$ dotnet run
