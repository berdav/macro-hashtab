/*****************************************************************************/
/* Trebucket list macro hash table implementation.                           */
/*                                                                           */
/* Copyright (C) 2015, 2016                                                  */
/* Davide Berardi                                                            */
/*                                                                           */
/* This program is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU General Public License               */
/* as published by the Free Software Foundation; either version 2            */
/* of the License, or any later version.                                     */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*                                                                           */
/* You should have received a copy of the GNU General Public License         */
/* along with this program; if not, write to the Free Software               */
/* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,*/
/* USA.                                                                      */
/*****************************************************************************/

/* TODO: other types hashing */
#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <stdlib.h>
#include <string.h>

#define HASH_SIZE_DEFAULT 4096
struct hasher {
	int key;
	void *values;
};

struct bucket {
	int size;
	int alloc_size;
	struct hasher *hash;
};

struct {
	int size;
	struct bucket *inner;
} typedef hashtable;

#define HASH_INIT(handle, hsize) ({\
	handle.size=hsize;\
	handle.inner = (struct bucket *)malloc(sizeof(struct bucket) * hsize);\
	if (handle.inner)\
		memset(handle.inner, 0, sizeof(struct bucket) * hsize);\
})

#define HASH_FINI(handle) ({if(handle.inner) free(handle.inner); handle.size=-1;})

#define HASH(handle, skey, rvalue) ({\
	int __i;\
	int __newval = -1;\
	int __oval= skey % handle.size;\
	for (__i = 0; __i < handle.inner[__oval].size; __i++)\
		if (handle.inner[__oval].hash[__i].key == skey) {\
			__newval = __i;\
			break;\
		}\
	if (__newval == -1){\
		handle.inner[__oval].size += 1;\
		if (handle.inner[__oval].size > handle.inner[__oval].alloc_size) {\
			if (handle.inner[__oval].alloc_size)\
				handle.inner[__oval].alloc_size *= 4;\
			else\
				handle.inner[__oval].alloc_size = 1;\
			handle.inner[__oval].hash = (struct hasher *)realloc(handle.inner[__oval].hash, handle.inner[__oval].alloc_size * (sizeof (struct hasher)));\
		}\
		__newval = handle.inner[__oval].size-1;\
		handle.inner[__oval].hash[__newval].values = malloc(sizeof(void*));\
	}\
	handle.inner[__oval].hash[__newval].key = skey;\
	handle.inner[__oval].hash[__newval].values = (void *) rvalue;\
})

#define GET_HASH(handle, skey, except) ({\
	int __i;\
	int __oval = 0;\
	int __newval = -1;\
	if (!handle.size) {\
		__newval = -1;\
	} else {\
		__oval=skey % handle.size;\
		for (__i = 0; __i < handle.inner[__oval].size; ++__i) {\
			if (handle.inner[__oval].hash[__i].values &&\
				(handle.inner[__oval].hash[__i].key) == skey) {\
				__newval = __i;\
				break;\
			}\
		}\
	}\
	(__newval != -1? (handle.inner[__oval].hash[__newval].values):except);\
})

#endif /* _HASHTABLE_H */
