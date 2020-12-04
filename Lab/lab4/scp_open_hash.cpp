/******************************************************************
## 文件名:scp_open_hash.h
## Copyright (c) 2020-2028 金证科技股份公司研发中心
## 版 本:0.0.0.1
## 创建人:
## 日期/时间:
## 描 述:
	1、采用开链地址法创建哈希表和哈希函数。
##依存关系:

---------------------------更新记录-----------------------------
## 修改人:
## 日期/时间:
## 描 述:
******************************************************************/


#include <scp_open_hash.h>

/*
n：哈希桶元素数目
size:元素长度
key_size: key的长度

调用scp_get_num_shift获得hash->size的幂
调用scp_get_max_prime获得最大素数

*/
scp_open_hash_t *scp_open_hash_init(uint32_t n, uint32_t size, uint32_t key_size, 
											scp_open_hash_func_pt handler, scp_open_hash_find_pt find, 
                                            scp_open_hash_add_pt add, scp_open_hash_compare_pt compare)
{
	scp_open_hash_t  *hash;
	
	hash = (scp_open_hash_t *)calloc(1, sizeof(scp_open_hash_t));
	if (!hash)
	{
		return NULL;
	}
	//实现代码
	if (!hash->bucket)
	{
		return NULL;
	}

	return hash;
	
}

//查找回调函数，调用compare_handler比较key
void *scp_open_hash_find_handler(scp_open_hash_t *hash, scp_open_hash_key_pointer_t key)
{
	uint32_t  		 	    n = hash->handler(hash, key);
	scp_open_hash_item_t   *next;
	scp_open_hash_item_t   *item = (scp_open_hash_item_t *)(hash->bucket + n * hash->size);
	
	//实现代码	
	return NULL;
	
}

//在hash表中增加一个元素，调用hash->handler获得下标
void *scp_open_hash_add_handler(scp_open_hash_t *hash, scp_open_hash_key_pointer_t key, scp_open_hash_value_pointer_t value)
{
	uint32_t 				n = hash->handler(hash, key);
	scp_open_hash_item_t   *next, *prev;
	int                     num = 0;

	next = (scp_open_hash_item_t *)(hash->bucket + n * hash->size);
	
	if (!next->value)
	{
		//实现代码	
		return next;
	}
	else
	{
		//实现代码	

		return next;
		
	}
	
}


//获得在2-n内的最大素数
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


void scp_open_hash_destroy(scp_open_hash_t *hash)
{
	//清除hash
}

