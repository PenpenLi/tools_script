/********************************************************************
	Copyright (c) 2014-2016  xuliming
	created:	2015-2-5   16:28
	filename: 	GameProto.h
	author:		xlm, foreverxuliming@gmail.com	
	brief:		

*********************************************************************/
#ifndef __GAME_PROTO_H__
#define __GAME_PROTO_H__

#if defined WIN32 || defined VUWIN32 || defined VUWINSTORE || defined VUWINPHONE
#pragma pack(1)
#endif
#if defined __GNUC__ || defined VUANDROID || defined VUIOS
#undef GNUC_ALIGN
#define GNUC_ALIGN __attribute__((aligned(4)))
#else
#define GNUC_ALIGN
#endif



typedef struct tagPKG_G_GET_SERVER_TIME_ACK
{
	//int					errCode;
	unsigned long long time;
	
} GNUC_ALIGN PKG_G_GET_SERVER_TIME_ACK;



typedef struct tagPKG_G_GET_SHOW_IMM_BUTTON_TIP_ACK
{
	int					errCode;
	int					byShow;
	
} GNUC_ALIGN PKG_G_GET_SHOW_IMM_BUTTON_TIP_ACK;


typedef struct tagPKG_G_GET_SERVER_TIEM_EX_ACK
{
	int					errCode;
	long long			byShow;

} GNUC_ALIGN PKG_G_GET_SERVER_TIEM_EX_ACK;






#if defined WIN32 || defined VUWIN32 || defined VUWINSTORE || defined VUWINPHONE
#pragma pack()
#endif



#endif