/******************************************************************
## �ļ���:scp_table.cpp
## Copyright (c) 2018-2028 ��֤�Ƽ��ɷݹ�˾�з�����
## �� ��:0.0.0.1
## ������:
## ����/ʱ��:
## �� ��:����һ���ڴ����ݽṹ��������ʽ��֧�ֶ�̬����
---------------------------���¼�¼-----------------------------
## �޸���:
## ����/ʱ��:
## �� ��:
******************************************************************/

#include "scp_table.h"

static scp_table_item_t* scp_table_item_create(scp_table_t* table);

/*****************************************************************
## ������:scp_table_create
## �� ��:
	uint32_t         n    -- ��Σ�item�ڵ�Ԫ�ظ���
	uint32_t size    size -- ��Σ�item��ÿ��Ԫ�ص�data�ռ�
## �� ��:��

## ����ֵ: scp_table_t*  --ʧ��ΪNULL������Ϊtableָ��
## ��������:
	����һ�������������һ�����͵����ݣ����ж�̬���书�ܣ�֧�����Ĳ飬֧�ֹ�������
	Ԥ�ȴ�������item
## ȫ�ֱ���:

## ������:
����scp_table_item_create����item

## �㷨˵����
##ʹ��Լ����

## �� ��:
## ����/ʱ��:
## �� ��:
## ����/ʱ��:
## �汾:1.0.0.0
****************************************************************/
scp_table_t* scp_table_create(uint32_t n, uint32_t size)
{
	scp_table_t* table;
	int          	   i;
	scp_table_item_t* item;

	table = (scp_table_t*)calloc(1, sizeof(scp_table_t));
	if (!table)
	{
		return NULL;
	}
	table->size = size;
	table->n = n;

	for (i = 0; i < 2; i++)
	{
		item = scp_table_item_create(table);
		if (!item)
		{
			return NULL;
		}
		table->head[i] = item;
	}
	//next?
	table->last = table->head[0];

	return table;
}

/*****************************************************************
## ������:scp_table_item_create
## �� ��:
	scp_table_t        *table -- ���

## �� ��:��

## ����ֵ: scp_table_t*  --ʧ��ΪNULL������Ϊtableָ��
## ��������:
	��table�ڴ���һ��item����ʼ��
## ȫ�ֱ���:

## ������:
���� scp_align_ptr����item->start

## �㷨˵����
##ʹ��Լ����

## �� ��:
## ����/ʱ��:
## �� ��:��ѩ
## ����/ʱ��:
## �汾:1.0.0.0
****************************************************************/
static scp_table_item_t* scp_table_item_create(scp_table_t* table)
{
	scp_table_item_t* item;

	//����item�����ʼ�����ӵ�������
	item = (scp_table_item_t*)calloc(1, sizeof(scp_table_item_t) + table->size * table->n);
	if (!item)
	{
		return NULL;
	}
	//ʵ��ʣ�����
	item->start = (u_char*)malloc(table->n * sizeof(u_char));
	item->end = item->start + table->n;
	item->last = item->start;
	item->data = table;
	return item;
}

/*****************************************************************
## ������:scp_table_set_handler
## �� ��:
	scp_table_t *table                  --��Σ�table
	scp_table_add_pt add_handler        --��Σ�����ָ��
	scp_table_get_pt get_handler        --��Σ�����ָ��

## �� ��:��

## ����ֵ: ��
## ��������:
	Ϊtable����table�������ص�����,add��get
## ȫ�ֱ���:

## ������:

## �㷨˵����
##ʹ��Լ����

## �� ��:��ѩ
## ����/ʱ��:2020-08-05, 19:27
## �� ��:��ѩ
## ����/ʱ��:
## �汾:1.0.0.0
****************************************************************/
void scp_table_set_handler(scp_table_t* table, scp_table_add_pt add_handler, scp_table_get_pt get_handler)
{
	table->add = add_handler;
	table->get = get_handler;
}

/*****************************************************************
## ������:scp_table_add_handler
## �� ��:
	scp_table_t            	    *table  --���,table
	scp_index_handle             sindex --���,����
	scp_open_hash_key_pointer_t  key --���,����ʹ�õĹؼ���
	void               		    *buf --���,����,buf==NULL,ֻ����ָ�룬������;�����ƣ�������ָ��
	int                          len --���,���ݳ���
## �� ��:��

## ����ֵ: ����һ������ָ��
## ��������:
	��һ��table�Ļص����������һ������,ͬʱ��ӵ���������
## ȫ�ֱ���:
## ������:
   ����������add_handler
   ʹ�����һ��itemʱҪ����scp_table_alloc_item�ٷ���һ��item

## �㷨˵����
##ʹ��Լ����

## �� ��:
## ����/ʱ��:
## �� ��:
## ����/ʱ��:
## ������

## �汾:1.0.0.0
****************************************************************/
void* scp_table_add_handler(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key, void* buf, int len)
{
	scp_table_item_t* item = table->last;
	u_char* data;
	//ʵ�ִ���
	if (item == table->end)
	{
		scp_table_alloc_item(table, item);
	}

	if (!buf)
	{
		return NULL;
	}
	else
	{
		table->n++;
		table->size = len;
		item->data = buf;
		table->last = item->last;
	}

	return data;

}

/*****************************************************************
## ������:scp_table_add_index
## �� ��:
	scp_table_t      *table     --��Σ�table
	scp_index_handle  pindex    --��Σ�����ָ��
## �� ��:��

## ����ֵ: -1��ʾʧ�ܣ�����Ϊ�����±�
## ��������:
	���һ������
## ȫ�ֱ���:
## ������:

## �㷨˵����
##ʹ��Լ����
## �� ��:
## ����/ʱ��:
## �� ��:
## ����/ʱ��:
## ������
## �汾:1.0.0.0
****************************************************************/
int scp_table_add_index(scp_table_t* table, scp_index_handle pindex)
{
	//ʵ�ִ���	
	if (table->sindex_used_num < SCP_TABLE_INDEX_NUM)
	{
		table->sindex[table->sindex_used_num] = pindex;
		return table->sindex_used_num++;
	}
	else
	{
		return -1;
	}
}

/*****************************************************************
## ������:scp_table_get_handler
## �� ��:
	scp_table_t                     *table    --���
	scp_index_handle                 sindex --���
	scp_open_hash_key_pointer_t      key    --���
## �� ��:��

## ����ֵ: ��
## ��������:
	��һ��table�Ļص������������ݳ�ʼ������ҵ�����(�ɷ�)
## ȫ�ֱ���:

## ������:
   ����������find_handler

## �㷨˵����
##ʹ��Լ����

## �� ��:
## ����/ʱ��:
## �� ��:
## ����/ʱ��:
## �汾:1.0.0.0
****************************************************************/
void* scp_table_get_handler(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key)
{
	scp_open_hash_item_t* item;
	//ʵ�ִ���

	
}


void scp_table_destroy(scp_table_t* table)
{
	delete table;
}



