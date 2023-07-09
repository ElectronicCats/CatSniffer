/******************************************************************************

 @file  airtag_spoofer.c

 @brief This file contains the the CatSniffer Airtag Spoofer application for use
        with the Bluetooth Low Energy Protocol Stack.

 Group: WCS, BTS
 Target Device: cc13xx_cc26xx

 ******************************************************************************
 
 Copyright (c) 2013-2023, Texas Instruments Incorporated
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 *  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

 *  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 *  Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 ******************************************************************************
 
 
 *****************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Queue.h>

#include <ti/display/Display.h>

#if (!(defined __TI_COMPILER_VERSION__) && !(defined __GNUC__))
#include <intrinsics.h>
#endif

#include <ti/drivers/GPIO.h>
#include <ti/drivers/utils/List.h>

#include <icall.h>
#include "util.h"
#include <bcomdef.h>
/* This Header file contains all BLE API and icall structure definition */
#include <icall_ble_api.h>

#include <devinfoservice.h>
#include <simple_gatt_profile.h>

#ifdef USE_RCOSC
#include <rcosc_calibration.h>
#endif //USE_RCOSC

#include <ti_drivers_config.h>
#include <board_key.h>

#include <airtag_spoofer.h>
#include "ti_ble_config.h"

#ifdef PTM_MODE
#include "npi_task.h"               // To allow RX event registration
#include "npi_ble.h"                // To enable transmission of messages to UART
#include "icall_hci_tl.h"   // To allow ICall HCI Transport Layer
#endif // PTM_MODE


/*********************************************************************
 * MACROS
 */
// Spin if the expression is not true
#define AIRTAGSPOOFER_ASSERT(expr) if (!(expr)) AirtagSpoofer_spin();

/*********************************************************************
 * CONSTANTS
 */
// How often to perform periodic event (in ms)
#define SP_PERIODIC_EVT_PERIOD               5000

// Task configuration
#define SP_TASK_PRIORITY                     1

#ifndef SP_TASK_STACK_SIZE
#define SP_TASK_STACK_SIZE                   1024
#endif

// Application events
#define SP_STATE_CHANGE_EVT                  0
#define SP_CHAR_CHANGE_EVT                   1
#define SP_KEY_CHANGE_EVT                    2
#define SP_ADV_EVT                           3
#define SP_PAIR_STATE_EVT                    4
#define SP_PASSCODE_EVT                      5
#define SP_PERIODIC_EVT                      6
#define SP_READ_RPA_EVT                      7
#define SP_SEND_PARAM_UPDATE_EVT             8
#define SP_CONN_EVT                          9

// Internal Events for RTOS application
#define SP_ICALL_EVT                         ICALL_MSG_EVENT_ID // Event_Id_31
#define SP_QUEUE_EVT                         UTIL_QUEUE_EVENT_ID // Event_Id_30

// Bitwise OR of all RTOS events to pend on
#define SP_ALL_EVENTS                        (SP_ICALL_EVT             | \
                                              SP_QUEUE_EVT)

// Size of string-converted device address ("0xXXXXXXXXXXXX")
#define SP_ADDR_STR_SIZE     15

// For storing the active connections
#define SP_RSSI_TRACK_CHNLS        1            // Max possible channels can be GAP_BONDINGS_MAX
#define SP_MAX_RSSI_STORE_DEPTH    5
#define SP_INVALID_HANDLE          0xFFFF
#define RSSI_2M_THRSHLD           -30           
#define RSSI_1M_THRSHLD           -40           
#define RSSI_S2_THRSHLD           -50           
#define RSSI_S8_THRSHLD           -60           
#define SP_PHY_NONE                LL_PHY_NONE  // No PHY set
#define AUTO_PHY_UPDATE            0xFF


/*********************************************************************
 * TYPEDEFS
 */

// Auto connect availble groups
enum
{
  AUTOCONNECT_DISABLE = 0,              // Disable
  AUTOCONNECT_GROUP_A = 1,              // Group A
  AUTOCONNECT_GROUP_B = 2               // Group B
};


// App event passed from stack modules. This type is defined by the application
// since it can queue events to itself however it wants.
typedef struct
{
  uint8_t event;                // event type
  void    *pData;               // pointer to message
} spEvt_t;

// Container to store passcode data when passing from gapbondmgr callback
// to app event. See the pfnPairStateCB_t documentation from the gapbondmgr.h
// header file for more information on each parameter.
typedef struct
{
  uint8_t state;
  uint16_t connHandle;
  uint8_t status;
} spPairStateData_t;

// Container to store passcode data when passing from gapbondmgr callback
// to app event. See the pfnPasscodeCB_t documentation from the gapbondmgr.h
// header file for more information on each parameter.
typedef struct
{
  uint8_t deviceAddr[B_ADDR_LEN];
  uint16_t connHandle;
  uint8_t uiInputs;
  uint8_t uiOutputs;
  uint32_t numComparison;
} spPasscodeData_t;

// Container to store advertising event data when passing from advertising
// callback to app event. See the respective event in GapAdvScan_Event_IDs
// in gap_advertiser.h for the type that pBuf should be cast to.
typedef struct
{
  uint32_t event;
  void *pBuf;
} spGapAdvEventData_t;

// Container to store information from clock expiration using a flexible array
// since data is not always needed
typedef struct
{
  uint8_t event;                //
  uint8_t data[];
} spClockEventData_t;

// List element for parameter update and PHY command status lists
typedef struct
{
  List_Elem elem;
  uint16_t  connHandle;
} spConnHandleEntry_t;

// Connected device information
typedef struct
{
  uint16_t         	    connHandle;                        // Connection Handle
  spClockEventData_t*   pParamUpdateEventData;
  Clock_Struct*    	    pUpdateClock;                      // pointer to clock struct
  int8_t           	    rssiArr[SP_MAX_RSSI_STORE_DEPTH];
  uint8_t          	    rssiCntr;
  int8_t           	    rssiAvg;
  bool             	    phyCngRq;                          // Set to true if PHY change request is in progress
  uint8_t          	    currPhy;
  uint8_t          	    rqPhy;
  uint8_t          	    phyRqFailCnt;                      // PHY change request count
  bool             	    isAutoPHYEnable;                   // Flag to indicate auto phy change
} spConnRec_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Display Interface
Display_Handle dispHandle = NULL;

// Task configuration
Task_Struct spTask;
#if defined __TI_COMPILER_VERSION__
#pragma DATA_ALIGN(spTaskStack, 8)
#else
#pragma data_alignment=8
#endif
uint8_t spTaskStack[SP_TASK_STACK_SIZE];

#define APP_EVT_EVENT_MAX 0x9
char *appEventStrings[] = {
  "APP_STATE_CHANGE_EVT     ",
  "APP_CHAR_CHANGE_EVT      ",
  "APP_KEY_CHANGE_EVT       ",
  "APP_ADV_EVT              ",
  "APP_PAIR_STATE_EVT       ",
  "APP_PASSCODE_EVT         ",
  "APP_READ_RPA_EVT         ",
  "APP_PERIODIC_EVT         ",
  "APP_SEND_PARAM_UPDATE_EVT",
  "APP_CONN_EVT             ",
};

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Event globally used to post local events and pend on system and
// local events.
static ICall_SyncHandle syncEvent;

// Queue object used for app messages
static Queue_Struct appMsgQueue;
static Queue_Handle appMsgQueueHandle;

// Clock instance for internal periodic events. Only one is needed since
// GattServApp will handle notifying all connected GATT clients
static Clock_Struct clkPeriodic;
// Clock instance for RPA read events.
static Clock_Struct clkRpaRead;

// Memory to pass periodic event ID to clock handler
spClockEventData_t argPeriodic =
{ .event = SP_PERIODIC_EVT };

// Memory to pass RPA read event ID to clock handler
spClockEventData_t argRpaRead =
{ .event = SP_READ_RPA_EVT };

// Per-handle connection info
static spConnRec_t connList[MAX_NUM_BLE_CONNS];

// List to store connection handles for set phy command status's
static List_List setPhyCommStatList;

// List to store connection handles for queued param updates
static List_List paramUpdateList;

// Auto connect Disabled/Enabled {0 - Disabled, 1- Group A , 2-Group B, ...}
uint8_t autoConnect = AUTOCONNECT_DISABLE;

// Advertising handles
static uint8 advHandleLegacy;
static uint8 advHandleLongRange;

// Address mode
static GAP_Addr_Modes_t addrMode = DEFAULT_ADDRESS_MODE;

