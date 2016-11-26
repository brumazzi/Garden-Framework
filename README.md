# Garden-Framework

Garden is a Framework in C to develop web applications.

Download Garden-Framework using:

    git clone https://github.com/brumazzi/Garden-Framework.git garden
    cd garden
    gcc -I include -ldl -lpthread src/*.c -O2 -Wall -std=c99 -o garden

## Usage

To init garden server use `garden -server`, can be change server settings, (port, threads, synchronized) using `garden -server -port 8080 -threads 6 -sync`.

-server		start garden server
-port <port>	define server port
-threads <nt>	define max threads connections
-sync		use synchronized threads

## View

Views are shared libraries `views.so`.

Garden call automatically the views by url.

## View sample

    #include <web-header.h>

    void _(http_header *hh, int(*resp)(http_header *, const char*)){
    	resp(hh, "<!doctype html><html><body><h1>Hello</h1></body></html>");
    } // send string to client browser when acess '/'
    
    void _wellcome(http_header *hh, int(*resp)(http_header *, const char*)){
    	resp(hh, "Well Come!");
    } // send Well Come when acess /wellcome
