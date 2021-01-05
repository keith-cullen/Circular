use super::moving::*;

#[test]
fn new() {
    Buffer::<u8>::new(16);
}

#[test]
#[should_panic]
fn new_with_len_too_small() {
    Buffer::<u8>::new(1);
}

#[test]
#[should_panic]
fn new_with_len_not_power_of_two() {
    Buffer::<u8>::new(3);
}

#[test]
fn push_pop() {
    let mut cb = Buffer::<u8>::new(8);

    // push 5 elements

    let mut push1 = Some(1);
    let ret1 = cb.push(&mut push1);
    assert_eq!(ret1, true);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);
    match push1 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push2 = Some(2);
    let ret2 = cb.push(&mut push2);
    assert_eq!(ret2, true);
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);
    match push2 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push3 = Some(3);
    let ret3 = cb.push(&mut push3);
    assert_eq!(ret3, true);
    assert_eq!(cb.head(), 3);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);
    match push3 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push4 = Some(4);
    let ret4 = cb.push(&mut push4);
    assert_eq!(ret4, true);
    assert_eq!(cb.head(), 4);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);
    match push4 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push5 = Some(5);
    let ret5 = cb.push(&mut push5);
    assert_eq!(ret5, true);
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 0);
    assert_eq!(cb.space(), 2);
    assert_eq!(cb.count(), 5);
    match push5 {
        Some(_) => panic!(),
        None => ()
    }

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

    let mut push6 = Some(6);
    let ret6 = cb.push(&mut push6);
    assert_eq!(ret6, true);
    assert_eq!(cb.head(), 6);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);
    match push6 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push7 = Some(7);
    let ret7 = cb.push(&mut push7);
    assert_eq!(ret7, true);
    assert_eq!(cb.head(), 7);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);
    match push7 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push8 = Some(8);
    let ret8 = cb.push(&mut push8);
    assert_eq!(ret8, true);
    assert_eq!(cb.head(), 0);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);
    match push8 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push9 = Some(9);
    let ret9 = cb.push(&mut push9);
    assert_eq!(ret9, true);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);
    match push9 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push10 = Some(10);
    let ret10 = cb.push(&mut push10);
    assert_eq!(ret10, true);
    assert_eq!(cb.head(), 2);
    assert_eq!(cb.tail(), 5);
    assert_eq!(cb.space(), 2);
    assert_eq!(cb.count(), 5);
    match push10 {
        Some(_) => panic!(),
        None => ()
    }

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

    let mut push11 = Some(11);
    let ret11 = cb.push(&mut push11);
    assert_eq!(ret11, true);
    assert_eq!(cb.head(), 3);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 6);
    assert_eq!(cb.count(), 1);
    match push11 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push12 = Some(12);
    let ret12 = cb.push(&mut push12);
    assert_eq!(ret12, true);
    assert_eq!(cb.head(), 4);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 5);
    assert_eq!(cb.count(), 2);
    match push12 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push13 = Some(13);
    let ret13 = cb.push(&mut push13);
    assert_eq!(ret13, true);
    assert_eq!(cb.head(), 5);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 4);
    assert_eq!(cb.count(), 3);
    match push13 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push14 = Some(14);
    let ret14 = cb.push(&mut push14);
    assert_eq!(ret14, true);
    assert_eq!(cb.head(), 6);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 3);
    assert_eq!(cb.count(), 4);
    match push14 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push15 = Some(15);
    let ret15 = cb.push(&mut push15);
    assert_eq!(ret15, true);
    assert_eq!(cb.head(), 7);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 2);
    assert_eq!(cb.count(), 5);
    match push15 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push16 = Some(16);
    let ret16 = cb.push(&mut push16);
    assert_eq!(ret16, true);
    assert_eq!(cb.head(), 0);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 1);
    assert_eq!(cb.count(), 6);
    match push16 {
        Some(_) => panic!(),
        None => ()
    }

    let mut push17 = Some(17);
    let ret17 = cb.push(&mut push17);
    assert_eq!(ret17, true);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 0);
    assert_eq!(cb.count(), 7);
    match push17 {
        Some(_) => panic!(),
        None => ()
    }

    // attempt to push into full buffer

    let mut pushf0 = Some(0);
    let retf0 = cb.push(&mut pushf0);
    assert_eq!(retf0, false);
    assert_eq!(cb.head(), 1);
    assert_eq!(cb.tail(), 2);
    assert_eq!(cb.space(), 0);
    assert_eq!(cb.count(), 7);
    match pushf0 {
        Some(_) => (),
        None => panic!()
    }
}