// Current Random Private Address
static uint8 rpa[B_ADDR_LEN] = {0};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void AirtagSpoofer_init( void );
static void AirtagSpoofer_taskFxn(UArg a0, UArg a1);

static uint8_t AirtagSpoofer_processStackMsg(ICall_Hdr *pMsg);
static uint8_t AirtagSpoofer_processGATTMsg(gattMsgEvent_t *pMsg);
static void AirtagSpoofer_processGapMessage(gapEventHdr_t *pMsg);
static void AirtagSpoofer_advCallback(uint32_t event, void *pBuf, uintptr_t arg);
static void AirtagSpoofer_processAdvEvent(spGapAdvEventData_t *pEventData);
static void AirtagSpoofer_processAppMsg(spEvt_t *pMsg);
static void AirtagSpoofer_processCharValueChangeEvt(uint8_t paramId);
static void AirtagSpoofer_performPeriodicTask(void);
static void AirtagSpoofer_updateRPA(void);
static void AirtagSpoofer_clockHandler(UArg arg);
static void AirtagSpoofer_passcodeCb(uint8_t *pDeviceAddr, uint16_t connHandle,
                                        uint8_t uiInputs, uint8_t uiOutputs,
                                        uint32_t numComparison);
static void AirtagSpoofer_pairStateCb(uint16_t connHandle, uint8_t state,
                                         uint8_t status);
static void AirtagSpoofer_processPairState(spPairStateData_t *pPairState);
static void AirtagSpoofer_processPasscode(spPasscodeData_t *pPasscodeData);
static void AirtagSpoofer_charValueChangeCB(uint8_t paramId);
static status_t AirtagSpoofer_enqueueMsg(uint8_t event, void *pData);
static void AirtagSpoofer_keyChangeHandler(uint8 keys);
static void AirtagSpoofer_processCmdCompleteEvt(hciEvt_CmdComplete_t *pMsg);
static void AirtagSpoofer_initPHYRSSIArray(void);
static void AirtagSpoofer_updatePHYStat(uint16_t eventCode, uint8_t *pMsg);
static uint8_t AirtagSpoofer_addConn(uint16_t connHandle);
static uint8_t AirtagSpoofer_getConnIndex(uint16_t connHandle);
static uint8_t AirtagSpoofer_removeConn(uint16_t connHandle);
static void AirtagSpoofer_processParamUpdate(uint16_t connHandle);
static status_t AirtagSpoofer_startAutoPhyChange(uint16_t connHandle);
static status_t AirtagSpoofer_stopAutoPhyChange(uint16_t connHandle);
static status_t AirtagSpoofer_setPhy(uint16_t connHandle, uint8_t allPhys,
                                        uint8_t txPhy, uint8_t rxPhy,
                                        uint16_t phyOpts);
static uint8_t AirtagSpoofer_clearConnListEntry(uint16_t connHandle);
static void AirtagSpoofer_connEvtCB(Gap_ConnEventRpt_t *pReport);
static void AirtagSpoofer_processConnEvt(Gap_ConnEventRpt_t *pReport);

/*********************************************************************
 * EXTERN FUNCTIONS
 */
extern void AssertHandler(uint8 assertCause, uint8 assertSubcause);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Bond Manager Callbacks
static gapBondCBs_t AirtagSpoofer_BondMgrCBs =
{
  AirtagSpoofer_passcodeCb,       // Passcode callback
  AirtagSpoofer_pairStateCb       // Pairing/Bonding state Callback
};

// Simple GATT Profile Callbacks
static simpleProfileCBs_t AirtagSpoofer_simpleProfileCBs =
{
  AirtagSpoofer_charValueChangeCB // Simple GATT Characteristic value change callback
};


/*********************************************************************
 * @fn      AirtagSpoofer_createTask
 *
 * @brief   Task creation function for the Simple Peripheral.
 */
void AirtagSpoofer_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = spTaskStack;
  taskParams.stackSize = SP_TASK_STACK_SIZE;
  taskParams.priority = SP_TASK_PRIORITY;

  Task_construct(&spTask, AirtagSpoofer_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      AirtagSpoofer_spin
 *
 * @brief   Spin forever
 *
 * @param   none
 */
static void AirtagSpoofer_spin(void)
{
  volatile uint8_t x;

  while(1)
  {
    x++;
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 */
static void AirtagSpoofer_init(void)
{
  BLE_LOG_INT_TIME(0, BLE_LOG_MODULE_APP, "APP : ---- init ", SP_TASK_PRIORITY);

  // ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &syncEvent);

#ifdef USE_RCOSC
  // Set device's Sleep Clock Accuracy
#if ( HOST_CONFIG & ( CENTRAL_CFG | PERIPHERAL_CFG ) )
  HCI_EXT_SetSCACmd(500);
#endif // (CENTRAL_CFG | PERIPHERAL_CFG)
  RCOSC_enableCalibration();
#endif // USE_RCOSC

  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueueHandle = Util_constructQueue(&appMsgQueue);

  // Create one-shot clock for internal periodic events.
  Util_constructClock(&clkPeriodic, AirtagSpoofer_clockHandler,
                      SP_PERIODIC_EVT_PERIOD, 0, false, (UArg)&argPeriodic);

  // Set the Device Name characteristic in the GAP GATT Service
  // For more information, see the section in the User's Guide:
  // http://software-dl.ti.com/lprf/ble5stack-latest/
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

  // Configure GAP
  {
    uint16_t paramUpdateDecision = DEFAULT_PARAM_UPDATE_REQ_DECISION;

    // Pass all parameter update requests to the app for it to decide
    GAP_SetParamValue(GAP_PARAM_LINK_UPDATE_DECISION, paramUpdateDecision);
  }

  // Setup the GAP Bond Manager. For more information see the GAP Bond Manager
  // section in the User's Guide
  setBondManagerParameters();

  // Initialize GATT attributes
  GGS_AddService(GAP_SERVICE);                 // GAP GATT Service
  GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT Service
  DevInfo_AddService();                        // Device Information Service
  SimpleProfile_AddService(GATT_ALL_SERVICES); // Simple GATT Profile

  // Setup the SimpleProfile Characteristic Values
  // For more information, see the GATT and GATTServApp sections in the User's Guide:
  // http://software-dl.ti.com/lprf/ble5stack-latest/
  {
    uint8_t charValue1 = 1;
    uint8_t charValue2 = 2;
    uint8_t charValue3 = 3;
    uint8_t charValue4 = 4;
    uint8_t charValue5[SIMPLEPROFILE_CHAR5_LEN] = { 1, 2, 3, 4, 5 };

    SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR1, sizeof(uint8_t),
                               &charValue1);
    SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR2, sizeof(uint8_t),
                               &charValue2);
    SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR3, sizeof(uint8_t),
                               &charValue3);
    SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR4, sizeof(uint8_t),
                               &charValue4);
    SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR5, SIMPLEPROFILE_CHAR5_LEN,
                               charValue5);
  }

  // Register callback with SimpleGATTprofile
  SimpleProfile_RegisterAppCBs(&AirtagSpoofer_simpleProfileCBs);

  // Start Bond Manager and register callback
  VOID GAPBondMgr_Register(&AirtagSpoofer_BondMgrCBs);

  // Register with GAP for HCI/Host messages. This is needed to receive HCI
  // events. For more information, see the HCI section in the User's Guide:
  // http://software-dl.ti.com/lprf/ble5stack-latest/
  GAP_RegisterForMsgs(selfEntity);

  // Register for GATT local events and ATT Responses pending for transmission
  GATT_RegisterForMsgs(selfEntity);

  // Set default values for Data Length Extension
  // Extended Data Length Feature is already enabled by default
  {
    // Set initial values to maximum, RX is set to max. by default(251 octets, 2120us)
    // Some brand smartphone is essentially needing 251/2120, so we set them here.
    #define APP_SUGGESTED_PDU_SIZE 251 //default is 27 octets(TX)
    #define APP_SUGGESTED_TX_TIME 2120 //default is 328us(TX)

    // This API is documented in hci.h
    // See the LE Data Length Extension section in the BLE5-Stack User's Guide for information on using this command:
    // http://software-dl.ti.com/lprf/ble5stack-latest/
    HCI_LE_WriteSuggestedDefaultDataLenCmd(APP_SUGGESTED_PDU_SIZE, APP_SUGGESTED_TX_TIME);
  }

  // Initialize GATT Client
  GATT_InitClient();

  // Initialize Connection List
  AirtagSpoofer_clearConnListEntry(LINKDB_CONNHANDLE_ALL);

  BLE_LOG_INT_TIME(0, BLE_LOG_MODULE_APP, "APP : ---- call GAP_DeviceInit", GAP_PROFILE_PERIPHERAL);
  //Initialize GAP layer for Peripheral role and register to receive GAP events
  GAP_DeviceInit(GAP_PROFILE_PERIPHERAL, selfEntity, addrMode, &pRandomAddress);

  // Initialize array to store connection handle and RSSI values
  AirtagSpoofer_initPHYRSSIArray();

  // The type of display is configured based on the BOARD_DISPLAY_USE...
  // preprocessor definitions
  dispHandle = Display_open(Display_Type_ANY, NULL);

}

