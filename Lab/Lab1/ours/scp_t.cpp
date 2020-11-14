#include"scp_t.h"

scp_t* scp_create(size_t size)
{
	//与初始化，创建一个scp_t
	scp_t* SCP = (scp_t*)malloc(sizeof(scp_t));

	//根据大小按需求分配合适的内存空间
	if (SCP_MAX_SIZE >= size)
	{
		SCP->dt.last = (char*)malloc(SCP_MAX_SIZE);
		SCP->dt.end = SCP->dt.last + SCP_MAX_SIZE;
	}
	else
	{
		SCP->dt.last = (char*)malloc(size);
		SCP->dt.end = SCP->dt.last + size;
	}

	//结构体数据的初始化
	SCP->next = NULL;
	SCP->large = (scp_big_t*)malloc(sizeof(scp_big_t));
	SCP->large->buf = NULL;
	SCP->large->next = NULL;
	SCP->current = SCP;

	return SCP;
}

void scp_destroy(scp_t* s)
{
	//free大数据块
	scp_big_t* tag = s->large;
	while (tag != NULL && tag->next != NULL)
	{
		free(tag->buf);
		scp_big_t* P_tag = tag;
		tag = tag->next;
		free(P_tag);
	}
	if (tag != NULL)free(tag);

	//free小数据块 
	scp_t* now = s->next;
	while (now!=NULL && now->next != NULL) 
	{
		scp_t* t = now;
		now = now->next;
		free(t);
	}
	free(now);

	//用递归的方式释放下一个内存池
	if (s->next != NULL)	scp_destroy(s->next);

	//释放内存池结构体 
	free(s);
}

void* scp_alloc(scp_t* pool, size_t size)
{
	if (size <= SCP_MAX_SIZE)		return scp_small_alloc(pool, size);
	else 						return scp_big_alloc(pool, size);
}

static void* scp_small_alloc(scp_t* pool, size_t size)
{
	//取出有关数据
	scp_t	*now	=pool->current;
	char	*last	=now->dt.last;
	char	*end	=now->dt.end;

	//判断是否需要新建block
	if (last + size > end)	return scp_block_alloc(pool, size);
	else
	{
		now->dt.last = last + size;
		return last;
	}
}

static void* scp_block_alloc(scp_t* pool, size_t size)
{
	//创建一个新的数据块
	pool->current->next = scp_create(size);
	//往后移动一位
	pool->current = pool->current->next;

	return scp_small_alloc(pool, size);
}

static void* scp_big_alloc(scp_t* pool, size_t size)
{
	//定义一个临时的大数据块指针
	scp_big_t* now = pool->large;

	//找到large链的最后一个数据块
	while (now != NULL && now->next != NULL)
	{
		now = now->next;
	}

	//数据初始化
	now->next = (scp_big_t*)malloc(sizeof(scp_big_t));
	now = now->next;
	now->buf = (char*)malloc(size);
	now->next = NULL;
	return now->buf;
}