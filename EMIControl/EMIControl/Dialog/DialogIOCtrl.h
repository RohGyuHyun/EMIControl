#pragma once
#include "afxcmn.h"


#define VIEWCLASS					CDialogIOCtrl

#define MAX_ITEM						1
#define MAX_SUB_ITEM					2

#define SUB_ITEM_INSP_LENGTH			2
#define SUB_ITEM_INSP_WIDTH				4
#define SUB_ITEM_INSP_HEIGHT			6
#define SUB_ITEM_INSP_AREA				8
#define SUB_ITEM_INSP_LENGTH_OFFSET		9
#define SUB_ITEM_INSP_WIDTH_OFFSET		10
#define SUB_ITEM_INSP_HEIGHT_OFFSET		11
#define SUB_ITEM_INSP_AREA_OFFSET		12

#define DI_MAX_NUM						96
#define DO_MAX_NUM						96

#define IO_PATH							"D:\\DATABASE\\SYSTEM"

typedef struct {
	BOOL bUse;
	short nDevNo;
} DIO_CFG;



enum DIO {     // I/O Setting date:2015.05.22 
	DI_START = 0,
	DI_X500 = DI_START,						// DI_X500 *DI_LOADER_EMO_SIG = DI_START,										// DI_X500 
	DI_STAGE_MC1_ON,						// DI_X501 *DI_STAGE_MC1_ALARM//EMO를 누루면 꺼진다
	DI_STAGE_MC2_ON,						// DI_X502 *DI_STAGE_MC2_ALARM//EMO를 누루면 꺼진다	
	DI_X503,								// DI_X503 *DI_STAGE_MC3_ALARM,													// DI_X503 //계속꺼져 있다.
	DI_X504,								// DI_X504
	DI_X505,								// DI_X505
	DI_ERROR_RESET_SW,						// DI_X506 //스위치
	DI_PM_SAFETY_MODE_OP_SW,				// DI_X507 //PM모드면 살아있고, SM모드면 죽는다
	DI_X508,								// DI_X508
	DI_X509,								// DI_X509
	DI_X50A,								// DI_X50A 
	DI_CHUCK_AIR_BLOW_ON_DETECT,			// DI_X50B //AIR_BLOW SENSOR 항상 off  *DI_MAIN_CDA_PRES,						// DI_X50B //양압
	DI_CHUCK_VACUUM_ON_DETECT,				// DI_X50C //VACUUM ON SENSOR 항상 off *DI_STAGE_VACUUM_PRES,					// DI_X50C
	DI_CYLINDER_CDA_PRESSURE,				// DI_X50D //압력 게이지 이상시 센서 감지 *DI_REVIEW_1_MIV,						// DI_X50D //사용안함	
	DI_IONIZER_CDA_PRESURE,					// DI_X50E //압력 게이지 이상시 센서 감지 *DI_REVIEW_1_IN_FOCUS,				// DI_X50E //사용안함
	DI_VACUUM_REGULATOR,					// DI_X50F //압력 게이지 이상시 센서 감지	*DI_X50F,							// DI_X50F
	DI_SAFETY_UNIT_MC_1_2_ON,				// DI_X510 //항상 ON EMO시 off 파란색 reset누르면 복귀
	DI_X511,								// DI_X511
	DI_SAFETY_UNIT_MC_3_4_ON,            	// DI_X512 //항상 ON EMO시 off 파란색 reset누르면 복귀
	DI_X513,								// DI_X513
	DI_DO1_OPEN_LEFT,						// DI_X514
	DI_X515,					      		// DI_X515 *DI_DO1_OPEN_RIGHT,			      									// DI_X515 
	DI_DO2_OPEN_LEFT,		    		    // DI_X516  
	DI_DO2_OPEN_RIGHT,						// DI_X517  
	DI_DO3_OPEN_LEFT,				        // DI_X518
	DI_DO3_OPEN_RIGHT,               	    // DI_X519
	DI_X51A,								// DI_X51A
	DI_EMI_OP,								// DI_X51B
	DI_EMI_LEFT,							// DI_X51C
	DI_EMI_BACK,							// DI_X51D
	DI_EMI_INSIDE,							// DI_X51E
	DI_X51F,								// DI_X51F //300 ~ 31F

