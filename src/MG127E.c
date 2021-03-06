/**
  ******************************************************************************
  * @file    :Main.c
  * @author  :MG Team
  * @version :V1.1
  * @date
  * @brief
  ******************************************************************************
***/

/* Includes ------------------------------------------------------------------*/
#include "Includes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/



/* Private macro -------------------------------------------------------------*/
//sleep interval(ms) between each channel
#define CNT_SLEEP 100

/* Private variables ---------------------------------------------------------*/
extern unsigned short tick;

#define LEN_BLE_ADDR 6

unsigned char rx_buf[39];
//BLE ADV_data .
//static uint8_t adv_data[30] = {0x02,0x01,0x04, 0x1a,0xff,0x4c,0x00,2,0x15, 0xfd,0xa5,0x06,0x93,0xa4,0xe2,0x4f,0xb1,0xaf,0xcf,0xc6,0xeb,0x07,0x64,0x78,0x25, 0x27,0x38,0x9d,0x85, 0xB6};
#ifdef NO_HELP
//#define LEN_DATA 18
//static uint8_t adv_data[LEN_DATA] = {2,1,4, 0x0a,0x09,0x4d,0x41,0x43,0x52,0x4f,0x47,0x49,0x47,0x41,0x03,0xff,0x00,0x09};
#define LEN_WECHAT 30
static uint8_t adv_wechat[LEN_WECHAT] = {0x02,0x01,0x04, 0x1a,0xff,0x4c,0x00,2,0x15, 0xfd,0xa5,0x06,0x93,0xa4,0xe2,0x4f,0xb1,0xaf,0xcf,0xc6,0xeb,0x07,0x64,0x78,0x25, 0x27,0x32,0x52,0xa9, 0xB6};
//#define LEN_WECHAT 18
//42 18 02 01 04 0A 09 43 50 53 44 4E 41 31 2E 30 03 FF 20 0B
//static uint8_t adv_wechat[LEN_WECHAT] = {0x02,0x01,0x04, 0xa, 9, 0x43, 0x50, 0x53, 0x44, 0x4E, 0x41, 0x31,0x2E, 0x30, 3,0xff, 0x20,0x0B};
#else

static uint8_t adv_data[30] = {0};

//iBeacon data format
static uint8_t iBeacon_adv_data[] =
{
	/*
	02 # Number of bytes that follow in first AD structure
	01 # Flags AD type
	04 # Flags value
	1A # Number of bytes that follow in second (and last) AD structure
	FF # Manufacturer specific data AD type
	4C 00 # Company identifier code (0x004C == Apple)
	02 # Byte 0 of iBeacon advertisement indicator
	15 # Byte 1 of iBeacon advertisement indicator
	*/
	//{
	0x02,															/* length 0x02*/
	BLE_GAP_AD_TYPE_FLAGS,											/* AD type = 01 */
	GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED|GAP_ADTYPE_FLAGS_GENERAL, 	/* LE Mode = 0x06 */
	//}


	0x1a,															/* length 0x1a*/
	BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,						/* AD type = 0xFF Manufacturer Specific Data */
	/*CompanyID (AppleID 0x004C)https://www.bluetooth.com/specifications/assigned-numbers/company-Identifiers*/
	0x4c,0x00,														/* CompanyID = 0x004C */
	0x02,															/* iBeacon flag = 0x02 */
	0x15, 															/* length 0x15 length :0x15 21byte  (16B UUID+ 2B major, 2B minor, 1B Txpower)*/
	/*UUID */
	0xfd,0xa5,0x06,0x93,0xa4,0xe2,0x4f,0xb1,0xaf,0xcf,0xc6,0xeb,0x07,0x64,0x78,0x25,
	/***************User set***********************/
	0x27,0x38,														/*Major*/
	0x9d,0x85,														/*Minjo*/
	0xB6															/*Txpower*/
	/***************User End***********************/

};