#[test]
fn write_read_smaller_buffer() {
    let mut cb = Buffer::<u8>::new(8);
    // write with sufficient space available
    {
        let e: [Option<u8>; 4] = [None, None, None, None];
        let mut v: [Option<u8>; 4] = [Some(1), Some(2), Some(3), Some(4)];
        let num = cb.write(&mut v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 4);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
        assert_eq!(v, e);
    }
    // write with insufficient space available
    {
        let e: [Option<u8>; 4] = [None, None, None, Some(8)];
        let mut v: [Option<u8>; 4] = [Some(5), Some(6), Some(7), Some(8)];
        let num = cb.write(&mut v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(v, e);
    }
    // read with sufficient bytes available
    {
        let e: [Option<u8>; 4] = [Some(1), Some(2), Some(3), Some(4)];
        let mut v: [Option<u8>; 4] = [None, None, None, None];
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
        let e: [Option<u8>; 4] = [Some(5), Some(6), Some(7), None];
        let mut v: [Option<u8>; 4] = [None, None, None, None];
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
    let mut cb = Buffer::<u8>::new(8);
    // write with insufficient space available
    {
        let e: [Option<u8>; 12] = [None, None, None, None, None, None,
                                   None, Some(8), Some(9), Some(10), Some(11), Some(12)];
        let mut v: [Option<u8>; 12] = [Some(1), Some(2), Some(3), Some(4), Some(5), Some(6),
                                       Some(7), Some(8), Some(9), Some(10), Some(11), Some(12)];
        let num = cb.write(&mut v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(v, e);
    }
    // write with insufficient space available
    {
        let e: [Option<u8>; 12] = [Some(1), Some(2), Some(3), Some(4), Some(5), Some(6),
                                   Some(7), Some(8), Some(9), Some(10), Some(11), Some(12)];
        let mut v: [Option<u8>; 12] = [Some(1), Some(2), Some(3), Some(4), Some(5), Some(6),
                                       Some(7), Some(8), Some(9), Some(10), Some(11), Some(12)];
        let num = cb.write(&mut v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 7);
        assert_eq!(cb.tail(), 0);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(v, e);
    }
    // read with insufficient bytes available
    {
        let e: [Option<u8>; 12] = [Some(1), Some(2), Some(3), Some(4), Some(5), Some(6),
                                   Some(7), None, None, None, None, None];
        let mut v: [Option<u8>; 12] = [None, None, None, None, None, None,
                                       None, None, None, None, None, None];
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
        let e: [Option<u8>; 12] = [None, None, None, None, None, None,
                                   None, None, None, None, None, None];
        let mut v: [Option<u8>; 12] = [None, None, None, None, None, None,
                                       None, None, None, None, None, None];
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
    let mut cb = Buffer::<u8>::new(8);
    // move head and tail to mid way
    {
        let mut v: [Option<u8>; 4] = [None, None, None, None];
        let num1 = cb.write(&mut v);
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
        let e: [Option<u8>; 4] = [None, None, None, None];
        let mut v: [Option<u8>; 4] = [Some(1), Some(2), Some(3), Some(4)];
        let num = cb.write(&mut v);
        assert_eq!(num, 4);
        assert_eq!(cb.head(), 0);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 3);
        assert_eq!(cb.count(), 4);
        assert_eq!(v, e);
    }
    // write with insufficient space available
    {
        let e: [Option<u8>; 4] = [None, None, None, Some(8)];
        let mut v: [Option<u8>; 4] = [Some(5), Some(6), Some(7), Some(8)];
        let num = cb.write(&mut v);
        assert_eq!(num, 3);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(v, e);
    }
    // read with sufficient bytes available
    {
        let e: [Option<u8>; 4] = [Some(1), Some(2), Some(3), Some(4)];
        let mut v: [Option<u8>; 4] = [None, None, None, None];
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
        let e: [Option<u8>; 4] = [Some(5), Some(6), Some(7), None];
        let mut v: [Option<u8>; 4] = [None, None, None, None];
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
    let mut cb = Buffer::<u8>::new(8);
    // move head and tail to mid way
    {
        let mut v: [Option<u8>; 4] = [None, None, None, None];
        let num1 = cb.write(&mut v);
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
        let e: [Option<u8>; 12] = [None, None, None, None, None, None,
                                   None, Some(8), Some(9), Some(10), Some(11), Some(12)];
        let mut v: [Option<u8>; 12] = [Some(1), Some(2), Some(3), Some(4), Some(5), Some(6),
                                       Some(7), Some(8), Some(9), Some(10), Some(11), Some(12)];
        let num = cb.write(&mut v);
        assert_eq!(num, 7);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(v, e);
    }
    // write with insufficient space available
    {
        let e: [Option<u8>; 12] = [Some(13), Some(14), Some(15), Some(16), Some(17), Some(18),
                                   Some(19), Some(20), Some(21), Some(22), Some(23), Some(24)];
        let mut v: [Option<u8>; 12] = [Some(13), Some(14), Some(15), Some(16), Some(17), Some(18),
                                       Some(19), Some(20), Some(21), Some(22), Some(23), Some(24)];
        let num = cb.write(&mut v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 4);
        assert_eq!(cb.space(), 0);
        assert_eq!(cb.count(), 7);
        assert_eq!(v, e);
    }
    // read with insufficient bytes available
    {
        let e: [Option<u8>; 12] = [Some(1), Some(2), Some(3), Some(4), Some(5), Some(6),
                                   Some(7), None, None, None, None, None];
        let mut v: [Option<u8>; 12] = [None, None, None, None, None, None,
                                       None, None, None, None, None, None];
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
        let e: [Option<u8>; 12] = [None, None, None, None, None, None,
                                   None, None, None, None, None, None];
        let mut v: [Option<u8>; 12] = [None, None, None, None, None, None,
                                       None, None, None, None, None, None];
        let num = cb.read(&mut v);
        assert_eq!(num, 0);
        assert_eq!(cb.head(), 3);
        assert_eq!(cb.tail(), 3);
        assert_eq!(cb.space(), 7);
        assert_eq!(cb.count(), 0);
        assert_eq!(v, e);
    }
}