	//내부배선임(S/W에서 관리 않함)
	DI_ELCB_1_TRIP_MAINT_LAMP,				// DI_X520	
	DI_ELCB_3_TRIP_EFU, 					// DI_X521	
	DI_ELCB_4_TRIP_METAL_SRC,				// DI_X522	
	DI_ELCB_5_TRIP_PC_CONCENT,				// DI_X523	
	DI_ELCB_9_TRIP_OPTIC_MOD,				// DI_X524
	DI_ELCB_8_TRIP_REVIEW_AF,				// DI_X525	
	DI_ELCB_2_TRIP_UPS_PL_LAMP,			    // DI_X526	
	DI_ELCB_6_TRIP_MAIN_PC_TRIGGER,		    // DI_X527	
	DI_ELCB_7_TRIP_MOD_PC,					// DI_X528	
	DI_ACP1_TRIP_AVC_METER,					// DI_X529	
	DI_ACP2_TRIP_AVC_POW_LAMP,				// DI_X52A	
	DI_ACP3_4_TRIP_MAINT_LAMP,	           	// DI_X52B          	
	DI_ACP5_TRIP_PC_RACK_FAN,				// DI_X52C	
	DI_ACP6_17_18_TRIP_REVIEW_AF,			// DI_X52D	
	DI_ACP7_8_16_TRIP_ALIGN_CAM,            // DI_X52E	
	DI_ACP9_12_13_TRIP_OPTIC_MOD1,    		// DI_X52F	
	DI_ACP10_14_15_TRIP_OPTIC_MOD2,			// DI_X530
	DI_UCP1_TRIP_UPS_METER,	     			// DI_X531
	DI_UCP2_TRIP_UPS_POW_LAMP,				// DI_X532
	DI_UCP3_4_TRIP_SAFETY,					// DI_X533
	DI_UCP7_8_TRIP_MC_COIL,					// DI_X534
	DI_UCP12_TRIP_IONIZER,					// DI_X535
	DI_UCP13_TRIP_PANEL_PAN,		       	// DI_X536
	/////////////////////
	DI_X537,								// DI_X537
	DI_IONIZER_HIGH_VOTAGE_ALARM,			// DI_X538 //전압이 높으면 on (항상 off) *DI_UPPER_IONIZER_ALARM1,				// DI_X538
	DI_IONIZER_TIP_CLEANING_ALARM,			// DI_X539 //CLEANING 알람 (항상 off) *DI_UPPER_IONIZER_ALARM2,					// DI_X539
	DI_IONIZER_RUN_STOP,					// DI_X53A //작동이 멈췄을때 알람 (항상 off) *DI_LOWER_IONIZER_ALARM1,			// DI_X53A //사용안함
	DI_IONIZER_LOW_ION_ALARM,				// DI_X53B //ION이 적으면 알람 (항상 off) *DI_LOWER_IONIZER_ALARM2,				// DI_X53B //사용안함
	DI_ROBOT_HAND_DETECT_LEFT,				// DI_X53C //감지되면 off
	DI_ROBOT_HAND_DETECT_RIGHT,				// DI_X53D //감지되면 off
	DI_OP_COOLING_FAN_ALARM,				// DI_X53E  전장외함 항상 ON 꺼지면 off *DI_OUTSIDE_COOLING_FAN_ALARM,			// DI_X53E  전장외함 항상 ON 꺼지면 off
	DI_REAR_COOLING_FAN_ALARM,				// DI_X53F	전장내함 항상 ON 꺼지면 off *DI_INSIDE_COOLING_FAN_ALARM,			// DI_X53F	전장내함 항상 ON 꺼지면 off
	
	DI_STAGE_ALIGN_FRONT1_FW,				// DI_X540		
	DI_STAGE_ALIGN_FRONT1_BW,				// DI_X541		
	DI_STAGE_ALIGN_FRONT2_FW,				// DI_X542		
	DI_STAGE_ALIGN_FRONT2_BW,				// DI_X543		
	DI_STAGE_ALIGN_REAR1_FW,				// DI_X544		
	DI_STAGE_ALIGN_REAR1_BW,				// DI_X545		
	DI_STAGE_ALIGN_REAR2_FW,				// DI_X546		
	DI_STAGE_ALIGN_REAR2_BW,				// DI_X547		 
	DI_STAGE_SIDE_LEFT1_FW,					// DI_X548		
	DI_STAGE_SIDE_LEFT1_BW,					// DI_X549		   
	DI_STAGE_SIDE_LEFT2_FW,					// DI_X54A		
	DI_STAGE_SIDE_LEFT2_BW,					// DI_X54B			
	DI_STAGE_SIDE_RIGHT1_FW,				// DI_X54C		
	DI_STAGE_SIDE_RIGHT1_BW,				// DI_X54D		
	DI_STAGE_SIDE_RIGHT2_FW,				// DI_X54E		
	DI_STAGE_SIDE_RIGHT2_BW,				// DI_X54F	