//Beacon remote data format
static uint8_t Beacon_Key_Press_adv_data[] =
{
	0x02,															/* length 0x02*/
	BLE_GAP_AD_TYPE_FLAGS,											/* AD type = 01 */
	GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED|GAP_ADTYPE_FLAGS_GENERAL, 	/* LE Mode = 0x06 */


	0x0a,															/* length 0x1a*/
	BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,							/* AD type = 0x09*/
	'K','e','y','-','P','r','e','s', 's',							/* use Ascii */


	0x03,															/* length 0x03*/
	BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,						/* AD type = 0xFF */
	/***************User set***********************/
	0x20,0x0b,
	/***************User End***********************/
};

//Beacon remote data format
static uint8_t Beacon_Key_uP_adv_data[] =
{
	0x02,															/* length 0x02*/
	BLE_GAP_AD_TYPE_FLAGS,											/* AD type = 01 */
	GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED|GAP_ADTYPE_FLAGS_GENERAL, 	/* LE Mode = 0x06 */

	0x0a,															/* length 0x1a*/
	BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,							/* AD type = 0x09*/
	'K','e','y','-','-','-','-','U', 'p',

	0x03,															/* length 0x03*/
	BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,						/* AD type = 0xFF */
	/***************User set***********************/
	0x10,0x0b,
	/***************User End***********************/
};

//Eddystone data format
static uint8_t Eddystone_Uri_adv_data[]=
{
	/***************User End***********************/
	/*Service Data - 16-bit UUID. */
	//BLE_GAP_AD_TYPE_SERVICE_DATA,
	/*Google 16bit UUID */
	//0xAA,0xFE,
	//{
    0x02, 															/* length 0x02*/
    BLE_GAP_AD_TYPE_FLAGS, 											/* AD type = 01 */
    GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED|GAP_ADTYPE_FLAGS_GENERAL, 	/* LE Mode = 0x06 */
	//}

	//{
    /* Eddystone(https://github.com/google/eddystone/blob/master/protocol-specification.md) */
    0x03, 															/* length 0x03 */
    BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, 					/* AD type = Complete list of 16-bit UUIDs available */
    0xAA,															/* Eddystone service FEAA */
    0xFE,
	//}

	//{
    0x0b, 															/* length 12byte*/
    BLE_GAP_AD_TYPE_SERVICE_DATA, 									/* AD type = Service Data type value */
    0xAA, 															/* Eddystone service FEAA */
    0xFE,

    /* Eddystone-URL(https://github.com/google/eddystone/tree/master/eddystone-url) */
    0x10,                   										/* Frame Type: URL */
    0xb6,                   										/* Ranging Data */
    0x00,                   										/* URL Scheme: https:// */
    'j','b','5','1',//'s','z','o','k','l','e',
    0x0a,
	//}
};

/*- INDICATION data -*/
uint8_t Eddystone_Uid_adv_data[] = {
	/*Service Data - 16-bit UUID. */
	//BLE_GAP_AD_TYPE_SERVICE_DATA,
	/*Google 16bit UUID */
	//0xAA,0xFE,
	//{
    0x02, 															/* length 0x02*/
    BLE_GAP_AD_TYPE_FLAGS, 											/* AD type = 01 */
    GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED|GAP_ADTYPE_FLAGS_GENERAL, 	/* LE Mode = 0x06 */
	//}


	//{
    /* Eddystone(https://github.com/google/eddystone/blob/master/protocol-specification.md) */
    0x03, 															/* length 0x03 */
    BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, 					/* AD type = Complete list of 16-bit UUIDs available */
    0xAA,															/* Eddystone service FEAA */
    0xFE,
	//}


	//{
    0x17, 															/* length 12byte*/
    BLE_GAP_AD_TYPE_SERVICE_DATA, 									/* AD type = Service Data type value */
    0xAA, 															/* Eddystone service FEAA */
    0xFE,


    0x00,               											/* Frame Type: UID */
    0xE7,              	 											/* Ranging Data */
    0x16, 0xF7, 0x42, 0xf6, 0xA8, 0x8C, 0x57, 0x5B, 0x53, 0x24,		/* Namespace:MSB 10Bytes*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01,                             /* Instance */
    0x00,               											/* Reserved */
    0x00,               											/* Reserved */
	//}
};



