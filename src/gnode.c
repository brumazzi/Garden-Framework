#include <gnode.h>
#include <mem.h>
#include <attrib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

struct pack __gnode_p__{
	void *data;
	struct __gnode_p__ *next;
	struct __gnode_p__ *prev;
};

struct pack __gnode__{
	size_t length;
	struct __gnode_p__ *first;
	struct __gnode_p__ *last;
	struct __gnode_p__ *current;
};

extern gnode *gnode_new(void){
	struct __gnode__ *no = mem_alloc(sizeof(struct __node__));
	no->length = 0;
	no->first = NULL;
	no->last = NULL;
	no->current = NULL;

	return no;
}

extern int gnode_item_insert(gnode *no, void *data, GNODE_DATA_POS dpos, ...){
	gnodep *nod;
	gnodep *buff;
	size_t st_len;

	nod = mem_alloc(sizeof(struct __gnode_p__));

	switch(dpos){
		case GNODE_POSITION_FIRST:
			if(!no->length){
				no->first = nod;
				no->last = nod;
				no->current = nod;
			}else{
				nod->next = no->first;
				no->first->prev = nod;
				no->first = nod;
			}
			break;
		case GNODE_POSITION_LAST:
			if(!no->length){
				no->first = nod;
				no->last = nod;
				no->current = nod;
			}else{
				nod->prev = no->last;
				no->last->next = nod;
				no->last = nod;
			}
			break;
		case GNODE_POSITION_SET:
			buff = no->first;
			va_list args;
			va_start(args, dpos);
			int position = va_arg(args, int);
			va_end(args);

			while(position--){
				if(!buff){
					mem_unalloc(nod);
					return GNODE_ERR_INVALID_POSITION;
				}
				buff = buff->next;
			}

			if(!no->length){
				no->first = nod;
				no->last = nod;
				no->current = nod;
				break;
			}

			if(!buff){
				nod->prev = no->last;
				no->last->next = nod;
				no->last = nod;
			}else if(!buff->prev){
				nod->next = no->first;
				no->first->prev = nod;
				no->first = nod;
			}else{
				nod->prev = buff->prev;
				nod->next = buff;
				buff->prev->next = nod;
				buff->prev = nod;
			}
			break;
		default:
			mem_unalloc(nod);
			return GNODE_ERR_INVALID_POSITION_TYPE;
	}

	nod->data = data;
	no->length++;
	return GNODE_SUCCESS;
}

extern int gnode_item_remove(gnode *no, GNODE_DATA_POS dpos, ...){
	gnodep *buff;
	if(!no->first)
		return GNODE_ERR_LIST_EMPTY;

	switch(dpos){
		case GNODE_POSITION_CURRENT:
			buff = no->current;
			break;
		case GNODE_POSITION_FIRST:
			buff = no->first;
			break;
		case GNODE_POSITION_LAST:
			buff = no->last;
			break;
		case GNODE_POSITION_SET:
			buff = no->first;
			va_list args;
			va_start(args, dpos);
			int position = va_arg(args, int);
			va_end(args);
			while(position--){
				if(!buff)
					return GNODE_ERR_INVALID_POSITION;
				buff = buff->next;
			}
			break;
		case GNODE_POSITION_NEXT:
			buff = no->current->next;
			break;
		case GNODE_POSITION_PREV:
			buff = no->current->prev;
		default:
			return GNODE_ERR_INVALID_POSITION_TYPE;
	}

	if(!buff)
		return GNODE_ERR_DATA_NOT_FOUND;
	else if(!buff->next && !buff->prev){
		no->first = NULL;
		no->last = NULL;
		no->current = NULL;
	}else if(!buff->next){
		no->last = buff->prev;
		no->last->next = NULL;
		if(no->current == buff)
			no->current = buff->prev;
	}else if(!buff->prev){
		no->first = buff->next;
		no->first->prev = NULL;
		if(no->current == buff)
			no->current = buff->next;
	}else{
		buff->next->prev = buff->prev;
		buff->prev->next = buff->next;
		if(no->current == buff)
			no->current = buff->next;
	}

	mem_unalloc(buff);
	no->length--;

	return GNODE_SUCCESS;
}

