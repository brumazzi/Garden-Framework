#include <mem.h>
#include <string.h>
#include <sys/mman.h>

#define initialize __attribute__((constructor))
#define finalize __attribute__((destructor))

struct __regs__{
	void *_ptr;
	size_t size;
	struct __regs__ *next;
};

struct __regs__ *reg;
void *mem_reg(size_t size);
short mem_unreg(void *_ptr, size_t size);

extern void *grd_alloc(size_t size){	
	struct __regs__ *_new = mem_reg(sizeof(struct __regs__));
	void *_ptr = mem_reg(size);
	bzero(_ptr, size);

	_new->_ptr = _ptr;
	_new->size = size;
	_new->next = reg;

	reg = _new;

	return _ptr;
}

extern void grd_free(void *_ptr){
	struct __regs__ *buff;
	struct __regs__ *prev;

	buff = reg;
	if(buff && buff->_ptr == _ptr){
		reg = reg->next;
		mem_unreg(buff->_ptr, buff->size);
		mem_unreg(buff, sizeof(struct __regs__));

		return;
	}

	prev = buff;
	buff = buff->next;
		
	while(buff){
		if(buff->_ptr == _ptr){
			prev->next = buff->next;
			mem_unreg(buff->_ptr, buff->size);
			mem_unreg(buff, sizeof(struct __regs__));

			return;
		}

		prev = buff;
		buff = buff->next;
	}

	return;
}

void *mem_reg(size_t size){
	void *_ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	return _ptr == MAP_FAILED ? 0 : _ptr;
}
short mem_unreg(void *_ptr, size_t size){
	short res = munmap(_ptr, size);
	return res == -1 ? res : 0;
}

void initialize mem_init(){
	reg = NULL;
}

void finalize mem_final(){
	while(reg){
		grd_free(reg->_ptr);
	}
}