uint8_t Eddystone_Tlm_adv_data[] = {
	/*Service Data - 16-bit UUID. */
	//BLE_GAP_AD_TYPE_SERVICE_DATA,
	/*Google 16bit UUID */
	//0xAA,0xFE,
	//{
    0x02, 															/* length 0x02*/
    BLE_GAP_AD_TYPE_FLAGS, 											/* AD type = 01 */
    GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED|GAP_ADTYPE_FLAGS_GENERAL, 	/* LE Mode = 0x06 */
	//}


	//{
    /* Eddystone(https://github.com/google/eddystone/blob/master/protocol-specification.md) */
    0x03, 															/* length 0x03 */
    BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE, 					/* AD type = Complete list of 16-bit UUIDs available */
    0xAA,															/* Eddystone service FEAA */
    0xFE,
	//}


	//{
    0x11, 															/* length 12byte*/
    BLE_GAP_AD_TYPE_SERVICE_DATA, 									/* AD type = Service Data type value */
    0xAA, 															/* Eddystone service FEAA */
    0xFE,

    /* Eddystone-TLM(https://github.com/google/eddystone/tree/master/eddystone-tlm) */
    0x20,                   										/* Frame Type: TLM */
    0x00,                   										/* TLM Version */
    0x0b, 0x54,             										/* Battery voltage 2900[mV] */
    0x0b, 0x54,             										/* Beacon Temperature */
    0x00, 0x00, 0x10, 0x00, 										/* Advertising PDU count */
    0x01, 0x00, 0x00, 0x00, 										/* Time since power-on or reboot */
    //}

};

#endif



/* Private function prototypes -----------------------------------------------*/



/*******************************************************************************
* Function   :     	BLE_Mode_Sleep
* Parameter  :     	void
* Returns    :     	void
* Description:
* Note:      : 		BLE enter sleep mode. current: 3ua
*******************************************************************************/
void BLE_Mode_Sleep(void)
{
    uint8_t	temp0[4];

    temp0[0] = 0x02;
    temp0[1] = 0xff;
    temp0[2] = 0xff;
    temp0[3] = 0xff;

    SPI_Write_Buffer(SLEEP_WAKEUP,temp0,4);
}
/*******************************************************************************
* Function   :     	BLE_Mode_Wakeup
* Parameter  :     	void
* Returns    :     	void
* Description:
* Note:      : 		BLE reg:0x00--0x1f. write operation must or 0x20
*******************************************************************************/
void BLE_Mode_Wakeup(void)
{
    SPI_Write_Reg(SLEEP_WAKEUP|0x20, 0x01);
}

/*******************************************************************************
* Function   :     	BLE_Set_StartTime
* Parameter  :     	uint32_t
* Returns    :     	void
* Description:
* Note:      :
*******************************************************************************/
void BLE_Set_StartTime(uint32_t htime)
{
    uint8_t temp0[3];
    uint32_t temp1 = htime;

    temp0[0] = temp1 & 0xFF;
    temp0[1] = (temp1>>8) & 0xFF;
    temp0[2] = (temp1>>16) & 0xFF;

    SPI_Write_Buffer(START_TIME,temp0,3);
}


/*******************************************************************************
* Function   :     	BLE_Set_TimeOut
* Parameter  :     	uint32_t data_us
* Returns    :     	void
* Description:      TX/RX timeout .unit:us
* Note:      :
*******************************************************************************/
void BLE_Set_TimeOut(uint32_t data_us)
{
	uint8_t temp0[3];

	if(data_us < 0x10000)
	{
        temp0[0] = data_us & 0xff;
        temp0[1] = (data_us >> 8) & 0xff;
        temp0[2] = 0;
        SPI_Write_Buffer(TIMEOUT, temp0, 3);
	}
	else
	{
		SPI_Read_Buffer(TIMEOUT,temp0,3);
		if(temp0[2] == 0)
		{
			temp0[2] = 1;
			SPI_Write_Buffer(TIMEOUT, temp0, 3);
		}
	}
}

