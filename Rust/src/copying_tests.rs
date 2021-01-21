// +--------------------------+
// |                          |
// |    Copyright (c) 2020    |
// |       Keith Cullen       |
// |                          |
// +--------------------------+

use super::copying::*;
use std::sync::Arc;
use std::sync::Mutex;
use std::thread;

#[test]
fn new() {
    CircBuf::<u8>::new(16);
}

#[test]
#[should_panic]
fn new_with_len_too_small() {
    CircBuf::<u8>::new(1);
}

#[test]
#[should_panic]
fn new_with_len_not_power_of_two() {
    CircBuf::<u8>::new(3);
}

#[test]
fn push_pop() {
    let mut cb = CircBuf::<u8>::new(8);

    // push 5 elements

    let ret1 = cb.push(1);
    assert_eq!(ret1, true);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);

    let ret2 = cb.push(2);
    assert_eq!(ret2, true);
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);

    let ret3 = cb.push(3);
    assert_eq!(ret3, true);
    assert_eq!(cb.head(), 3);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);

    let ret4 = cb.push(4);
    assert_eq!(ret4, true);
    assert_eq!(cb.head(), 4);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);

    let ret5 = cb.push(5);
    assert_eq!(ret5, true);
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 2);
    assert_eq!(cb.count(), 5);

    // pop 5 elements

    let pop1 = cb.pop();
    match pop1 {
        Some(1) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 1);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);

    let pop2 = cb.pop();
    match pop2 {
        Some(2) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);

    let pop3 = cb.pop();
    match pop3 {
        Some(3) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 3);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);

    let pop4 = cb.pop();
    match pop4 {
        Some(4) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 4);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);

    let pop5 = cb.pop();
    match pop5 {
        Some(5) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 7);
    assert_eq!(cb.count(), 0);

    // attempt to pop from empty buffer

    let pope0 = cb.pop();
    match pope0 {
        Some(_) => panic!(),
        None => ()
    }
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 7);
    assert_eq!(cb.count(), 0);

    // push 5 elements

    let ret6 = cb.push(6);
    assert_eq!(ret6, true);
    assert_eq!(cb.head(), 6);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);

    let ret7 = cb.push(7);
    assert_eq!(ret7, true);
    assert_eq!(cb.head(), 7);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);

    let ret8 = cb.push(8);
    assert_eq!(ret8, true);
    assert_eq!(cb.head(), 0);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);

    let ret9 = cb.push(9);
    assert_eq!(ret9, true);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);

    let ret10 = cb.push(10);
    assert_eq!(ret10, true);
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 2);
    assert_eq!(cb.count(), 5);

    // pop 5 elements

    let pop6 = cb.pop();
    match pop6 {
        Some(6) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 6);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);

    let pop7 = cb.pop();
    match pop7 {
        Some(7) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 7);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);

    let pop8 = cb.pop();
    match pop8 {
        Some(8) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);

    let pop9 = cb.pop();
    match pop9 {
        Some(9) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 1);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);

    let pop10 = cb.pop();
    match pop10 {
        Some(10) => (),
        _ => panic!()
    }
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 7);
    assert_eq!(cb.count(), 0);

    // attempt to pop from empty buffer

    let pope1 = cb.pop();
    match pope1 {
        Some(_) => panic!(),
        None => ()
    }
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 7);
    assert_eq!(cb.count(), 0);

    // push 7 elements

    let ret11 = cb.push(11);
    assert_eq!(ret11, true);
    assert_eq!(cb.head(), 3);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);

    let ret12 = cb.push(12);
    assert_eq!(ret12, true);
    assert_eq!(cb.head(), 4);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);

    let ret13 = cb.push(13);
    assert_eq!(ret13, true);
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);

    let ret14 = cb.push(14);
    assert_eq!(ret14, true);
    assert_eq!(cb.head(), 6);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);

    let ret15 = cb.push(15);
    assert_eq!(ret15, true);
    assert_eq!(cb.head(), 7);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 2);
    assert_eq!(cb.count(), 5);

    let ret16 = cb.push(16);
    assert_eq!(ret16, true);
    assert_eq!(cb.head(), 0);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 1);
    assert_eq!(cb.count(), 6);

    let ret17 = cb.push(17);
    assert_eq!(ret17, true);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 0);
    assert_eq!(cb.count(), 7);

    // attempt to push into full buffer

    let retf0 = cb.push(0);
    assert_eq!(retf0, false);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 0);
    assert_eq!(cb.count(), 7);
}