	DI_PIN_GLS_DETECT_FRONT,				// DI_X550		
	DI_PIN_GLS_DETECT_REAR,					// DI_X551      
 	DI_STAGE_GLS_EDGE_LEFT_FRONT,			// DI_X552      
	DI_STAGE_GLS_EDGE_RIGHT_FRONT,			// DI_X553       
	DI_STAGE_GLS_EDGE_LEFT_REAR,			// DI_X554
	DI_STAGE_GLS_EDGE_RIGHT_REAR,			// DI_X555

	
	DI_X556,								// DI_X556
	DI_X557,								// DI_X557
	DI_X558,								// DI_X558
	DI_X559,								// DI_X559
	DI_X55A,								// DI_X55A
	DI_X55B,								// DI_X55B
	DI_X55C,								// DI_X55C
	DI_X55D,								// DI_X55D
	DI_X55E,								// DI_X55E
	DI_X55F,								// DI_X55F
	
	/*
	DI_X560,			             		// DI_X560		//20131008
	DI_X561,   			            		// DI_X561
	DI_X562,		            			// DI_X562
	DI_X563,								// DI_X563
	DI_X564,								// DI_X564
	DI_X565,			            		// DI_X565	
	DI_X566,			            		// DI_X566
	DI_X567,								// DI_X567
	DI_X568,								// DI_X568
	DI_X569,								// DI_X569
	DI_X56A,								// DI_X56A
	DI_X56B,								// DI_X56B
	DI_X56C,								// DI_X56C
	DI_X56D,								// DI_X56D
	DI_X56E,								// DI_X56E
	DI_X56F,								// DI_X56F
	DI_X570,		         				// DI_X570
	DI_X571,                    			// DI_X571
	DI_X572,                				// DI_X572
	DI_X573,		                		// DI_X573
	DI_X574,								// DI_X574
	DI_X575,								// DI_X575
	DI_X576,								// DI_X576
	DI_X577,								// DI_X577
	DI_X578,								// DI_X578
	DI_X579,								// DI_X579
	DI_X57A,								// DI_X57A
	DI_X57B,								// DI_X57B
	DI_X57C,								// DI_X57C
	DI_X57D,								// DI_X57D
	DI_X57E,								// DI_X57E	
	DI_X57F,								// DI_X57F		//x360~x037f
	
	DI_X580,                		      	// DI_X580
	DI_X581,                    			// DI_X581

	DI_X582,                				// DI_X582
	DI_X583,	                			// DI_X583

	DI_X584,		                		// DI_X584
	DI_X585,	                			// DI_X585

	DI_X586,                				// DI_X586
	DI_X587,		                     	// DI_X587

	DI_X588,              					// DI_X588
	DI_X589,	                			// DI_X589

	DI_X58A,								// DI_X58A
	DI_X58B,								// DI_X58B

	DI_X58C,								// DI_X58C
	DI_X58D,								// DI_X58D
	DI_X58E,             				    // DI_X58E          
	DI_X58F,              			     	// DI_X58F          
	DI_X590,								// DI_X590
	DI_X591,								// DI_X591
	DI_X592,								// DI_X592
	DI_X593,								// DI_X593
	DI_X594,                     			// DI_X594
	DI_X595,                     			// DI_X595
	DI_X596,	                    		// DI_X596
	DI_X597,                     			// DI_X597
	DI_X598,								// DI_X598
	DI_X599,								// DI_X599
	DI_X59A,					           	// DI_X59A
	DI_X59B,				                // DI_X59B
	DI_X59C ,		                     	// DI_X59C
	DI_X59D,		      					// DI_X59D
	DI_X59E,       			             	// DI_X59E
	DI_X59F,        						// DI_X59F

	DI_X5A0,                     			// DI_X5A0  //20131008
	DI_X5A1,                     			// DI_X5A1

	DI_X5A2,                				// DI_X5A2
	DI_X5A3,                 				// DI_X5A3

	DI_X5A4,                				// DI_X5A4
	DI_X5A5,	                 			// DI_X5A5

	DI_X5A6,                    			// DI_X5A6
	DI_X5A7,	                      		// DI_X5A7

	DI_X5A8,             					// DI_X5A8
	DI_X5A9,                				// DI_X5A9

	DI_X5AA,								// DI_X5AA
	DI_X5AB,								// DI_X5AB

	DI_X5AC,								// DI_X5AC
	DI_X5AD,								// DI_X5AD
	DI_X5AE,								// DI_X5AE
	DI_X5AF,								// DI_X5AF
	DI_X5B0,								// DI_X5B0
	DI_X5B1,								// DI_X5B1
	DI_X5B2,								// DI_X5B2
	DI_X5B3,								// DI_X5B3
	DI_X5B4,	                     		// DI_X5B4
	DI_X5B5,                      			// DI_X5B5
	DI_X5B6,								// DI_X5B6
	DI_X5B7,								// DI_X5B7
	DI_X5B8,								// DI_X5B8
	DI_X5B9,								// DI_X5B9
	DI_X5BA,								// DI_X5BA
	DI_X5BB,								// DI_X5BB
	DI_X5BC,								// DI_X5BC	
	DI_X5BD,								// DI_X5BD	
	DI_X5BE,								// DI_X5BE	
	DI_X5BF,								// DI_X5BF	//X03A0 ~ X03BF


	DI_X5C0,								// DI_X5C0  //20131008
	DI_X5C1,								// DI_X5C1
	DI_X5C2,								// DI_X5C2
	DI_X5C3,								// DI_X5C3
	DI_X5C4,								// DI_X5C4
	DI_X5C5,								// DI_X5C5
	DI_X5C6,								// DI_X5C6
	DI_X5C7,								// DI_X5C7
	DI_X5C8,								// DI_X5C8
	DI_X5C9,								// DI_X5C9
	DI_X5CA,								// DI_X5CA
	DI_X5CB,								// DI_X5CB
	DI_X5CC,								// DI_X5CC
	DI_X5CD,								// DI_X5CD
	DI_X5CE,								// DI_X5CE
	DI_X5CF,								// DI_X5CF
	DI_X5D0,								// DI_X5D0
	DI_X5D1,								// DI_X5D1
	DI_X5D2,								// DI_X5D2
	DI_X5D3,								// DI_X5D3
	DI_X5D4,								// DI_X5D4
	DI_X5D5,								// DI_X5D5
	DI_X5D6,								// DI_X5D6
	DI_X5D7,								// DI_X5D7
	DI_X5D8,								// DI_X5D8
	DI_X5D9,								// DI_X5D9
	DI_X5DA,								// DI_X5DA
	DI_X5DB,								// DI_X5DB
	DI_X5DC,								// DI_X5DC	
	DI_X5DD,								// DI_X5DD	
	DI_X5DE,								// DI_X5DE	
	DI_X5DF,								// DI_X5DF	//X3C0 ~ X3DF
	*/
	DI_END = DI_X55F,
	DI_TOTAL = DI_END - DI_START,

	
	DO_START = DI_END+1,					//OUTPUT START
	DO_GLASS_DETECT_SENSOR_RELAY_ON_OFF,	// DO_Y600 *DO_GLASS_EXIST_SENS_ON,		        	// DO_Y600
	DO_DOOR1_UNLOCK_LEFT,					// DO_Y601
	DO_DOOR1_UNLOCK_RIGHT,					// DO_Y602
	DO_DOOR2_UNLOCK_LEFT,					// DO_Y603
	DO_DOOR2_UNLOCK_RIGHT,			   		// DO_Y604
	DO_DOOR3_UNLOCK_LEFT,					// DO_Y605
	DO_DOOR3_UNLOCK_RIGHT,					// DO_Y606
	DO_Y607,								// DO_Y607
	DO_Y608,								// DO_Y608
	DO_Y609,								// DO_Y609
	DO_Y60A,    							// DO_Y60A
	DO_Y60B,								// DO_Y60B
	DO_Y60C,								// DO_Y60C
	DO_Y60D,								// DO_Y60D
	DO_Y60E,								// DO_Y60E
	DO_Y60F,								// DO_Y60F
	DO_BUZZER_MELODY1,						// DO_Y610DO_LED_ALIGN_B0
	DO_BUZZER_MELODY2,						// DO_Y611
	DO_BUZZER_MELODY3,						// DO_Y612
	DO_BUZZER_MELODY4,						// DO_Y613
	DO_SIG_TOWER_FRONT_RED,				 	// DO_Y614
	DO_SIG_TOWER_FRONT_YEL,					// DO_Y615
	DO_SIG_TOWER_FRONT_GRE,					// DO_Y616
	DO_SIG_TOWER_RIGHT_RED,					// DO_Y617
	DO_SIG_TOWER_RIGHT_YEL,					// DO_Y618
	DO_SIG_TOWER_RIGHT_GRE,					// DO_Y619
	DO_Y61A,								// DO_Y61A *DO_REVIEW_1_ESTOP,						// DO_Y61A
	DO_IONIZER_CDA_SOL_ON_OFF,				// DO_Y61B //IONIZER 공압 켜지면 ON(항상 ON 상태유지) *DO_REVIEW_1_CSYNC,						// DO_Y61B
	DO_IONIZER_ION_ON_OFF,            		// DO_Y61C //IONIZER ION ON(STOP)/OFF(RUN) 항상 OFF 상태 유지 *DO_ION1_ON_OFF,            				// DO_Y61C
	DO_Y61D,		      					// DO_Y61D 
	DO_STAGE_VACUUM_SOL,					// DO_Y61E // ROBOT_ENABLE
	DO_STAGE_AIR_BLOW_SOL,					// DO_Y61F *DO_STAGE_PURGE_SOL 