/*******************************************************************************
* Function   :     	BLE_Set_Xtal
* Parameter  :     	uint8_t
* Returns    :     	void
* Description:
* Note:      :
*******************************************************************************/
void BLE_Set_Xtal(uint8_t on_flag)
{
    SPI_Write_Reg(0x50, 0x53);
    SPI_Write_Reg(0x3D, 0x18|(on_flag<<2));
    SPI_Write_Reg(0x50, 0x56);
}

static void BLE_Get_Pdu(uint8_t *ptr, uint8_t *len)
{
    uint8_t hdr_type;
    uint8_t len_tmp;
    uint8_t bank_buf[6];

    SPI_Read_Buffer(ADV_HDR_RX, bank_buf, 2);

    *len = bank_buf[1] + 2;
    ptr[0] = bank_buf[0];
    ptr[1] = bank_buf[1];

    hdr_type = bank_buf[0] & 0xF;
    len_tmp = bank_buf[1];
    switch(hdr_type){
        case ADV_IND:  //advA+0~31
        case ADV_NONCONN_IND:
        //case ADV_SCAN_IND:
        //case ADV_SCAN_RSP:
            SPI_Read_Buffer(INITA_RX, &ptr[2], LEN_BLE_ADDR);  //INITA
            len_tmp -= LEN_BLE_ADDR;
            break;
/*
        case ADV_DIRECT_IND:  //advA+InitA
            if(len_tmp == 12){
                SPI_Read_Buffer(ADVA_RX, &ptr[2], LEN_BLE_ADDR);  //advA
                len_tmp -= LEN_BLE_ADDR;
                SPI_Read_Buffer(INITA_RX, &ptr[2+LEN_BLE_ADDR], LEN_BLE_ADDR);  //INITA
                len_tmp -= LEN_BLE_ADDR;
            }
	    len_tmp = 0;
            break;
        case ADV_SCAN_REQ:  //scanA + advA
            if(len_tmp == 12){
                SPI_Read_Buffer(INITA_RX, &ptr[2], LEN_BLE_ADDR);  //INITA
                len_tmp -= LEN_BLE_ADDR;
                SPI_Read_Buffer(ADVA_RX, &ptr[2+LEN_BLE_ADDR], LEN_BLE_ADDR);  //advA
                len_tmp -= LEN_BLE_ADDR;
            }
	    len_tmp = 0;
            break;
        case ADV_CONN_REQ:  //InitA + advA + LL(22B)
            if(len_tmp == 34){
                SPI_Read_Buffer(INITA_RX, &ptr[2], LEN_BLE_ADDR);  //INITA
                SPI_Read_Buffer(ADVA_RX, &ptr[2+LEN_BLE_ADDR], LEN_BLE_ADDR);  //advA
                SPI_Read_Buffer(R_RX_PAYLOAD, &ptr[2+LEN_BLE_ADDR+LEN_BLE_ADDR], 22);
            }
	    len_tmp = 0;
            break;
*/
        default:
	    len_tmp = 0;
            break;
    }

    if(len_tmp > 0){
        SPI_Read_Buffer(R_RX_PAYLOAD, &ptr[2+LEN_BLE_ADDR], len_tmp);
    }
}



#define TXGAIN_DFF 19

