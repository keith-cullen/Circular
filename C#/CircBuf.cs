using System;

namespace Circular
{
    class CircBuf<T>
    {
        public int Head { get; set; }
        public int Tail { get; set; }
        public int Len  { get; set; }
        public T[] Buf  { get; set; }

        private bool powerOf2(int x)
        {
            for (int i = 1; i < 12; i++)
            {
                if (x == (1 << i))
                {
                    return true;
                }
            }
            return false;
        }

        public CircBuf(int len)
        {
            if (!powerOf2(len))
            {
                throw new ArgumentOutOfRangeException("len", "len must be an integer power of 2.");
            }
            Head = 0;
            Tail = 0;
            Len = len;
            Buf = new T[len];
        }

        private int countToEnd(int tail)
        {
            int countEndLinearBuf = Len - tail;
            int countEndCircBuf = (Head + countEndLinearBuf) & (Len - 1);
            return countEndCircBuf < countEndLinearBuf ? countEndCircBuf : countEndLinearBuf;
        }

        private int countToEnd()
        {
            return countToEnd(Tail);
        }

        private int spaceToEnd()
        {
            int spaceEndLinearBuf = Len - Head;
            int spaceEndCircBuf = (Tail + spaceEndLinearBuf - 1) & (Len - 1);
            return spaceEndLinearBuf < spaceEndCircBuf ? spaceEndLinearBuf : spaceEndCircBuf;
        }

        public int Count()
        {
            return (Head - Tail) & (Len - 1);
        }

        public int Space()
        {
            return (Tail - Head - 1) & (Len - 1);
        }

        public int Pop(ref T t)
        {
            if (Count() == 0)
            {
                return 0;
            }
            t = Buf[Tail];
            Tail = (Tail + 1) & (Len - 1);
            return 1;
        }

        public int Push(T t)
        {
            if (Space() == 0)
            {
                return 0;
            }
            Buf[Head] = t;
            Head = (Head + 1) & (Len - 1);
            return 1;
        }

        public int Read(T[] t)
        {
            if (t.Length <= 0)
            {
                throw new ArgumentOutOfRangeException("t", "Length must be greater than zero.");
            }

            int len = t.Length;
            int ret = 0;
            int j = 0;
            while (true)
            {
                int num = countToEnd();
                if (len < num)
                {
                    num = len;
                }
                if (num <= 0)
                {
                    break;
                }
                for (int i = 0; i < num; i++)
                {
                    t[i + j] = Buf[i + Tail];
                }
                Tail = (Tail + num) & (Len - 1);
                j += num;
                len -= num;
                ret += num;
            }
            return ret;
        }

        public int Write(T[] t)
        {
            if (t.Length <= 0)
            {
                throw new ArgumentOutOfRangeException("t", "Length must be greater than zero.");
            }

            int len = t.Length;
            int ret = 0;
            int j = 0;
            while (true)
            {
                int num = spaceToEnd();
                if (len < num)
                {
                    num = len;
                }
                if (num <= 0)
                {
                    break;
                }
                for (int i = 0; i < num; i++)
                {
                    Buf[i + Head] = t[i + j];
                }
                Head = (Head + num) & (Len - 1);
                j += num;
                len -= num;
                ret += num;
            }
            return ret;
        }

        public int Peek(T[] t)
        {
            if (t.Length <= 0)
            {
                throw new ArgumentOutOfRangeException("t", "Length must be greater than zero.");
            }

            int len = t.Length;
            int tail = Tail;
            int ret = 0;
            int j = 0;
            while (true)
            {
                int num = countToEnd(tail);
                if (len < num)
                {
                    num = len;
                }
                if (num <= 0)
                {
                    break;
                }
                for (int i = 0; i < num; i++)
                {
                    t[i + j] = Buf[i + tail];
                }
                tail = (tail + num) & (Len - 1);
                j += num;
                len -= num;
                ret += num;
            }
            return ret;
        }

        public int Consume(T[] t)
        {
            if (t.Length <= 0)
            {
                throw new ArgumentOutOfRangeException("t", "Length must be greater than zero.");
            }

            int len = t.Length;
            int ret = 0;
            while (true)
            {
                int num = countToEnd();
                if (len < num)
                {
                    num = len;
                }
                if (num <= 0)
                {
                    break;
                }
                Tail = (Tail + num) & (Len - 1);
                len -= num;
                ret += num;
            }
            return ret;
        }
    }
}