	DO_Y620,								// DO_Y620	//20131008
	DO_Y621,								// DO_Y621
	DO_DOME_D0,								// DO_Y622
	DO_DOME_D1,								// DO_Y623
	DO_DOME_D2,								// DO_Y624	 
	DO_DOME_D3,								// DO_Y625
	DO_DOME_D4,								// DO_Y626
	DO_DOME_D5,								// DO_Y627
	DO_DOME_D6,								// DO_Y628
	DO_DOME_D7,								// DO_Y629
	DO_Y62A,								// DO_Y62A
	DO_Y62B,								// DO_Y62B
	DO_Y62C,								// DO_Y62C
	DO_Y62D,								// DO_Y62D
	DO_Y62E,								// DO_Y62E
	DO_Y62F,								// DO_Y62F
	DO_LINE_LEMP_EXT_,				        // DO_Y630     
	DO_LINE_LEMP_BIT0_LSB,		            // DO_Y631
	DO_LINE_LEMP_BIT1,		     			// DO_Y632
	DO_LINE_LEMP_BIT2,  					// DO_Y633
	DO_LINE_LEMP_BIT3,						// DO_Y634
	DO_LINE_LEMP_BIT4,						// DO_Y635
	DO_LINE_LEMP_BIT5,						// DO_Y636
	DO_LINE_LEMP_BIT6,						// DO_Y637
	DO_LINE_LEMP_BIT7_MSB,  				// DO_Y638
	DO_Y639,		        			   	// DO_Y639
	DO_Y63A,         						// DO_Y63A
	DO_Y63B,	         					// DO_Y63B
	DO_Y63C,								// DO_Y63C
	DO_Y63D,								// DO_Y63D
	DO_Y63E,								// DO_Y63E
	DO_Y63F,								// DO_Y63F		//Y0520~Y053F