/*********************************************************************
 * @fn      AirtagSpoofer_taskFxn
 *
 * @brief   Application task entry point for the Simple Peripheral.
 *
 * @param   a0, a1 - not used.
 */
static void AirtagSpoofer_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  AirtagSpoofer_init();
  Display_printf(dispHandle, 0, 0, "CatSniffer:AirTagSpoofer");
  // Application main loop
  for (;;)
  {
    uint32_t events;

    // Waits for an event to be posted associated with the calling thread.
    // Note that an event associated with a thread is posted when a
    // message is queued to the message receive queue of the thread
    events = Event_pend(syncEvent, Event_Id_NONE, SP_ALL_EVENTS,
                        ICALL_TIMEOUT_FOREVER);

    if (events)
    {
      ICall_EntityID dest;
      ICall_ServiceEnum src;
      ICall_HciExtEvt *pMsg = NULL;

      // Fetch any available messages that might have been sent from the stack
      if (ICall_fetchServiceMsg(&src, &dest,
                                (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
      {
        uint8 safeToDealloc = TRUE;

        if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == selfEntity))
        {
          ICall_Stack_Event *pEvt = (ICall_Stack_Event *)pMsg;

          // Check for BLE stack events first
          if (pEvt->signature != 0xffff)
          {
            // Process inter-task message
            safeToDealloc = AirtagSpoofer_processStackMsg((ICall_Hdr *)pMsg);
          }
        }

        if (pMsg && safeToDealloc)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      if (events & SP_QUEUE_EVT)
      {
        while (!Queue_empty(appMsgQueueHandle))
        {
          spEvt_t *pMsg = (spEvt_t *)Util_dequeueMsg(appMsgQueueHandle);
          if (pMsg)
          {
            // Process message.
            AirtagSpoofer_processAppMsg(pMsg);

            // Free the space from the message.
            ICall_free(pMsg);
          }
        }
      }
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  TRUE if safe to deallocate incoming message, FALSE otherwise.
 */
static uint8_t AirtagSpoofer_processStackMsg(ICall_Hdr *pMsg)
{
  // Always dealloc pMsg unless set otherwise
  uint8_t safeToDealloc = TRUE;

  BLE_LOG_INT_INT(0, BLE_LOG_MODULE_APP, "APP : Stack msg status=%d, event=0x%x\n", pMsg->status, pMsg->event);

  switch (pMsg->event)
  {
    case GAP_MSG_EVENT:
      AirtagSpoofer_processGapMessage((gapEventHdr_t*) pMsg);
      break;

    case GATT_MSG_EVENT:
      // Process GATT message
      safeToDealloc = AirtagSpoofer_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    case HCI_GAP_EVENT_EVENT:
    {
      // Process HCI message
      switch(pMsg->status)
      {
        case HCI_COMMAND_COMPLETE_EVENT_CODE:
        // Process HCI Command Complete Events here
        {
          AirtagSpoofer_processCmdCompleteEvt((hciEvt_CmdComplete_t *) pMsg);
          break;
        }

        case HCI_BLE_HARDWARE_ERROR_EVENT_CODE:
          AssertHandler(HAL_ASSERT_CAUSE_HARDWARE_ERROR,0);
          break;

        // HCI Commands Events
        case HCI_COMMAND_STATUS_EVENT_CODE:
        {
          hciEvt_CommandStatus_t *pMyMsg = (hciEvt_CommandStatus_t *)pMsg;
          switch ( pMyMsg->cmdOpcode )
          {
            case HCI_LE_SET_PHY:
            {
              if (pMyMsg->cmdStatus == HCI_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE)
              {
                //Display_printf(dispHandle, SP_ROW_STATUS_1, 0,"PHY Change failure, peer does not support this");
              }
              else
              {
                //Display_printf(dispHandle, SP_ROW_STATUS_1, 0,"PHY Update Status Event: 0x%x",pMyMsg->cmdStatus);
              }

              AirtagSpoofer_updatePHYStat(HCI_LE_SET_PHY, (uint8_t *)pMsg);
              break;
            }

            default:
              break;
          }
          break;
        }

        // LE Events
        case HCI_LE_EVENT_CODE:
        {
          hciEvt_BLEPhyUpdateComplete_t *pPUC =
            (hciEvt_BLEPhyUpdateComplete_t*) pMsg;

          // A Phy Update Has Completed or Failed
          if (pPUC->BLEEventCode == HCI_BLE_PHY_UPDATE_COMPLETE_EVENT)
          {
            if (pPUC->status != SUCCESS)
            {
              //Display_printf(dispHandle, SP_ROW_STATUS_1, 0,"PHY Change failure");
            }
            else
            {
              // Only symmetrical PHY is supported.
              // rxPhy should be equal to txPhy.
              //Display_printf(dispHandle, SP_ROW_STATUS_2, 0,
                //   "PHY Updated to %s",
                //    (pPUC->rxPhy == PHY_UPDATE_COMPLETE_EVENT_1M) ? "1M" :
                //   (pPUC->rxPhy == PHY_UPDATE_COMPLETE_EVENT_2M) ? "2M" :
                //  (pPUC->rxPhy == PHY_UPDATE_COMPLETE_EVENT_CODED) ? "CODED" : "Unexpected PHY Value");
            }

            AirtagSpoofer_updatePHYStat(HCI_BLE_PHY_UPDATE_COMPLETE_EVENT, (uint8_t *)pMsg);
          }
          break;
        }

        default:
          break;
      }

      break;
    }

    default:
      // do nothing
      break;
  }

  return (safeToDealloc);
}

/*********************************************************************
 * @fn      AirtagSpoofer_processGATTMsg
 *
 * @brief   Process GATT messages and events.
 *
 * @return  TRUE if safe to deallocate incoming message, FALSE otherwise.
 */
static uint8_t AirtagSpoofer_processGATTMsg(gattMsgEvent_t *pMsg)
{
  if (pMsg->method == ATT_FLOW_CTRL_VIOLATED_EVENT)
  {
    // ATT request-response or indication-confirmation flow control is
    // violated. All subsequent ATT requests or indications will be dropped.
    // The app is informed in case it wants to drop the connection.

    // Display the opcode of the message that caused the violation.
    //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "FC Violated: %d", pMsg->msg.flowCtrlEvt.opcode);
  }
  else if (pMsg->method == ATT_MTU_UPDATED_EVENT)
  {
    // MTU size updated
    //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "MTU Size: %d", pMsg->msg.mtuEvt.MTU);
  }

  // Free message payload. Needed only for ATT Protocol messages
  GATT_bm_free(&pMsg->msg, pMsg->method);

  // It's safe to free the incoming message
  return (TRUE);
}

/*********************************************************************
 * @fn      AirtagSpoofer_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */
static void AirtagSpoofer_processAppMsg(spEvt_t *pMsg)
{
  bool dealloc = TRUE;

  if (pMsg->event <= APP_EVT_EVENT_MAX)
  {
    BLE_LOG_INT_STR(0, BLE_LOG_MODULE_APP, "APP : App msg status=%d, event=%s\n", 0, appEventStrings[pMsg->event]);
  }
  else
  {
    BLE_LOG_INT_INT(0, BLE_LOG_MODULE_APP, "APP : App msg status=%d, event=0x%x\n", 0, pMsg->event);
  }

  switch (pMsg->event)
  {
    case SP_CHAR_CHANGE_EVT:
      AirtagSpoofer_processCharValueChangeEvt(*(uint8_t*)(pMsg->pData));
      break;

    case SP_ADV_EVT:
      AirtagSpoofer_processAdvEvent((spGapAdvEventData_t*)(pMsg->pData));
      break;

    case SP_PAIR_STATE_EVT:
      AirtagSpoofer_processPairState((spPairStateData_t*)(pMsg->pData));
      break;

    case SP_PASSCODE_EVT:
      AirtagSpoofer_processPasscode((spPasscodeData_t*)(pMsg->pData));
      break;

    case SP_PERIODIC_EVT:
      AirtagSpoofer_performPeriodicTask();
      break;

    case SP_READ_RPA_EVT:
      AirtagSpoofer_updateRPA();
      break;

    case SP_SEND_PARAM_UPDATE_EVT:
    {
      // Extract connection handle from data
      uint16_t connHandle = *(uint16_t *)(((spClockEventData_t *)pMsg->pData)->data);

      AirtagSpoofer_processParamUpdate(connHandle);

      // This data is not dynamically allocated
      dealloc = FALSE;
      break;
    }

    case SP_CONN_EVT:
      AirtagSpoofer_processConnEvt((Gap_ConnEventRpt_t *)(pMsg->pData));
      break;

    default:
      // Do nothing.
      break;
  }

  // Free message data if it exists and we are to dealloc
  if ((dealloc == TRUE) && (pMsg->pData != NULL))
  {
    ICall_free(pMsg->pData);
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processGapMessage
 *
 * @brief   Process an incoming GAP event.
 *
 * @param   pMsg - message to process
 */
static void AirtagSpoofer_processGapMessage(gapEventHdr_t *pMsg)
{
  switch(pMsg->opcode)
  {
    case GAP_DEVICE_INIT_DONE_EVENT:
    {
      bStatus_t status = FAILURE;

      gapDeviceInitDoneEvent_t *pPkt = (gapDeviceInitDoneEvent_t *)pMsg;

      if(pPkt->hdr.status == SUCCESS)
      {
        // Store the system ID
        uint8_t systemId[DEVINFO_SYSTEM_ID_LEN];

        // use 6 bytes of device address for 8 bytes of system ID value
        systemId[0] = pPkt->devAddr[0];
        systemId[1] = pPkt->devAddr[1];
        systemId[2] = pPkt->devAddr[2];

        // set middle bytes to zero
        systemId[4] = 0x00;
        systemId[3] = 0x00;

        // shift three bytes up
        systemId[7] = pPkt->devAddr[5];
        systemId[6] = pPkt->devAddr[4];
        systemId[5] = pPkt->devAddr[3];

        // Set Device Info Service Parameter
        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);

        //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "Initialized");

        BLE_LOG_INT_TIME(0, BLE_LOG_MODULE_APP, "APP : ---- got GAP_DEVICE_INIT_DONE_EVENT", 0);
        // Setup and start Advertising
        // For more information, see the GAP section in the User's Guide:
        // http://software-dl.ti.com/lprf/ble5stack-latest/
        advParams1.primIntMin = 0x80;
        advParams1.primIntMax = 0x100;

        advParams1.txPower = 0;
        advParams1.primChanMap = GAP_ADV_CHAN_ALL;
        BLE_LOG_INT_INT(0, BLE_LOG_MODULE_APP, "APP : ---- call GapAdv_create set=%d,%d\n", 0, 0);
        // Create Advertisement set #1 and assign handle
        status = GapAdv_create(&AirtagSpoofer_advCallback, &advParams1,
                               &advHandleLegacy);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);

        uint8_t airtag_adv_data[] = {
                                     0x1E, 0xFF, 0x4C, 0x00, 0x12, 0x19, 0x10, 0x62,
                                     0x99, 0x96, 0x85, 0xAB, 0x4C, 0x0A, 0xB4, 0xA1,
                                     0xD6, 0xA4, 0x13, 0xA1, 0x9B, 0x30, 0x84, 0x4D,
                                     0x60, 0x70, 0x59, 0xCF, 0xA2, 0x01, 0x45
            };
        // Load advertising data for set #1 that is statically allocated by the app
        status = GapAdv_loadByHandle(advHandleLegacy, GAP_ADV_DATA_TYPE_ADV,
                                     sizeof(airtag_adv_data), airtag_adv_data);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);

        // Load scan response data for set #1 that is statically allocated by the app
        status = GapAdv_loadByHandle(advHandleLegacy, GAP_ADV_DATA_TYPE_SCAN_RSP,
                                     sizeof(scanResData1), scanResData1);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);

        // Set event mask for set #1
        status = GapAdv_setEventMask(advHandleLegacy,
                                     GAP_ADV_EVT_MASK_START_AFTER_ENABLE |
                                     GAP_ADV_EVT_MASK_END_AFTER_DISABLE |
                                     GAP_ADV_EVT_MASK_SET_TERMINATED);

        // Enable legacy advertising for set #1
        status = GapAdv_enable(advHandleLegacy, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);

        BLE_LOG_INT_INT(0, BLE_LOG_MODULE_APP, "APP : ---- call GapAdv_create set=%d,%d\n", 1, 0);
        // Create Advertisement set #2 and assign handle
        status = GapAdv_create(&AirtagSpoofer_advCallback, &advParams2,
                               &advHandleLongRange);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);

        // Load advertising data for set #2 that is statically allocated by the app
        status = GapAdv_loadByHandle(advHandleLongRange, GAP_ADV_DATA_TYPE_ADV,
                                     sizeof(advData2), advData2);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);

        // Set event mask for set #2
        status = GapAdv_setEventMask(advHandleLongRange,
                                     GAP_ADV_EVT_MASK_START_AFTER_ENABLE |
                                     GAP_ADV_EVT_MASK_END_AFTER_DISABLE |
                                     GAP_ADV_EVT_MASK_SET_TERMINATED);

        BLE_LOG_INT_TIME(0, BLE_LOG_MODULE_APP, "APP : ---- GapAdv_enable", 0);
        // Enable long range advertising for set #2
        status = GapAdv_enable(advHandleLongRange, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);
        AIRTAGSPOOFER_ASSERT(status == SUCCESS);


        if (addrMode > ADDRMODE_RANDOM)
        {
          AirtagSpoofer_updateRPA();

          // Create one-shot clock for RPA check event.
          Util_constructClock(&clkRpaRead, AirtagSpoofer_clockHandler,
                              READ_RPA_PERIOD, 0, true,
                              (UArg) &argRpaRead);
        }

      }

      break;
    }

    case GAP_LINK_ESTABLISHED_EVENT:
    {
      gapEstLinkReqEvent_t *pPkt = (gapEstLinkReqEvent_t *)pMsg;

      BLE_LOG_INT_TIME(0, BLE_LOG_MODULE_APP, "APP : ---- got GAP_LINK_ESTABLISHED_EVENT", 0);
      // Display the amount of current connections
      uint8_t numActive = linkDB_NumActive();

      if (pPkt->hdr.status == SUCCESS)
      {
        // Add connection to list and start RSSI
        AirtagSpoofer_addConn(pPkt->connectionHandle);

        // Start Periodic Clock.
        Util_startClock(&clkPeriodic);
      }
      if ((numActive < MAX_NUM_BLE_CONNS) && (autoConnect == AUTOCONNECT_DISABLE))
      {
        // Start advertising since there is room for more connections
        GapAdv_enable(advHandleLegacy, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);
        GapAdv_enable(advHandleLongRange, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);
      }
      else
      {
        // Stop advertising since there is no room for more connections
        GapAdv_disable(advHandleLongRange);
        GapAdv_disable(advHandleLegacy);
      }
      break;
    }

    case GAP_LINK_TERMINATED_EVENT:
    {
      gapTerminateLinkEvent_t *pPkt = (gapTerminateLinkEvent_t *)pMsg;

      // Display the amount of current connections
      uint8_t numActive = linkDB_NumActive();

      // Remove the connection from the list and disable RSSI if needed
      AirtagSpoofer_removeConn(pPkt->connectionHandle);

      // If no active connections
      if (numActive == 0)
      {
        // Stop periodic clock
        Util_stopClock(&clkPeriodic);


      }

      BLE_LOG_INT_STR(0, BLE_LOG_MODULE_APP, "APP : GAP msg: status=%d, opcode=%s\n", 0, "GAP_LINK_TERMINATED_EVENT");
      // Start advertising since there is room for more connections
      GapAdv_enable(advHandleLegacy, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);
      GapAdv_enable(advHandleLongRange, GAP_ADV_ENABLE_OPTIONS_USE_MAX , 0);



      break;
    }

    case GAP_UPDATE_LINK_PARAM_REQ_EVENT:
    {
      gapUpdateLinkParamReqReply_t rsp;

      gapUpdateLinkParamReqEvent_t *pReq = (gapUpdateLinkParamReqEvent_t *)pMsg;

      rsp.connectionHandle = pReq->req.connectionHandle;
      rsp.signalIdentifier = pReq->req.signalIdentifier;

      // Only accept connection intervals with slave latency of 0
      // This is just an example of how the application can send a response
      if(pReq->req.connLatency == 0)
      {
        rsp.intervalMin = pReq->req.intervalMin;
        rsp.intervalMax = pReq->req.intervalMax;
        rsp.connLatency = pReq->req.connLatency;
        rsp.connTimeout = pReq->req.connTimeout;
        rsp.accepted = TRUE;
      }
      else
      {
        rsp.accepted = FALSE;
      }

      // Send Reply
      VOID GAP_UpdateLinkParamReqReply(&rsp);

      break;
    }

    case GAP_LINK_PARAM_UPDATE_EVENT:
    {
      gapLinkUpdateEvent_t *pPkt = (gapLinkUpdateEvent_t *)pMsg;

      // Get the address from the connection handle
      linkDBInfo_t linkInfo;
      linkDB_GetInfo(pPkt->connectionHandle, &linkInfo);


      // Check if there are any queued parameter updates
      spConnHandleEntry_t *connHandleEntry = (spConnHandleEntry_t *)List_get(&paramUpdateList);
      if (connHandleEntry != NULL)
      {
        // Attempt to send queued update now
        AirtagSpoofer_processParamUpdate(connHandleEntry->connHandle);

        // Free list element
        ICall_free(connHandleEntry);
      }

      break;
    }

#if defined ( NOTIFY_PARAM_UPDATE_RJCT )
    case GAP_LINK_PARAM_UPDATE_REJECT_EVENT:
    {
      linkDBInfo_t linkInfo;
      gapLinkUpdateEvent_t *pPkt = (gapLinkUpdateEvent_t *)pMsg;

      // Get the address from the connection handle
      linkDB_GetInfo(pPkt->connectionHandle, &linkInfo);

      break;
    }
#endif

    default:

      break;
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_charValueChangeCB
 *
 * @brief   Callback from Simple Profile indicating a characteristic
 *          value change.
 *
 * @param   paramId - parameter Id of the value that was changed.
 *
 * @return  None.
 */
static void AirtagSpoofer_charValueChangeCB(uint8_t paramId)
{
  uint8_t *pValue = ICall_malloc(sizeof(uint8_t));

  if (pValue)
  {
    *pValue = paramId;

    if (AirtagSpoofer_enqueueMsg(SP_CHAR_CHANGE_EVT, pValue) != SUCCESS)
    {
      ICall_free(pValue);
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processCharValueChangeEvt
 *
 * @brief   Process a pending Simple Profile characteristic value change
 *          event.
 *
 * @param   paramID - parameter ID of the value that was changed.
 */
static void AirtagSpoofer_processCharValueChangeEvt(uint8_t paramId)
{
  uint8_t newValue;

  switch(paramId)
  {
    case SIMPLEPROFILE_CHAR1:
      SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR1, &newValue);

      //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "Char 1: %d", (uint16_t)newValue);
      break;

    case SIMPLEPROFILE_CHAR3:
      SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR3, &newValue);

      //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "Char 3: %d", (uint16_t)newValue);
      break;

    default:
      // should not reach here!
      break;
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_performPeriodicTask
 *
 * @brief   Perform a periodic application task. This function gets called
 *          every five seconds (SP_PERIODIC_EVT_PERIOD). In this example,
 *          the value of the third characteristic in the SimpleGATTProfile
 *          service is retrieved from the profile, and then copied into the
 *          value of the the fourth characteristic.
 *
 * @param   None.
 *
 * @return  None.
 */
static void AirtagSpoofer_performPeriodicTask(void)
{
  uint8_t valueToCopy;

  // Call to retrieve the value of the third characteristic in the profile
  if (SimpleProfile_GetParameter(SIMPLEPROFILE_CHAR3, &valueToCopy) == SUCCESS)
  {
    // Call to set that value of the fourth characteristic in the profile.
    // Note that if notifications of the fourth characteristic have been
    // enabled by a GATT client device, then a notification will be sent
    // every time this function is called.
    SimpleProfile_SetParameter(SIMPLEPROFILE_CHAR4, sizeof(uint8_t),
                               &valueToCopy);
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_updateRPA
 *
 * @brief   Read the current RPA from the stack and update display
 *          if the RPA has changed.
 *
 * @param   None.
 *
 * @return  None.
 */
static void AirtagSpoofer_updateRPA(void)
{
  uint8_t* pRpaNew;

  // Read the current RPA.
  pRpaNew = GAP_GetDevAddress(FALSE);

  if (memcmp(pRpaNew, rpa, B_ADDR_LEN))
  {
    // If the RPA has changed, update the display
    //Display_printf(dispHandle, SP_ROW_RPA, 0, "RP Addr: %s",
                  // Util_convertBdAddr2Str(pRpaNew));
    memcpy(rpa, pRpaNew, B_ADDR_LEN);
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_clockHandler
 *
 * @brief   Handler function for clock timeouts.
 *
 * @param   arg - event type
 *
 * @return  None.
 */
static void AirtagSpoofer_clockHandler(UArg arg)
{
  spClockEventData_t *pData = (spClockEventData_t *)arg;

 if (pData->event == SP_PERIODIC_EVT)
 {
   // Start the next period
   Util_startClock(&clkPeriodic);

   // Post event to wake up the application
   AirtagSpoofer_enqueueMsg(SP_PERIODIC_EVT, NULL);
 }
 else if (pData->event == SP_READ_RPA_EVT)
 {
   // Start the next period
   Util_startClock(&clkRpaRead);

   // Post event to read the current RPA
   AirtagSpoofer_enqueueMsg(SP_READ_RPA_EVT, NULL);
 }
 else if (pData->event == SP_SEND_PARAM_UPDATE_EVT)
 {
    // Send message to app
    AirtagSpoofer_enqueueMsg(SP_SEND_PARAM_UPDATE_EVT, pData);
 }
}

/*********************************************************************
 * @fn      AirtagSpoofer_keyChangeHandler
 *
 * @brief   Key event handler function
 *
 * @param   keys - bitmap of pressed keys
 *
 * @return  none
 */
static void AirtagSpoofer_keyChangeHandler(uint8_t keys)
{
  uint8_t *pValue = ICall_malloc(sizeof(uint8_t));

  if (pValue)
  {
    *pValue = keys;

    if(AirtagSpoofer_enqueueMsg(SP_KEY_CHANGE_EVT, pValue) != SUCCESS)
    {
      ICall_free(pValue);
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_advCallback
 *
 * @brief   GapAdv module callback
 *
 * @param   pMsg - message to process
 */
static void AirtagSpoofer_advCallback(uint32_t event, void *pBuf, uintptr_t arg)
{
  spGapAdvEventData_t *pData = ICall_malloc(sizeof(spGapAdvEventData_t));

  if (pData)
  {
    pData->event = event;
    pData->pBuf = pBuf;

    if(AirtagSpoofer_enqueueMsg(SP_ADV_EVT, pData) != SUCCESS)
    {
      ICall_free(pData);
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processAdvEvent
 *
 * @brief   Process advertising event in app context
 *
 * @param   pEventData
 */
static void AirtagSpoofer_processAdvEvent(spGapAdvEventData_t *pEventData)
{
  switch (pEventData->event)
  {
    case GAP_EVT_ADV_START_AFTER_ENABLE:
      BLE_LOG_INT_TIME(0, BLE_LOG_MODULE_APP, "APP : ---- GAP_EVT_ADV_START_AFTER_ENABLE", 0);
      //Display_printf(dispHandle, SP_ROW_ADVSTATE, 0, "Adv Set %d Enabled",
                    // *(uint8_t *)(pEventData->pBuf));
      break;

    case GAP_EVT_ADV_END_AFTER_DISABLE:
      //Display_printf(dispHandle, SP_ROW_ADVSTATE, 0, "Adv Set %d Disabled",
                    // *(uint8_t *)(pEventData->pBuf));
      break;

    case GAP_EVT_ADV_START:
      break;

    case GAP_EVT_ADV_END:
      break;

    case GAP_EVT_ADV_SET_TERMINATED:
    {
#ifndef Display_DISABLE_ALL
      GapAdv_setTerm_t *advSetTerm = (GapAdv_setTerm_t *)(pEventData->pBuf);

      //Display_printf(dispHandle, SP_ROW_ADVSTATE, 0, "Adv Set %d disabled after conn %d",
                     //advSetTerm->handle, advSetTerm->connHandle );
#endif
    }
    break;

    case GAP_EVT_SCAN_REQ_RECEIVED:
      break;

    case GAP_EVT_INSUFFICIENT_MEMORY:
      break;

    default:
      break;
  }

  // All events have associated memory to free except the insufficient memory
  // event
  if (pEventData->event != GAP_EVT_INSUFFICIENT_MEMORY)
  {
    ICall_free(pEventData->pBuf);
  }
}


/*********************************************************************
 * @fn      AirtagSpoofer_pairStateCb
 *
 * @brief   Pairing state callback.
 *
 * @return  none
 */
static void AirtagSpoofer_pairStateCb(uint16_t connHandle, uint8_t state,
                                         uint8_t status)
{
  spPairStateData_t *pData = ICall_malloc(sizeof(spPairStateData_t));

  // Allocate space for the event data.
  if (pData)
  {
    pData->state = state;
    pData->connHandle = connHandle;
    pData->status = status;

    // Queue the event.
    if(AirtagSpoofer_enqueueMsg(SP_PAIR_STATE_EVT, pData) != SUCCESS)
    {
      ICall_free(pData);
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_passcodeCb
 *
 * @brief   Passcode callback.
 *
 * @return  none
 */
static void AirtagSpoofer_passcodeCb(uint8_t *pDeviceAddr,
                                        uint16_t connHandle,
                                        uint8_t uiInputs,
                                        uint8_t uiOutputs,
                                        uint32_t numComparison)
{
  spPasscodeData_t *pData = ICall_malloc(sizeof(spPasscodeData_t));

  // Allocate space for the passcode event.
  if (pData )
  {
    pData->connHandle = connHandle;
    memcpy(pData->deviceAddr, pDeviceAddr, B_ADDR_LEN);
    pData->uiInputs = uiInputs;
    pData->uiOutputs = uiOutputs;
    pData->numComparison = numComparison;

    // Enqueue the event.
    if(AirtagSpoofer_enqueueMsg(SP_PASSCODE_EVT, pData) != SUCCESS)
    {
      ICall_free(pData);
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processPairState
 *
 * @brief   Process the new paring state.
 *
 * @return  none
 */
static void AirtagSpoofer_processPairState(spPairStateData_t *pPairData)
{
  uint8_t state = pPairData->state;
  uint8_t status = pPairData->status;

  switch (state)
  {
    case GAPBOND_PAIRING_STATE_STARTED:
      //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Pairing started");
      break;

    case GAPBOND_PAIRING_STATE_COMPLETE:
      if (status == SUCCESS)
      {
        //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Pairing success");
      }
      else
      {
        //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Pairing fail: %d", status);
      }
      break;

    case GAPBOND_PAIRING_STATE_ENCRYPTED:
      if (status == SUCCESS)
      {
        //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Encryption success");
      }
      else
      {
        //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Encryption failed: %d", status);
      }
      break;

    case GAPBOND_PAIRING_STATE_BOND_SAVED:
      if (status == SUCCESS)
      {
        //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Bond save success");
      }
      else
      {
        //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Bond save failed: %d", status);
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processPasscode
 *
 * @brief   Process the Passcode request.
 *
 * @return  none
 */
static void AirtagSpoofer_processPasscode(spPasscodeData_t *pPasscodeData)
{
  // Display passcode to user
  if (pPasscodeData->uiOutputs != 0)
  {
    //Display_printf(dispHandle, SP_ROW_CONNECTION, 0, "Passcode: %d",
//                   B_APP_DEFAULT_PASSCODE);
  }

  // Send passcode response
  GAPBondMgr_PasscodeRsp(pPasscodeData->connHandle , SUCCESS,
                         B_APP_DEFAULT_PASSCODE);
}

/*********************************************************************
 * @fn      AirtagSpoofer_connEvtCB
 *
 * @brief   Connection event callback.
 *
 * @param pReport pointer to connection event report
 */
static void AirtagSpoofer_connEvtCB(Gap_ConnEventRpt_t *pReport)
{
  // Enqueue the event for processing in the app context.
  if(AirtagSpoofer_enqueueMsg(SP_CONN_EVT, pReport) != SUCCESS)
  {
    ICall_free(pReport);
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_processConnEvt
 *
 * @brief   Process connection event.
 *
 * @param pReport pointer to connection event report
 */
static void AirtagSpoofer_processConnEvt(Gap_ConnEventRpt_t *pReport)
{
  // Get index from handle
  uint8_t connIndex = AirtagSpoofer_getConnIndex(pReport->handle);

  if (connIndex >= MAX_NUM_BLE_CONNS)
  {
    //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "Connection handle is not in the connList !!!");
    return;
  }

  // If auto phy change is enabled
  if (connList[connIndex].isAutoPHYEnable == TRUE)
  {
    // Read the RSSI
    HCI_ReadRssiCmd(pReport->handle);
  }
}


/*********************************************************************
 * @fn      AirtagSpoofer_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 */
static status_t AirtagSpoofer_enqueueMsg(uint8_t event, void *pData)
{
  uint8_t success;
  spEvt_t *pMsg = ICall_malloc(sizeof(spEvt_t));

  // Create dynamic pointer to message.
  if(pMsg)
  {
    pMsg->event = event;
    pMsg->pData = pData;

    // Enqueue the message.
    success = Util_enqueueMsg(appMsgQueueHandle, syncEvent, (uint8_t *)pMsg);
    return (success) ? SUCCESS : FAILURE;
  }

  return(bleMemAllocError);
}


/*********************************************************************
 * @fn      AirtagSpoofer_addConn
 *
 * @brief   Add a device to the connected device list
 *
 * @return  index of the connected device list entry where the new connection
 *          info is put in.
 *          if there is no room, MAX_NUM_BLE_CONNS will be returned.
 */
static uint8_t AirtagSpoofer_addConn(uint16_t connHandle)
{
  uint8_t i;
  uint8_t status = bleNoResources;

  // Try to find an available entry
  for (i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    if (connList[i].connHandle == LINKDB_CONNHANDLE_INVALID)
    {
      // Found available entry to put a new connection info in
      connList[i].connHandle = connHandle;

#ifdef DEFAULT_SEND_PARAM_UPDATE_REQ
      // Allocate data to send through clock handler
      connList[i].pParamUpdateEventData = ICall_malloc(sizeof(spClockEventData_t) +
                                                       sizeof (uint16_t));
      if(connList[i].pParamUpdateEventData)
      {
        connList[i].pParamUpdateEventData->event = SP_SEND_PARAM_UPDATE_EVT;
        *((uint16_t *)connList[i].pParamUpdateEventData->data) = connHandle;

        // Create a clock object and start
        connList[i].pUpdateClock
          = (Clock_Struct*) ICall_malloc(sizeof(Clock_Struct));

        if (connList[i].pUpdateClock)
        {
          Util_constructClock(connList[i].pUpdateClock,
                              AirtagSpoofer_clockHandler,
                              SEND_PARAM_UPDATE_DELAY, 0, true,
                              (UArg) (connList[i].pParamUpdateEventData));
        }
        else
        {
            ICall_free(connList[i].pParamUpdateEventData);
        }
      }
      else
      {
        status = bleMemAllocError;
      }
#endif

      // Set default PHY to 1M
      connList[i].currPhy = HCI_PHY_1_MBPS;

      break;
    }
  }

  return status;
}

/*********************************************************************
 * @fn      AirtagSpoofer_getConnIndex
 *
 * @brief   Find index in the connected device list by connHandle
 *
 * @return  the index of the entry that has the given connection handle.
 *          if there is no match, MAX_NUM_BLE_CONNS will be returned.
 */
static uint8_t AirtagSpoofer_getConnIndex(uint16_t connHandle)
{
  uint8_t i;

  for (i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    if (connList[i].connHandle == connHandle)
    {
      return i;
    }
  }

  return(MAX_NUM_BLE_CONNS);
}

/*********************************************************************
 * @fn      AirtagSpoofer_getConnIndex
 *
 * @brief   Find index in the connected device list by connHandle
 *
 * @return  SUCCESS if connHandle found valid index or bleInvalidRange
 *          if index wasn't found. LINKDB_CONNHANDLE_ALL will always succeed.
 */
static uint8_t AirtagSpoofer_clearConnListEntry(uint16_t connHandle)
{
  uint8_t i;
  // Set to invalid connection index initially
  uint8_t connIndex = MAX_NUM_BLE_CONNS;

  if(connHandle != LINKDB_CONNHANDLE_ALL)
  {
    // Get connection index from handle
    connIndex = AirtagSpoofer_getConnIndex(connHandle);
    if(connIndex >= MAX_NUM_BLE_CONNS)
	{
	  return(bleInvalidRange);
	}
  }

  // Clear specific handle or all handles
  for(i = 0; i < MAX_NUM_BLE_CONNS; i++)
  {
    if((connIndex == i) || (connHandle == LINKDB_CONNHANDLE_ALL))
    {
      connList[i].connHandle = LINKDB_CONNHANDLE_INVALID;
      connList[i].currPhy = 0;
      connList[i].phyCngRq = 0;
      connList[i].phyRqFailCnt = 0;
      connList[i].rqPhy = 0;
      memset(connList[i].rssiArr, 0, SP_MAX_RSSI_STORE_DEPTH);
      connList[i].rssiAvg = 0;
      connList[i].rssiCntr = 0;
      connList[i].isAutoPHYEnable = FALSE;
    }
  }

  return(SUCCESS);
}

/*********************************************************************
 * @fn      AirtagSpoofer_clearPendingParamUpdate
 *
 * @brief   clean pending param update request in the paramUpdateList list
 *
 * @param   connHandle - connection handle to clean
 *
 * @return  none
 */
void AirtagSpoofer_clearPendingParamUpdate(uint16_t connHandle)
{
  List_Elem *curr;

  for (curr = List_head(&paramUpdateList); curr != NULL; curr = List_next(curr)) 
  {
    if (((spConnHandleEntry_t *)curr)->connHandle == connHandle)
    {
      List_remove(&paramUpdateList, curr);
    }
  }
}

/*********************************************************************
 * @fn      AirtagSpoofer_removeConn
 *
 * @brief   Remove a device from the connected device list
 *
 * @return  index of the connected device list entry where the new connection
 *          info is removed from.
 *          if connHandle is not found, MAX_NUM_BLE_CONNS will be returned.
 */
static uint8_t AirtagSpoofer_removeConn(uint16_t connHandle)
{
  uint8_t connIndex = AirtagSpoofer_getConnIndex(connHandle);

  if(connIndex != MAX_NUM_BLE_CONNS)
  {
    Clock_Struct* pUpdateClock = connList[connIndex].pUpdateClock;

    if (pUpdateClock != NULL)
    {
      // Stop and destruct the RTOS clock if it's still alive
      if (Util_isActive(pUpdateClock))
      {
        Util_stopClock(pUpdateClock);
      }

      // Destruct the clock object
      Clock_destruct(pUpdateClock);
      // Free clock struct
      ICall_free(pUpdateClock);
      // Free ParamUpdateEventData
      ICall_free(connList[connIndex].pParamUpdateEventData);
    }
    // Clear pending update requests from paramUpdateList
    AirtagSpoofer_clearPendingParamUpdate(connHandle);
    // Stop Auto PHY Change
    AirtagSpoofer_stopAutoPhyChange(connHandle);
    // Clear Connection List Entry
    AirtagSpoofer_clearConnListEntry(connHandle);
  }

  return connIndex;
}

/*********************************************************************
 * @fn      AirtagSpoofer_processParamUpdate
 *
 * @brief   Process a parameters update request
 *
 * @return  None
 */
static void AirtagSpoofer_processParamUpdate(uint16_t connHandle)
{
  gapUpdateLinkParamReq_t req;
  uint8_t connIndex;

  req.connectionHandle = connHandle;
#ifdef DEFAULT_SEND_PARAM_UPDATE_REQ
  req.connLatency = DEFAULT_DESIRED_SLAVE_LATENCY;
  req.connTimeout = DEFAULT_DESIRED_CONN_TIMEOUT;
  req.intervalMin = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
  req.intervalMax = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
#endif

  connIndex = AirtagSpoofer_getConnIndex(connHandle);
  if (connIndex >= MAX_NUM_BLE_CONNS)
  {
    //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "Connection handle is not in the connList !!!");
    return;
  }

  // Deconstruct the clock object
  Clock_destruct(connList[connIndex].pUpdateClock);
  // Free clock struct, only in case it is not NULL
  if (connList[connIndex].pUpdateClock != NULL)
  {
      ICall_free(connList[connIndex].pUpdateClock);
      connList[connIndex].pUpdateClock = NULL;
  }
  // Free ParamUpdateEventData, only in case it is not NULL
  if (connList[connIndex].pParamUpdateEventData != NULL)
      ICall_free(connList[connIndex].pParamUpdateEventData);

  // Send parameter update
  bStatus_t status = GAP_UpdateLinkParamReq(&req);

  // If there is an ongoing update, queue this for when the udpate completes
  if (status == bleAlreadyInRequestedMode)
  {
    spConnHandleEntry_t *connHandleEntry = ICall_malloc(sizeof(spConnHandleEntry_t));
    if (connHandleEntry)
    {
      connHandleEntry->connHandle = connHandle;

      List_put(&paramUpdateList, (List_Elem *)connHandleEntry);
    }
  }
}

/*********************************************************************
 * @fn      SimpleCentral_processCmdCompleteEvt
 *
 * @brief   Process an incoming OSAL HCI Command Complete Event.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void AirtagSpoofer_processCmdCompleteEvt(hciEvt_CmdComplete_t *pMsg)
{
  uint8_t status = pMsg->pReturnParam[0];

  //Find which command this command complete is for
  switch (pMsg->cmdOpcode)
  {
    case HCI_READ_RSSI:
    {
      int8 rssi = (int8)pMsg->pReturnParam[3];

      // Display RSSI value, if RSSI is higher than threshold, change to faster PHY
      if (status == SUCCESS)
      {
        uint16_t handle = BUILD_UINT16(pMsg->pReturnParam[1], pMsg->pReturnParam[2]);

        uint8_t index = AirtagSpoofer_getConnIndex(handle);
        if (index >= MAX_NUM_BLE_CONNS)
        {
          //Display_printf(dispHandle, SP_ROW_STATUS_1, 0, "Connection handle is not in the connList !!!");
          return;
        }

        if (rssi != LL_RSSI_NOT_AVAILABLE)
        {
          connList[index].rssiArr[connList[index].rssiCntr++] = rssi;
          connList[index].rssiCntr %= SP_MAX_RSSI_STORE_DEPTH;

          int16_t sum_rssi = 0;
          for(uint8_t cnt=0; cnt<SP_MAX_RSSI_STORE_DEPTH; cnt++)
          {
            sum_rssi += connList[index].rssiArr[cnt];
          }
          connList[index].rssiAvg = (uint32_t)(sum_rssi/SP_MAX_RSSI_STORE_DEPTH);

          uint8_t phyRq = SP_PHY_NONE;
          uint8_t phyRqS = SP_PHY_NONE;
          uint8_t phyOpt = LL_PHY_OPT_NONE;

          if(connList[index].phyCngRq == FALSE)
          {
            if((connList[index].rssiAvg >= RSSI_2M_THRSHLD) &&
            (connList[index].currPhy != HCI_PHY_2_MBPS) &&
                 (connList[index].currPhy != SP_PHY_NONE))
            {
              // try to go to higher data rate
              phyRqS = phyRq = HCI_PHY_2_MBPS;
            }
            else if((connList[index].rssiAvg < RSSI_2M_THRSHLD) &&
                    (connList[index].rssiAvg >= RSSI_1M_THRSHLD) &&
                    (connList[index].currPhy != HCI_PHY_1_MBPS) &&
                    (connList[index].currPhy != SP_PHY_NONE))
            {
              // try to go to legacy regular data rate
              phyRqS = phyRq = HCI_PHY_1_MBPS;
            }
            else if((connList[index].rssiAvg >= RSSI_S2_THRSHLD) &&
                    (connList[index].rssiAvg < RSSI_1M_THRSHLD) &&
                    (connList[index].currPhy != SP_PHY_NONE))
            {
              // try to go to lower data rate S=2(500kb/s)
              phyRqS = HCI_PHY_CODED;
              phyOpt = LL_PHY_OPT_S2;
              phyRq = BLE5_CODED_S2_PHY;
            }
            else if(connList[index].rssiAvg < RSSI_S2_THRSHLD )
            {
              // try to go to lowest data rate S=8(125kb/s)
              phyRqS = HCI_PHY_CODED;
              phyOpt = LL_PHY_OPT_S8;
              phyRq = BLE5_CODED_S8_PHY;
            }
            if((phyRq != SP_PHY_NONE) &&
               // First check if the request for this phy change is already not honored then don't request for change
               (((connList[index].rqPhy == phyRq) &&
                 (connList[index].phyRqFailCnt < 2)) ||
                 (connList[index].rqPhy != phyRq)))
            {
              //Initiate PHY change based on RSSI
              AirtagSpoofer_setPhy(connList[index].connHandle, 0,
                                      phyRqS, phyRqS, phyOpt);
              connList[index].phyCngRq = TRUE;

              // If it a request for different phy than failed request, reset the count
              if(connList[index].rqPhy != phyRq)
              {
                // then reset the request phy counter and requested phy
                connList[index].phyRqFailCnt = 0;
              }

              if(phyOpt == LL_PHY_OPT_NONE)
              {
                connList[index].rqPhy = phyRq;
              }
              else if(phyOpt == LL_PHY_OPT_S2)
              {
                connList[index].rqPhy = BLE5_CODED_S2_PHY;
              }
              else
              {
                connList[index].rqPhy = BLE5_CODED_S8_PHY;
              }

            } // end of if ((phyRq != SP_PHY_NONE) && ...
          } // end of if (connList[index].phyCngRq == FALSE)
        } // end of if (rssi != LL_RSSI_NOT_AVAILABLE)

        //Display_printf(dispHandle, SP_ROW_RSSI, 0,
//                       "RSSI:%d dBm, AVG RSSI:%d dBm",
//                       (uint32_t)(rssi),
//                       connList[index].rssiAvg);

	  } // end of if (status == SUCCESS)
      break;
    }

    case HCI_LE_READ_PHY:
    {
      if (status == SUCCESS)
      {
        //Display_printf(dispHandle, SP_ROW_RSSI + 2, 0, "RXPh: %d, TXPh: %d",
                      // pMsg->pReturnParam[3], pMsg->pReturnParam[4]);
      }
      break;
    }

    default:
      break;
  } // end of switch (pMsg->cmdOpcode)
}

/*********************************************************************
* @fn      AirtagSpoofer_initPHYRSSIArray
*
* @brief   Initializes the array of structure/s to store data related
*          RSSI based auto PHy change
*
* @param   connHandle - the connection handle
*
* @param   addr - pointer to device address
*
* @return  index of connection handle
*/
static void AirtagSpoofer_initPHYRSSIArray(void)
{
  //Initialize array to store connection handle and RSSI values
  memset(connList, 0, sizeof(connList));
  for (uint8_t index = 0; index < MAX_NUM_BLE_CONNS; index++)
  {
    connList[index].connHandle = SP_INVALID_HANDLE;
  }
}
/*********************************************************************
      // Set default PHY to 1M
 * @fn      AirtagSpoofer_startAutoPhyChange
 *
 * @brief   Start periodic RSSI reads on a link.
 *
 * @param   connHandle - connection handle of link
 * @param   devAddr - device address
 *
 * @return  SUCCESS: Terminate started
 *          bleIncorrectMode: No link
 *          bleNoResources: No resources
 */
static status_t AirtagSpoofer_startAutoPhyChange(uint16_t connHandle)
{
  status_t status = FAILURE;

  // Get connection index from handle
  uint8_t connIndex = AirtagSpoofer_getConnIndex(connHandle);
  AIRTAGSPOOFER_ASSERT(connIndex < MAX_NUM_BLE_CONNS);

  // Start Connection Event notice for RSSI calculation
  status = Gap_RegisterConnEventCb(AirtagSpoofer_connEvtCB, GAP_CB_REGISTER, GAP_CB_CONN_EVENT_ALL, connHandle);

  // Flag in connection info if successful
  if (status == SUCCESS)
  {
    connList[connIndex].isAutoPHYEnable = TRUE;
  }

  return status;
}

/*********************************************************************
 * @fn      AirtagSpoofer_stopAutoPhyChange
 *
 * @brief   Cancel periodic RSSI reads on a link.
 *
 * @param   connHandle - connection handle of link
 *
 * @return  SUCCESS: Operation successful
 *          bleIncorrectMode: No link
 */
static status_t AirtagSpoofer_stopAutoPhyChange(uint16_t connHandle)
{
  // Get connection index from handle
  uint8_t connIndex = AirtagSpoofer_getConnIndex(connHandle);
  AIRTAGSPOOFER_ASSERT(connIndex < MAX_NUM_BLE_CONNS);

  // Stop connection event notice
  Gap_RegisterConnEventCb(NULL, GAP_CB_UNREGISTER, GAP_CB_CONN_EVENT_ALL, connHandle);

  // Also update the phychange request status for active RSSI tracking connection
  connList[connIndex].phyCngRq = FALSE;
  connList[connIndex].isAutoPHYEnable = FALSE;

  return SUCCESS;
}

/*********************************************************************
 * @fn      AirtagSpoofer_setPhy
 *
 * @brief   Call the HCI set phy API and and add the handle to a
 *          list to match it to an incoming command status event
 */
static status_t AirtagSpoofer_setPhy(uint16_t connHandle, uint8_t allPhys,
                                        uint8_t txPhy, uint8_t rxPhy,
                                        uint16_t phyOpts)
{
  // Allocate list entry to store handle for command status
  spConnHandleEntry_t *connHandleEntry = ICall_malloc(sizeof(spConnHandleEntry_t));

  if (connHandleEntry)
  {
    connHandleEntry->connHandle = connHandle;

    // Add entry to the phy command status list
    List_put(&setPhyCommStatList, (List_Elem *)connHandleEntry);

    // Send PHY Update
    HCI_LE_SetPhyCmd(connHandle, allPhys, txPhy, rxPhy, phyOpts);
  }

  return SUCCESS;
}

/*********************************************************************
* @fn      AirtagSpoofer_updatePHYStat
*
* @brief   Update the auto phy update state machine
*
* @param   connHandle - the connection handle
*
* @return  None
*/
static void AirtagSpoofer_updatePHYStat(uint16_t eventCode, uint8_t *pMsg)
{
  uint8_t connIndex;

  switch (eventCode)
  {
    case HCI_LE_SET_PHY:
    {
      // Get connection handle from list
      spConnHandleEntry_t *connHandleEntry =
                           (spConnHandleEntry_t *)List_get(&setPhyCommStatList);

      if (connHandleEntry)
      {
        // Get index from connection handle
        connIndex = AirtagSpoofer_getConnIndex(connHandleEntry->connHandle);

        ICall_free(connHandleEntry);

        // Is this connection still valid?
        if (connIndex < MAX_NUM_BLE_CONNS)
        {
          hciEvt_CommandStatus_t *pMyMsg = (hciEvt_CommandStatus_t *)pMsg;

          if (pMyMsg->cmdStatus == HCI_ERROR_CODE_UNSUPPORTED_REMOTE_FEATURE)
          {
            // Update the phychange request status for active RSSI tracking connection
            connList[connIndex].phyCngRq = FALSE;
            connList[connIndex].phyRqFailCnt++;
          }
        }
      }
      break;
    }

    // LE Event - a Phy update has completed or failed
    case HCI_BLE_PHY_UPDATE_COMPLETE_EVENT:
    {
      hciEvt_BLEPhyUpdateComplete_t *pPUC =
                                     (hciEvt_BLEPhyUpdateComplete_t*) pMsg;

      if(pPUC)
      {
        // Get index from connection handle
        connIndex = AirtagSpoofer_getConnIndex(pPUC->connHandle);

        // Is this connection still valid?
        if (connIndex < MAX_NUM_BLE_CONNS)
        {
          // Update the phychange request status for active RSSI tracking connection
          connList[connIndex].phyCngRq = FALSE;

          if (pPUC->status == SUCCESS)
          {
            connList[connIndex].currPhy = pPUC->rxPhy;
          }
          if(pPUC->rxPhy != connList[connIndex].rqPhy)
          {
            connList[connIndex].phyRqFailCnt++;
          }
          else
          {
            // Reset the request phy counter and requested phy
            connList[connIndex].phyRqFailCnt = 0;
            connList[connIndex].rqPhy = 0;
          }
        }
      }

      break;
    }

    default:
      break;
  } // end of switch (eventCode)
}

/*********************************************************************
*********************************************************************/
