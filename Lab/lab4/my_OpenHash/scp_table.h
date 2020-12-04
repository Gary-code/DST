/******************************************************************
## �ļ���:scp_table.h
## Copyright (c) 2018-2028 ��֤�Ƽ��ɷݹ�˾�з�����
## �� ��:0.0.0.1
## ������:
## ����/ʱ��:
## �� ��:����һ���ڴ����ݽṹ��������ʽ������������������Ӧ�����ڴ�֧�ֶ�̬����
---------------------------���¼�¼-----------------------------
## �޸���:��ѩ
## ����/ʱ��:
## �� ��:
******************************************************************/

#ifndef _SCP_TABLE_H_INCLUDED_
#define _SCP_TABLE_H_INCLUDED_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include"scp_open_hash.h"

#define SCP_TABLE_INDEX_NUM         4

//ָ����빫ʽ
#define scp_align_ptr(p, a)                                                   \
    (u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#define SCP_ALIGNMENT   sizeof(unsigned long)    /* platform word */

enum scp_index_type_t
{
	KUT_HASH = 0,
	KUT_SKIPLIST, //��Ծ��
	KUT_RWTREE,   //�����
	KUT_BTREE,    //B��
	KUT_TTREE,    //T��
	KUT_AVLTREE   //AVL��
};

typedef void* scp_index_handle; //����ָ��
typedef scp_table_s   scp_table_t; //��

//���庯��ԭ��
typedef void* (*scp_table_add_pt)(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key, void* buf, int len);
typedef void* (*scp_table_get_pt)(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key);

#pragma pack(push, 4)

struct scp_table_item_t
{
	u_char* start;  //begin
	u_char* last;   //items�е�һ������
	u_char* end;    //itmes�е����һ�����ʹ�ã�ֻ���ж�
	scp_table_item_t* next;   //��һ��item
	scp_table_item_t* prev;   //��һ��item
	void* data;   //������table
};

typedef struct
{
	scp_table_item_t* head[2];   //scp_bpu_pool_item_t array, Ԥ�ȷ�2��
	scp_table_item_t* last;      //��ǰ����ʹ�õ�item;
	scp_table_item_t* end;       //���һ��item;��last==endʱ,�ٷ���1��
	uint32_t            		n;         //item��Ԫ�ظ���
	uint32_t            		size;      //ÿ��Ԫ�ؿռ�
	scp_index_handle            sindex[SCP_TABLE_INDEX_NUM];   //����ָ������
	short                       sindex_used_num; //�Ѿ�������������
	scp_table_add_pt            add;
	scp_table_get_pt            get;
} scp_table_s;


#pragma pack(pop)

scp_table_t* scp_table_create(uint32_t n, uint32_t size);
void scp_table_destroy(scp_table_t* table);

void scp_table_set_handler(scp_table_t* table, scp_table_add_pt add_handler, scp_table_get_pt get_handler);

void* scp_table_add_handler(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key, void* buf, int len);

void* scp_table_get_handler(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key);

int scp_table_add_index(scp_table_t* table, scp_index_handle pindex);

//����һ���µ�item�������뵽������
inline void scp_table_alloc_item(scp_table_t* table, scp_table_item_t** item)
{
	if ((*item)->last == (*item)->end)
	{
		*item = table->last->next;
		if (table->last == table->end)
		{
			scp_cache_item_create(table);
		}
		*item = table->last;
	}

}

#endif
