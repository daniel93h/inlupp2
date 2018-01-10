#ifndef __lager_h__
#define __lager_h__

typedef struct item item_t;

item_t *make_item(char *name, char *desc, int price, char *shelf_name, int amount);

#endif
