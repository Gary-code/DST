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

	//创建索引
	for (i = 0; i < index_num; i++)
	{
		item = scp_init_htindex_item(bpu, i);
		if (!item)
		{
			return -1;
		}
		else if (bpu->list)
		{
			//加入索引链表
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

	//创建订单数据
	rv = scp_init_order(bpu);
	if (rv)
	{
		return -1;
	}
	return 0;
}

/**********************************************************************************************
创建并初始化索引，预先按照公式计算出每个item中的每个scp_htindex_data_t的ordersn的值并填充
index：0开始的item序号
***********************************************************************************************/
scp_htindex_item_t *scp_init_htindex_item(scp_bpu_t *bpu, int index)			//index：索引空间编号
{
	scp_htindex_item_t *item = (scp_htindex_item_t *)malloc(sizeof(scp_htindex_item_t));
	item->index = index;
	item->start = (scp_htindex_data_t *)malloc(sizeof(scp_htindex_data_t) << 10);
	item->end = item->start + SCP_INDEX_ORDERSN_NUM;
	item->last = item->start;

	int ans = index *SCP_INDEX_ORDERSN_NUM*SCP_BPU_NUM + bpu->index *SCP_BPU_NUM;				//当前BPU对应的当前索引空间的第一个指针的订单号
	int ordersn1=0;
	for (scp_htindex_data_t*it = item->start;it != item->end;it++, ordersn1++)
	{
		it->ordersn = ans + ordersn1;
	}

	return item;
}

/**********************************************************************************************
创建并初始化每个bpu的订单数组，按照计算公式填充每个订单(scp_stk_order_t)的ordersn，并把订单指针赋值给对应的索引项order
***********************************************************************************************/
int scp_init_order(scp_bpu_t *bpu)
{
	int rv;

	scp_stk_order_t *order = (scp_stk_order_t *)calloc(bpu->order_num * bpu->index_num, sizeof(scp_stk_order_t));	//创建订单数组
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
在创建order_array时，给每个元素分配ordersn，把每项的指针加到索引表，scp_htindex_data_t内的order = order
返回ordersn，-1失败
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
//按照给定的订单号取得该订单号的订单数据指针，按照公式计算。
//第一步：计算所属的BPU
//第二部：计算获得该BPU下的item的顺序号，通过链表操作获得item
//第三步：计算在item中的偏移值
//第四步：获得对应ordersn的order并返回
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


