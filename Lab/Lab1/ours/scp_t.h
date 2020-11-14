#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<iostream>

#define SCP_MAX_SIZE 4095
#define scp_align_ptr(p, a) (u_char *) (((uint64_t) (p) + ((uint64_t) a -1)) & ~((uint64_t) a -1))

//���ݵ�Ԫ
struct scp_info_t
{
	char name[32];
	int age;
	char origin[16];
	char data[2];
};

//�����ݿ�
struct scp_big_t
{
	scp_big_t* next;					//��һ��scp_big_t
	void* buf;							//������ָ��
};

//С���ݿ�
typedef struct
{
	char* last;							//��ǰ�ڴ����δʹ�õ������׵�ַ
	char* end;							//��ǰ�ڴ���ڵ�βָ��
} scp_data_t;

//�ڴ��
struct scp_t
{
	scp_data_t dt;						//�ڴ�ص����ݿ�
	scp_t* current;						//�ڴ����������ʹ�õ��ڴ��ָ��
	scp_big_t* large;					//�ڴ�صĴ����ݿ������ͷָ��
	scp_t* next;						//��һ���ڴ��ָ��
};

scp_t* scp_create(size_t size);
void scp_destroy(scp_t* s);
void* scp_alloc(scp_t* pool, size_t size);
static void* scp_small_alloc(scp_t* pool, size_t size);
static void* scp_big_alloc(scp_t* pool, size_t size);
static void* scp_block_alloc(scp_t* pool, size_t size);