void BLE_Do_Cal()  //calibration
{
    uint8_t data_buf[2];

    SPI_Write_Reg(0x3F, 0x03);
    do{
        data_buf[0] = SPI_Read_Reg(0x1F);
    }while(data_buf[0]&0x03);

    SPI_Write_Reg(0x3F, 0x03);
    do{
        data_buf[0] = SPI_Read_Reg(0x1F);
    }while(data_buf[0]&0x03);

    //////////////////////////////////////////////////
    SPI_Write_Reg(0x35,0x01); //testm for tx/temp
    SPI_Write_Reg(0x32,0xA0);
    SPI_Write_Reg(0x2a,0x04);
    SPI_Write_Reg(0x2a,0x00);

    SPI_Write_Reg(0x32,0x88);

    data_buf[0] = 0x01;
    data_buf[1] = 0x21;
    SPI_Write_Buffer(0x13, data_buf, 2);
    data_buf[0] = 0x01;
    data_buf[1] = 0x20;
    SPI_Write_Buffer(0x13, data_buf, 2);
    SPI_Write_Reg(0x35,0x00);  //exist testm
    ////////////////////////////////////////////////////
    SPI_Write_Reg(0x50, 0x56);
}

/*******************************************************************************
* Function   :     	BLE_Init
* Parameter  :     	void
* Returns    :     	void
* Description:      power on .BLE must initialize reg .
* Note:      : 		delay 30ms .
*******************************************************************************/
void BLE_Init(void)
{
    uint8_t status;
    uint8_t data_buf[4];
    uint8_t ble_Addr[6];

    LED_RED_ON();
	Delay_ms(30);

	Uart_Send_String("BLE init \r\n");

    SPI_Write_Reg(0x50, 0x51);
    SPI_Write_Reg(0x50, 0x53);
    SPI_Write_Reg(0x35, 0x00);
    SPI_Write_Reg(0x3D, 0x18);
    SPI_Write_Reg(0x50, 0x51);

    do{
	SPI_Write_Reg(0x50, 0x53);
        data_buf[0] = 0;
        data_buf[1] = 0;
        data_buf[2] = 1;
        SPI_Write_Buffer(0x00, data_buf, 3);

        SPI_Write_Reg(0x36, 0x8e);
        SPI_Write_Reg(0x37, 0x8e);
        SPI_Write_Reg(0x38, 0x88);
        SPI_Write_Reg(0x39, 0x8e);

        SPI_Write_Reg(0x50, 0x51);

        SPI_Read_Reg(0x1e);

        status = SPI_Read_Reg(CHIP_OK);
    }while(status != 0x80);

    LED_RED_OFF();

    //read chip version
    status = SPI_Read_Reg(0x1e);
    Uart_Send_String("chip version=");
    Uart_Send_Byte(status);
    Uart_Send_String("\r\n");


    SPI_Write_Reg(0X20, 0x78);//power down,tx, for hot reset
    SPI_Write_Reg(0X26, 0x06);//1Mbps
    SPI_Write_Reg(0X20, 0x7a);//power up

    SPI_Write_Reg(0x50, 0x56);

    BLE_Mode_Sleep();

    //read BLE address. BLE MAC Address
    SPI_Read_Buffer(0x08, ble_Addr, 6);

	Uart_Send_String("BleAddr=");
	Uart_Send_Byte(ble_Addr[5]);
	Uart_Send_Byte(ble_Addr[4]);
	Uart_Send_Byte(ble_Addr[3]);
	Uart_Send_Byte(ble_Addr[2]);
	Uart_Send_Byte(ble_Addr[1]);
	Uart_Send_Byte(ble_Addr[0]);
	Uart_Send_String("\r\n");

#ifndef NO_HELP
	//Workmode change .ready ble data
	if(Work_Mode == Work_Mode_iBeacon)
	{
		for(temp0=0;temp0<(sizeof(iBeacon_adv_data)/sizeof(uint8_t));temp0++)
		{
			adv_data[temp0] = iBeacon_adv_data[temp0];
		}
	}

	else if(Work_Mode == Work_Mode_Eddystone_Uri)
	{
		for(temp0=0;temp0<(sizeof(Eddystone_Uri_adv_data)/sizeof(uint8_t));temp0++)
		{
			adv_data[temp0] = Eddystone_Uri_adv_data[temp0];
		}
	}

	else if(Work_Mode == Work_Mode_Eddystone_Uid)
	{
		for(temp0=0;temp0<(sizeof(Eddystone_Uid_adv_data)/sizeof(uint8_t));temp0++)
		{
			adv_data[temp0] = Eddystone_Uid_adv_data[temp0];
		}
	}

	else if(Work_Mode == Work_Mode_Eddystone_Tlm)
	{
		for(temp0=0;temp0<(sizeof(Eddystone_Tlm_adv_data)/sizeof(uint8_t));temp0++)
		{
			adv_data[temp0] = Eddystone_Tlm_adv_data[temp0];
		}
	}

	else if(Work_Mode == Work_Mode_Remote)
	{
		if(KEY_GET() != RESET)
		{
			for(temp0=0;temp0<(sizeof(Beacon_Key_Press_adv_data)/sizeof(uint8_t));temp0++)
			{
				adv_data[temp0] = Beacon_Key_Press_adv_data[temp0];
			}
		}
		else
		{
			for(temp0=0;temp0<(sizeof(Beacon_Key_uP_adv_data)/sizeof(uint8_t));temp0++)
			{
				adv_data[temp0] = Beacon_Key_uP_adv_data[temp0];
			}
		}
	}
#endif

    //set BLE TX default channel:37.38.39
	SPI_Write_Reg(CH_NO|0X20, 37);

	//BLT FIFO write adv_data . max len:31 byte
	SPI_Write_Buffer(W_TX_PAYLOAD, adv_wechat, sizeof(adv_wechat));

	//set BLT PDU length:adv_data+6 mac adress.
	data_buf[0] = 0x02;
	data_buf[1] = sizeof(adv_wechat)+LEN_BLE_ADDR;

	//PDU TYPE: 2  non-connectable undirected advertising . tx add:random address
	SPI_Write_Buffer(ADV_HDR_TX, data_buf, 2);

    SPI_Write_Reg(0x50, 0x53);

	//data_buf[0] = 0x8f;
	//data_buf[1] = 0x80; //xocc
	//SPI_Write_Buffer(0x14,data_buf,2);

    //set BLE TX Power
	data_buf[0] = 0x02;
	data_buf[1] = BLE_TX_POWER;
	SPI_Write_Buffer(0x0f,data_buf,2);
	
    data_buf[1] = SPI_Read_Reg(0x08);  //txgain
    if(0 == data_buf[1]){
      data_buf[1] = TXGAIN_DFF;
    }
    data_buf[0] = 0xc0;
    data_buf[2] = 0x1D; 
    SPI_Write_Buffer(0x4, data_buf, 3);


    data_buf[0] = 0;
    data_buf[1] = 0x00; //default08  20161212
    SPI_Write_Buffer(0x0C, data_buf, 2);

    data_buf[0] = 0x81;
    data_buf[1] = 0x22;
    SPI_Write_Buffer(0x13, data_buf, 2);

    SPI_Write_Reg(0x3C, 0x30);
    SPI_Write_Reg(0x3E, 0x30);

    data_buf[0] = 0x10;
    data_buf[1] = 0x02;
    SPI_Write_Buffer(0xA, data_buf, 2);

    data_buf[0] = 0x80;
    data_buf[1] = 0x40;
    data_buf[2] = 0x21;
    data_buf[3] = 0x10;
    SPI_Write_Buffer(0xB, data_buf, 4);

    data_buf[0] = 0x02;
    data_buf[1] = 0x12;
    SPI_Write_Buffer(0xD, data_buf, 2);

    data_buf[0] = 0x01;
    data_buf[1] = 0x07;
    SPI_Write_Buffer(0xE, data_buf, 2);

    BLE_Do_Cal();

    Uart_Send_String("BLE_Beacon mode ");
    Uart_Send_Byte(Work_Mode-'0');
    Uart_Send_String("\r\n");

    //clear all interrupt
    data_buf[0] = 0xff;
    data_buf[1] = 0x80;
    SPI_Write_Buffer(INT_FLAG, data_buf, 2);
    
    //BLE Wakeup mode
    BLE_Mode_Wakeup();
}

