
/******************************************************************
## 描 述:
    1、采用开链地址法创建哈希表和哈希函数。
##依存关系:

---------------------------更新记录-----------------------------
## 修改人:
## 日期/时间:
## 描 述:
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#ifndef _SCP_OPEN_HASH_H_INCLUDED_
#define _SCP_OPEN_HASH_H_INCLUDED_
struct scp_open_hash_s;
struct scp_open_hash_item_t;
typedef scp_open_hash_s  	scp_open_hash_t;
typedef void* scp_open_hash_key_pointer_t;
typedef void* scp_open_hash_value_pointer_t;
//#define uint32_t unsigned int
typedef char u_char;
typedef unsigned int uint32_t;
//#define u_char char
//函数原型定义
typedef uint32_t(*scp_open_hash_func_pt) (scp_open_hash_t* hash, scp_open_hash_key_pointer_t key);
typedef void* (*scp_open_hash_find_pt) (scp_open_hash_t* hash, scp_open_hash_key_pointer_t key);
typedef void* (*scp_open_hash_add_pt)(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key, scp_open_hash_value_pointer_t value);
typedef int (*scp_open_hash_compare_pt) (scp_open_hash_t* hash, scp_open_hash_key_pointer_t k1, scp_open_hash_key_pointer_t k2);

scp_open_hash_t* scp_open_hash_init(uint32_t n, uint32_t size, uint32_t key_size, scp_open_hash_func_pt handler, scp_open_hash_find_pt find,
    scp_open_hash_add_pt add, scp_open_hash_compare_pt compare);

void* scp_open_hash_add_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key, scp_open_hash_value_pointer_t value);
void* scp_open_hash_find_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key);
void scp_open_hash_destroy(scp_open_hash_t* hash);
uint32_t scp_get_max_prime(uint32_t n);

#pragma pack(push, 4)

//变长数据结构，适应变长的key
struct scp_open_hash_item_t
{
    scp_open_hash_item_t* next;
    void* value;
    u_char         	  	   key[1];     //可变关键字
};

struct scp_open_hash_s
{
    u_char* bucket; 	  		 //数组,元素个数=bucket_size+1024(存储拉链元素，不够则动态扩充)
    uint32_t        	   		 n;     	  		 //哈希桶元素个数
    uint32_t               		 size;  	         //每个元素空间
    uint32_t                     size_shift;         //空间shift
    uint32_t                     key_size;           //key空间大小
    uint32_t        	   		 max_prime;          //最大素数(<=哈希桶元素个数)
    scp_open_hash_func_pt  		 handler;            //哈希计算函数
    scp_open_hash_find_pt        find_handler;        //查找函数
    scp_open_hash_add_pt         add_handler;         //增加函数
    scp_open_hash_compare_pt     compare_handler;     //key比较函数
};

#pragma pack(pop)


inline uint32_t scp_open_hash_handler(scp_open_hash_t* hash, uint32_t key)
{
    return key % hash->max_prime;
}

inline uint32_t scp_get_num_shift(uint32_t n)
{
    uint32_t i;
    uint32_t m = 0;

    for (i = n; i >>= 1; m++)
    {
        /* void */
    }
    return m;
}



#endif /* _SCP_OPEN_HASH_H_INCLUDED_ */
