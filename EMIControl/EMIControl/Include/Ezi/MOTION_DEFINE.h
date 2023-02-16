
#pragma once
#include <tchar.h>

//------------------------------------------------------------------
//                 Device Type Defines.
//------------------------------------------------------------------
#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ST
#define	DEVTYPE_EZI_SERVO_PLUS_R_ST			1
#define DEVNAME_EZI_SERVO_PLUS_R_ST			"Ezi-SERVO Plus-R-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ADC_ST
#define	DEVTYPE_EZI_SERVO_PLUS_R_ADC_ST			3
#define DEVNAME_EZI_SERVO_PLUS_R_ADC_ST			"Ezi-SERVO Plus-R-ADC"
#endif

#ifndef DEVTYPE_EZI_STEP_PLUS_R_ST
#define	DEVTYPE_EZI_STEP_PLUS_R_ST			20
#define DEVNAME_EZI_STEP_PLUS_R_ST			"Ezi-STEP Plus-R-ST"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL_ST
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL_ST			30
#define DEVNAME_EZI_SERVO_PLUS_R_ALL_ST			"Ezi-SERVO Plus-R-ALL"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_ALL_ABS
#define	DEVTYPE_EZI_SERVO_PLUS_R_ALL_ABS		35
#define DEVNAME_EZI_SERVO_PLUS_R_ALL_ABS		"Ezi-SERVO Plus-R-ALL Abs"
#endif

#ifndef DEVTYPE_EZI_STEP_PLUS_R_ALL_ST
#define	DEVTYPE_EZI_STEP_PLUS_R_ALL_ST			40
#define DEVNAME_EZI_STEP_PLUS_R_ALL_ST			"Ezi-STEP Plus-R-ALL"
#endif

#ifndef DEVTYPE_EZI_SERVO_PLUS_R_MINI
#define	DEVTYPE_EZI_SERVO_PLUS_R_MINI			50
#define DEVNAME_EZI_SERVO_PLUS_R_MINI			"Ezi-SERVO Plus-R-Mini"
#endif

#ifndef DEVTYPE_EZI_STEP_PLUS_R_MINI
#define	DEVTYPE_EZI_STEP_PLUS_R_MINI			60
#define DEVNAME_EZI_STEP_PLUS_R_MINI			"Ezi-STEP Plus-R-Mini"
#endif

//------------------------------------------------------------------
//                 Motion Direction Defines.
//------------------------------------------------------------------

static const int	DIR_INC = 1;
static const int	DIR_DEC = 0;

//------------------------------------------------------------------
//                 Axis Status Flag Defines.
//------------------------------------------------------------------

static const int	MAX_AXIS_STATUS		= 32;

//------------------------------------------------------------------
//                 Input/Output Assigning Defines.
//------------------------------------------------------------------

static const BYTE	LEVEL_LOW_ACTIVE	= 0;
static const BYTE	LEVEL_HIGH_ACTIVE	= 1;

static const BYTE	IN_LOGIC_NONE	= 0;
static const BYTE	OUT_LOGIC_NONE	= 0;

//------------------------------------------------------------------
//                 POSITION TABLE Defines.
//------------------------------------------------------------------

static const WORD	MAX_LOOP_COUNT	= 100;
static const WORD	MAX_WAIT_TIME	= 60000;

static const WORD	PUSH_RATIO_MIN	= 10;
static const WORD	PUSH_RATIO_MAX	= 100;

typedef enum
{
	CMD_ABS_LOWSPEED = 0,
	CMD_ABS_HIGHSPEED,
	CMD_ABS_HIGHSPEEDDECEL,
	CMD_ABS_NORMALMOTION,
	CMD_INC_LOWSPEED,
	CMD_INC_HIGHSPEED,
	CMD_INC_HIGHSPEEDDECEL,
	CMD_INC_NORMALMOTION,
	CMD_MOVE_ORIGIN,
	CMD_COUNTERCLEAR,
	CMD_PUSH_ABSMOTION,

	CMD_MAX_COUNT,

	CMD_NO_COMMAND = 0xFFFF,
} COMMAND_LIST;

#ifndef	DEFINE_ITEM_NODE
#define DEFINE_ITEM_NODE

#pragma pack(2)

typedef union
{
	WORD	wBuffer[32];		// 64 bytes

	struct
	{
		LONG	lPosition;
		
		DWORD	dwStartSpd;
		DWORD	dwMoveSpd;
		
		WORD	wAccelRate;
		WORD	wDecelRate;
		
		WORD	wCommand;
		WORD	wWaitTime;
		WORD	wContinuous;
		WORD	wBranch;
		
		WORD	wCond_branch0;
		WORD	wCond_branch1;
		WORD	wCond_branch2;
		WORD	wLoopCount;
		WORD	wBranchAfterLoop;
		WORD	wPTSet;
		WORD	wLoopCountCLR;

		WORD	bCheckInpos;		// 0 : Check Inpos, 1 : Don't Check.
		
		LONG	lTriggerPos;
		WORD	wTriggerOnTime;

		WORD	wPushRatio;
		DWORD	dwPushSpeed;
		LONG	lPushPosition;
	};
} ITEM_NODE, *LPITEM_NODE;

