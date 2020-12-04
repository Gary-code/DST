/******************************************************************
## �ļ���:scp_open_hash.h
## Copyright (c) 2020-2028 ��֤�Ƽ��ɷݹ�˾�з�����
## �� ��:0.0.0.1
## ������:
## ����/ʱ��:
## �� ��:
	1�����ÿ�����ַ��������ϣ��͹�ϣ������
##�����ϵ:

---------------------------���¼�¼-----------------------------
## �޸���:
## ����/ʱ��:
## �� ��:
******************************************************************/


#include "scp_open_hash.h"

/*
n����ϣͰԪ����Ŀ
size:Ԫ�س���
key_size: key�ĳ���

����scp_get_num_shift���hash->size����
����scp_get_max_prime����������

*/
scp_open_hash_t* scp_open_hash_init(uint32_t n, uint32_t size, uint32_t key_size,
	scp_open_hash_func_pt handler, scp_open_hash_find_pt find,
	scp_open_hash_add_pt add, scp_open_hash_compare_pt compare)
{
	scp_open_hash_t* hash;

	hash = (scp_open_hash_t*)calloc(1, sizeof(scp_open_hash_t));
	if (!hash)
	{
		return NULL;
	}
	//ʵ�ִ���]
	hash->n = n;
	hash->size = size;
	hash->key_size = key_size;
	//hash->size_shift=
	hash->handler = handler;
	hash->find_handler = find;
	hash->add_handler = add;
	hash->compare_handler = compare;
	hash->size = scp_get_num_shift(n);
	hash->max_prime = scp_get_max_prime(n);
	hash->bucket = (u_char*)calloc(1024 + n ,size);

	if (!hash->bucket)
	{
		return NULL;
	}

	return hash;

}

//���һص�����������compare_handler�Ƚ�key
void* scp_open_hash_find_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key)
{
	uint32_t  		 	    n = hash->handler(hash, key);
	scp_open_hash_item_t* next;
	scp_open_hash_item_t* item = (scp_open_hash_item_t*)(hash->bucket + n * hash->size);

	//ʵ�ִ���	
	while (item)
	{
		if (hash->compare_handler(hash, key, item->key) == 0)return item;
		item = item->next;
	}
	return NULL;

}

//��hash��������һ��Ԫ�أ�����hash->handler����±�
void* scp_open_hash_add_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key, scp_open_hash_value_pointer_t value)
{
	uint32_t 				n = hash->handler(hash, key);
	scp_open_hash_item_t* next, * prev;
	int                     num = 0;

	next = (scp_open_hash_item_t*)(hash->bucket + n * hash->size);

	if (!next->value)
	{
		//ʵ�ִ���
		next->value = value;
		memcpy(next->key, key, hash->key_size);
		return next;
	}
	else
	{
		//ʵ�ִ���
		while (next)
		{
			if(hash->compare_handler)
		}
	}

}


//�����2-n�ڵ��������
uint32_t scp_get_max_prime(uint32_t n)
{
	uint32_t i, j;

	for (i = n; i != 1; i--)
	{
		for (j = 2; j < i; j++)
		{
			if ((i % j) == 0)
			{
				break;
			}
		}
		if (j >= i)
		{
			return i;
		}
	}
	return 0;
}


void scp_open_hash_destroy(scp_open_hash_t* hash)
{
	delete hash->bucket;
}

