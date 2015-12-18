KyTea wrapper for python
==========================

2011/07/15 chezou

Mykytea-ruby is a ruby wrapper module for KyTea, a general text analysis toolkit.
KyTea is developed by KyTea Development Team

Detailed information of KyTea can be found at
http://www.phontron.com/kytea

Install Dependencies
--------------------

You need to install KyTea before build.

To build Mykytea-python, run
--------------------

    % make

If you want to install, run

    % sudo make install

If you fail to make, please try to install SWIG and run

    % swig -c++ -python -I/usr/local/include mykytea.i

Or if you still fail on Max OS X, run with some variables
    % ARCHFLAGS="-arch x86_64" CC=gcc CXX=g++ make

If you compiled kytea with clang, you need ARCHFLAGS only.

How to use?
--------------------

  See 'mykytea_test.py' as a sample program.

License
--------------------

MIT License

swig -c++ -python -I/usr/local/include  mykytea_out2.i
g++ -c mykytea_out2_wrap.cxx -I/home/tanakai/.pythonbrew/pythons/Python-2.7.3/include/python2.7  -fPIC
g++ -c mykytea_out2.cpp -I/usr/local/include  -L/usr/local/lib -fPIC
g++ -shared mykytea_out2.o mykytea_out2_wrap.o -o _Mykytea.so -lkytea
