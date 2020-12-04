/******************************************************************
## �ļ���:scp_bus.h
## Copyright (c) 2018-2028 ��֤�Ƽ��ɷݹ�˾�з�����
## �� ��:0.0.0.1
## ������:
## ����/ʱ��:
## �� ��:����һ���ڴ����ݽṹ��������ʽ������������������Ӧ�����ڴ�֧�ֶ�̬����
---------------------------���¼�¼-----------------------------
## �޸���:��ѩ
## ����/ʱ��:
## �� ��:
******************************************************************/

#ifndef _SCP_BUS_H_INCLUDED_
#define _SCP_BUS_H_INCLUDED_

#define int64_t long long
#define uint32_t unsigned int

#pragma pack(push, 4)

// �ͻ��ĳֲ���Ϣ
struct scp_stk_asset_t
{
	int  	istockcode;		    // ֤ȯ����,6λ���������֣���600446-��֤�Ƽ���000001-����ƣ�etc
	char    szstkbd[4];         // ���װ��, '00':�����г���'01':�Ϻ��г�
	int64_t llstkavl;           // ��������
	int64_t llstkfrz;           // ��������
	int64_t llstkufz;           // �ⶳ����
	int64_t llstktrdfrz;        // ���׶���
	int64_t llstktrdufz;        // ���׽ⶳ
	int64_t llstktrdotd;        // ������;
	int64_t llstktrdbln;        // ��������
	int64_t llstktrdetfctn;     // etf�깺
	int64_t llstktrdetfrmn;     // etf���
};

//hash key
struct scp_asset_key_t
{
	int  	istockcode;		    // ֤ȯ����,6λ���������֣���600446-��֤�Ƽ���000001-����ƣ�etc
	char    szstkbd[4];         // ���װ��, '00':�����г���'10':�Ϻ��г�
};

//asset hash item
struct scp_asset_hash_item_t
{
	scp_asset_hash_item_t* next;
	void* value;
	scp_asset_key_t         key;
};

#pragma pack(pop)

#endif