	DO_STAGE_ALIGN_FRONT1_FW_SOL,  			// DO_Y640 
	DO_STAGE_ALIGN_FRONT1_BW_SOL,			// DO_Y641
	DO_STAGE_SIDE_RIGHT1_FW_SOL,			// DO_Y642
	DO_STAGE_SIDE_RIGHT1_BW_SOL,			// DO_Y643
	DO_STAGE_SIDE_LIFT1_FW_SOL,				// DO_Y644
	DO_STAGE_SIDE_LIFT1_BW_SOL,			    // DO_Y645
	DO_STAGE_ALIGN_REAR1_FW_SOL,	       	// DO_Y646
	DO_STAGE_ALIGN_REAR1_BW_SOL,		    // DO_Y647

	DO_Y648,			          			// DO_Y648
	DO_Y649,		         				// DO_Y649

	DO_Y64A,								// DO_Y64A
	DO_Y64B,								// DO_Y64B

	DO_Y64C,								// DO_Y64C
	DO_Y64D,								// DO_Y64D
	DO_Y64E,								// DO_Y64E
	DO_Y64F,								// DO_Y64F
	DO_Y650,								// DO_Y650
	DO_Y651,								// DO_Y651
	DO_Y652,								// DO_Y652
	DO_Y653,								// DO_Y653
	DO_Y654,								// DO_Y654
	DO_Y655,								// DO_Y655
	DO_Y656,								// DO_Y656
	DO_Y657,								// DO_Y657
	DO_Y658,	     						// DO_Y658
	DO_Y659,			        			// DO_Y659
	DO_Y65A,				        		// DO_Y65A
	DO_Y65B,	         					// DO_Y65B
	DO_Y65C,	          					// DO_Y65C
	DO_Y65D,         						// DO_Y65D
	DO_Y65E,                 				// DO_Y65E
	DO_Y65F,             					// DO_Y65F