#pragma pack()

#endif

static const WORD	OFFSET_POSITION		= 0;
static const WORD	OFFSET_LOWSPEED		= 4;
static const WORD	OFFSET_HIGHSPEED	= 8;
static const WORD	OFFSET_ACCELTIME	= 12;
static const WORD	OFFSET_DECELTIME	= 14;
static const WORD	OFFSET_COMMAND		= 16;
static const WORD	OFFSET_WAITTIME		= 18;
static const WORD	OFFSET_CONTINUOUS	= 20;
static const WORD	OFFSET_JUMPTABLENO	= 22;
static const WORD	OFFSET_JUMPPT0		= 24;
static const WORD	OFFSET_JUMPPT1		= 26;
static const WORD	OFFSET_JUMPPT2		= 28;
static const WORD	OFFSET_LOOPCOUNT		= 30;
static const WORD	OFFSET_LOOPJUMPTABLENO	= 32;
static const WORD	OFFSET_PTSET			= 34;
static const WORD	OFFSET_LOOPCOUNTCLEAR	= 36;
static const WORD	OFFSET_CHECKINPOSITION	= 38;
static const WORD	OFFSET_TRIGGERPOSITION	= 40;
static const WORD	OFFSET_TRIGGERONTIME	= 44;
static const WORD	OFFSET_PUSHRATIO		= 46;
static const WORD	OFFSET_PUSHSPEED		= 48;
static const WORD	OFFSET_PUSHPOSITION		= 52;

static const WORD	OFFSET_BLANK			= 56;

//------------------------------------------------------------------
//                 EX Commands Option Defines.
//------------------------------------------------------------------
#pragma pack(1)

typedef union
{
	BYTE	byBuffer[32];

	struct
	{
		union
		{
			DWORD dwOptionFlag;
			struct
			{
				unsigned BIT_IGNOREEXSTOP	: 1;
				
				unsigned BIT_USE_CUSTOMACCEL	: 1;
				unsigned BIT_USE_CUSTOMDECEL	: 1;
				
				//unsigned BITS_RESERVED	: 13;
			};
		} flagOption;

		WORD	wCustomAccelTime;
		WORD	wCustomDecelTime;

		//BYTE	buffReserved[24];
	};
} MOTION_OPTION_EX;

typedef union
{
	BYTE	byBuffer[32];

	struct
	{
		union
		{
			DWORD dwOptionFlag;
			struct
			{
				unsigned BIT_IGNOREEXSTOP	: 1;
				unsigned BIT_USE_CUSTOMACCDEC	: 1;
				
				//unsigned BITS_RESERVED	: 14;
			};
		} flagOption;

		WORD	wCustomAccDecTime;

		//BYTE	buffReserved[26];
	};
} VELOCITY_OPTION_EX;

#pragma pack()

//------------------------------------------------------------------
//                 Alarm Type Defines.
//------------------------------------------------------------------
typedef enum
{
	ALARM_NONE = 0,

	/*  1 */ ALARM_OVERCURRENT,
	/*  2 */ ALARM_OVERSPEED,
	/*  3 */ ALARM_STEPOUT,
	/*  4 */ ALARM_OVERLOAD,
	/*  5 */ ALARM_OVERTEMPERATURE,
	/*  6 */ ALARM_OVERBACKEMF,
	/*  7 */ ALARM_MOTORCONNECT,
	/*  8 */ ALARM_ENCODERCONNECT,
	/*  9 */ ALARM_LOWMOTORPOWER,
	/* 10 */ ALARM_INPOSITION,
	/* 11 */ ALARM_SYSTEMHALT,
	/* 12 */ ALARM_ROMDEVICE,
	/* 13 */ ALARM_RESERVED0,
	/* 14 */ ALARM_HIGHINPUTVOLTAGE,
	/* 15 */ ALARM_POSITIONOVERFLOW,
	/* 16 */ ALARM_POSITIONCHANGED,

	MAX_ALARM_NUM
} ALARM_TYPE;

static LPCTSTR ALARM_DESCRIPTION[MAX_ALARM_NUM] = 
{
	NULL,
	/*  1 */ _T("Over Current"),
	/*  2 */ _T("Over Speed"),
	/*  3 */ _T("Position Tracking"),
	/*  4 */ _T("Over Load"),
	/*  5 */ _T("Over Temperature"),
	/*  6 */ _T("Over Back EMF"),
	/*  7 */ _T("No Motor Connect"),
	/*  8 */ _T("No Encoder Connect"),
	/*  9 */ _T("Low Motor Power"),
	/* 10 */ _T("Inposition Error"),
	/* 11 */ _T("System Halt"),
	/* 12 */ _T("ROM Device Error"),
	/* 13 */ NULL,
	/* 14 */ _T("High Input Voltage"),
	/* 15 */ _T("Position Overflow"),
	/* 16 */ _T("Position Changed")
};
