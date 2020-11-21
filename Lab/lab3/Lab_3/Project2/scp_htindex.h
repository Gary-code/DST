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
//������Ϣ
struct scp_stk_order_s
{
	int 	ordersn;	  		// ������
	int 	stockcode;          // ֤ȯ����
	int 	stkbiz;            // ֤ȯҵ��
	int 	stkbizaction;      // ҵ����Ϊ
	int64_t orderprice;		   // ί�м۸�
	int64_t orderqty; 		   // ί������
};

//����������Ԫ��
struct scp_htindex_data_s
{
	uint32_t  			 ordersn; 	//������
	scp_stk_order_t      *order;     //��������ָ��
};

//���������������������scp_htindex_data_t���͵�����
struct scp_htindex_item_s
{
	scp_htindex_data_t	    *start; //��������ʼ��ַ
	scp_htindex_data_t	    *last;  //������δʹ�õ�ordersn���׵�ַ
	scp_htindex_data_t		*end;   //���������һ���ַ
	int                      index; //�������������е�˳���
	scp_htindex_item_t      *next;  //��һ��������
	scp_htindex_item_t      *prev;  //ǰһ��������
};

//����Ԫ�����ж���������������������
struct scp_bpu_s
{
	scp_htindex_item_t *list;        //����������item������
	scp_htindex_item_t *last;		 //��ǰ������
	int                 index_num;   //����item����
	scp_stk_order_t    *order_array; //��������
	int                 order_num;   //��������Ԫ�ظ���=
	int                 index;       //��BPU����������(��0��ʼ)
};
#pragma pack(pop)

#define SCP_BPU_NUM           		4					//BPU �ĸ���
#define SCP_INDEX_ORDERSN_NUM 	    1024				//�����ռ�Ķ����Ÿ���
#define SCP_INDEX_ITEM_NUM    		4					//ÿ��BPU�Ķ����ռ����


//����һ��bpu����
int scp_create_bpus(int num);

//��ʼ��һ��bpu
int scp_init_bpu(scp_bpu_t *bpu, int index_num, int data_num);

/**********************************************************************************************
��������ʼ��������Ԥ�Ȱ��չ�ʽ�����ÿ��item�е�ÿ��scp_htindex_data_t��ordersn��ֵ�����
index��0��ʼ��item���
***********************************************************************************************/
scp_htindex_item_t *scp_init_htindex_item(scp_bpu_t *bpu, int index);

/**********************************************************************************************
��������ʼ��ÿ��bpu�Ķ������飬���ռ��㹫ʽ���ÿ������(scp_stk_order_t)��ordersn�����Ѷ���ָ�븳ֵ����Ӧ��������order
***********************************************************************************************/
int scp_init_order(scp_bpu_t *bpu);

/**********************************************************************************************
�ڴ���order_arrayʱ����ÿ��Ԫ�ط���ordersn����ÿ���ָ��ӵ�������scp_htindex_data_t�ڵ�order = order
����ordersn��-1ʧ��
***********************************************************************************************/
int scp_put_order(scp_bpu_t *bpu, scp_stk_order_t *order);

/**********************************************************************************************
//���ո����Ķ�����ȡ�øö����ŵĶ�������ָ�룬���չ�ʽ���㡣
//��һ��������������BPU
//�ڶ����������ø�BPU�µ�item��˳��ţ�ͨ������������item
//��������������item�е�ƫ��ֵ
//���Ĳ�����ö�Ӧordersn��order������
***********************************************************************************************/
scp_stk_order_t *scp_get_by_ordersn(uint32_t ordersn);

/*******************************************************************************************

˼·����m��nΪ���Ӽ���ÿ��item�ڵ�Ԫ�صĶ����š�
���ո����Ķ����ź�m��n�ļ������ĳ��BPU�ϵĶ�Ӧ���ݡ�

�����������ӹ������㹫ʽ��

m������Ԫ������
n�������ռ�Ķ����Ÿ�����2���ݡ�
x��ÿ������Ԫ�Ķ����ռ������2���ݡ�

���� m = 4;  SCP_BPU_NUM
n = 16; SCP_INDEX_ORDERSN_NUM
x = 2;  SCP_BPU_ITEM_NUM

����Ԫ                         0                 1                    2                  3            ...
������          ��һ�������ռ�         0-15              16-31               32-47               48-63
������          �ڶ��������ռ�         64-79             80-95               96-111              112-127      ...
��n���ռ�         ...                ...                 ...                 ...

*******************************************************************************************/

#endif
