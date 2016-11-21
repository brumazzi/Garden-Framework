#ifndef  __GNODE_H__
#define  __GNODE_H__

#include <stddef.h>

typedef struct __node_p__ nodep;

typedef struct __node__ node;

enum{
	GNODE_SUCCESS,
	GNODE_ERR_INVALID_POSITION,
	GNODE_ERR_INVALID_POSITION_TYPE,
	GNODE_ERR_END_OF_LIST,
	GNODE_ERR_BEGIN_OF_LIST,
	GNODE_ERR_LIST_EMPTY,
	GNODE_ERR_DATA_NOT_FOUND
};

typedef unsigned int GNODE_TYPE;
enum{
	GNODE_TYPE_POINTER = 0,	// insert data pointer in list
	GNODE_TYPE_INTEGER,	// insert data integer in list
	GNODE_TYPE_FLOAT,	// insert data float in list
	GNODE_TYPE_CHAR		// insert data char in list
};

typedef unsigned int GNODE_DATA_POS;
enum{
	GNODE_POSITION_CURRENT = 4,	// used to change value of list
	GNODE_POSITION_NEXT,		// move to next position of list
	GNODE_POSITION_PREV,		// move to previus position of list
	GNODE_POSITION_FIRST,		// insert data in first possition
	GNODE_POSITION_LAST,		// insert data in last possition
	GNODE_POSITION_SET		// insert data in selected possition
};

typedef unsigned int GNODE_SG;
enum{
	GNODE_SET,	// used to set value in existing lists
	GNODE_GET	// used to get value of existing lists
};

gnode *gnode_new(void);
int gnode_item_insert(gnode *, void *, GNODE_DATA_POS, ...);
int gnode_item_remove(gnode *, GNODE_DATA_POS, ...);
void *gnode_item_data(gnode *, GNODE_SG, GNODE_DATA_POS, ...);
void *gnode_item_get(gnode *, GNODE_DATA_POS, ...);
size_t gnode_item_position(gnode *, GNODE_SG, GNODE_DATA_POS, ...);
size_t gnode_size(gnode *);

int gnode_clear(gnode *);

void *gnodep_data(gnodep *);
int gnodep_next(gnodep **);
int gnodep_prev(gnodep **);

#define gnode_push(no, data) gnode_item_insert(no, (void *) data, GNODE_POSITION_FIRST)
#define gnode_append(no, data) gnode_item_insert(no, (void *) data, GNODE_POSITION_LAST)
#define gnode_insert(no, data, index) gnode_item_insert(no, (void *) data, GNODE_POSITION_SET, index)

#define gnode_pop(no) gnode_item_remove(no, GNODE_POSITION_FIRST)
#define gnode_remove(no, index) gnode_item_remove(no, GNODE_POSITION_SET, index)
#define gnode_remove_last(no) gnode_item_remove(no, GNODE_POSITION_LAST)

#define gnode_get_data(no, index, type) (type) gnode_item_data(no, GNODE_GET, GNODE_POSITION_SET, index)
#define gnode_get_data_current(no, type) (type) gnode_item_data(no, GNODE_GET, GNODE_POSITION_CURRENT)
#define gnode_get_data_first(no, type) (type) gnode_item_data(no, GNODE_GET, GNODE_POSITION_FIRST)
#define gnode_get_data_last(no, type) (type) gnode_item_data(no, GNODE_GET, GNODE_POSITION_LAST)

#define gnode_set_data(no, data, index) gnode_item_data(no, GNODE_SET, GNODE_POSITION_SET, index, (void *) data)
#define gnode_set_data_current(no, data) gnode_item_data(no, GNODE_SET, GNODE_POSITION_CURRENT, (void *) data)
#define gnode_set_data_first(no, data) gnode_item_data(no, GNODE_SET, GNODE_POSITION_FIRST, (void *) data)
#define gnode_set_data_last(no, data) gnode_item_data(no, GNODE_SET, GNODE_POSITION_LAST, (void *) data)

#define gnode_get(no, index) gnode_item_data(no, GNODE_POSITION_SET, index)
#define gnode_get_current(no) gnode_item_data(no, GNODE_POSITION_CURRENT)
#define gnode_get_first(no) gnode_item_data(no, GNODE_POSITION_FIRST)
#define gnode_get_last(no) gnode_item_data(no, GNODE_POSITION_LAST)

#define gnode_get_position(no) gnode_item_position(no, GNODE_GET, GNODE_POSITION_CURRENT)
#define gnode_set_position(no, index) gnode_item_position(no, GNODE_SET, GNODE_POSITION_SET, index)
#define gnode_set_position_first(no) gnode_item_position(no, GNODE_SET, GNODE_POSITION_FIRST)
#define gnode_set_position_last(no) gnode_item_position(no, GNODE_SET, GNODE_POSITION_LAST)
#define gnode_set_position_next(no) gnode_item_position(no, GNODE_SET, GNODE_POSITION_NEXT)
#define gnode_set_position_prev(no) gnode_item_position(no, GNODE_SET, GNODE_POSITION_PREV)

#define foreach(no, var) for(gnodep *var = gnode_item_get(no, GNODE_POSITION_FIRST); var != NULL; gnodep_next(&var))
#define foreach_r(no, var) for(gnodep *var = gnode_item_get(no, GNODE_POSITION_LAST); var != NULL; gnodep_prev(&var))

#endif  /*GNODE_H*/
