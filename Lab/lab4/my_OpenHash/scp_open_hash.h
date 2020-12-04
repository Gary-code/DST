/******************************************************************
## �� ��:
	1�����ÿ�����ַ��������ϣ��͹�ϣ������
##�����ϵ:

---------------------------���¼�¼-----------------------------
## �޸���:
## ����/ʱ��:
## �� ��:
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#ifndef _SCP_OPEN_HASH_H_INCLUDED_
#define _SCP_OPEN_HASH_H_INCLUDED_

#define int64_t long long
#define uint32_t unsigned int

typedef struct scp_open_hash_s  	scp_open_hash_t;
typedef void* scp_open_hash_key_pointer_t;
typedef void* scp_open_hash_value_pointer_t;

//����ԭ�Ͷ���
typedef uint32_t(*scp_open_hash_func_pt) (scp_open_hash_t* hash, scp_open_hash_key_pointer_t key);
typedef void* (*scp_open_hash_find_pt) (scp_open_hash_t* hash, scp_open_hash_key_pointer_t key);
typedef void* (*scp_open_hash_add_pt)(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key, scp_open_hash_value_pointer_t value);
typedef int (*scp_open_hash_compare_pt) (scp_open_hash_t* hash, scp_open_hash_key_pointer_t k1, scp_open_hash_key_pointer_t k2);

typedef unsigned char u_char;

#pragma pack(push, 4)

//�䳤���ݽṹ����Ӧ�䳤��key
struct scp_open_hash_item_t
{
	scp_open_hash_item_t* next;
	void* value;
	u_char         	  	   key[1];     //�ɱ�ؼ���
};

struct scp_open_hash_s
{
	u_char* bucket; 	  		 //����,Ԫ�ظ���=bucket_size+1024(�洢����Ԫ�أ�������̬����)
	uint32_t        	   		 n;     	  		 //��ϣͰԪ�ظ���
	uint32_t               		 size;  	         //ÿ��Ԫ�ؿռ�
	uint32_t                     size_shift;         //�ռ�shift
	uint32_t                     key_size;           //key�ռ��С
	uint32_t        	   		 max_prime;          //�������(<=��ϣͰԪ�ظ���)
	scp_open_hash_func_pt  		 handler;            //��ϣ���㺯��
	scp_open_hash_find_pt        find_handler;        //���Һ���
	scp_open_hash_add_pt         add_handler;         //���Ӻ���
	scp_open_hash_compare_pt     compare_handler;     //key�ȽϺ���
};

#pragma pack(pop)

scp_open_hash_t* scp_open_hash_init(uint32_t n, uint32_t size, uint32_t key_size, scp_open_hash_func_pt handler, scp_open_hash_find_pt find, \
	scp_open_hash_add_pt add, scp_open_hash_compare_pt compare);

void* scp_open_hash_add_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key, scp_open_hash_value_pointer_t value);
void* scp_open_hash_find_handler(scp_open_hash_t* hash, scp_open_hash_key_pointer_t key);
void scp_open_hash_destroy(scp_open_hash_t* hash);
uint32_t scp_get_max_prime(uint32_t n);


inline uint32_t scp_open_hash_handler(scp_open_hash_t* hash, uint32_t key)
{
	return key % hash->max_prime;
}

inline uint32_t scp_get_num_shift(uint32_t n)
{
	uint32_t i;
	uint32_t m= 0;

	for (i = n; i >>= 1; m++)
	{
		/* void */
	}
	return m;
}

#endif /* _SCP_OPEN_HASH_H_INCLUDED_ */
