build:
	clang++ ControllerBase.cpp ControllerFactoryBase.cpp ModelBase.cpp main.cpp -g3 -O0 -o appserv -rdynamic -std=c++11 -I/usr/local/include -L/usr/local/lib -lfcgi++ -lfcgi -lpthread
	clang++ -c -std=c++11 -I/usr/local/include -fPIC ControllerBase.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC ControllerFactoryBase.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC controllers/AngularTestControllerComplete.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC controllers/BuggyControllerComplete.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC controllers/DerivedControllerComplete.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC controllers/IndexControllerComplete.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC controllers/ManageControllerComplete.cpp
	clang++ -c -std=c++11 -I/usr/local/include -fPIC controllers/TestControllerComplete.cpp
	clang++ -shared -fPIC -Wl,-soname,angular.so -o modules/angular.so ControllerBase.o ControllerFactoryBase.o AngularTestControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,buggy.so -o modules/buggy.so ControllerBase.o ControllerFactoryBase.o BuggyControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,derived.so -o modules/derived.so ControllerBase.o ControllerFactoryBase.o DerivedControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,index.so -o modules/index.so ControllerBase.o ControllerFactoryBase.o IndexControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,manage.so -o modules/manage.so ControllerBase.o ControllerFactoryBase.o ManageControllerComplete.o
	clang++ -shared -fPIC -Wl,-soname,debug.so -o modules/debug.so ControllerBase.o ControllerFactoryBase.o TestControllerComplete.o

restart:
	sudo pkill appserv
	sudo spawn-fcgi -u www -U www -f ./appserv -s /var/run/fcgiwrap/fcgiwrap.sock

run:
	sudo spawn-fcgi -u www -U www -f ./appserv -s /var/run/fcgiwrap/fcgiwrap.sock
