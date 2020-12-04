/******************************************************************
## 文件名:scp_table.cpp
## Copyright (c) 2018-2028 金证科技股份公司研发中心
## 版 本:0.0.0.1
## 创建人:
## 日期/时间:
## 描 述:定义一个内存数据结构，链表形式，支持动态扩充
---------------------------更新记录-----------------------------
## 修改人:
## 日期/时间:
## 描 述:
******************************************************************/

#include "scp_table.h"

static scp_table_item_t* scp_table_item_create(scp_table_t* table);

/*****************************************************************
## 函数名:scp_table_create
## 输 入:
	uint32_t         n    -- 入参，item内的元素个数
	uint32_t size    size -- 入参，item内每个元素的data空间
## 输 出:无

## 返回值: scp_table_t*  --失败为NULL，否则为table指针
## 功能描述:
	创建一个缓冲区，存放一种类型的数据，具有动态扩充功能，支持增改查，支持构建索引
	预先创建两个item
## 全局变量:

## 上下文:
调用scp_table_item_create创建item

## 算法说明：
##使用约束：

## 作 者:
## 日期/时间:
## 修 改:
## 日期/时间:
## 版本:1.0.0.0
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
## 函数名:scp_table_item_create
## 输 入:
	scp_table_t        *table -- 入参

## 输 出:无

## 返回值: scp_table_t*  --失败为NULL，否则为table指针
## 功能描述:
	在table内创建一个item并初始化
## 全局变量:

## 上下文:
调用 scp_align_ptr对齐item->start

## 算法说明：
##使用约束：

## 作 者:
## 日期/时间:
## 修 改:田雪
## 日期/时间:
## 版本:1.0.0.0
****************************************************************/
static scp_table_item_t* scp_table_item_create(scp_table_t* table)
{
	scp_table_item_t* item;

	//创建item代码初始化并加到链表中
	item = (scp_table_item_t*)calloc(1, sizeof(scp_table_item_t) + table->size * table->n);
	if (!item)
	{
		return NULL;
	}
	//实现剩余代码
	item->start = (u_char*)malloc(table->n * sizeof(u_char));
	item->end = item->start + table->n;
	item->last = item->start;
	item->data = table;
	return item;
}

/*****************************************************************
## 函数名:scp_table_set_handler
## 输 入:
	scp_table_t *table                  --入参，table
	scp_table_add_pt add_handler        --入参，函数指针
	scp_table_get_pt get_handler        --入参，函数指针

## 输 出:无

## 返回值: 无
## 功能描述:
	为table设置table的两个回调函数,add和get
## 全局变量:

## 上下文:

## 算法说明：
##使用约束：

## 作 者:田雪
## 日期/时间:2020-08-05, 19:27
## 修 改:田雪
## 日期/时间:
## 版本:1.0.0.0
****************************************************************/
void scp_table_set_handler(scp_table_t* table, scp_table_add_pt add_handler, scp_table_get_pt get_handler)
{
	table->add = add_handler;
	table->get = get_handler;
}

/*****************************************************************
## 函数名:scp_table_add_handler
## 输 入:
	scp_table_t            	    *table  --入参,table
	scp_index_handle             sindex --入参,索引
	scp_open_hash_key_pointer_t  key --入参,索引使用的关键字
	void               		    *buf --入参,数据,buf==NULL,只返回指针，不复制;否则复制，并返回指针
	int                          len --入参,数据长度
## 输 出:无

## 返回值: 返回一个数据指针
## 功能描述:
	是一个table的回调函数，添加一条数据,同时添加到索引表中
## 全局变量:
## 上下文:
   调用索引的add_handler
   使用最后一个item时要调用scp_table_alloc_item再分配一个item

## 算法说明：
##使用约束：

## 作 者:
## 日期/时间:
## 修 改:
## 日期/时间:
## 描述：

## 版本:1.0.0.0
****************************************************************/
void* scp_table_add_handler(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key, void* buf, int len)
{
	scp_table_item_t* item = table->last;
	u_char* data;
	//实现代码
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
## 函数名:scp_table_add_index
## 输 入:
	scp_table_t      *table     --入参，table
	scp_index_handle  pindex    --入参，索引指针
## 输 出:无

## 返回值: -1表示失败，否则为索引下标
## 功能描述:
	添加一个索引
## 全局变量:
## 上下文:

## 算法说明：
##使用约束：
## 作 者:
## 日期/时间:
## 修 改:
## 日期/时间:
## 描述：
## 版本:1.0.0.0
****************************************************************/
int scp_table_add_index(scp_table_t* table, scp_index_handle pindex)
{
	//实现代码	
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
## 函数名:scp_table_get_handler
## 输 入:
	scp_table_t                     *table    --入参
	scp_index_handle                 sindex --入参
	scp_open_hash_key_pointer_t      key    --入参
## 输 出:无

## 返回值: 无
## 功能描述:
	是一个table的回调函数，由数据初始化后者业务调用(股份)
## 全局变量:

## 上下文:
   调用索引的find_handler

## 算法说明：
##使用约束：

## 作 者:
## 日期/时间:
## 修 改:
## 日期/时间:
## 版本:1.0.0.0
****************************************************************/
void* scp_table_get_handler(scp_table_t* table, scp_index_handle sindex, scp_open_hash_key_pointer_t key)
{
	scp_open_hash_item_t* item;
	//实现代码

	
}


void scp_table_destroy(scp_table_t* table)
{
	delete table;
}



