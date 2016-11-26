#include <web-header.h>

// garden framework use routes to every things,
// when use routes, the programer can send page
// and files to client without files path in url.

/*How to create router functions*/

/*
 * _ represents main page (index or /)
 *
 * replace / by _ to create router function
 * example:
 * _register		=> acess register page /register
 * _register_new	=> acess new register /register/new
 */

/*
 * Can be use set_response to declare respose method
 * response method args are const http_header* and const char *
 *
 * void _(const http_header *hh, set_response(resp)){
 * }
*/

void _(const http_header *hh, int (*response)(const http_header*, const char *)){
	/* here put all page logic */

	const char *my_page = "<!doctype html>\n"
		"<html>\n"
		"<head>\n"
		"	<title>Hello World</title>\n"
		"	<meta charset=\"UTF-8\">\n"
		"</head>\n"
		"<body>\n"
		"	<h1>Hello World!</h1>\n"
		"</body>\n"
		"</html>\n";

	response(hh, my_page);
}

void _hello(const http_header *hh, set_response(resp)){
	/* here put all page logic */

	const char *my_page = "<!doctype html>\n"
		"<html>\n"
		"<head>\n"
		"	<title>Hello World</title>\n"
		"	<meta charset=\"UTF-8\">\n"
		"</head>\n"
		"<body>\n"
		"	<h1>you open hello page!</h1>\n"
		"</body>\n"
		"</html>\n";

	resp(hh, my_page);
}