extern void *gnode_item_data(gnode *no, GNODE_SG sg, GNODE_DATA_POS dpos, ...){
	va_list args;
	va_start(args, dpos);
	gnodep *buff;
	int position;

	switch(dpos){
		case GNODE_POSITION_CURRENT:
			buff = no->current;
			break;
		case GNODE_POSITION_FIRST:
			buff = no->first;
			break;
		case GNODE_POSITION_LAST:
			buff = no->last;
			break;
		case GNODE_POSITION_SET:
			position = va_arg(args, int);

			buff = no->first;
			while(buff && position--)
				buff = buff->next;

			if(!buff){
				va_end(args);
				return sg ? NULL : GNODE_ERR_INVALID_POSITION;
			}

			break;
		default:
			va_end(args);
			return sg ? NULL : GNODE_ERR_INVALID_POSITION_TYPE;
	}

	if(!sg){
		buff->data = va_arg(args, void *);
	}

	va_end(args);
	return sg ? buff->data : GNODE_SUCCESS;
}

extern void *gnode_item_get(gnode *no, GNODE_DATA_POS dpos, ...){
	va_list args;
	va_start(args, dpos);
	gnodep *buff;
	int position;

	switch(dpos){
		case GNODE_POSITION_CURRENT:
			buff = no->current;
			break;
		case GNODE_POSITION_FIRST:
			buff = no->first;
			break;
		case GNODE_POSITION_LAST:
			buff = no->last;
			break;
		case GNODE_POSITION_SET:
			position = va_arg(args, int);

			buff = no->first;
			while(buff && position--)
				buff = buff->next;

			if(!buff){
				va_end(args);
				return GNODE_ERR_INVALID_POSITION;
			}

			break;
		default:
			va_end(args);
			return GNODE_ERR_INVALID_POSITION_TYPE;
	}

	va_end(args);
	return buff;
}

extern size_t gnode_item_position(gnode *no, GNODE_SG sg, GNODE_DATA_POS dpos, ...){
	gnodep *buff;
	switch(dpos){
		case GNODE_POSITION_NEXT:
			if(no->current->next)
				no->current = no->current->next;
			else
				return GNODE_ERR_END_OF_LIST;
			break;
		case GNODE_POSITION_PREV:
			if(no->current->prev)
				no->current = no->current->prev;
			else
				return GNODE_ERR_BEGIN_OF_LIST;
			break;
		case GNODE_POSITION_FIRST:
			no->current = no->first;
			break;
		case GNODE_POSITION_LAST:
			no->current = no->last;
			break;
		case GNODE_POSITION_SET:
			buff = no->first;
			va_list args;
			va_start(args, dpos);
			int position = va_arg(args, int);
			va_end(args);
			while(position--){
				if(!buff)
					return GNODE_ERR_INVALID_POSITION;
				buff = buff->next;
			}
			break;
		default:
			return GNODE_ERR_INVALID_POSITION_TYPE;
	}

	return GNODE_SUCCESS;
}

extern int gnode_clear(gnode *no){
	if(!no->first)
		return GNODE_ERR_LIST_EMPTY;
	while(!gnode_item_remove(no, GNODE_POSITION_FIRST));

	return GNODE_SUCCESS;
}

extern size_t gnode_size(gnode *no){
	if(!no)
		return GNODE_ERR_LIST_EMPTY;

	return no->length;
}

extern void *gnodep_data(gnodep *no){
	if(!no)
		return NULL;
	return no->data;
}

extern int gnodep_next(gnodep **no){
	if(!*no)
		return GNODE_ERR_DATA_NOT_FOUND;
	(*no) = (*no)->next;

	return GNODE_SUCCESS;
}

extern int gnodep_prev(gnodep **no){
	if(!*no)
		return GNODE_ERR_DATA_NOT_FOUND;
	(*no) = (*no)->prev;

	return GNODE_SUCCESS;
}