#[test]
fn write_read_smaller_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // write with sufficient space available
    {
        let v: [u8; 4] = [1, 2, 3, 4];
        let num = cb.write(&v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
    }
    // write with insufficient space available
    {
        let v: [u8; 4] = [5, 6, 7, 8];
        let num = cb.write(&v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // read with sufficient bytes available
    {
        let e: [u8; 4] = [1, 2, 3, 4];
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(v, e);
    }
    // read with insufficient bytes available
    {
        let e: [u8; 4] = [5, 6, 7, 0];
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
}

#[test]
fn write_read_larger_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // write with insufficient space available
    {
        let v: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];
        let num = cb.write(&v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // write with insufficient space available
    {
        let v: [u8; 12] = [13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24];
        let num = cb.write(&v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // read with insufficient bytes available
    {
        let e: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0];
        let mut v: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
    // read with insufficient bytes available
    {
        let e: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let mut v: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
}

#[test]
fn start_mid_write_read_smaller_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // move head and tail to mid way
    {
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num1 = cb.write(&v);
        assert_eq!(num1, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
        let num2 = cb.read(&mut v);
        assert_eq!(num2, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
    }
    // write with sufficient space available
    {
        let v: [u8; 4] = [1, 2, 3, 4];
        let num = cb.write(&v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 0);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
    }
    // write with insufficient space available
    {
        let v: [u8; 4] = [5, 6, 7, 8];
        let num = cb.write(&v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // read with sufficient bytes available
    {
        let e: [u8; 4] = [1, 2, 3, 4];
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(v, e);
    }
    // read with insufficient bytes available
    {
        let e: [u8; 4] = [5, 6, 7, 0];
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
}

#[test]
fn start_mid_write_read_larger_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // move head and tail to mid way
    {
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num1 = cb.write(&v);
        assert_eq!(num1, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
        let num2 = cb.read(&mut v);
        assert_eq!(num2, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
    }
    // write with insufficient space available
    {
        let v: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];
        let num = cb.write(&v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // write with insufficient space available
    {
        let v: [u8; 12] = [13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24];
        let num = cb.write(&v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // read with insufficient bytes available
    {
        let e: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0];
        let mut v: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
    // read with insufficient bytes available
    {
        let e: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let mut v: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let num = cb.read(&mut v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
}

#[test]
fn write_peek_consume_smaller_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // write with sufficient space available
    {
        let v: [u8; 4] = [1, 2, 3, 4];
        let num = cb.write(&v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
    }
    // write with insufficient space available
    {
        let v: [u8; 4] = [5, 6, 7, 8];
        let num = cb.write(&v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // peek, peek, consume with sufficient bytes available
    {
        let ep1: [u8; 4] = [1, 2, 3, 4];
        let mut vp1: [u8; 4] = [0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 4);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 4] = [1, 2, 3, 4];
        let mut vp2: [u8; 4] = [0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 4);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp2, ep2);
        let ec: [u8; 4] = [0, 0, 0, 0];
        let vc: [u8; 4] = [0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 4);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(vc, ec);
    }
    // peek, peek, consume with insufficient bytes available
    {
        let ep1: [u8; 4] = [5, 6, 7, 0];
        let mut vp1: [u8; 4] = [0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 4] = [5, 6, 7, 0];
        let mut vp2: [u8; 4] = [0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(vp2, ep2);
        let ec: [u8; 4] = [0, 0, 0, 0];
        let vc: [u8; 4] = [0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vc, ec);
    }
}

#[test]
fn write_peek_consume_larger_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // write with insufficient space available
    {
        let v: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];
        let num = cb.write(&v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // write with insufficient space available
    {
        let v: [u8; 12] = [13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24];
        let num = cb.write(&v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // peek, peek, consume with insufficient bytes available
    {
        let ep1: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0];
        let mut vp1: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0];
        let mut vp2: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp2, ep2);
        let ec: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let vc: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vc, ec);
    }
    // peek, peek, consume with insufficient bytes available
    {
        let ep1: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let mut vp1: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let mut vp2: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vp2, ep2);
        let ec: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let vc: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 7);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vc, ec);
    }
}

#[test]
fn start_mid_write_peek_consume_smaller_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // move head and tail to mid way
    {
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num1 = cb.write(&v);
        assert_eq!(num1, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
        let num2 = cb.read(&mut v);
        assert_eq!(num2, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
    }
    // write with sufficient space available
    {
        let v: [u8; 4] = [1, 2, 3, 4];
        let num = cb.write(&v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 0);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
    }
    // write with insufficient space available
    {
        let v: [u8; 4] = [5, 6, 7, 8];
        let num = cb.write(&v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // peek, peek, consume with sufficient bytes available
    {
        let ep1: [u8; 4] = [1, 2, 3, 4];
        let mut vp1: [u8; 4] = [0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 4);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 4] = [1, 2, 3, 4];
        let mut vp2: [u8; 4] = [0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 4);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp2, ep2);
        let ec: [u8; 4] = [0, 0, 0, 0];
        let vc: [u8; 4] = [0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 4);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(vc, ec);
    }
    // peek, peek, consume with insufficient bytes available
    {
        let ep1: [u8; 4] = [5, 6, 7, 0];
        let mut vp1: [u8; 4] = [0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 4] = [5, 6, 7, 0];
        let mut vp2: [u8; 4] = [0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 4);
        assert_eq!(cb.count(), 3);
        assert_eq!(vp2, ep2);
        let ec: [u8; 4] = [0, 0, 0, 0];
        let vc: [u8; 4] = [0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vc, ec);
    }
}

#[test]
fn start_mid_write_peek_consume_larger_buffer() {
    let mut cb = CircBuf::<u8>::new(8);
    // move head and tail to mid way
    {
        let mut v: [u8; 4] = [0, 0, 0, 0];
        let num1 = cb.write(&v);
        assert_eq!(num1, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
        let num2 = cb.read(&mut v);
        assert_eq!(num2, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
    }
    // write with insufficient space available
    {
        let v: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];
        let num = cb.write(&v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // write with insufficient space available
    {
        let v: [u8; 12] = [13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24];
        let num = cb.write(&v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
    }
    // peek, peek, consume with insufficient bytes available
    {
        let ep1: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0];
        let mut vp1: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 12] = [1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0];
        let mut vp2: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(vp2, ep2);
        let ec: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let vc: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vc, ec);
    }
    // peek, peek, consume with insufficient bytes available
    {
        let ep1: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let mut vp1: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump1 = cb.peek(&mut vp1);
        assert_eq!(nump1, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vp1, ep1);
        let ep2: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let mut vp2: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let nump2 = cb.peek(&mut vp2);
        assert_eq!(nump2, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vp2, ep2);
        let ec: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let vc: [u8; 12] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
        let numc = cb.consume(&vc);
        assert_eq!(numc, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(vc, ec);
    }
}

#[test]
fn start_multi_threaded() {
    let sender = Arc::new(Mutex::new(CircBuf::<u8>::new(8)));
    let receiver = Arc::clone(&sender);
    let ret1 = sender.lock().unwrap().push(1);
    assert_eq!(ret1, true);
    let ret2 = sender.lock().unwrap().push(2);
    assert_eq!(ret2, true);
    let thread_handle = thread::spawn(move || {
        let pop1 = receiver.lock().unwrap().pop();
        match pop1 {
           Some(v) => assert_eq!(v, 1),
           _ => panic!()
        }
        let pop2 = receiver.lock().unwrap().pop();
        match pop2 {
           Some(v) => assert_eq!(v, 2),
           _ => panic!()
        }
    });
    thread_handle.join().unwrap();
}
