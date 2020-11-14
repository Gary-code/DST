#include"scp_t.h"

scp_t* scp_create(size_t size)
{
	//���ʼ��������һ��scp_t
	scp_t* SCP = (scp_t*)malloc(sizeof(scp_t));

	//���ݴ�С�����������ʵ��ڴ�ռ�
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

	//�ṹ�����ݵĳ�ʼ��
	SCP->next = NULL;
	SCP->large = (scp_big_t*)malloc(sizeof(scp_big_t));
	SCP->large->buf = NULL;
	SCP->large->next = NULL;
	SCP->current = SCP;

	return SCP;
}

void scp_destroy(scp_t* s)
{
	//free�����ݿ�
	scp_big_t* tag = s->large;
	while (tag != NULL && tag->next != NULL)
	{
		free(tag->buf);
		scp_big_t* P_tag = tag;
		tag = tag->next;
		free(P_tag);
	}
	if (tag != NULL)free(tag);

	//freeС���ݿ� 
	scp_t* now = s->next;
	while (now!=NULL && now->next != NULL) 
	{
		scp_t* t = now;
		now = now->next;
		free(t);
	}
	free(now);

	//�õݹ�ķ�ʽ�ͷ���һ���ڴ��
	if (s->next != NULL)	scp_destroy(s->next);

	//�ͷ��ڴ�ؽṹ�� 
	free(s);
}

void* scp_alloc(scp_t* pool, size_t size)
{
	if (size <= SCP_MAX_SIZE)		return scp_small_alloc(pool, size);
	else 						return scp_big_alloc(pool, size);
}

static void* scp_small_alloc(scp_t* pool, size_t size)
{
	//ȡ���й�����
	scp_t	*now	=pool->current;
	char	*last	=now->dt.last;
	char	*end	=now->dt.end;

	//�ж��Ƿ���Ҫ�½�block
	if (last + size > end)	return scp_block_alloc(pool, size);
	else
	{
		now->dt.last = last + size;
		return last;
	}
}

static void* scp_block_alloc(scp_t* pool, size_t size)
{
	//����һ���µ����ݿ�
	pool->current->next = scp_create(size);
	//�����ƶ�һλ
	pool->current = pool->current->next;

	return scp_small_alloc(pool, size);
}

static void* scp_big_alloc(scp_t* pool, size_t size)
{
	//����һ����ʱ�Ĵ����ݿ�ָ��
	scp_big_t* now = pool->large;

	//�ҵ�large�������һ�����ݿ�
	while (now != NULL && now->next != NULL)
	{
		now = now->next;
	}

	//���ݳ�ʼ��
	now->next = (scp_big_t*)malloc(sizeof(scp_big_t));
	now = now->next;
	now->buf = (char*)malloc(size);
	now->next = NULL;
	return now->buf;
}