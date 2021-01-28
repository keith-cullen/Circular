circular
========

Circular buffers implemented in Rust and C++ with copy and move semantics

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

$ testCircBuf

C++ Circular::Copying::CircBuf
------------------------------
Suitable for copying single elements or sequences of elements

$ cd C++/copying

$ make

$ testCircBuf

C++ Circular::Chan
------------------
Suitable for moving single elements using blocking operations

$ cd C++/Chan

$ make

$ testChan
