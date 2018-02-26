all:
	swig -c++ -python voxical.i 
	c++ -c voxical.cpp -std=c++11 -I. -fPIC
	c++ -c voxical_wrap.cxx -I/usr/include/python2.7/ -I. -std=c++11 -fPIC
	c++ -shared voxical.o voxical_wrap.o -o _voxical.so