	/*
	DO_Y660,								// DO_Y660
	DO_Y661,								// DO_Y661
	DO_Y662,								// DO_Y662
	DO_Y663,								// DO_Y663
	DO_Y664,								// DO_Y664
	DO_Y665,								// DO_Y665
	DO_Y666,								// DO_Y666
	DO_Y667,								// DO_Y667
	DO_Y668,								// DO_Y668
	DO_Y669,								// DO_Y669
	DO_Y66A,								// DO_Y66A
	DO_Y66B,								// DO_Y66B
	DO_Y66C,								// DO_Y66C
	DO_Y66D,								// DO_Y66D
	DO_Y66E,								// DO_Y66E
	DO_Y66F,								// DO_Y66F
	DO_Y670,								// DO_Y670
	DO_Y671,								// DO_Y671
	DO_Y672,								// DO_Y672
	DO_Y673,								// DO_Y673
	DO_Y674,								// DO_Y674
	DO_Y675,								// DO_Y675
	DO_Y676,								// DO_Y676
	DO_Y677,								// DO_Y677
	DO_Y678,								// DO_Y678
	DO_Y679,								// DO_Y679
	DO_Y67A,								// DO_Y67A
	DO_Y67B,								// DO_Y67B
	DO_Y67C,								// DO_Y67C
	DO_Y67D,								// DO_Y67D
	DO_Y67E,								// DO_Y67E
	DO_Y67F,								// DO_Y67F



	DO_Y680,								// DO_Y680 
	DO_Y681,								// DO_Y681
	DO_Y682,								// DO_Y682
	DO_Y683,								// DO_Y683
	DO_Y684,								// DO_Y684
	DO_Y685,								// DO_Y685
	DO_Y686,								// DO_Y686
	DO_Y687,								// DO_Y687
	DO_Y688,								// DO_Y688
	DO_Y689,								// DO_Y689
	DO_Y68A,								// DO_Y68A
	DO_Y68B,								// DO_Y68B
	DO_Y68C,								// DO_Y68C
	DO_Y68D,								// DO_Y68D
	DO_Y68E,								// DO_Y68E
	DO_Y68F,								// DO_Y68F
	DO_Y690,								// DO_Y690
	DO_Y691,								// DO_Y691
	DO_Y692,								// DO_Y692
	DO_Y693,								// DO_Y693
	DO_Y694,								// DO_Y694
	DO_Y695,								// DO_Y695
	DO_Y696,								// DO_Y696
	DO_Y697,								// DO_Y697
	DO_Y698,								// DO_Y698
	DO_Y699,								// DO_Y699
	DO_Y69A,								// DO_Y69A
	DO_Y69B,								// DO_Y69B
	DO_Y69C,								// DO_Y69C
	DO_Y69D,								// DO_Y69D
	DO_Y69E,								// DO_Y69E
	DO_Y69F,								// DO_Y69F


	DO_Y6A0,								// DO_Y6A0
	DO_Y6A1,								// DO_Y6A1
	DO_Y6A2,								// DO_Y6A2
	DO_Y6A3,								// DO_Y6A3
	DO_Y6A4,								// DO_Y6A4
	DO_Y6A5,								// DO_Y6A5
	DO_Y6A6,								// DO_Y6A6
	DO_Y6A7,								// DO_Y6A7
	DO_Y6A8,								// DO_Y6A8
	DO_Y6A9,								// DO_Y6A9
	DO_Y6AA,								// DO_Y6AA
	DO_Y6AB,								// DO_Y6AB
	DO_Y6AC,								// DO_Y6AC
	DO_Y6AD,								// DO_Y6AD
	DO_Y6AE,								// DO_Y6AE
	DO_Y6AF,								// DO_Y6AF
	DO_Y6B0,								// DO_Y6B0
	DO_Y6B1,								// DO_Y6B1
	DO_Y6B2,								// DO_Y6B2
	DO_Y6B3,								// DO_Y6B3
	DO_Y6B4,								// DO_Y6B4
	DO_Y6B5,								// DO_Y6B5
	DO_Y6B6,								// DO_Y6B6
	DO_Y6B7,								// DO_Y6B7
	DO_Y6B8,								// DO_Y6B8
	DO_Y6B9,								// DO_Y6B9
	DO_Y6BA,								// DO_Y6BA
	DO_Y6BB,								// DO_Y6BB
	DO_Y6BC,								// DO_Y6BC
	DO_Y6BD,								// DO_Y6BD
	DO_Y6BE,								// DO_Y6BE
	DO_Y6BF,								// DO_Y6BF

	DO_Y6C0,								// DO_Y6C0
	DO_Y6C1,								// DO_Y6C1
	DO_Y6C2,								// DO_Y6C2
	DO_Y6C3,								// DO_Y6C3
	DO_Y6C4,								// DO_Y6C4
	DO_Y6C5,								// DO_Y6C5
	DO_Y6C6,								// DO_Y6C6
	DO_Y6C7,								// DO_Y6C7
	DO_Y6C8,								// DO_Y6C8
	DO_Y6C9,								// DO_Y6C9
	DO_Y6CA,								// DO_Y6CA
	DO_Y6CB,								// DO_Y6CB
	DO_Y6CC,								// DO_Y6CC
	DO_Y6CD,								// DO_Y6CD
	DO_Y6CE,								// DO_Y6CE
	DO_Y6CF,								// DO_Y6CF
	DO_Y6D0,								// DO_Y6D0
	DO_Y6D1,								// DO_Y6D1
	DO_Y6D2,								// DO_Y6D2
	DO_Y6D3,								// DO_Y6D3
	DO_Y6D4,								// DO_Y6D4
	DO_Y6D5,								// DO_Y6D5
	DO_Y6D6,								// DO_Y6D6
	DO_Y6D7,								// DO_Y6D7
	DO_Y6D8,								// DO_Y6D8
	DO_Y6D9,								// DO_Y6D9
	DO_Y6DA,								// DO_Y6DA
	DO_Y6DB,								// DO_Y6DB
	DO_Y6DC,								// DO_Y6DC
	DO_Y6DD,								// DO_Y6DD
	DO_Y6DE,								// DO_Y6DE
	DO_Y6DF,								// DO_Y6DF
	*/
	DO_END = DO_Y65F,
	DO_TOTAL = DO_END - DO_START,

};

