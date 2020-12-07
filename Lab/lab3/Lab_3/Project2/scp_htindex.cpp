#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "scp_htindex.h"

static scp_bpu_s    *scp_bpus=NULL;

int scp_create_bpus(int num)
{
	int        i, j;
	scp_bpu_t *ap;
	int        rv;

	ap = (scp_bpu_t *)calloc(num, sizeof(scp_bpu_t));
	if (!ap)
	{
		return errno;
	}
	scp_bpus = ap;

	for (i = 0; i < num; i++)
	{
		scp_bpus[i].index = i;
		rv = scp_init_bpu(&scp_bpus[i], SCP_INDEX_ITEM_NUM, SCP_INDEX_ORDERSN_NUM);
		if (rv)
		{
			return rv;
		}
	}
}

int scp_init_bpu(scp_bpu_t *bpu, int index_num, int data_num)
{
	int 					rv;
	int 					i;
	scp_htindex_item_t 	   *item, *p, *r;

	bpu->index_num = index_num;
	bpu->order_num = data_num;

	//��������
	for (i = 0; i < index_num; i++)
	{
		item = scp_init_htindex_item(bpu, i);
		if (!item)
		{
			return -1;
		}
		else if (bpu->list)
		{
			//������������
			r = bpu->list;
			p = bpu->list->next;
			while (p)
			{
				r = p;
				p = p->next;
			}
			r->next = item;
			item->prev = r;
		}
		else
		{
			bpu->list = item;
			item->prev = NULL;
		}
		item->next = NULL;
	}

	bpu->last = bpu->list;

	//������������
	rv = scp_init_order(bpu);
	if (rv)
	{
		return -1;
	}
	return 0;
}

/**********************************************************************************************
��������ʼ��������Ԥ�Ȱ��չ�ʽ�����ÿ��item�е�ÿ��scp_htindex_data_t��ordersn��ֵ�����
index��0��ʼ��item���
***********************************************************************************************/
scp_htindex_item_t *scp_init_htindex_item(scp_bpu_t *bpu, int index)			//index�������ռ���
{
	scp_htindex_item_t *item = (scp_htindex_item_t *)malloc(sizeof(scp_htindex_item_t));
	item->index = index;
	item->start = (scp_htindex_data_t *)malloc(sizeof(scp_htindex_data_t) << 10);
	item->end = item->start + SCP_INDEX_ORDERSN_NUM;
	item->last = item->start;

	int ans = index *SCP_INDEX_ORDERSN_NUM*SCP_BPU_NUM + bpu->index *SCP_BPU_NUM;				//��ǰBPU��Ӧ�ĵ�ǰ�����ռ�ĵ�һ��ָ��Ķ�����
	int ordersn1=0;
	for (scp_htindex_data_t*it = item->start;it != item->end;it++, ordersn1++)
	{
		it->ordersn = ans + ordersn1;
	}

	return item;
}

/**********************************************************************************************
��������ʼ��ÿ��bpu�Ķ������飬���ռ��㹫ʽ���ÿ������(scp_stk_order_t)��ordersn�����Ѷ���ָ�븳ֵ����Ӧ��������order
***********************************************************************************************/
int scp_init_order(scp_bpu_t *bpu)
{
	int rv;

	scp_stk_order_t *order = (scp_stk_order_t *)calloc(bpu->order_num * bpu->index_num, sizeof(scp_stk_order_t));	//������������
	if (!order)
	{
		return -1;
	}
	bpu->order_array = order;
	for (int i = 0; i < bpu->order_num * bpu->index_num; i++)
	{
		rv = scp_put_order(bpu, &bpu->order_array[i]);
		if (rv)
		{
			return -1;
		}
	}
	return 0;
}


/**********************************************************************************************
�ڴ���order_arrayʱ����ÿ��Ԫ�ط���ordersn����ÿ���ָ��ӵ�������scp_htindex_data_t�ڵ�order = order
����ordersn��-1ʧ��
***********************************************************************************************/
int scp_put_order(scp_bpu_t *bpu, scp_stk_order_t *order)
{
	if (!bpu->last)
	{
		return -1;
	}
	if (bpu->last->last <= bpu->last->end)
	{
		bpu->last->last->p1 = order;
		order->ordersn = bpu->last->last->key;
		bpu->last->last++;
		return 0;
	}
	else
	{
		bpu->last = bpu->last->next;
		if (bpu->last && bpu->last->last <= bpu->last->end)
		{
			bpu->last->last->p1 = order;
			order->ordersn = bpu->last->last->key;
			bpu->last->last++;
			return 0;
		}
	}
	return -1;
}

/**********************************************************************************************
//���ո����Ķ�����ȡ�øö����ŵĶ�������ָ�룬���չ�ʽ���㡣
//��һ��������������BPU
//�ڶ����������ø�BPU�µ�item��˳��ţ�ͨ������������item
//��������������item�е�ƫ��ֵ
//���Ĳ�����ö�Ӧordersn��order������
***********************************************************************************************/
scp_stk_order_t *scp_get_by_ordersn(uint32_t ordersn)
{
	int bpu_num = ordersn % (SCP_INDEX_ORDERSN_NUM*SCP_BPU_NUM) / SCP_INDEX_ORDERSN_NUM;
	int item_num = ordersn / (SCP_INDEX_ORDERSN_NUM*SCP_BPU_NUM);
	int pianyi_num = ordersn % (SCP_INDEX_ORDERSN_NUM*SCP_BPU_NUM) % SCP_INDEX_ORDERSN_NUM;

	scp_htindex_item_t *now = (scp_bpus+bpu_num)->list;
	while (now->index != item_num)
	{
		now = now->next;
	}
	scp_htindex_data_t *p = now->start + pianyi_num;
	scp_stk_order_t *ans = p->order;
	return ans;
}


