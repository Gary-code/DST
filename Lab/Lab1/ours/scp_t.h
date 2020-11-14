#include<cstring>
#include<cstdio>
#include<cstdlib>
#include<iostream>

#define SCP_MAX_SIZE 4095
#define scp_align_ptr(p, a) (u_char *) (((uint64_t) (p) + ((uint64_t) a -1)) & ~((uint64_t) a -1))

//数据单元
struct scp_info_t
{
	char name[32];
	int age;
	char origin[16];
	char data[2];
};

//大数据块
struct scp_big_t
{
	scp_big_t* next;					//下一个scp_big_t
	void* buf;							//缓冲区指针
};

//小数据块
typedef struct
{
	char* last;							//当前内存池内未使用的区域首地址
	char* end;							//当前内存池内的尾指针
} scp_data_t;

//内存池
struct scp_t
{
	scp_data_t dt;						//内存池的数据块
	scp_t* current;						//内存池链表的最近使用的内存池指针
	scp_big_t* large;					//内存池的大数据块链表的头指针
	scp_t* next;						//下一个内存池指针
};

scp_t* scp_create(size_t size);
void scp_destroy(scp_t* s);
void* scp_alloc(scp_t* pool, size_t size);
static void* scp_small_alloc(scp_t* pool, size_t size);
static void* scp_big_alloc(scp_t* pool, size_t size);
static void* scp_block_alloc(scp_t* pool, size_t size);