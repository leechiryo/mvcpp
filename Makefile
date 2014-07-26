build:
	clang++ ModelBase.cpp main.cpp -o appserv -rdynamic -g3 -O0 -std=c++11 -L /usr/local/lib -lfcgi++ -lfcgi
	clang++ -c -std=c++11 controllers/TestController.cpp -o modules/debug.o
	clang++ -shared -fPIC -Wl,-soname,debug.so -o modules/debug.so modules/debug.o
	clang++ -c -std=c++11 controllers/IndexController.cpp -o modules/index.o
	clang++ -shared -fPIC -Wl,-soname,index.so -o modules/index.so modules/index.o

restart:
	sudo pkill appserv
	sudo spawn-fcgi -u www -U www -f ./appserv -s /var/run/fcgiwrap/fcgiwrap.sock

run:
	sudo spawn-fcgi -u www -U www -f ./appserv -s /var/run/fcgiwrap/fcgiwrap.sock
