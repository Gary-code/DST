#pragma once
#ifndef __SCP_HTINDEX_H_
#define __SCP_HTINDEX_H_

#include<cstdint>

#define scp_htindex_data_t scp_htindex_data_s     
#define scp_htindex_item_t scp_htindex_item_s     
#define scp_stk_order_t scp_stk_order_s        
#define scp_bpu_t scp_bpu_s              

#pragma pack(push, 4)
#define key ordersn
#define p1 order
//订单信息
struct scp_stk_order_s
{
	int 	ordersn;	  		// 订单号
	int 	stockcode;          // 证券代码
	int 	stkbiz;            // 证券业务
	int 	stkbizaction;      // 业务行为
	int64_t orderprice;		   // 委托价格
	int64_t orderqty; 		   // 委托数量
};

//订单号索引元素
struct scp_htindex_data_s
{
	uint32_t  			 ordersn; 	//订单号
	scp_stk_order_t      *order;     //订单数据指针
};

//订单号索引缓冲区，存放scp_htindex_data_t类型的数据
struct scp_htindex_item_s
{
	scp_htindex_data_t	    *start; //缓冲区启始地址
	scp_htindex_data_t	    *last;  //缓冲区未使用的ordersn的首地址
	scp_htindex_data_t		*end;   //缓冲区最后一项地址
	int                      index; //缓冲区在链表中的顺序号
	scp_htindex_item_t      *next;  //下一个缓冲区
	scp_htindex_item_t      *prev;  //前一个缓冲区
};

//处理单元，含有订单号索引链表，订单数据
struct scp_bpu_s
{
	scp_htindex_item_t *list;        //订单号索引item的链表
	scp_htindex_item_t *last;		 //当前缓冲区
	int                 index_num;   //索引item个数
	scp_stk_order_t    *order_array; //订单数组
	int                 order_num;   //订单数组元素个数=
	int                 index;       //该BPU在数组的序号(从0开始)
};
#pragma pack(pop)

#define SCP_BPU_NUM           		4					//BPU 的个数
#define SCP_INDEX_ORDERSN_NUM 	    1024				//订单空间的订单号个数
#define SCP_INDEX_ITEM_NUM    		4					//每个BPU的订单空间个数


//创建一个bpu数组
int scp_create_bpus(int num);

//初始化一个bpu
int scp_init_bpu(scp_bpu_t *bpu, int index_num, int data_num);

/**********************************************************************************************
创建并初始化索引，预先按照公式计算出每个item中的每个scp_htindex_data_t的ordersn的值并填充
index：0开始的item序号
***********************************************************************************************/
scp_htindex_item_t *scp_init_htindex_item(scp_bpu_t *bpu, int index);

/**********************************************************************************************
创建并初始化每个bpu的订单数组，按照计算公式填充每个订单(scp_stk_order_t)的ordersn，并把订单指针赋值给对应的索引项order
***********************************************************************************************/
int scp_init_order(scp_bpu_t *bpu);

/**********************************************************************************************
在创建order_array时，给每个元素分配ordersn，把每项的指针加到索引表，scp_htindex_data_t内的order = order
返回ordersn，-1失败
***********************************************************************************************/
int scp_put_order(scp_bpu_t *bpu, scp_stk_order_t *order);

/**********************************************************************************************
//按照给定的订单号取得该订单号的订单数据指针，按照公式计算。
//第一步：计算所属的BPU
//第二部：计算获得该BPU下的item的顺序号，通过链表操作获得item
//第三步：计算在item中的偏移值
//第四步：获得对应ordersn的order并返回
***********************************************************************************************/
scp_stk_order_t *scp_get_by_ordersn(uint32_t ordersn);

/*******************************************************************************************

思路：以m、n为因子计算每个item内的元素的订单号。
按照给定的订单号和m、n的计算求得某个BPU上的对应数据。

按照以下例子构建计算公式。

m：处理单元个数。
n：订单空间的订单号个数，2的幂。
x：每个处理单元的订单空间个数，2的幂。

假设 m = 4;  SCP_BPU_NUM
n = 16; SCP_INDEX_ORDERSN_NUM
x = 2;  SCP_BPU_ITEM_NUM

处理单元                         0                 1                    2                  3            ...
订单号          第一个索引空间         0-15              16-31               32-47               48-63
订单号          第二个索引空间         64-79             80-95               96-111              112-127      ...
第n个空间         ...                ...                 ...                 ...

*******************************************************************************************/

#endif
