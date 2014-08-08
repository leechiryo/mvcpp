build:
	clang++ ControllerBase.cpp ControllerFactoryBase.cpp ModelBase.cpp main.cpp -g3 -O0 -o appserv -rdynamic -std=c++11 -L /usr/local/lib -lfcgi++ -lfcgi
	clang++ -c -std=c++11 ControllerBase.cpp
	clang++ -c -std=c++11 ControllerFactoryBase.cpp
	clang++ -c -std=c++11 controllers/TestControllerComplete.cpp
	clang++ -c -std=c++11 controllers/IndexControllerComplete.cpp
	clang++ -c -std=c++11 controllers/DerivedControllerComplete.cpp
	clang++ -c -std=c++11 controllers/ManageControllerComplete.cpp
	clang++ -c -std=c++11 controllers/BuggyControllerComplete.cpp
	clang++ -shared -fPIC -Wl,-soname,debug.so -o modules/debug.so ControllerBase.o ControllerFactoryBase.o TestControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,index.so -o modules/index.so ControllerBase.o ControllerFactoryBase.o IndexControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,derived.so -o modules/derived.so ControllerBase.o ControllerFactoryBase.o DerivedControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,derived.so -o modules/manage.so ControllerBase.o ControllerFactoryBase.o ManageControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,derived.so -o modules/buggy.so ControllerBase.o ControllerFactoryBase.o BuggyControllerComplete.o

restart:
	sudo pkill appserv
	sudo spawn-fcgi -u www -U www -f ./appserv -s /var/run/fcgiwrap/fcgiwrap.sock

run:
	sudo spawn-fcgi -u www -U www -f ./appserv -s /var/run/fcgiwrap/fcgiwrap.sock
