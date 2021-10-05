using System;

namespace Circular
{
    class TestCircBuf
    {
        private const int cbLen = 8;

        static void Main(string[] args)
        {
            TestValidLen();
            TestNegLen();
            TestOddLen();
            TestSpace();
            TestCount();
            TestPopStartFront();
            TestPopStartMid();
            TestPushStartFront();
            TestPushStartMid();
            TestReadIntoSmallerBuffer();
            TestReadIntoLargerBuffer();
            TestReadIntoSmallerBufferStartMid();
            TestReadIntoLargerBufferStartMid();
            TestWriteFromSmallerBuffer();
            TestWriteFromLargerBuffer();
            TestWriteFromSmallerBufferStartMid();
            TestWriteFromLargerBufferStartMid();
            TestPeekConsumeIntoSmallerBuffer();
            TestPeekConsumeIntoLargerBuffer();
            TestPeekConsumeIntoSmallerBufferStartMid();
            TestPeekConsumeIntoLargerBufferStartMid();
        }

        static void TestValidLen()
        {
            bool pass = true;

            try
            {
                CircBuf<byte> cb = new CircBuf<byte>(8);
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestValidLen: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestNegLen()
        {
            bool pass = false;

            try
            {
                CircBuf<byte> cb = new CircBuf<byte>(-8);
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = true;
            }
            Console.Write("TestNegLen: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestOddLen()
        {
            bool pass = false;

            try
            {
                CircBuf<byte> cb = new CircBuf<byte>(5);
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = true;
            }
            Console.Write("TestOddLen: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestSpace()
        {
            bool pass = true;
            int[] numWrite = new int[2] {6, 4};
            int[] numRead = new int[2] {6, 4};

            try
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);

                int n = 0;
                for (int j = 0; j < 2; j++)
                {
                    n = cb.Space();
                    for (int i = 0; i < numWrite[j]; i++)
                    {
                        cb.Push((byte)(i + 1));
                        if (cb.Space() != n - i - 1)
                        {
                            pass = false;
                        }
                    }
                    n = cb.Space();
                    for (int i = 0; i < numRead[j]; i++)
                    {
                        byte b = 0;
                        cb.Pop(ref b);
                        if (cb.Space() != n + i + 1)
                        {
                            pass = false;
                        }
                    }
                }
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestSpace: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestCount()
        {
            bool pass = true;
            int[] numWrite = new int[2] {6, 4};
            int[] numRead = new int[2] {6, 4};

            try
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);

                int n = 0;
                for (int j = 0; j < 2; j++)
                {
                    n = cb.Count();
                    for (int i = 0; i < numWrite[j]; i++)
                    {
                        cb.Push((byte)(i + 1));
                        if (cb.Count() != n + i + 1)
                        {
                            pass = false;
                        }
                    }
                    n = cb.Count();
                    for (int i = 0; i < numRead[j]; i++)
                    {
                        byte b = 0;
                        cb.Pop(ref b);
                        if (cb.Count() != n - i - 1)
                        {
                            pass = false;
                        }
                    }
                }
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestCount: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestPopStartFront()
        {
            bool pass = true;

            try
            {
                TestPop test = new TestPop(8, new byte[8] {1, 2, 3, 4, 5, 6, 7, 0},
                                           0, 7, 12,
                                           new int[12] {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                                           new byte[12] {1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPopStartFront: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestPopStartMid()
        {
            bool pass = true;

            try
            {
                TestPop test = new TestPop(8, new byte[8] {1, 2, 3, 4, 5, 6, 7, 0},
                                           5, 4, 12,
                                           new int[12] {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                                           new byte[12] {1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPopStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        class TestPop
        {
            private int len;
            private byte[] str;
            private int start;
            private int end;
            private int numIter;
            private int[] expectedNum;
            private byte[] expected;

            public TestPop(int len, byte[] str, int start, int end, int numIter, int[] expectedNum, byte[] expected)
            {
                this.len = len;
                this.str = str;
                this.start = start;
                this.end = end;
                this.numIter = numIter;
                this.expectedNum = expectedNum;
                this.expected = expected;
            }

            public bool Run()
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);
                bool pass = true;
                int n = start;

                for (int i = 0; i < str.Length; i++)
                {
                    cb.Buf[n] = str[i];
                    if (++n >= cb.Len)
                    {
                        n = 0;
                    }
                }
                cb.Head = end;
                cb.Tail = start;
                for (int i = 0; i < numIter; i++)
                {
                    byte val = 0;
                    int num = cb.Pop(ref val);
                    if ((num != expectedNum[i]) || (val != expected[i]))
                    {
                        pass = false;
                    }
                }
                return pass;
            }
        }

        static void TestPushStartFront()
        {
            bool pass = true;

            try
            {
                TestPush test = new TestPush(8, new byte[12] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
                                             0, 0, 12,
                                             new int[12] {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                                             new byte[12, 8] {{1, 0, 0, 0, 0, 0, 0, 0},
                                                              {1, 2, 0, 0, 0, 0, 0, 0},
                                                              {1, 2, 3, 0, 0, 0, 0, 0},
                                                              {1, 2, 3, 4, 0, 0, 0, 0},
                                                              {1, 2, 3, 4, 5, 0, 0, 0},
                                                              {1, 2, 3, 4, 5, 6, 0, 0},
                                                              {1, 2, 3, 4, 5, 6, 7, 0},
                                                              {1, 2, 3, 4, 5, 6, 7, 0},
                                                              {1, 2, 3, 4, 5, 6, 7, 0},
                                                              {1, 2, 3, 4, 5, 6, 7, 0},
                                                              {1, 2, 3, 4, 5, 6, 7, 0},
                                                              {1, 2, 3, 4, 5, 6, 7, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPushStartFront: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestPushStartMid()
        {
            bool pass = true;

            try
            {
                TestPush test = new TestPush(8, new byte[12] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
                                             6, 6, 12,
                                             new int[12] {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
                                             new byte[12, 8] {{0, 0, 0, 0, 0, 0, 1, 0},
                                                              {0, 0, 0, 0, 0, 0, 1, 2},
                                                              {3, 0, 0, 0, 0, 0, 1, 2},
                                                              {3, 4, 0, 0, 0, 0, 1, 2},
                                                              {3, 4, 5, 0, 0, 0, 1, 2},
                                                              {3, 4, 5, 6, 0, 0, 1, 2},
                                                              {3, 4, 5, 6, 7, 0, 1, 2},
                                                              {3, 4, 5, 6, 7, 0, 1, 2},
                                                              {3, 4, 5, 6, 7, 0, 1, 2},
                                                              {3, 4, 5, 6, 7, 0, 1, 2},
                                                              {3, 4, 5, 6, 7, 0, 1, 2},
                                                              {3, 4, 5, 6, 7, 0, 1, 2}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPushStartFront: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        class TestPush
        {
            private int len;
            private byte[] str;
            private int start;
            private int end;
            private int numIter;
            private int[] expectedNum;
            private byte[,] expected;

            public TestPush(int len, byte[] str, int start, int end, int numIter, int[] expectedNum, byte[,] expected)
            {
                this.len = len;
                this.str = str;
                this.start = start;
                this.end = end;
                this.numIter = numIter;
                this.expectedNum = expectedNum;
                this.expected = expected;
            }

            public bool Run()
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);
                bool pass = true;

                cb.Head = end;
                cb.Tail = start;
                for (int i = 0; i < numIter; i++)
                {
                    byte val = str[i];
                    int num = cb.Push(val);
                    if (num != expectedNum[i])
                    {
                        pass = false;
                    }
                    for (int j = 0; j < cb.Len; j++)
                    {
                        if (cb.Buf[j] != expected[i, j])
                        {
                            pass = false;
                        }
                    }
                }
                return pass;
            }
        }

        static void TestReadIntoSmallerBuffer()
        {
            bool pass = true;

            try
            {
                TestRead test = new TestRead(6, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                             0, 7, 3,
                                             new int[3] {6, 1, 0},
                                             new byte[3, 6] {{1, 2, 3, 4, 5, 6},
                                                             {7, 0, 0, 0, 0, 0},
                                                             {0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestReadIntoSmallerBuffer: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestReadIntoLargerBuffer()
        {
            bool pass = true;

            try
            {
                TestRead test = new TestRead(12, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                             0, 7, 2,
                                             new int[2] {7, 0},
                                             new byte[2, 12] {{1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0},
                                                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestReadIntoLargerBuffer: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestReadIntoSmallerBufferStartMid()
        {
            bool pass = true;

            try
            {
                TestRead test = new TestRead(6, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                             5, 4, 3,
                                             new int[3] {6, 1, 0},
                                             new byte[3, 6] {{1, 2, 3, 4, 5, 6},
                                                             {7, 0, 0, 0, 0, 0},
                                                             {0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestReadIntoSmallerBufferStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestReadIntoLargerBufferStartMid()
        {
            bool pass = true;

            try
            {
                TestRead test = new TestRead(12, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                             5, 4, 2,
                                             new int[2] {7, 0},
                                             new byte[2, 12] {{1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0},
                                                              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestReadIntoLargerBufferStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        class TestRead
        {
            private int len;
            private byte[] str;
            private int start;
            private int end;
            private int numIter;
            private int[] expectedNum;
            private byte[,] expected;

            public TestRead(int len, byte[] str, int start, int end, int numIter, int[] expectedNum, byte[,] expected)
            {
                this.len = len;
                this.str = str;
                this.start = start;
                this.end = end;
                this.numIter = numIter;
                this.expectedNum = expectedNum;
                this.expected = expected;
            }

            public bool Run()
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);
                bool pass = true;
                int n = start;

                for (int i = 0; i < str.Length; i++)
                {
                    cb.Buf[n] = str[i];
                    if (++n >= cb.Len)
                    {
                        n = 0;
                    }
                }
                cb.Head = end;
                cb.Tail = start;
                for (int i = 0; i < numIter; i++)
                {
                    byte[] buf = new byte[len];
                    int num = cb.Read(buf);
                    if (num != expectedNum[i])
                    {
                        pass = false;
                    }
                    for (int j = 0; j < len; j++)
                    {
                        if (buf[j] != expected[i, j])
                        {
                            pass = false;
                        }
                    }
                }
                return pass;
            }
        }

        static void TestWriteFromSmallerBuffer()
        {
            bool pass = true;

            try
            {
                TestWrite test = new TestWrite(new byte[3] {1, 2, 3},
                                               0, 0, 4,
                                               new int[4] {3, 3, 1, 0},
                                               new byte[4, cbLen] {{1, 2, 3, 0, 0, 0, 0, 0},
                                                                   {1, 2, 3, 1, 2, 3, 0, 0},
                                                                   {1, 2, 3, 1, 2, 3, 1, 0},
                                                                   {1, 2, 3, 1, 2, 3, 1, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestWriteFromSmallerBuffer: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestWriteFromLargerBuffer()
        {
            bool pass = true;

            try
            {
                TestWrite test = new TestWrite(new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                               0, 0, 2,
                                               new int[2] {7, 0},
                                               new byte[2, cbLen] {{1, 2, 3, 4, 5, 6, 7, 0},
                                                                   {1, 2, 3, 4, 5, 6, 7, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestWriteFromLargerBuffer: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestWriteFromSmallerBufferStartMid()
        {
            bool pass = true;

            try
            {
                TestWrite test = new TestWrite(new byte[3] {1, 2, 3},
                                               6, 6, 4,
                                               new int[4] {3, 3, 1, 0},
                                               new byte[4, cbLen] {{3, 0, 0, 0, 0, 0, 1, 2},
                                                                   {3, 1, 2, 3, 0, 0, 1, 2},
                                                                   {3, 1, 2, 3, 1, 0, 1, 2},
                                                                   {3, 1, 2, 3, 1, 0, 1, 2}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestWriteFromSmallerBufferStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestWriteFromLargerBufferStartMid()
        {
            bool pass = true;

            try
            {
                TestWrite test = new TestWrite(new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                               6, 6, 2,
                                               new int[2] {7, 0},
                                               new byte[2, cbLen] {{3, 4, 5, 6, 7, 0, 1, 2},
                                                                   {3, 4, 5, 6, 7, 0, 1, 2}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestWriteFromLargerBufferStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        class TestWrite
        {
            private byte[] str;
            private int start;
            private int end;
            private int numIter;
            private int[] expectedNum;
            private byte[,] expected;

            public TestWrite(byte[] str, int start, int end, int numIter, int[] expectedNum, byte[,] expected)
            {
                this.str = str;
                this.start = start;
                this.end = end;
                this.numIter = numIter;
                this.expectedNum = expectedNum;
                this.expected = expected;
            }

            public bool Run()
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);
                bool pass = true;

                cb.Head = end;
                cb.Tail = start;
                for (int i = 0; i < numIter; i++)
                {
                    int num = cb.Write(str);
                    if (num != expectedNum[i])
                    {
                        pass = false;
                    }
                    for (int j = 0; j < cb.Len; j++)
                    {
                        if (cb.Buf[j] != expected[i, j])
                        {
                            pass = false;
                        }
                    }
                }
                return pass;
            }
        }

        static void TestPeekConsumeIntoSmallerBuffer()
        {
            bool pass = true;

            try
            {
                TestPeekConsume test = new TestPeekConsume(6, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                                           0, 7, 3,
                                                           new int[3] {6, 1, 0},
                                                           new byte[3, 6] {{1, 2, 3, 4, 5, 6},
                                                                           {7, 0, 0, 0, 0, 0},
                                                                           {0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPeekConsumeIntoSmallerBuffer: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestPeekConsumeIntoLargerBuffer()
        {
            bool pass = true;

            try
            {
                TestPeekConsume test = new TestPeekConsume(12, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                                           0, 7, 2,
                                                           new int[2] {7, 0},
                                                           new byte[2, 12] {{1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0},
                                                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPeekConsumeIntoLargerBuffer: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestPeekConsumeIntoSmallerBufferStartMid()
        {
            bool pass = true;

            try
            {
                TestPeekConsume test = new TestPeekConsume(6, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                                           5, 4, 3,
                                                           new int[3] {6, 1, 0},
                                                           new byte[3, 6] {{1, 2, 3, 4, 5, 6},
                                                                           {7, 0, 0, 0, 0, 0},
                                                                           {0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPeekConsumeIntoSmallerBufferStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        static void TestPeekConsumeIntoLargerBufferStartMid()
        {
            bool pass = true;

            try
            {
                TestPeekConsume test = new TestPeekConsume(12, new byte[7] {1, 2, 3, 4, 5, 6, 7},
                                                           5, 4, 2,
                                                           new int[2] {7, 0},
                                                           new byte[2, 12] {{1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0},
                                                                            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}});
                pass = test.Run();
            }
            catch (ArgumentOutOfRangeException e)
            {
                Console.Error.WriteLine($"Exception: {e.Message}");
                pass = false;
            }
            Console.Write("TestPeekConsumeIntoLargerBufferStartMid: ");
            if (pass)
            {
                Console.WriteLine("pass");
            }
            else
            {
                Console.WriteLine("fail");
            }
        }

        class TestPeekConsume
        {
            private int len;
            private byte[] str;
            private int start;
            private int end;
            private int numIter;
            private int[] expectedNum;
            private byte[,] expected;

            public TestPeekConsume(int len, byte[] str, int start, int end, int numIter, int[] expectedNum, byte[,] expected)
            {
                this.len = len;
                this.str = str;
                this.start = start;
                this.end = end;
                this.numIter = numIter;
                this.expectedNum = expectedNum;
                this.expected = expected;
            }

            public bool Run()
            {
                CircBuf<byte> cb = new CircBuf<byte>(cbLen);
                byte[] buf;
                bool pass = true;
                int num;
                int n = start;

                for (int i = 0; i < str.Length; i++)
                {
                    cb.Buf[n] = str[i];
                    if (++n >= cb.Len)
                    {
                        n = 0;
                    }
                }
                cb.Head = end;
                cb.Tail = start;
                for (int i = 0; i < numIter; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        buf = new byte[len];
                        num = cb.Peek(buf);
                        if (num != expectedNum[i])
                        {
                            pass = false;
                        }
                        for (int k = 0; k < len; k++)
                        {
                            if (buf[k] != expected[i, k])
                            {
                                pass = false;
                            }
                        }
                    }
                    buf = new byte[len];
                    num = cb.Consume(buf);
                    if (num != expectedNum[i])
                    {
                        pass = false;
                    }
                }
                return pass;
            }
        }
    }
}
