/******************************************************************
## 文件名:scp_table.h
## Copyright (c) 2018-2028 金证科技股份公司研发中心
## 版 本:0.0.0.1
## 创建人:
## 日期/时间:
## 描 述:定义一个内存数据结构，链表形式，无索引，如需索引应另建，内存支持动态扩充
---------------------------更新记录-----------------------------
## 修改人:田雪
## 日期/时间:
## 描 述:
******************************************************************/

#ifndef _SCP_TABLE_H_INCLUDED_
#define _SCP_TABLE_H_INCLUDED_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "scp_open_hash.h"

#define SCP_TABLE_INDEX_NUM         4
typedef unsigned int uint32_t;
typedef char u_char;
//指针对齐公式
#define scp_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))
    
#define SCP_ALIGNMENT   sizeof(unsigned long)    /* platform word */

enum scp_index_type_t
{
	KUT_HASH = 0, 
	KUT_SKIPLIST, //跳跃表
	KUT_RWTREE,   //红黑树
	KUT_BTREE,    //B树
	KUT_TTREE,    //T树
	KUT_AVLTREE   //AVL树
};
struct scp_table_s;
typedef void *scp_index_handle; //索引指针
typedef scp_table_s   scp_table_t; //表

//定义函数原型
typedef void * (*scp_table_add_pt)(scp_table_t *table, scp_index_handle sindex, scp_open_hash_key_pointer_t key, void *buf, int len);
typedef void * (*scp_table_get_pt)(scp_table_t *table, scp_index_handle sindex, scp_open_hash_key_pointer_t key);

#pragma pack(push, 4)

struct scp_table_item_t
{
	u_char       	     *start;  //begin
	u_char               *last;   //items中第一个空闲
	u_char               *end;    //itmes中的最后一项，不能使用，只做判断
	scp_table_item_t     *next;   //下一个item
	scp_table_item_t     *prev;   //上一个item
	void                 *data;   //所属的table
};

struct scp_table_s
{
	scp_table_item_t  		   *head[2];   //scp_bpu_pool_item_t array, 预先分2个
	scp_table_item_t 	   	   *last;      //当前正在使用的item;
	scp_table_item_t           *end;       //最后一个item;当last==end时,再分配1个
	uint32_t            		n;         //item内元素个数
	uint32_t            		size;      //每个元素空间
	scp_index_handle            sindex[SCP_TABLE_INDEX_NUM];   //索引指针数组
	short                       sindex_used_num; //已经分配索引个数
	scp_table_add_pt            add;
	scp_table_get_pt            get;
} ;


#pragma pack(pop)

scp_table_t *scp_table_create(uint32_t n, uint32_t size);

void scp_table_destroy(scp_table_t *table);

void scp_table_set_handler(scp_table_t *table, scp_table_add_pt add_handler, scp_table_get_pt get_handler);

void *scp_table_add_handler(scp_table_t *table, scp_index_handle sindex, scp_open_hash_key_pointer_t key, void *buf, int len);

void *scp_table_get_handler(scp_table_t *table, scp_index_handle sindex, scp_open_hash_key_pointer_t key);

int scp_table_add_index(scp_table_t *table, scp_index_handle pindex);

#endif