/*******************************************************************************
* Function   :     	BLE_TRX
* Parameter  :     	txcnt, rxcnt
* Returns    :     	void
* Description:      Beacon data .process .
* Note:      :
*******************************************************************************/
void BLE_TRX(uint8_t txcnt, uint8_t rxcnt)
{
    uint8_t status = 0;
    uint8_t ch = 37;
    static uint8_t flag_pressed = 0;
    uint8_t flag_tx = 1;
    uint8_t cnt = txcnt;
    uint8_t len_pdu = 0;
//    uint8_t loop = 0;

    LED_RED_ON();
    BLE_Set_TimeOut(0x7fff); //3fff-16,383us   7fff-32,767us
    tick = CNT_SLEEP;

    while(1)
    {
        //BLE IRQ LOW
        if (!BLE_IRQ_GET())
        {
            //clear interrupt flag
            status = SPI_Read_Reg(INT_FLAG);
            SPI_Write_Reg(INT_FLAG|0X20, status);

            if(INT_TYPE_WAKEUP == status)//wakeup
            {
                if(cnt > 0){
                    cnt --;
                    if(flag_tx){
                        SPI_Write_Reg(MODE_TYPE|0X20, RADIO_MODE_ADV_TX);
                        BLE_Set_StartTime(BLE_START_TIME);
                    }else{
                        SPI_Write_Reg(MODE_TYPE|0X20, RADIO_MODE_ADV_RX);
                        BLE_Set_StartTime(BLE_START_TIME);
                    }
                    continue; //goto while(1)
                }
            }

            BLE_Mode_Sleep();

            if(INT_TYPE_PDU_OK & status){  //for rx. no need in tx only application
                LED_RED_ON();
                BLE_Get_Pdu(rx_buf, &len_pdu);
#if 0 //dbg print
                Uart_Send_String("\r\n");
                for(loop=0; loop<len_pdu; loop++){
                    Uart_Send_Byte(rx_buf[loop]);
                    Uart_Send_String(" ");
                }
#endif
            }else if(INT_TYPE_TX_START & status){  //for tx. no need in rx only application
                LED_GREEN_ON();
            }

            if(INT_TYPE_SLEEP & status)//sleep
            {
                LED_GREEN_OFF();
                LED_RED_OFF();
                IWDG_ReloadCounter();
                BLE_Set_Xtal(0);
                tick = CNT_SLEEP;

		//BLE channel
                if (++ch > 39){
                    ch = 37;
                }
                SPI_Write_Reg(CH_NO|0X20, ch);

                if(1 == flag_pressed)
                {
                    TIM2_Cmd(DISABLE);
                    SPI_Write_Reg(0X50, 0x51);
                    SPI_Write_Reg(0X20, 0x78); //pwr down
                    SPI_Write_Reg(0X50, 0x56);
                }

                if(cnt == 0){
                    if(flag_tx && (rxcnt>0)){
                        flag_tx = 0;
                    }else if((flag_tx==0) && (txcnt>0)){
                        flag_tx = 1;
                    }
                    if(flag_tx){
                        cnt = txcnt;
                    }else{
                        cnt = rxcnt;
                    }
			
		    SPI_Write_Reg(0X50, 0x53);
		    BLE_Do_Cal();
                }
            }

	}
        else{
            if(tick == 0){
                BLE_Set_Xtal(1);

                tick = 2; //2ms delay then wakeup
                while(tick){};

                BLE_Mode_Wakeup();
                tick = 0xffff;
            }
        }

        if(KEY_GET() == RESET)
        {
            if(flag_pressed == 0){
                flag_pressed = 1;
            }
            LED_RED_ON();
            IWDG_ReloadCounter();

        }else {
            if(flag_pressed == 1) {
                LED_RED_OFF();
                TIM2_Cmd(ENABLE);
                SPI_Write_Reg(0X50, 0x51);
                SPI_Write_Reg(0X20, 0x7a); //pwr up
                SPI_Write_Reg(0X50, 0x56);
                tick = 1;
            }
            flag_pressed = 0;
        }
    }
}