// CDialogIOCtrl 대화 상자입니다.

class CDialogIOCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogIOCtrl)

public:
	CDialogIOCtrl(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogIOCtrl();


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_IO_CTRL };
public:
	void SetWriteDO(int item,BOOL status);
	BOOL GetReadDI(int in_idx);
	BOOL GetReadDO(int out_idx);
	void SetGlassCtrlDO(int item,BOOL status);
	BOOL GetGlassCtrlReadDI(int in_idx);
	BOOL GetGlassCtrlReadDO(int out_idx);
	BOOL GetMotorCtrlReadDI(int in_idx);
	BOOL GetMotorCtrlReadDO(int out_idx);

	//20160715 ngh
	BOOL GetVacCheck();

	//20160727 ngh
	BOOL GetGlassEdgeStatus();

	//20160726 ngh test
	BOOL m_bTest;
protected:
	LONG m_lPath;
	CFont m_font;

	short OpenCCLink(short nChen);
	short CloseCCLink();
	int InitCCLink(short nChen);
	BOOL ReadDIO(int io);
	BOOL WriteDO(int io, BOOL bValue);

	BOOL LoadDILabelPara(CString path);
	void ReadDisplayInputStatus(BOOL disp);
	void ReadDisplayOutputStatus(BOOL disp);

	wchar_t m_sDILabel[DI_MAX_NUM][100];
	wchar_t m_sDOLabel[DO_MAX_NUM][100];
	BOOL m_bDI[DI_MAX_NUM];
	BOOL m_bDO[DO_MAX_NUM];
	BOOL m_bDI_list[DI_MAX_NUM];
	BOOL m_bDO_list[DO_MAX_NUM];
	CString m_strlog;
	CString m_strDILabel[DI_MAX_NUM];
	CString m_strDOLabel[DO_MAX_NUM];
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_input;
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_list_output;
	afx_msg void OnNMDblclkListIoOutput(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/*
X500=LOADER EMO SIG 
X501=STAGE MC1 ALARM
X502=STAGE MC2 ALARM
X503=STAGE MC3 ALARM
X504=
X505=
X506=ERROR RESET SW 
X507=PM/SAFETY MODE (OP S/W)
X508=
X509=
X50A=REVIEW #1 MIV
X50B=REVIEW #1 IN FOCUS
X50C=
X50D=MAIN CDA PRES
X50E=STAGE VACUUM PRES
X50F=
X510=SAFETY UNIT MC #1,2 ON
X511=
X512=SAFETY UNIT MC #3,4 ON
X513=
X514=DOOR1 OPEN LEFT
X515=DOOR1 OPEN RIGHT
X516=DOOR2 OPEN LEFT
X517=DOOR2 OPEN RIGHT
X518=DOOR3 OPEN LEFT
X519=DOOR3 OPEN RIGHT
X51A=
X51B=EMO OP
X51C=EMO LEFT
X51D=EMO BACK
X51E=EMO 내부
X51F=
X520=ELCB #01 TRIP (MAINT LAMP)  
X521=ELCB #03 TRIP (EFU) 
X522=ELCB #04 TRIP (METAL SOURCE) 
X523=ELCB #05 TRIP (PC CONCENT) 
X524=ELCB #09 TRIP (OPTIC MODULE) 
X525=ELCB #08 TRIP (REVIEW, AF) 
X526=ELCB #02 TRIP (UPS PL LAMP) 
X527=ELCB #06 TRIP (MAIN PC, TRIGGER) 
X528=ELCB #07 TRIP (MODULE PC) 
X529=ACP01 TRIP (AVC METER) 
X52A=ACP02 TRIP (AVC POWER LAMP)
X52B=ACP03, ACP04 TRIP (MAINT LAMP)
X52C=ACP05 TRIP (PC RACK FAN) 
X52D=ACP06, ACP17, ACP18 TRIP (REVIEW, AF)
X52E=ACP07, ACP08, ACP16 TRIP (ALIGN CAMERA)
X52F=ACP09, ACP12, ACP13 TRIP (OPTIC MODULE 1)
X530=ACP10, ACP14, ACP15 TRIP (OPTIC MODULE 2)
X531=UCP01 TRIP (UPS METER)
X532=UCP02 TRIP (UPS POWER LAMP)
X533=UCP03, UCP04 TRIP (SAFETY)
X534=UCP07, UCP08 TRIP (MC COIL)
X535=UCP12 TRIP (IONIZER)
X536=UCP13 TRIP (PANEL FAN)
X537=
X538=UPPER IONIZER ALARM 01
X539=UPPER IONIZER ALARM 01
X53A=LOWER IONIZER ALARM 01
X53B=LOWER IONIZER ALARM 01
X53C=ROBOT HAND DETECT LIFT
X53D=ROBOT HAND DETECT RIGHT
X53E=전장외함 COOLING FAN ALARM
X53F=전장내함 COOLING FAN ALARM
X540=STAGE ALIGN FRONT 1 FORWARD
X541=STAGE ALIGN FRONT 1 BACKWARD
X542=STAGE ALIGN FRONT 2 FORWARD
X543=STAGE ALIGN FRONT 2 BACKWARD
X544=STAGE ALIGN REAR 1 FORWARD
X545=STAGE ALIGN REAR 1 BACKWARD
X546=STAGE ALIGN REAR 2 FORWARD
X547=STAGE ALIGN REAR 2 BACKWARD
X548=STAGE SIDE LEFT 1 FORWARD
X549=STAGE SIDE LEFT 1 BACKWARD
X54A=STAGE SIDE LEFT 2 FORWARD
X54B=STAGE SIDE LEFT 2 BACKWARD
X54C=STAGE SIDE RIGHT 1 FORWARD
X54D=STAGE SIDE RIGHT 1 BACKWARD
X54E=STAGE SIDE RIGHT 2 FORWARD
X54F=STAGE SIDE RIGHT 2 BACKWARD
X550=PIN GLS DETECT FRONT
X551=PIN GLS DETECT REAR
X552=STAGE GLS EDGE LEFT FRONT
X553=STAGE GLS EDGE RIGHT FRONT
X554=STAGE GLS EDGE LEFT REAR
X555=STAGE GLS EDGE RIGHT REAR
X556=
X557=
X558=
X559=
X55A=
X55B=
X55C=
X55D=
X55E=
X55F=
*/
/*
Y600=SAFETY RESET LAMP
Y601=DOOR1 UNLOCK LEFT
Y602=DOOR1 UNLOCK RIGHT
Y603=DOOR2 UNLOCK LEFT
Y604=DOOR2 UNLOCK RIGHT
Y605=DOOR3 UNLOCK LEFT
Y606=DOOR3 UNLOCK RIGHT
Y607=
Y608=
Y609=
Y60A=
Y60B=
Y60C=
Y60D=
Y60E=
Y60F=
Y610=BUZZER MELODY 1
Y611=BUZZER MELODY 2
Y612=BUZZER MELODY 3
Y613=BUZZER MELODY 4
Y614=SIGNAL TOWER FRONT RED
Y615=SIGNAL TOWER FORNT YELLOW
Y616=SIGNAL TOWER FRONT GREEN
Y617=SIGNAL TOWER RIGHT RED
Y618=SIGNAL TOWER RIGHT YELLOW
Y619=SIGNAL TOWER RIGHT GREEN
Y61A=REVIEW #1 E-STOP
Y61B=REVIEW #1 C_sync
Y61C=IONIZER #1 ON/OFF
Y61D=IONIZER #2 ON/OFF
Y61E=STAGE VACUUM SOL
Y61F=STAGE PURGE SOL
Y620=
Y621=
Y622=
Y623=
Y624=
Y625=
Y626=
Y627=
Y628=
Y629=
Y62A=
Y62B=
Y62C=
Y62D=
Y62E=
Y62F=
Y630=
Y631=
Y632=
Y633=
Y634=
Y635=
Y636=
Y637=
Y638=
Y639=
Y63A=
Y63B=
Y63C=
Y63D=
Y63E=
Y63F=
Y640=STAGE ALIGN FRONT 1 FORWARD SOL
Y641=STAGE ALIGN FRONT 1 BACKWARD SOL
Y642=STAGE ALIGN REAR 1 FORWARD SOL
Y643=STAGE ALIGN REAR 1 BACKWARD SOL
Y644=STAGE SIDE LIFT 1 FORWARD SOL
Y645=STAGE SIDE LIFT 1 BACKWARD SOL
Y646=STAGE SIDE RIGHT 1 FORWARD SOL
Y647=STAGE SIDE RIGHT 1BACKWARD SOL
Y648=
Y649=
Y64A=
Y64B=
Y64C=
Y64D=
Y64E=
Y64F=
Y650=
Y651=
Y652=
Y653=
Y654=
Y655=
Y656=
Y657=
Y658=
Y659=
Y65A=
Y65B=
Y65C=
Y65D=
Y65E=
Y65F=
*/