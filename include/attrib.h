#ifndef  __ATTRIB_H__
#define  __ATTRIB_H__

#define pack __attribute__((__packed__))
#define initialize __attribute__((constructor))
#define finalize __attribute__((destructor))
#ifndef externc
#	ifdef __cplusplus
#		define externc extern "C"
#	else
#		define externc extern
#	endif
#endif

#ifdef DEBUG
#define debug(msg, ...) printf(msg, __VA_ARGS__)
#else
#define debug(msg, ...) printf("");
#endif

#endif  /*ATTRIB_H*/
