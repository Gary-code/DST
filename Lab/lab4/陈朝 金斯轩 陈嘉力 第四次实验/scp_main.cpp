/******************************************************************
## 文件名:scp_main.cpp
## Copyright (c) 2018-2028 金证科技股份公司研发中心
## 版 本:0.0.0.1
## 创建人:
## 日期/时间:
## 描 述:
---------------------------更新记录-----------------------------
## 修改人:
## 日期/时间:
## 描 述:
******************************************************************/
#ifdef WIN32
#include <windows.h>
#endif

#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <errno.h>

#include "scp_bus.h"
#include "scp_open_hash.h"
#include "scp_table.h"

#define SCP_SZ_STKBD          "00"
#define SCP_SH_STKBD          "10"

#define SCP_HASH_SIZE         8192 //hash size
#define SCP_TABLE_SIZE        16384 //table size
#define SCP_STK_ASSET_SIZE    4096 //bus size

//typedef unsigned int uint32_t;

//随机生成一个区间数 max_range < >=min_range
inline int scp_make_rand(int max_range, int min_range)
{
	return  (int)((double)rand() / (max_range + 1) * (max_range - min_range) + min_range);
}
//生成深圳股票代码
int scp_make_sz_stk()
{
	return scp_make_rand(690000, 600000);
}
//生成上海股票代码
int scp_make_sh_stk()
{
	return scp_make_rand(300999, 1); 
}

//哈希值计算函数
//key:scp_asset_key_t *类型
//根据证券代码和交易市场计算一个值
uint32_t scp_asset_handler(scp_open_hash_t *hash, scp_open_hash_key_pointer_t key)
{
	//实现代码
	scp_asset_key_t* k = (scp_asset_key_t*)key;
	uint32_t hash_value = k->istockcode * 100 + (k->szstkbd[0] - '0') * 10 + (k->szstkbd[1] - '0');
	return scp_open_hash_handler(hash, hash_value);
}
//key比较函数,返回值：0表示相等。
int scp_asset_compare_handler(scp_open_hash_t *hash, scp_open_hash_key_pointer_t k1, scp_open_hash_key_pointer_t k2)
{
	if (((scp_asset_key_t *)k1)->istockcode == ((scp_asset_key_t *)k2)->istockcode && 
		memcmp(((scp_asset_key_t *)k1)->szstkbd, ((scp_asset_key_t *)k2)->szstkbd, 2) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int main(int argc, char *argv[])
{
	int              	   i;
	int              	   k;
	scp_table_t 	      *table;
	scp_open_hash_t       *hash;
	scp_stk_asset_t        asset;
	scp_asset_key_t        key;
	scp_stk_asset_t       *item;

	//创建table	
	table = scp_table_create(SCP_TABLE_SIZE, sizeof(scp_stk_asset_t));
	if (!table)
	{
		printf("scp_table_create failed, errno=%d\n", errno);
		exit(0);
	}

	//设置回调处理函数
	scp_table_set_handler(table, scp_table_add_handler, scp_table_get_handler);
	
	//创建hash
	hash = scp_open_hash_init(SCP_HASH_SIZE, sizeof(scp_asset_hash_item_t),sizeof(scp_asset_key_t), scp_asset_handler,
		scp_open_hash_find_handler, scp_open_hash_add_handler, scp_asset_compare_handler);
	if (!hash)
	{
		printf("scp_table_create failed, errno=%d\n", errno);
		exit(0);
	}

	//加索引到table中
	scp_table_add_index(table, hash);

	//加数据到table中
	srand(time(NULL));
	for (i = 0; i < SCP_STK_ASSET_SIZE; i++)
	{
		k = i % 2;
		asset.istockcode = k ? scp_make_sh_stk() : scp_make_sz_stk();
		if (k) 
		{
			asset.istockcode = scp_make_sh_stk();
			strcpy(asset.szstkbd, SCP_SH_STKBD);
		}
		else
		{
			asset.istockcode = scp_make_sz_stk();
			strcpy(asset.szstkbd, SCP_SZ_STKBD);
		}

		key.istockcode = asset.istockcode;
		strcpy(key.szstkbd, asset.szstkbd);
		//printf("insert:%06d  %s\n", asset.istockcode, key.szstkbd);
		item =(scp_stk_asset_t*) table->add(table, hash, &key, &asset, sizeof(scp_stk_asset_t));
		if (!item)
		{
			goto _ERROR;
		}
	}

	//输入一个证券代码和市场，在table中查找
	key.istockcode = 600446;
	strcpy(key.szstkbd, "10");
	item = (scp_stk_asset_t*)table->get(table, hash, &key);
	if (item)
	{
		printf("stockcode:%d, szstkbd=%s\n", item->istockcode, item->szstkbd);
	}
	else
	{
		printf("do not find\n");
	}


	
_ERROR:
	//销毁hash
	scp_open_hash_destroy(hash);
	//销毁table
	scp_table_destroy(table);
	exit(0);
}
