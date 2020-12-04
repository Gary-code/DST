/******************************************************************
## 文件名:scp_bus.h
## Copyright (c) 2018-2028 金证科技股份公司研发中心
## 版 本:0.0.0.1
## 创建人:
## 日期/时间:
## 描 述:定义一个内存数据结构，链表形式，无索引，如需索引应另建，内存支持动态扩充
---------------------------更新记录-----------------------------
## 修改人:田雪
## 日期/时间:
## 描 述:
******************************************************************/

#ifndef _SCP_BUS_H_INCLUDED_
#define _SCP_BUS_H_INCLUDED_

typedef long long int64_t;


#pragma pack(push, 4)

// 客户的持仓信息
struct scp_stk_asset_t
{
	int  	istockcode;		    // 证券代码,6位阿拉伯数字，如600446-金证科技，000001-深万科，etc
	char    szstkbd[4];         // 交易板块, '00':深圳市场，'01':上海市场
	int64_t llstkavl;           // 可用数量
	int64_t llstkfrz;           // 冻结数量
	int64_t llstkufz;           // 解冻数量
	int64_t llstktrdfrz;        // 交易冻结
	int64_t llstktrdufz;        // 交易解冻
	int64_t llstktrdotd;        // 交易在途
	int64_t llstktrdbln;        // 交易轧差
	int64_t llstktrdetfctn;     // etf申购
	int64_t llstktrdetfrmn;     // etf赎回
};

//hash key
struct scp_asset_key_t
{
	int  	istockcode;		    // 证券代码,6位阿拉伯数字，如600446-金证科技，000001-深万科，etc
	char    szstkbd[4];         // 交易板块, '00':深圳市场，'10':上海市场
};

//asset hash item
struct scp_asset_hash_item_t
{
	scp_asset_hash_item_t  *next;
 	void  				   *value;
	scp_asset_key_t         key;
};

#pragma pack(pop)

#endif
