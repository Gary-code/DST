/******************************************************************
## �ļ���:scp_main.cpp
## Copyright (c) 2018-2028 ��֤�Ƽ��ɷݹ�˾�з�����
## �� ��:0.0.0.1
## ������:
## ����/ʱ��:
## �� ��:
---------------------------���¼�¼-----------------------------
## �޸���:
## ����/ʱ��:
## �� ��:
******************************************************************/
#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <errno.h>


#include "scp_bus.h"
#include "scp_open_hash.h"
#include "scp_table.h"

#define int64_t long long
#define uint32_t unsigned int

#define SCP_SZ_STKBD          "00"
#define SCP_SH_STKBD          "10"

#define SCP_HASH_SIZE         8192 //hash size
#define SCP_TABLE_SIZE        16384 //table size
#define SCP_STK_ASSET_SIZE    4096 //bus size

//�������һ�������� max_range < >=min_range
inline int scp_make_rand(int max_range, int min_range)
{
	return (int)((double)rand() / (max_range + 1) * (max_range - min_range) + min_range);
}
//�������ڹ�Ʊ����
int scp_make_sz_stk()
{
	return scp_make_rand(690000, 600000);
}
//�����Ϻ���Ʊ����
int scp_make_sh_stk()
{
	return scp_make_rand(300999, 1);
}

//��ϣֵ���㺯��
//key:scp_asset_key_t *����
//����֤ȯ����ͽ����г�����һ��ֵ
uint32_t scp_asset_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key)
{
	//ʵ�ִ���
}
//key�ȽϺ���,����ֵ��0��ʾ��ȡ�
int scp_asset_compare_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t k1, scp_open_hash_key_pointer_t k2)
{
	if ((scp_asset_key_t*)k1->istockcode == (scp_asset_key_t*)k2->istockcode &&
		memcmp((scp_asset_key_t*)k1->szstkbd, (scp_asset_key_t*)k2->szstkbd, 2) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int main(int argc, char* argv[])
{
	int              	   i;
	int              	   k;
	scp_table_t* table;
	scp_open_hash_t* hash;
	scp_stk_asset_t        asset;
	scp_asset_key_t        key;
	scp_stk_asset_t* item;

	//����table	
	table = scp_table_create(SCP_TABLE_SIZE, sizeof(scp_stk_asset_t));
	if (!table)
	{
		printf("scp_table_create failed, errno=%d\n", errno);
		exit(0);
	}

	//���ûص�������
	scp_table_set_handler(table, scp_table_add_handler, scp_table_get_handler);

	//����hash
	hash = scp_open_hash_init(SCP_HASH_SIZE, sizeof(scp_asset_hash_item_t), scp_asset_handler, \
		scp_open_hash_find_handler, scp_open_hash_add_handler, scp_asset_compare_handler);
	if (!hash)
	{
		printf("scp_table_create failed, errno=%d\n", errno);
		exit(0);
	}

	//��������table��
	scp_table_add_index(table, hash);

	//�����ݵ�table��
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
		item = table->add(table, hash, &key, &asset, sizeof(scp_stk_asset_t));
		if (!item)
		{
			goto ERROR;
		}
	}

	//����һ��֤ȯ������г�����table�в���
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

ERROR:



	//����hash

	//����table

	exit(0);

}
