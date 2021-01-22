#include <iostream>
#include <utility>

struct Copyable
{
    Copyable() = default;
    Copyable(const Copyable &c) : i{c.i} {}
    Copyable &operator=(const Copyable &c) {i = c.i; return *this;}
    int i{0};
};

struct Moveable
{
    Moveable() = default;
    Moveable(Moveable &&m) {std::swap(i, m.i);}
    Moveable &operator=(Moveable &&m) {std::swap(i, m.i); return *this;}
    int i{0};
};

struct Flexable
{
    Flexable() = default;
    Flexable(const Flexable &f) : i{f.i} {}
    Flexable(Flexable &&f) {std::swap(i, f.i);}
    Flexable &operator=(const Flexable &f) {i = f.i; return *this;}
    Flexable &operator=(Flexable &&f) {std::swap(i, f.i); return *this;}
    int i{0};
};

void fc(Copyable *c1, Copyable *c2)
{
    *c1 = std::move(*c2);
}

void fm(Moveable *m1, Moveable *m2)
{
    *m1 = std::move(*m2);
}

void ff(Flexable *f1, Flexable *f2)
{
    *f1 = std::move(*f2);
}

int main(void)
{
    std::cout << "----------------------------------------" << std::endl;
    {
        Copyable c1, c2;
        c1.i = 1;
        c2.i = 2;

        std::cout << "Before" << std::endl;
        std::cout << "c1.i: " << c1.i << std::endl;
        std::cout << "c2.i: " << c2.i << std::endl;

        fc(&c1, &c2);

        std::cout << "After:" << std::endl;
        std::cout << "c1.i: " << c1.i << std::endl;
        std::cout << "c2.i: " << c2.i << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    {
        Moveable m1, m2;
        m1.i = 1;
        m2.i = 2;

        std::cout << "Before" << std::endl;
        std::cout << "m1.i: " << m1.i << std::endl;
        std::cout << "m2.i: " << m2.i << std::endl;

        fm(&m1, &m2);

        std::cout << "After:" << std::endl;
        std::cout << "m1.i: " << m1.i << std::endl;
        std::cout << "m2.i: " << m2.i << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    {
        Flexable f1, f2;
        f1.i = 1;
        f2.i = 2;

        std::cout << "Before" << std::endl;
        std::cout << "f1.i: " << f1.i << std::endl;
        std::cout << "f2.i: " << f2.i << std::endl;

        ff(&f1, &f2);

        std::cout << "After:" << std::endl;
        std::cout << "f1.i: " << f1.i << std::endl;
        std::cout << "f2.i: " << f2.i << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}
