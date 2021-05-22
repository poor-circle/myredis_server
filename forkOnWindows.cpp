
/*
Name:            Ntdll.h
BaseLib:        <Ntdll.lib>
BaseHead:        <stdarg.h> <winbase.h> <windef.h>
Author:            gwsbhqt@163.com
LastCode:        20160102
Description:    Reference the undeclared Native API in Ntdll.lib
*/

#pragma once

#ifndef __NTDLL_H__
#define __NTDLL_H__

#pragma region HEAD

#include <stdarg.h>
#include <winbase.h>
#include <windef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _NTDDK_
#error This head file can not be compiled together with Ntddk.h
#endif

#ifndef WIN64
#pragma comment(lib, "Lib\\x86\\Ntdll.lib")
#else
#pragma comment(lib, "Lib\\x64\\Ntdll.lib")
#endif

#pragma endregion

#pragma region NTDLL BASE DEFINE

#ifndef NTSTATUS
    typedef LONG NTSTATUS;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif // NT_SUCCESS

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#endif // STATUS_SUCCESS

#ifndef STATUS_UNSUCCESSFUL
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif // STATUS_UNSUCCESSFUL

#ifndef ASSERT
#ifdef _DEBUG
#define ASSERT(x) assert(x)
#else // _DEBUG
#define ASSERT(x)
#endif // _DEBUG
#endif // ASSERT

#pragma endregion

#pragma region DEFINE

#define DEVICE_TYPE DWORD

#define FLG_STOP_ON_EXCEPTION            0x0000001L
#define FLG_SHOW_LDR_SNAPS                0x0000002L
#define FLG_DEBUG_INITIAL_COMMAND        0x0000004L
#define FLG_STOP_ON_HUNG_GUI            0x0000008L
#define FLG_HEAP_ENABLE_TAIL_CHECK        0x0000010L
#define FLG_HEAP_ENABLE_FREE_CHECK        0x0000020L
#define FLG_HEAP_VALIDATE_PARAMETERS    0x0000040L
#define FLG_HEAP_VALIDATE_ALL            0x0000080L
#define FLG_POOL_ENABLE_TAIL_CHECK        0x0000100L
#define FLG_POOL_ENABLE_FREE_CHECK        0x0000200L
#define FLG_POOL_ENABLE_TAGGING            0x0000400L
#define FLG_HEAP_ENABLE_TAGGING            0x0000800L
#define FLG_USER_STACK_TRACE_DB            0x0001000L
#define FLG_KERNEL_STACK_TRACE_DB        0x0002000L
#define FLG_MAINTAIN_OBJECT_TYPELIST    0x0004000L
#define FLG_HEAP_ENABLE_TAG_BY_DLL        0x0008000L
#define FLG_IGNORE_DEBUG_PRIV            0x0010000L
#define FLG_ENABLE_CSRDEBUG                0x0020000L
#define FLG_ENABLE_KDEBUG_SYMBOL_LOAD    0x0040000L
#define FLG_DISABLE_PAGE_KERNEL_STACKS    0x0080000L
#define FLG_HEAP_ENABLE_CALL_TRACING    0x0100000L
#define FLG_HEAP_DISABLE_COALESCING        0x0200000L
#define FLG_ENABLE_CLOSE_EXCEPTIONS        0x0400000L
#define FLG_ENABLE_EXCEPTION_LOGGING    0x0800000L
#define FLG_ENABLE_DBGPRINT_BUFFERING    0x8000000L

#define PROTECT_FROM_CLOSE    0x1L
#define INHERIT                0x2L

#define FLG_SYSOBJINFO_SINGLE_HANDLE_ENTRY        0x40L
#define FLG_SYSOBJINFO_DEFAULT_SECURITY_QUOTA    0x20L
#define FLG_SYSOBJINFO_PERMANENT                0x10L
#define FLG_SYSOBJINFO_EXCLUSIVE                0x08L
#define FLG_SYSOBJINFO_CREATOR_INFO                0x04L
#define FLG_SYSOBJINFO_KERNEL_MODE                0x02L

#define PERMANENT                         0x10L
#define EXCLUSIVE                         0x20L

#define WSLE_PAGE_READONLY                0x001L
#define WSLE_PAGE_EXECUTE                 0x002L
#define WSLE_PAGE_READWRITE               0x004L
#define WSLE_PAGE_EXECUTE_READ            0x003L
#define WSLE_PAGE_WRITECOPY               0x005L
#define WSLE_PAGE_EXECUTE_READWRITE       0x006L
#define WSLE_PAGE_EXECUTE_WRITECOPY       0x007L
#define WSLE_PAGE_SHARE_COUNT_MASK        0x0E0L
#define WSLE_PAGE_SHAREABLE               0x100L

#define LOCK_VM_IN_WSL                    0x1L
#define LOCK_VM_IN_RAM                    0x2L

#define PC_IDLE                           0x1L
#define PC_NORMAL                         0x2L
#define PC_HIGH                           0x3L
#define PC_REALTIME                       0x4L
#define PC_BELOW_NORMAL                   0x5L
#define PC_ABOVE_NORMAL                   0x6L

#define PDI_MODULES                       0x01L
#define PDI_BACKTRACE                     0x02L
#define PDI_HEAPS                         0x04L
#define PDI_HEAP_TAGS                     0x08L
#define PDI_HEAP_BLOCKS                   0x10L
#define PDI_LOCKS                         0x20L

#define LDRP_STATIC_LINK                  0x000002L
#define LDRP_IMAGE_DLL                    0x000004L
#define LDRP_LOAD_IN_PROGRESS             0x001000L
#define LDRP_UNLOAD_IN_PROGRESS           0x002000L
#define LDRP_ENTRY_PROCESSED              0x004000L
#define LDRP_ENTRY_INSERTED               0x008000L
#define LDRP_CURRENT_LOAD                 0x010000L
#define LDRP_FAILED_BUILTIN_LOAD          0x020000L
#define LDRP_DONT_CALL_FOR_THREADS        0x040000L
#define LDRP_PROCESS_ATTACH_CALLED        0x080000L
#define LDRP_DEBUG_SYMBOLS_LOADED         0x100000L
#define LDRP_IMAGE_NOT_AT_BASE            0x200000L
#define LDRP_WX86_IGNORE_MACHINETYPE      0x400000L

#define LPC_MESSAGE_BASE_SIZE    0x18L

#define FILE_SUPERSEDE                  0x0L
#define FILE_OPEN                       0x1L
#define FILE_CREATE                     0x2L
#define FILE_OPEN_IF                    0x3L
#define FILE_OVERWRITE                  0x4L
#define FILE_OVERWRITE_IF               0x5L
#define FILE_MAXIMUM_DISPOSITION        0x5L

#define FILE_SUPERSEDED                 0x0L
#define FILE_OPENED                     0x1L
#define FILE_CREATED                    0x2L
#define FILE_OVERWRITTEN                0x3L
#define FILE_EXISTS                     0x4L
#define FILE_DOES_NOT_EXIST             0x5L

#define REG_MONITOR_SINGLE_KEY          0x0L
#define REG_MONITOR_SECOND_KEY          0x1L

#define HASH_STRING_ALGORITHM_DEFAULT   0x00000000L
#define HASH_STRING_ALGORITHM_X65599    0x00000001L
#define HASH_STRING_ALGORITHM_INVALID   0xFFFFFFFFL

#define SE_MIN_WELL_KNOWN_PRIVILEGE            0x02L
#define SE_CREATE_TOKEN_PRIVILEGE            0x02L
#define SE_ASSIGNPRIMARYTOKEN_PRIVILEGE        0x03L
#define SE_LOCK_MEMORY_PRIVILEGE            0x04L
#define SE_INCREASE_QUOTA_PRIVILEGE            0x05L
#define SE_UNSOLICITED_INPUT_PRIVILEGE        0x06L
#define SE_MACHINE_ACCOUNT_PRIVILEGE        0x06L
#define SE_TCB_PRIVILEGE                    0x07L
#define SE_SECURITY_PRIVILEGE                0x08L
#define SE_TAKE_OWNERSHIP_PRIVILEGE            0x09L
#define SE_LOAD_DRIVER_PRIVILEGE            0x0AL
#define SE_SYSTEM_PROFILE_PRIVILEGE            0x0BL
#define SE_SYSTEMTIME_PRIVILEGE                0x0CL
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE    0x0DL
#define SE_INC_BASE_PRIORITY_PRIVILEGE        0x0EL
#define SE_CREATE_PAGEFILE_PRIVILEGE        0x0FL
#define SE_CREATE_PERMANENT_PRIVILEGE        0x10L
#define SE_BACKUP_PRIVILEGE                    0x11L
#define SE_RESTORE_PRIVILEGE                0x12L
#define SE_SHUTDOWN_PRIVILEGE                0x13L
#define SE_DEBUG_PRIVILEGE                    0x14L
#define SE_AUDIT_PRIVILEGE                    0x15L
#define SE_SYSTEM_ENVIRONMENT_PRIVILEGE        0x16L
#define SE_CHANGE_NOTIFY_PRIVILEGE            0x17L
#define SE_REMOTE_SHUTDOWN_PRIVILEGE        0x18L
#define SE_UNDOCK_PRIVILEGE                    0x19L
#define SE_SYNC_AGENT_PRIVILEGE                0x1AL
#define SE_ENABLE_DELEGATION_PRIVILEGE        0x1BL
#define SE_MANAGE_VOLUME_PRIVILEGE            0x1CL
#define SE_IMPERSONATE_PRIVILEGE            0x1DL
#define SE_CREATE_GLOBAL_PRIVILEGE            0x1EL
#define SE_MAX_WELL_KNOWN_PRIVILEGE            SE_CREATE_GLOBAL_PRIVILEGE

#define OBJ_INHERIT                0x002L
#define OBJ_PERMANENT            0x010L
#define OBJ_EXCLUSIVE            0x020L
#define OBJ_CASE_INSENSITIVE    0x040L
#define OBJ_OPENIF                0x080L
#define OBJ_OPENLINK            0x100L
#define OBJ_KERNEL_HANDLE        0x200L
#define OBJ_FORCE_ACCESS_CHECK    0x400L
#define OBJ_VALID_ATTRIBUTES    0x7F2L

#define DIRECTORY_QUERY                    0x0001L
#define DIRECTORY_TRAVERSE                0x0002L
#define DIRECTORY_CREATE_OBJECT            0x0004L
#define DIRECTORY_CREATE_SUBDIRECTORY    0x0008L
#define DIRECTORY_ALL_ACCESS            (STANDARD_RIGHTS_REQUIRED | 0x000FL)

#define LEVEL_HANDLE_ID            0x74000000L
#define LEVEL_HANDLE_ID_MASK    0xFF000000L
#define LEVEL_HANDLE_INDEX_MASK    0x00FFFFFFL

#define RTL_QUERY_REGISTRY_SUBKEY    0x01L
#define RTL_QUERY_REGISTRY_TOPKEY    0x02L
#define RTL_QUERY_REGISTRY_REQUIRED    0x04L
#define RTL_QUERY_REGISTRY_NOVALUE    0x08L
#define RTL_QUERY_REGISTRY_NOEXPAND    0x10L
#define RTL_QUERY_REGISTRY_DIRECT    0x20L
#define RTL_QUERY_REGISTRY_DELETE    0x40L

#define RTL_REGISTRY_ABSOLUTE    0x00000000L
#define RTL_REGISTRY_SERVICES    0x00000001L
#define RTL_REGISTRY_CONTROL    0x00000002L
#define RTL_REGISTRY_WINDOWS_NT    0x00000003L
#define RTL_REGISTRY_DEVICEMAP    0x00000004L
#define RTL_REGISTRY_USER        0x00000005L
#define RTL_REGISTRY_MAXIMUM    0x00000006L
#define RTL_REGISTRY_HANDLE        0x40000000L
#define RTL_REGISTRY_OPTIONAL    0x80000000L

#define OLD_DOS_VOLID    0x8L

#define FILE_DIRECTORY_FILE                     0x000001L
#define FILE_WRITE_THROUGH                      0x000002L
#define FILE_SEQUENTIAL_ONLY                    0x000004L
#define FILE_NO_INTERMEDIATE_BUFFERING          0x000008L
#define FILE_SYNCHRONOUS_IO_ALERT               0x000010L
#define FILE_SYNCHRONOUS_IO_NONALERT            0x000020L
#define FILE_NON_DIRECTORY_FILE                 0x000040L
#define FILE_CREATE_TREE_CONNECTION             0x000080L
#define FILE_COMPLETE_IF_OPLOCKED               0x000100L
#define FILE_NO_EA_KNOWLEDGE                    0x000200L
#define FILE_OPEN_FOR_RECOVERY                  0x000400L
#define FILE_RANDOM_ACCESS                      0x000800L
#define FILE_DELETE_ON_CLOSE                    0x001000L
#define FILE_OPEN_BY_FILE_ID                    0x002000L
#define FILE_OPEN_FOR_BACKUP_INTENT             0x004000L
#define FILE_NO_COMPRESSION                     0x008000L
#define FILE_OPEN_REQUIRING_OPLOCK              0x010000L
#define FILE_DISALLOW_EXCLUSIVE                 0x020000L
#define FILE_RESERVE_OPFILTER                   0x100000L
#define FILE_OPEN_REPARSE_POINT                 0x200000L
#define FILE_OPEN_NO_RECALL                     0x400000L
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x800000L

#define GDI_HANDLE_BUFFER_SIZE    0x22L

#define MEM_EXECUTE_OPTION_DISABLE   0x01L
#define MEM_EXECUTE_OPTION_ENABLE    0x02L
#define MEM_EXECUTE_OPTION_PERMANENT 0x08L

#define MAX_LPC_DATA 0x130L

#define ALPC_REQUEST            0x2000L | LPC_REQUEST
#define ALPC_CONNECTION_REQUEST 0x2000L | LPC_CONNECTION_REQUEST

#define SYMBOLIC_LINK_QUERY            0x1L
#define SYMBOLIC_LINK_ALL_ACCESS    STANDARD_RIGHTS_REQUIRED | 0x1L

#define EVENT_PAIR_ALL_ACCESS STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE 

#pragma endregion

#pragma region TYPEDEF

    typedef LONG KPRIORITY;
    typedef PVOID PLANGID;
    typedef DWORD ULONG_PTR;
    typedef ULONG_PTR KAFFINITY;
    typedef USHORT RTL_ATOM, * PRTL_ATOM;
    typedef LARGE_INTEGER PHYSICAL_ADDRESS;

#pragma endregion

#pragma region ENUM

    typedef enum _THREADINFOCLASS
    {
        ThreadBasicInformation,
        ThreadTimes,
        ThreadPriority,
        ThreadBasePriority,
        ThreadAffinityMask,
        ThreadImpersonationToken,
        ThreadDescriptorTableEntry,
        ThreadEnableAlignmentFaultFixup,
        ThreadEventPair_Reusable,
        ThreadQuerySetWin32StartAddress,
        ThreadZeroTlsCell,
        ThreadPerformanceCount,
        ThreadAmILastThread,
        ThreadIdealProcessor,
        ThreadPriorityBoost,
        ThreadSetTlsArrayAddress,
        ThreadIsIoPending,
        ThreadHideFromDebugger,
        ThreadBreakOnTermination,
        MaxThreadInfoClass
    } THREADINFOCLASS;

    typedef enum _KPROFILE_SOURCE
    {
        ProfileTime,
        ProfileAlignmentFixup,
        ProfileTotalIssues,
        ProfilePipelineDry,
        ProfileLoadInstructions,
        ProfilePipelineFrozen,
        ProfileBranchInstructions,
        ProfileTotalNonissues,
        ProfileDcacheMisses,
        ProfileIcacheMisses,
        ProfileCacheMisses,
        ProfileBranchMispredictions,
        ProfileStoreInstructions,
        ProfileFpInstructions,
        ProfileIntegerInstructions,
        Profile2Issue,
        Profile3Issue,
        Profile4Issue,
        ProfileSpecialInstructions,
        ProfileTotalCycles,
        ProfileIcacheIssues,
        ProfileDcacheAccesses,
        ProfileMemoryBarrierCycles,
        ProfileLoadLinkedIssues,
        ProfileMaximum
    } KPROFILE_SOURCE;

    typedef enum _KWAIT_REASON
    {
        Executive,
        FreePage,
        PageIn,
        PoolAllocation,
        DelayExecution,
        Suspended,
        UserRequest,
        WrExecutive,
        WrFreePage,
        WrPageIn,
        WrPoolAllocation,
        WrDelayExecution,
        WrSuspended,
        WrUserRequest,
        WrEventPair,
        WrQueue,
        WrLpcReceive,
        WrLpcReply,
        WrVirtualMemory,
        WrPageOut,
        WrRendezvous,
        Spare2,
        Spare3,
        Spare4,
        Spare5,
        Spare6,
        WrKernel,
        MaximumWaitReason
    } KWAIT_REASON;

    typedef enum _POOL_TYPE
    {
        NonPagedPool,
        PagedPool,
        NonPagedPoolMustSucceed,
        DontUseThisType,
        NonPagedPoolCacheAligned,
        PagedPoolCacheAligned,
        NonPagedPoolCacheAlignedMustS,
        MaxPoolType,
        NonPagedPoolSession = 32,
        PagedPoolSession,
        NonPagedPoolMustSucceedSession,
        DontUseThisTypeSession,
        NonPagedPoolCacheAlignedSession,
        PagedPoolCacheAlignedSession,
        NonPagedPoolCacheAlignedMustSSession
    } POOL_TYPE;

    typedef enum _THREAD_STATE
    {
        StateInitialized,
        StateReady,
        StateRunning,
        StateStandby,
        StateTerminated,
        StateWait,
        StateTransition,
        StateUnknown
    } THREAD_STATE;

    typedef enum _SYSTEM_HANDLE_TYPE
    {
        OB_TYPE_UNKNOWN,
        OB_TYPE_TYPE,
        OB_TYPE_DIRECTORY,
        OB_TYPE_SYMBOLIC_LINK,
        OB_TYPE_TOKEN,
        OB_TYPE_PROCESS,
        OB_TYPE_THREAD,
        OB_TYPE_UNKNOWN_7,
        OB_TYPE_EVENT,
        OB_TYPE_EVENT_PAIR,
        OB_TYPE_MUTANT,
        OB_TYPE_UNKNOWN_11,
        OB_TYPE_SEMAPHORE,
        OB_TYPE_TIMER,
        OB_TYPE_PROFILE,
        OB_TYPE_WINDOW_STATION,
        OB_TYPE_DESKTOP,
        OB_TYPE_SECTION,
        OB_TYPE_KEY,
        OB_TYPE_PORT,
        OB_TYPE_WAITABLE_PORT,
        OB_TYPE_UNKNOWN_21,
        OB_TYPE_UNKNOWN_22,
        OB_TYPE_UNKNOWN_23,
        OB_TYPE_UNKNOWN_24,
        OB_TYPE_IO_COMPLETION,
        OB_TYPE_FILE
    }SYSTEM_HANDLE_TYPE;

    typedef enum _DEBUG_CONTROL_CODE
    {
        DebugGetTraceInformation = 1,
        DebugSetInternalBreakpoint,
        DebugSetSpecialCall,
        DebugClearSpecialCalls,
        DebugQuerySpecialCalls,
        DebugDbgBreakPoint,
        DebugMaximum
    } DEBUG_CONTROL_CODE;

    typedef enum _SYSDBG_COMMAND
    {
        SysDbgQueryModuleInformation = 0,
        SysDbgQueryTraceInformation,
        SysDbgSetTracepoint,
        SysDbgSetSpecialCall,
        SysDbgClearSpecialCalls,
        SysDbgQuerySpecialCalls,
        SysDbgBreakPoint,
        SysDbgQueryVersion,
        SysDbgReadVirtual,
        SysDbgWriteVirtual,
        SysDbgReadPhysical,
        SysDbgWritePhysical,
        SysDbgReadControlSpace,
        SysDbgWriteControlSpace,
        SysDbgReadIoSpace,
        SysDbgWriteIoSpace,
        SysDbgReadMsr,
        SysDbgWriteMsr,
        SysDbgReadBusData,
        SysDbgWriteBusData,
        SysDbgCheckLowMemory,
        SysDbgEnableKernelDebugger,
        SysDbgDisableKernelDebugger,
        SysDbgGetAutoKdEnable,
        SysDbgSetAutoKdEnable,
        SysDbgGetPrintBufferSize,
        SysDbgSetPrintBufferSize,
        SysDbgGetKdUmExceptionEnable,
        SysDbgSetKdUmExceptionEnable,
        SysDbgGetTriageDump,
        SysDbgGetKdBlockEnable,
        SysDbgSetKdBlockEnable,
    } SYSDBG_COMMAND, * PSYSDBG_COMMAND;

    typedef enum _INTERFACE_TYPE
    {
        InterfaceTypeUndefined = -1,
        Internal,
        Isa,
        Eisa,
        MicroChannel,
        TurboChannel,
        PCIBus,
        VMEBus,
        NuBus,
        PCMCIABus,
        CBus,
        MPIBus,
        MPSABus,
        ProcessorInternal,
        InternalPowerBus,
        PNPISABus,
        PNPBus,
        MaximumInterfaceType
    }INTERFACE_TYPE, * PINTERFACE_TYPE;

    typedef enum _BUS_DATA_TYPE
    {
        ConfigurationSpaceUndefined = -1,
        Cmos,
        EisaConfiguration,
        Pos,
        CbusConfiguration,
        PCIConfiguration,
        VMEConfiguration,
        NuBusConfiguration,
        PCMCIAConfiguration,
        MPIConfiguration,
        MPSAConfiguration,
        PNPISAConfiguration,
        SgiInternalConfiguration,
        MaximumBusDataType
    } BUS_DATA_TYPE, * PBUS_DATA_TYPE;

    typedef enum _OBJECT_INFORMATION_CLASS
    {
        ObjectBasicInformation,
        ObjectNameInformation,
        ObjectTypeInformation,
        ObjectAllTypesInformation,
        ObjectHandleInformation
    } OBJECT_INFORMATION_CLASS;

    typedef enum _LPC_TYPE
    {
        LPC_NEW_MESSAGE,
        LPC_REQUEST,
        LPC_REPLY,
        LPC_DATAGRAM,
        LPC_LOST_REPLY,
        LPC_PORT_CLOSED,
        LPC_CLIENT_DIED,
        LPC_EXCEPTION,
        LPC_DEBUG_EVENT,
        LPC_ERROR_EVENT,
        LPC_CONNECTION_REQUEST,
        LPC_CONNECTION_REFUSED,
        LPC_MAXIMUM
    } LPC_TYPE;

    typedef enum _KEY_SET_INFORMATION_CLASS
    {
        KeyLastWriteTimeInformation
    } KEY_SET_INFORMATION_CLASS;

    typedef enum _HARDERROR_RESPONSE_OPTION
    {
        OptionAbortRetryIgnore,
        OptionOk,
        OptionOkCancel,
        OptionRetryCancel,
        OptionYesNo,
        OptionYesNoCancel,
        OptionShutdownSystem
    } HARDERROR_RESPONSE_OPTION, * PHARDERROR_RESPONSE_OPTION;

    typedef enum _HARDERROR_RESPONSE
    {
        ResponseReturnToCaller,
        ResponseNotHandled,
        ResponseAbort,
        ResponseCancel,
        ResponseIgnore,
        ResponseNo,
        ResponseOk,
        ResponseRetry,
        ResponseYes
    } HARDERROR_RESPONSE, * PHARDERROR_RESPONSE;

    typedef enum _ATOM_INFORMATION_CLASS
    {
        AtomBasicInformation,
        AtomListInformation
    } ATOM_INFORMATION_CLASS;

    typedef enum _PORT_INFORMATION_CLASS
    {
        PortBasicInformation
    } PORT_INFORMATION_CLASS;

    typedef enum _EVENT_TYPE
    {
        NotificationEvent,
        SynchronizationEvent
    } EVENT_TYPE;

    typedef enum _RTL_GENERIC_COMPARE_RESULTS
    {
        GenericLessThan,
        GenericGreaterThan,
        GenericEqual
    } RTL_GENERIC_COMPARE_RESULTS;

    typedef enum _SECTION_INHERIT
    {
        ViewShare = 1,
        ViewUnmap = 2
    } SECTION_INHERIT;

    typedef enum _KEY_VALUE_INFORMATION_CLASS
    {
        KeyValueBasicInformation,
        KeyValueFullInformation,
        KeyValuePartialInformation,
        KeyValueFullInformationAlign64,
        KeyValuePartialInformationAlign64,
        MaxKeyValueInfoClass
    } KEY_VALUE_INFORMATION_CLASS;

    typedef enum _KEY_INFORMATION_CLASS
    {
        KeyBasicInformation,
        KeyNodeInformation,
        KeyFullInformation,
        KeyNameInformation,
        KeyCachedInformation,
        KeyFlagsInformation,
        MaxKeyInfoClass
    } KEY_INFORMATION_CLASS;

    typedef enum _SYSTEM_INFORMATION_CLASS
    {
        SystemBasicInformation,
        SystemProcessorInformation,
        SystemPerformanceInformation,
        SystemTimeOfDayInformation,
        SystemPathInformation,
        SystemProcessInformation,
        SystemCallCountInformation,
        SystemDeviceInformation,
        SystemProcessorPerformanceInformation,
        SystemFlagsInformation,
        SystemCallTimeInformation,
        SystemModuleInformation,
        SystemLocksInformation,
        SystemStackTraceInformation,
        SystemPagedPoolInformation,
        SystemNonPagedPoolInformation,
        SystemHandleInformation,
        SystemObjectInformation,
        SystemPageFileInformation,
        SystemVdmInstemulInformation,
        SystemVdmBopInformation,
        SystemFileCacheInformation,
        SystemPoolTagInformation,
        SystemInterruptInformation,
        SystemDpcBehaviorInformation,
        SystemFullMemoryInformation,
        SystemLoadGdiDriverInformation,
        SystemUnloadGdiDriverInformation,
        SystemTimeAdjustmentInformation,
        SystemSummaryMemoryInformation,
        SystemMirrorMemoryInformation,
        SystemPerformanceTraceInformation,
        SystemObsolete0,
        SystemExceptionInformation,
        SystemCrashDumpStateInformation,
        SystemKernelDebuggerInformation,
        SystemContextSwitchInformation,
        SystemRegistryQuotaInformation,
        SystemExtendServiceTableInformation,
        SystemPrioritySeperation,
        SystemPlugPlayBusInformation,
        SystemDockInformation,
        SystemPowerInformationNative,
        SystemProcessorSpeedInformation,
        SystemCurrentTimeZoneInformation,
        SystemLookasideInformation,
        SystemTimeSlipNotification,
        SystemSessionCreate,
        SystemSessionDetach,
        SystemSessionInformation,
        SystemRangeStartInformation,
        SystemVerifierInformation,
        SystemAddVerifier,
        SystemSessionProcessesInformation,
        SystemLoadGdiDriverInSystemSpaceInformation,
        SystemNumaProcessorMap,
        SystemPrefetcherInformation,
        SystemExtendedProcessInformation,
        SystemRecommendedSharedDataAlignment,
        SystemComPlusPackage,
        SystemNumaAvailableMemory,
        SystemProcessorPowerInformation,
        SystemEmulationBasicInformation,
        SystemEmulationProcessorInformation,
        SystemExtendedHanfleInformation,
        SystemLostDelayedWriteInformation,
        SystemBigPoolInformation,
        SystemSessionPoolTagInformation,
        SystemSessionMappedViewInformation,
        SystemHotpatchInformation,
        SystemObjectSecurityMode,
        SystemWatchDogTimerHandler,
        SystemWatchDogTimerInformation,
        SystemLogicalProcessorInformation,
        SystemWo64SharedInformationObosolete,
        SystemRegisterFirmwareTableInformationHandler,
        SystemFirmwareTableInformation,
        SystemModuleInformationEx,
        SystemVerifierTriageInformation,
        SystemSuperfetchInformation,
        SystemMemoryListInformation,
        SystemFileCacheInformationEx,
        SystemThreadPriorityClientIdInformation,
        SystemProcessorIdleCycleTimeInformation,
        SystemVerifierCancellationInformation,
        SystemProcessorPowerInformationEx,
        SystemRefTraceInformation,
        SystemSpecialPoolInformation,
        SystemProcessIdInformation,
        SystemErrorPortInformation,
        SystemBootEnvironmentInformation,
        SystemHypervisorInformation,
        SystemVerifierInformationEx,
        SystemTimeZoneInformation,
        SystemImageFileExecutionOptionsInformation,
        SystemCoverageInformation,
        SystemPrefetchPathInformation,
        SystemVerifierFaultsInformation,
        MaxSystemInfoClass
    } SYSTEM_INFORMATION_CLASS, * PSYSTEM_INFORMATION_CLASS;

    typedef enum _SHUTDOWN_ACTION
    {
        ShutdownNoReboot,
        ShutdownReboot,
        ShutdownPowerOff
    } SHUTDOWN_ACTION, * PSHUTDOWN_ACTION;

    typedef enum _FILE_INFORMATION_CLASS
    {
        FileDirectoryInformation = 1,
        FileFullDirectoryInformation,
        FileBothDirectoryInformation,
        FileBasicInformation,
        FileStandardInformation,
        FileInternalInformation,
        FileEaInformation,
        FileAccessInformation,
        FileNameInformation,
        FileRenameInformation,
        FileLinkInformation,
        FileNamesInformation,
        FileDispositionInformation,
        FilePositionInformation,
        FileFullEaInformation,
        FileModeInformation,
        FileAlignmentInformation,
        FileAllInformation,
        FileAllocationInformation,
        FileEndOfFileInformation,
        FileAlternateNameInformation,
        FileStreamInformation,
        FilePipeInformation,
        FilePipeLocalInformation,
        FilePipeRemoteInformation,
        FileMailslotQueryInformation,
        FileMailslotSetInformation,
        FileCompressionInformation,
        FileObjectIdInformation,
        FileCompletionInformation,
        FileMoveClusterInformation,
        FileQuotaInformation,
        FileReparsePointInformation,
        FileNetworkOpenInformation,
        FileAttributeTagInformation,
        FileTrackingInformation,
        FileIdBothDirectoryInformation,
        FileIdFullDirectoryInformation,
        FileValidDataLengthInformation,
        FileShortNameInformation,
        FileIoCompletionNotificationInformation,
        FileIoStatusBlockRangeInformation,
        FileIoPriorityHintInformation,
        FileSfioReserveInformation,
        FileSfioVolumeInformation,
        FileHardLinkInformation,
        FileProcessIdsUsingFileInformation,
        FileNormalizedNameInformation,
        FileNetworkPhysicalNameInformation,
        FileIdGlobalTxDirectoryInformation,
        FileIsRemoteDeviceInformation,
        FileAttributeCacheInformation,
        FileNumaNodeInformation,
        FileStandardLinkInformation,
        FileRemoteProtocolInformation,
        FileMaximumInformation
    } FILE_INFORMATION_CLASS, * PFILE_INFORMATION_CLASS;

    typedef enum _IO_PRIORITY_HINT
    {
        IoPriorityVeryLow,
        IoPriorityLow,
        IoPriorityNormal,
        IoPriorityHigh,
        IoPriorityCritical,
        MaxIoPriorityTypes
    } IO_PRIORITY_HINT;

    typedef enum _FSINFOCLASS
    {
        FileFsVolumeInformation = 1,
        FileFsLabelInformation,
        FileFsSizeInformation,
        FileFsDeviceInformation,
        FileFsAttributeInformation,
        FileFsControlInformation,
        FileFsFullSizeInformation,
        FileFsObjectIdInformation,
        FileFsDriverPathInformation,
        FileFsVolumeFlagsInformation,
        FileFsMaximumInformation
    } FS_INFORMATION_CLASS, * PFS_INFORMATION_CLASS;

    typedef enum _PROCESSINFOCLASS
    {
        ProcessBasicInformation,
        ProcessQuotaLimits,
        ProcessIoCounters,
        ProcessVmCounters,
        ProcessTimes,
        ProcessBasePriority,
        ProcessRaisePriority,
        ProcessDebugPort,
        ProcessExceptionPort,
        ProcessAccessToken,
        ProcessLdtInformation,
        ProcessLdtSize,
        ProcessDefaultHardErrorMode,
        ProcessIoPortHandlers,
        ProcessPooledUsageAndLimits,
        ProcessWorkingSetWatch,
        ProcessUserModeIOPL,
        ProcessEnableAlignmentFaultFixup,
        ProcessPriorityClass,
        ProcessWx86Information,
        ProcessHandleCount,
        ProcessAffinityMask,
        ProcessPriorityBoost,
        ProcessDeviceMap,
        ProcessSessionInformation,
        ProcessForegroundInformation,
        ProcessWow64Information,
        ProcessImageFileName,
        ProcessLUIDDeviceMapsEnabled,
        ProcessBreakOnTermination,
        ProcessDebugObjectHandle,
        ProcessDebugFlags,
        ProcessHandleTracing,
        ProcessIoPriority,
        ProcessExecuteFlags,
        ProcessTlsInformation,
        ProcessCookie,
        ProcessImageInformation,
        ProcessCycleTime,
        ProcessPagePriority,
        ProcessInstrumentationCallback,
        ProcessThreadStackAllocation,
        ProcessWorkingSetWatchEx,
        ProcessImageFileNameWin32,
        ProcessImageFileMapping,
        ProcessAffinityUpdateMode,
        ProcessMemoryAllocationMode,
        ProcessGroupInformation,
        ProcessTokenVirtualizationEnabled,
        ProcessConsoleHostProcess,
        ProcessWindowInformation,
        MaxProcessInfoClass
    } PROCESSINFOCLASS;

    typedef enum _MEMORY_INFORMATION_CLASS
    {
        MemoryBasicInformation,
        MemoryWorkingSetInformation,
        MemoryMappedFilenameInformation,
        MemoryRegionInformation,
        MemoryWorkingSetExInformation
    } MEMORY_INFORMATION_CLASS;

    typedef enum _WAIT_TYPE
    {
        WaitAll,
        WaitAny
    } WAIT_TYPE;

    typedef enum _EVENT_INFORMATION_CLASS
    {
        EventBasicInformation
    } EVENT_INFORMATION_CLASS;

    typedef enum _SECTION_INFORMATION_CLASS
    {
        SectionBasicInformation,
        SectionImageInformation
    } SECTION_INFORMATION_CLASS, * PSECTION_INFORMATION_CLASS;

#pragma endregion

#pragma region STRUCT

    typedef struct _STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PCHAR Buffer;
    } STRING, ANSI_STRING, OEM_STRING, * PSTRING, * PANSI_STRING, * PCANSI_STRING, * POEM_STRING;
    typedef const STRING* PCOEM_STRING;

    typedef struct _UNICODE_STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR  Buffer;
    } UNICODE_STRING, * PUNICODE_STRING;
    typedef const UNICODE_STRING* PCUNICODE_STRING;

    typedef struct _CLIENT_ID
    {
        HANDLE  UniqueProcess;
        HANDLE  UniqueThread;
    } CLIENT_ID, * PCLIENT_ID;

    typedef struct _CURDIR
    {
        UNICODE_STRING DosPath;
        HANDLE Handle;
    } CURDIR, * PCURDIR;

    typedef struct _OBJECT_ATTRIBUTES
    {
        ULONG Length;
        HANDLE RootDirectory;
        PUNICODE_STRING ObjectName;
        ULONG Attributes;
        PVOID SecurityDescriptor;
        PVOID SecurityQualityOfService;
    } OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

    typedef struct _PEB_FREE_BLOCK
    {
        struct _PEB_FREE_BLOCK* Next;
        ULONG Size;
    } PEB_FREE_BLOCK, * PPEB_FREE_BLOCK;

    typedef struct _PEB_LDR_DATA
    {
        ULONG Length;
        BOOLEAN Initialized;
        HANDLE SsHandle;
        LIST_ENTRY InLoadOrderModuleList;
        LIST_ENTRY InMemoryOrderModuleList;
        LIST_ENTRY InInitializationOrderModuleList;
        PVOID      EntryInProgress;
    } PEB_LDR_DATA, * PPEB_LDR_DATA;

    typedef struct _RTL_DRIVE_LETTER_CURDIR
    {
        USHORT Flags;
        USHORT Length;
        ULONG  TimeStamp;
        STRING DosPath;
    } RTL_DRIVE_LETTER_CURDIR, * PRTL_DRIVE_LETTER_CURDIR;

    typedef struct _RTL_USER_PROCESS_PARAMETERS
    {
        ULONG MaximumLength;
        ULONG Length;
        ULONG Flags;
        ULONG DebugFlags;
        PVOID ConsoleHandle;
        ULONG ConsoleFlags;
        HANDLE StandardInput;
        HANDLE StandardOutput;
        HANDLE StandardError;
        CURDIR CurrentDirectory;
        UNICODE_STRING DllPath;
        UNICODE_STRING ImagePathName;
        UNICODE_STRING CommandLine;
        PVOID Environment;
        ULONG StartingX;
        ULONG StartingY;
        ULONG CountX;
        ULONG CountY;
        ULONG CountCharsX;
        ULONG CountCharsY;
        ULONG FillAttribute;
        ULONG WindowFlags;
        ULONG ShowWindowFlags;
        UNICODE_STRING WindowTitle;
        UNICODE_STRING DesktopInfo;
        UNICODE_STRING ShellInfo;
        UNICODE_STRING RuntimeData;
        RTL_DRIVE_LETTER_CURDIR CurrentDirectores[0x20];
    } RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

    typedef struct _PEB
    {
        BOOLEAN InheritedAddressSpace;
        BOOLEAN ReadImageFileExecOptions;
        BOOLEAN BeingDebugged;
        BOOLEAN SpareBool;
        HANDLE Mutant;
        PVOID ImageBaseAddress;
        PPEB_LDR_DATA Ldr;
        PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
        PVOID SubSystemData;
        PVOID ProcessHeap;
        PVOID FastPebLock;
        PVOID FastPebLockRoutine;
        PVOID FastPebUnlockRoutine;
        ULONG EnvironmentUpdateCount;
        PVOID KernelCallbackTable;
        HANDLE SystemReserved;
        PVOID  AtlThunkSListPtr32;
        PPEB_FREE_BLOCK FreeList;
        ULONG TlsExpansionCounter;
        PVOID TlsBitmap;
        ULONG TlsBitmapBits[2];
        PVOID ReadOnlySharedMemoryBase;
        PVOID ReadOnlySharedMemoryHeap;
        PVOID* ReadOnlyStaticServerData;
        PVOID AnsiCodePageData;
        PVOID OemCodePageData;
        PVOID UnicodeCaseTableData;
        ULONG NumberOfProcessors;
        ULONG NtGlobalFlag;
        LARGE_INTEGER CriticalSectionTimeout;
        ULONG HeapSegmentReserve;
        ULONG HeapSegmentCommit;
        ULONG HeapDeCommitTotalFreeThreshold;
        ULONG HeapDeCommitFreeBlockThreshold;
        ULONG NumberOfHeaps;
        ULONG MaximumNumberOfHeaps;
        PVOID* ProcessHeaps;
        PVOID GdiSharedHandleTable;
        PVOID ProcessStarterHelper;
        PVOID GdiDCAttributeList;
        PVOID LoaderLock;
        ULONG OSMajorVersion;
        ULONG OSMinorVersion;
        USHORT OSBuildNumber;
        USHORT OSCSDVersion;
        ULONG OSPlatformId;
        ULONG ImageSubsystem;
        ULONG ImageSubsystemMajorVersion;
        ULONG ImageSubsystemMinorVersion;
        ULONG ImageProcessAffinityMask;
        ULONG GdiHandleBuffer[GDI_HANDLE_BUFFER_SIZE];
    } PEB, * PPEB;

    typedef struct _TEB
    {
        NT_TIB NtTib;
        PVOID  EnvironmentPointer;
        CLIENT_ID ClientId;
        PVOID ActiveRpcHandle;
        PVOID ThreadLocalStoragePointer;
        PPEB ProcessEnvironmentBlock;
        ULONG LastErrorValue;
        ULONG CountOfOwnedCriticalSections;
        PVOID CsrClientThread;
        PVOID Win32ThreadInfo;
    } TEB, * PTEB;

    typedef struct _PROCESS_BASIC_INFORMATION
    {
        NTSTATUS  ExitStatus;
        PPEB  PebBaseAddress;
        KAFFINITY  AffinityMask;
        KPRIORITY  BasePriority;
        ULONG  UniqueProcessId;
        ULONG  InheritedFromUniqueProcessId;
    } PROCESS_BASIC_INFORMATION, * PPROCESS_BASIC_INFORMATION;

    typedef struct _FILE_NETWORK_OPEN_INFORMATION
    {
        LARGE_INTEGER  CreationTime;
        LARGE_INTEGER  LastAccessTime;
        LARGE_INTEGER  LastWriteTime;
        LARGE_INTEGER  ChangeTime;
        LARGE_INTEGER  AllocationSize;
        LARGE_INTEGER  EndOfFile;
        ULONG  FileAttributes;
    } FILE_NETWORK_OPEN_INFORMATION, * PFILE_NETWORK_OPEN_INFORMATION;

    typedef struct _IO_STATUS_BLOCK
    {
        union
        {
            NTSTATUS Status;
            PVOID Pointer;
        };
        ULONG_PTR Information;
    } IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

    typedef struct _KEY_VALUE_ENTRY
    {
        PUNICODE_STRING ValueName;
        ULONG           DataLength;
        ULONG           DataOffset;
        ULONG           Type;
    } KEY_VALUE_ENTRY, * PKEY_VALUE_ENTRY;

    typedef struct _SYSTEM_BASIC_INFORMATION
    {
        ULONG Reserved;
        ULONG TimerResolution;
        ULONG PageSize;
        ULONG NumberOfPhysicalPages;
        ULONG LowestPhysicalPageNumber;
        ULONG HighestPhysicalPageNumber;
        ULONG AllocationGranularity;
        ULONG MinimumUserModeAddress;
        ULONG MaximumUserModeAddress;
        KAFFINITY ActiveProcessorsAffinityMask;
        CCHAR NumberOfProcessors;
    } SYSTEM_BASIC_INFORMATION, * PSYSTEM_BASIC_INFORMATION;

    typedef struct _SYSTEM_PROCESSOR_INFORMATION
    {
        USHORT  ProcessorArchitecture;
        USHORT  ProcessorLevel;
        USHORT  ProcessorRevision;
        USHORT  Unknown;
        ULONG  FeatureBits;
    } SYSTEM_PROCESSOR_INFORMATION, * PSYSTEM_PROCESSOR_INFORMATION;

    typedef struct _SYSTEM_PERFORMANCE_INFORMATION
    {
        LARGE_INTEGER IdleProcessTime;
        LARGE_INTEGER IoReadTransferCount;
        LARGE_INTEGER IoWriteTransferCount;
        LARGE_INTEGER IoOtherTransferCount;
        ULONG IoReadOperationCount;
        ULONG IoWriteOperationCount;
        ULONG IoOtherOperationCount;
        ULONG AvailablePages;
        ULONG CommittedPages;
        ULONG CommitLimit;
        ULONG PeakCommitment;
        ULONG PageFaultCount;
        ULONG CopyOnWriteCount;
        ULONG TransitionCount;
        ULONG CacheTransitionCount;
        ULONG DemandZeroCount;
        ULONG PageReadCount;
        ULONG PageReadIoCount;
        ULONG CacheReadCount;
        ULONG CacheIoCount;
        ULONG DirtyPagesWriteCount;
        ULONG DirtyWriteIoCount;
        ULONG MappedPagesWriteCount;
        ULONG MappedWriteIoCount;
        ULONG PagedPoolPages;
        ULONG NonPagedPoolPages;
        ULONG PagedPoolAllocs;
        ULONG PagedPoolFrees;
        ULONG NonPagedPoolAllocs;
        ULONG NonPagedPoolFrees;
        ULONG FreeSystemPtes;
        ULONG ResidentSystemCodePage;
        ULONG TotalSystemDriverPages;
        ULONG TotalSystemCodePages;
        ULONG NonPagedPoolLookasideHits;
        ULONG PagedPoolLookasideHits;
        ULONG Spare3Count;
        ULONG ResidentSystemCachePage;
        ULONG ResidentPagedPoolPage;
        ULONG ResidentSystemDriverPage;
        ULONG CcFastReadNoWait;
        ULONG CcFastReadWait;
        ULONG CcFastReadResourceMiss;
        ULONG CcFastReadNotPossible;
        ULONG CcFastMdlReadNoWait;
        ULONG CcFastMdlReadWait;
        ULONG CcFastMdlReadResourceMiss;
        ULONG CcFastMdlReadNotPossible;
        ULONG CcMapDataNoWait;
        ULONG CcMapDataWait;
        ULONG CcMapDataNoWaitMiss;
        ULONG CcMapDataWaitMiss;
        ULONG CcPinMappedDataCount;
        ULONG CcPinReadNoWait;
        ULONG CcPinReadWait;
        ULONG CcPinReadNoWaitMiss;
        ULONG CcPinReadWaitMiss;
        ULONG CcCopyReadNoWait;
        ULONG CcCopyReadWait;
        ULONG CcCopyReadNoWaitMiss;
        ULONG CcCopyReadWaitMiss;
        ULONG CcMdlReadNoWait;
        ULONG CcMdlReadWait;
        ULONG CcMdlReadNoWaitMiss;
        ULONG CcMdlReadWaitMiss;
        ULONG CcReadAheadIos;
        ULONG CcLazyWriteIos;
        ULONG CcLazyWritePages;
        ULONG CcDataFlushes;
        ULONG CcDataPages;
        ULONG ContextSwitches;
        ULONG FirstLevelTbFills;
        ULONG SecondLevelTbFills;
        ULONG SystemCalls;
    } SYSTEM_PERFORMANCE_INFORMATION, * PSYSTEM_PERFORMANCE_INFORMATION;

    typedef struct _SYSTEM_TIME_OF_DAY_INFORMATION
    {
        LARGE_INTEGER  BootTime;
        LARGE_INTEGER  CurrentTime;
        LARGE_INTEGER  TimeZoneBias;
        ULONG  CurrentTimeZoneId;
    } SYSTEM_TIME_OF_DAY_INFORMATION, * PSYSTEM_TIME_OF_DAY_INFORMATION;

    typedef struct _VM_COUNTERS
    {
        ULONG  PeakVirtualSize;
        ULONG  VirtualSize;
        ULONG  PageFaultCount;
        ULONG  PeakWorkingSetSize;
        ULONG  WorkingSetSize;
        ULONG  QuotaPeakPagedPoolUsage;
        ULONG  QuotaPagedPoolUsage;
        ULONG  QuotaPeakNonPagedPoolUsage;
        ULONG  QuotaNonPagedPoolUsage;
        ULONG  PagefileUsage;
        ULONG  PeakPagefileUsage;
        ULONG  PrivatePageCount;
    } VM_COUNTERS;

    typedef struct _SYSTEM_THREADS
    {
        LARGE_INTEGER  KernelTime;
        LARGE_INTEGER  UserTime;
        LARGE_INTEGER  CreateTime;
        ULONG  WaitTime;
        PVOID  StartAddress;
        CLIENT_ID  ClientId;
        KPRIORITY  Priority;
        KPRIORITY  BasePriority;
        ULONG  ContextSwitchCount;
        THREAD_STATE  State;
        KWAIT_REASON  WaitReason;
        ULONG Reversed;
    } SYSTEM_THREADS, * PSYSTEM_THREADS;

    typedef struct _SYSTEM_PROCESSES
    {
        ULONG  NextEntryDelta;
        ULONG  ThreadCount;
        LARGE_INTEGER  Reserved1[3];
        LARGE_INTEGER  CreateTime;
        LARGE_INTEGER  UserTime;
        LARGE_INTEGER  KernelTime;
        UNICODE_STRING  ProcessName;
        KPRIORITY  BasePriority;
        ULONG  ProcessId;
        ULONG  InheritedFromProcessId;
        ULONG  HandleCount;
        ULONG SessionId;
        ULONG_PTR PageDirectoryBase;
        VM_COUNTERS  VmCounters;
        ULONG  PrivatePageCount;
        IO_COUNTERS  IoCounters;
        SYSTEM_THREADS  Threads[1];
    } SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

    typedef struct _SYSTEM_CALLS_INFORMATION
    {
        ULONG  Size;
        ULONG  NumberOfDescriptorTables;
        ULONG  NumberOfRoutinesInTable[1];
        ULONG  CallCounts[ANYSIZE_ARRAY];
    } SYSTEM_CALLS_INFORMATION, * PSYSTEM_CALLS_INFORMATION;

    typedef struct _SYSTEM_CONFIGURATION_INFORMATION
    {
        ULONG  DiskCount;
        ULONG  FloppyCount;
        ULONG  CdRomCount;
        ULONG  TapeCount;
        ULONG  SerialCount;
        ULONG  ParallelCount;
    } SYSTEM_CONFIGURATION_INFORMATION, * PSYSTEM_CONFIGURATION_INFORMATION;

    typedef struct _SYSTEM_PROCESSOR_TIMES
    {
        LARGE_INTEGER  IdleTime;
        LARGE_INTEGER  KernelTime;
        LARGE_INTEGER  UserTime;
        LARGE_INTEGER  DpcTime;
        LARGE_INTEGER  InterruptTime;
        ULONG  InterruptCount;
    } SYSTEM_PROCESSOR_TIMES, * PSYSTEM_PROCESSOR_TIMES;

    typedef struct _SYSTEM_GLOBAL_FLAG
    {
        ULONG  GlobalFlag;
    } SYSTEM_GLOBAL_FLAG, * PSYSTEM_GLOBAL_FLAG;

    typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY
    {
        HANDLE Section;
        PVOID  MappedBase;
        PVOID  Base;
        ULONG  Size;
        ULONG  Flags;
        USHORT  LoadOrderIndex;
        USHORT    InitOrderIndex;
        USHORT  LoadCount;
        USHORT  PathLength;
        CHAR  ImageName[256];
    } SYSTEM_MODULE_INFORMATION_ENTRY, * PSYSTEM_MODULE_INFORMATION_ENTRY;

    typedef struct _SYSTEM_MODULE
    {
        HANDLE Section;
        PVOID  MappedBase;
        PVOID  ImageBase;
        ULONG  ImageSize;
        ULONG  Flags;
        USHORT LoadOrderIndex;
        USHORT InitOrderIndex;
        USHORT LoadCount;
        USHORT OffsetToFileName;
        CHAR   ImageName[256];
    } SYSTEM_MODULE, * PSYSTEM_MODULE;

    typedef struct _SYSTEM_MODULE_INFORMATION
    {
        ULONG         ModulesCount;
        SYSTEM_MODULE Modules[1];
    } SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

    typedef struct _SYSTEM_LOCK_INFORMATION
    {
        PVOID  Address;
        USHORT  Type;
        USHORT  Reserved1;
        ULONG  ExclusiveOwnerThreadId;
        ULONG  ActiveCount;
        ULONG  ContentionCount;
        ULONG  Reserved2[2];
        ULONG  NumberOfSharedWaiters;
        ULONG  NumberOfExclusiveWaiters;
    } SYSTEM_LOCK_INFORMATION, * PSYSTEM_LOCK_INFORMATION;

    typedef struct _SYSTEM_HANDLE_INFORMATION
    {
        ULONG  ProcessId;
        UCHAR  ObjectTypeNumber;
        UCHAR  Flags;
        USHORT  Handle;
        PVOID  Object;
        ACCESS_MASK  GrantedAccess;
    } SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

    typedef struct _SYSTEM_OBJECT_TYPE_INFORMATION
    {
        ULONG  NextEntryOffset;
        ULONG  ObjectCount;
        ULONG  HandleCount;
        ULONG  TypeNumber;
        ULONG  InvalidAttributes;
        GENERIC_MAPPING  GenericMapping;
        ACCESS_MASK  ValidAccessMask;
        POOL_TYPE  PoolType;
        UCHAR  Unknown;
        UNICODE_STRING  Name;
    } SYSTEM_OBJECT_TYPE_INFORMATION, * PSYSTEM_OBJECT_TYPE_INFORMATION;

    typedef struct _SYSTEM_OBJECT_INFORMATION
    {
        ULONG  NextEntryOffset;
        PVOID  Object;
        ULONG  CreatorProcessId;
        USHORT  Unknown;
        USHORT  Flags;
        ULONG  PointerCount;
        ULONG  HandleCount;
        ULONG  PagedPoolUsage;
        ULONG  NonPagedPoolUsage;
        ULONG  ExclusiveProcessId;
        PSECURITY_DESCRIPTOR  SecurityDescriptor;
        UNICODE_STRING  Name;
    } SYSTEM_OBJECT_INFORMATION, * PSYSTEM_OBJECT_INFORMATION;

    typedef struct _SYSTEM_PAGEFILE_INFORMATION
    {
        ULONG  NextEntryOffset;
        ULONG  CurrentSize;
        ULONG  TotalUsed;
        ULONG  PeakUsed;
        UNICODE_STRING  FileName;
    } SYSTEM_PAGEFILE_INFORMATION, * PSYSTEM_PAGEFILE_INFORMATION;

    typedef struct _SYSTEM_INSTRUCTION_EMULATION_INFORMATION
    {
        ULONG  SegmentNotPresent;
        ULONG  TwoByteOpcode;
        ULONG  ESprefix;
        ULONG  CSprefix;
        ULONG  SSprefix;
        ULONG  DSprefix;
        ULONG  FSPrefix;
        ULONG  GSprefix;
        ULONG  OPER32prefix;
        ULONG  ADDR32prefix;
        ULONG  INSB;
        ULONG  INSW;
        ULONG  OUTSB;
        ULONG  OUTSW;
        ULONG  PUSHFD;
        ULONG  POPFD;
        ULONG  INTnn;
        ULONG  INTO;
        ULONG  IRETD;
        ULONG  INBimm;
        ULONG  INWimm;
        ULONG  OUTBimm;
        ULONG  OUTWimm;
        ULONG  INB;
        ULONG  INW;
        ULONG  OUTB;
        ULONG  OUTW;
        ULONG  LOCKprefix;
        ULONG  REPNEprefix;
        ULONG  REPprefix;
        ULONG  HLT;
        ULONG  CLI;
        ULONG  STI;
        ULONG  GenericInvalidOpcode;
    } SYSTEM_INSTRUCTION_EMULATION_INFORMATION, * PSYSTEM_INSTRUCTION_EMULATION_INFORMATION;

    typedef struct _SYSTEM_POOL_TAG_INFORMATION
    {
        CHAR  Tag[4];
        ULONG  PagedPoolAllocs;
        ULONG  PagedPoolFrees;
        ULONG  PagedPoolUsage;
        ULONG  NonPagedPoolAllocs;
        ULONG  NonPagedPoolFrees;
        ULONG  NonPagedPoolUsage;
    } SYSTEM_POOL_TAG_INFORMATION, * PSYSTEM_POOL_TAG_INFORMATION;

    typedef struct _SYSTEM_PROCESSOR_STATISTICS
    {
        ULONG  ContextSwitches;
        ULONG  DpcCount;
        ULONG  DpcRequestRate;
        ULONG  TimeIncrement;
        ULONG  DpcBypassCount;
        ULONG  ApcBypassCount;
    } SYSTEM_PROCESSOR_STATISTICS, * PSYSTEM_PROCESSOR_STATISTICS;

    typedef struct _SYSTEM_DPC_INFORMATION
    {
        ULONG  Reserved;
        ULONG  MaximumDpcQueueDepth;
        ULONG  MinimumDpcRate;
        ULONG  AdjustDpcThreshold;
        ULONG  IdealDpcRate;
    } SYSTEM_DPC_INFORMATION, * PSYSTEM_DPC_INFORMATION;

    typedef struct _SYSTEM_LOAD_IMAGE
    {
        UNICODE_STRING  ModuleName;
        PVOID  ModuleBase;
        PVOID  SectionPointer;
        PVOID  EntryPoint;
        PVOID  ExportDirectory;
    } SYSTEM_LOAD_IMAGE, * PSYSTEM_LOAD_IMAGE;

    typedef struct _SYSTEM_UNLOAD_IMAGE
    {
        PVOID  ModuleBase;
    } SYSTEM_UNLOAD_IMAGE, * PSYSTEM_UNLOAD_IMAGE;

    typedef struct _SYSTEM_QUERY_TIME_ADJUSTMENT
    {
        ULONG  TimeAdjustment;
        ULONG  MaximumIncrement;
        BOOLEAN  TimeSynchronization;
    } SYSTEM_QUERY_TIME_ADJUSTMENT, * PSYSTEM_QUERY_TIME_ADJUSTMENT;

    typedef struct _SYSTEM_SET_TIME_ADJUSTMENT
    {
        ULONG  TimeAdjustment;
        BOOLEAN  TimeSynchronization;
    } SYSTEM_SET_TIME_ADJUSTMENT, * PSYSTEM_SET_TIME_ADJUSTMENT;

    typedef struct _SYSTEM_CRASH_DUMP_INFORMATION
    {
        HANDLE  CrashDumpSectionHandle;
        HANDLE  Unknown;
    } SYSTEM_CRASH_DUMP_INFORMATION, * PSYSTEM_CRASH_DUMP_INFORMATION;

    typedef struct _SYSTEM_EXCEPTION_INFORMATION
    {
        ULONG  AlignmentFixupCount;
        ULONG  ExceptionDispatchCount;
        ULONG  FloatingEmulationCount;
        ULONG  Reserved;
    } SYSTEM_EXCEPTION_INFORMATION, * PSYSTEM_EXCEPTION_INFORMATION;

    typedef struct _SYSTEM_CRASH_DUMP_STATE_INFORMATION
    {
        ULONG  CrashDumpSectionExists;
        ULONG  Unknown;
    } SYSTEM_CRASH_DUMP_STATE_INFORMATION, * PSYSTEM_CRASH_DUMP_STATE_INFORMATION;

    typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION
    {
        BOOLEAN KernelDebuggerEnabled;
        BOOLEAN KernelDebuggerNotPresent;
    } SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

    typedef struct _SYSTEM_CONTEXT_SWITCH_INFORMATION
    {
        ULONG  ContextSwitches;
        ULONG  ContextSwitchCounters[11];
    } SYSTEM_CONTEXT_SWITCH_INFORMATION, * PSYSTEM_CONTEXT_SWITCH_INFORMATION;

    typedef struct _SYSTEM_REGISTRY_QUOTA_INFORMATION
    {
        ULONG RegistryQuotaAllowed;
        ULONG RegistryQuotaUsed;
        ULONG PagedPoolSize;
    } SYSTEM_REGISTRY_QUOTA_INFORMATION, * PSYSTEM_REGISTRY_QUOTA_INFORMATION;

    typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE
    {
        UNICODE_STRING  ModuleName;
    } SYSTEM_LOAD_AND_CALL_IMAGE, * PSYSTEM_LOAD_AND_CALL_IMAGE;

    typedef struct _SYSTEM_PRIORITY_SEPARATION
    {
        ULONG  PrioritySeparation;
    } SYSTEM_PRIORITY_SEPARATION, * PSYSTEM_PRIORITY_SEPARATION;

    typedef struct _SYSTEM_TIME_ZONE_INFORMATION
    {
        LONG  Bias;
        WCHAR  StandardName[32];
        LARGE_INTEGER  StandardDate;
        LONG  StandardBias;
        WCHAR  DaylightName[32];
        LARGE_INTEGER  DaylightDate;
        LONG  DaylightBias;
    } SYSTEM_TIME_ZONE_INFORMATION, * PSYSTEM_TIME_ZONE_INFORMATION;

    typedef struct _SYSTEM_LOOKASIDE_INFORMATION
    {
        USHORT  Depth;
        USHORT  MaximumDepth;
        ULONG  TotalAllocates;
        ULONG  AllocateMisses;
        ULONG  TotalFrees;
        ULONG  FreeMisses;
        POOL_TYPE  Type;
        ULONG  Tag;
        ULONG  Size;
    } SYSTEM_LOOKASIDE_INFORMATION, * PSYSTEM_LOOKASIDE_INFORMATION;

    typedef struct _SYSTEM_SET_TIME_SLIP_EVENT
    {
        HANDLE  TimeSlipEvent;
    } SYSTEM_SET_TIME_SLIP_EVENT, * PSYSTEM_SET_TIME_SLIP_EVENT;

    typedef struct _SYSTEM_CREATE_SESSION
    {
        ULONG  SessionId;
    } SYSTEM_CREATE_SESSION, * PSYSTEM_CREATE_SESSION;

    typedef struct _SYSTEM_DELETE_SESSION
    {
        ULONG  SessionId;
    } SYSTEM_DELETE_SESSION, * PSYSTEM_DELETE_SESSION;

    typedef struct _SYSTEM_RANGE_START_INFORMATION
    {
        PVOID  SystemRangeStart;
    } SYSTEM_RANGE_START_INFORMATION, * PSYSTEM_RANGE_START_INFORMATION;

    typedef struct _SYSTEM_SESSION_PROCESSES_INFORMATION
    {
        ULONG  SessionId;
        ULONG  BufferSize;
        PVOID  Buffer;
    } SYSTEM_SESSION_PROCESSES_INFORMATION, * PSYSTEM_SESSION_PROCESSES_INFORMATION;

    typedef struct _SYSTEM_POOL_BLOCK
    {
        BOOLEAN  Allocated;
        USHORT  Unknown;
        ULONG  Size;
        CHAR  Tag[4];
    } SYSTEM_POOL_BLOCK, * PSYSTEM_POOL_BLOCK;

    typedef struct _SYSTEM_POOL_BLOCKS_INFORMATION
    {
        ULONG  PoolSize;
        PVOID  PoolBase;
        USHORT  Unknown;
        ULONG  NumberOfBlocks;
        SYSTEM_POOL_BLOCK  PoolBlocks[1];
    } SYSTEM_POOL_BLOCKS_INFORMATION, * PSYSTEM_POOL_BLOCKS_INFORMATION;

    typedef struct _SYSTEM_MEMORY_USAGE
    {
        PVOID  Name;
        USHORT  Valid;
        USHORT  Standby;
        USHORT  Modified;
        USHORT  PageTables;
    } SYSTEM_MEMORY_USAGE, * PSYSTEM_MEMORY_USAGE;

    typedef struct _SYSTEM_MEMORY_USAGE_INFORMATION
    {
        ULONG  Reserved;
        PVOID  EndOfData;
        SYSTEM_MEMORY_USAGE  MemoryUsage[1];
    } SYSTEM_MEMORY_USAGE_INFORMATION, * PSYSTEM_MEMORY_USAGE_INFORMATION;

    typedef struct _SYSDBG_PHYSICAL
    {
        PHYSICAL_ADDRESS Address;
        PVOID Buffer;
        ULONG Request;
    } SYSDBG_PHYSICAL, * PSYSDBG_PHYSICAL;

    typedef struct _SYSDBG_CONTROL_SPACE
    {
        ULONG64 Address;
        PVOID Buffer;
        ULONG Request;
        ULONG Processor;
    } SYSDBG_CONTROL_SPACE, * PSYSDBG_CONTROL_SPACE;

    typedef struct _SYSDBG_IO_SPACE
    {
        ULONG64 Address;
        PVOID Buffer;
        ULONG Request;
        INTERFACE_TYPE InterfaceType;
        ULONG BusNumber;
        ULONG AddressSpace;
    } SYSDBG_IO_SPACE, * PSYSDBG_IO_SPACE;

    typedef struct _SYSDBG_MSR
    {
        ULONG Msr;
        ULONG64 Data;
    } SYSDBG_MSR, * PSYSDBG_MSR;

    typedef struct _SYSDBG_BUS_DATA
    {
        ULONG Address;
        PVOID Buffer;
        ULONG Request;
        BUS_DATA_TYPE BusDataType;
        ULONG BusNumber;
        ULONG SlotNumber;
    } SYSDBG_BUS_DATA, * PSYSDBG_BUS_DATA;

    typedef struct _DBGKD_GET_VERSION
    {
        USHORT  MajorVersion;
        USHORT  MinorVersion;
        USHORT  ProtocolVersion;
        USHORT  Flags;
        USHORT  MachineType;
        UCHAR   MaxPacketType;
        UCHAR   MaxStateChange;
        UCHAR   MaxManipulate;
        UCHAR   Simulation;
        USHORT  Unused[1];
        ULONG64 KernBase;
        ULONG64 PsLoadedModuleList;
        ULONG64 DebuggerDataList;
    } DBGKD_GET_VERSION, * PDBGKD_GET_VERSION;

    typedef struct _SYSDBG_VIRTUAL
    {
        PVOID Address;
        PVOID Buffer;
        ULONG Request;
    } SYSDBG_VIRTUAL, * PSYSDBG_VIRTUAL;

    typedef struct _OBJECT_BASIC_INFORMATION
    {
        ULONG Attributes;
        ACCESS_MASK GrantedAccess;
        ULONG HandleCount;
        ULONG PointerCount;
        ULONG PagedPoolCharge;
        ULONG NonPagedPoolCharge;
        ULONG Reserved[3];
        ULONG NameInfoSize;
        ULONG TypeInfoSize;
        ULONG SecurityDescriptorSize;
        LARGE_INTEGER CreationTime;
    } OBJECT_BASIC_INFORMATION, * POBJECT_BASIC_INFORMATION;

    typedef struct _OBJECT_NAME_INFORMATION
    {
        UNICODE_STRING Name;
    } OBJECT_NAME_INFORMATION, * POBJECT_NAME_INFORMATION;

    typedef struct _OBJECT_TYPE_INFORMATION
    {
        UNICODE_STRING TypeName;
        ULONG TotalNumberOfObjects;
        ULONG TotalNumberOfHandles;
        ULONG TotalPagedPoolUsage;
        ULONG TotalNonPagedPoolUsage;
        ULONG TotalNamePoolUsage;
        ULONG TotalHandleTableUsage;
        ULONG HighWaterNumberOfObjects;
        ULONG HighWaterNumberOfHandles;
        ULONG HighWaterPagedPoolUsage;
        ULONG HighWaterNonPagedPoolUsage;
        ULONG HighWaterNamePoolUsage;
        ULONG HighWaterHandleTableUsage;
        ULONG InvalidAttributes;
        GENERIC_MAPPING GenericMapping;
        ULONG ValidAccessMask;
        BOOLEAN SecurityRequired;
        BOOLEAN MaintainHandleCount;
        POOL_TYPE PoolType;
        ULONG DefaultPagedPoolCharge;
        ULONG DefaultNonPagedPoolCharge;
    } OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

    typedef struct _OBJECT_ALL_TYPES_INFORMATION
    {
        ULONG  NumberOfTypes;
        OBJECT_TYPE_INFORMATION  TypeInformation;
    } OBJECT_ALL_TYPES_INFORMATION, * POBJECT_ALL_TYPES_INFORMATION;

    typedef struct _OBJECT_HANDLE_ATTRIBUTE_INFORMATION
    {
        BOOLEAN  Inherit;
        BOOLEAN  ProtectFromClose;
    } OBJECT_HANDLE_ATTRIBUTE_INFORMATION, * POBJECT_HANDLE_ATTRIBUTE_INFORMATION;

    typedef struct _OBJECT_HANDLE_FLAG_INFORMATION
    {
        BOOLEAN Inherit;
        BOOLEAN ProtectFromClose;
    } OBJECT_HANDLE_FLAG_INFORMATION, * POBJECT_HANDLE_FLAG_INFORMATION;

    typedef struct _OBJECT_DIRECTORY_INFORMATION
    {
        UNICODE_STRING Name;
        UNICODE_STRING TypeName;
    } OBJECT_DIRECTORY_INFORMATION, * POBJECT_DIRECTORY_INFORMATION;

    typedef struct _DIRECTORY_BASIC_INFORMATION
    {
        UNICODE_STRING  ObjectName;
        UNICODE_STRING  ObjectTypeName;
    } DIRECTORY_BASIC_INFORMATION, * PDIRECTORY_BASIC_INFORMATION;

    typedef struct _MEMORY_WORKING_SET_LIST
    {
        ULONG  NumberOfPages;
        ULONG  WorkingSetList[1];
    } MEMORY_WORKING_SET_LIST, * PMEMORY_WORKING_SET_LIST;

    typedef struct _MEMORY_SECTION_NAME
    {
        UNICODE_STRING  SectionFileName;
    } MEMORY_SECTION_NAME, * PMEMORY_SECTION_NAME;

    typedef struct _USER_STACK
    {
        PVOID  FixedStackBase;
        PVOID  FixedStackLimit;
        PVOID  ExpandableStackBase;
        PVOID  ExpandableStackLimit;
        PVOID  ExpandableStackBottom;
    } USER_STACK, * PUSER_STACK;

    typedef struct _THREAD_BASIC_INFORMATION
    {
        NTSTATUS  ExitStatus;
        PNT_TIB  TebBaseAddress;
        CLIENT_ID  ClientId;
        KAFFINITY  AffinityMask;
        KPRIORITY  Priority;
        KPRIORITY  BasePriority;
    } THREAD_BASIC_INFORMATION, * PTHREAD_BASIC_INFORMATION;

    typedef struct _KERNEL_USER_TIMES
    {
        LARGE_INTEGER  CreateTime;
        LARGE_INTEGER  ExitTime;
        LARGE_INTEGER  KernelTime;
        LARGE_INTEGER  UserTime;
    } KERNEL_USER_TIMES, * PKERNEL_USER_TIMES;

    typedef struct _PROCESS_ACCESS_TOKEN
    {
        HANDLE  Token;
        HANDLE  Thread;
    } PROCESS_ACCESS_TOKEN, * PPROCESS_ACCESS_TOKEN;

    typedef struct _POOLED_USAGE_AND_LIMITS
    {
        ULONG  PeakPagedPoolUsage;
        ULONG  PagedPoolUsage;
        ULONG  PagedPoolLimit;
        ULONG  PeakNonPagedPoolUsage;
        ULONG  NonPagedPoolUsage;
        ULONG  NonPagedPoolLimit;
        ULONG  PeakPagefileUsage;
        ULONG  PagefileUsage;
        ULONG  PagefileLimit;
    } POOLED_USAGE_AND_LIMITS, * PPOOLED_USAGE_AND_LIMITS;

    typedef struct _PROCESS_WS_WATCH_INFORMATION
    {
        PVOID  FaultingPc;
        PVOID  FaultingVa;
    } PROCESS_WS_WATCH_INFORMATION, * PPROCESS_WS_WATCH_INFORMATION;

    typedef struct _PROCESS_PRIORITY_CLASS
    {
        BOOLEAN  Foreground;
        UCHAR  PriorityClass;
    } PROCESS_PRIORITY_CLASS, * PPROCESS_PRIORITY_CLASS;

    typedef struct _PROCESS_DEVICEMAP_INFORMATION
    {
        union
        {
            struct
            {
                HANDLE DirectoryHandle;
            } Set;
            struct
            {
                ULONG DriveMap;
                UCHAR DriveType[32];
            } Query;
        };
    } PROCESS_DEVICEMAP_INFORMATION, * PPROCESS_DEVICEMAP_INFORMATION;

    typedef struct _PROCESS_SESSION_INFORMATION
    {
        ULONG  SessionId;
    } PROCESS_SESSION_INFORMATION, * PPROCESS_SESSION_INFORMATION;

    typedef struct _DEBUG_BUFFER
    {
        HANDLE  SectionHandle;
        PVOID  SectionBase;
        PVOID  RemoteSectionBase;
        ULONG  SectionBaseDelta;
        HANDLE  EventPairHandle;
        ULONG  Unknown[2];
        HANDLE  RemoteThreadHandle;
        ULONG  InfoClassMask;
        ULONG  SizeOfInfo;
        ULONG  AllocatedSize;
        ULONG  SectionSize;
        PVOID  ModuleInformation;
        PVOID  BackTraceInformation;
        PVOID  HeapInformation;
        PVOID  LockInformation;
        PVOID  Reserved[8];
    } DEBUG_BUFFER, * PDEBUG_BUFFER;

    typedef struct _DEBUG_MODULE_INFORMATION
    {
        ULONG  Reserved[2];
        ULONG  Base;
        ULONG  Size;
        ULONG  Flags;
        USHORT  Index;
        USHORT  Unknown;
        USHORT  LoadCount;
        USHORT  ModuleNameOffset;
        CHAR  ImageName[256];
    } DEBUG_MODULE_INFORMATION, * PDEBUG_MODULE_INFORMATION;

    typedef struct _DEBUG_HEAP_INFORMATION
    {
        ULONG  Base;
        ULONG  Flags;
        USHORT  Granularity;
        USHORT  Unknown;
        ULONG  Allocated;
        ULONG  Committed;
        ULONG  TagCount;
        ULONG  BlockCount;
        ULONG  Reserved[7];
        PVOID  Tags;
        PVOID  Blocks;
    } DEBUG_HEAP_INFORMATION, * PDEBUG_HEAP_INFORMATION;

    typedef struct _DEBUG_LOCK_INFORMATION
    {
        PVOID  Address;
        USHORT  Type;
        USHORT  CreatorBackTraceIndex;
        ULONG  OwnerThreadId;
        ULONG  ActiveCount;
        ULONG  ContentionCount;
        ULONG  EntryCount;
        ULONG  RecursionCount;
        ULONG  NumberOfSharedWaiters;
        ULONG  NumberOfExclusiveWaiters;
    } DEBUG_LOCK_INFORMATION, * PDEBUG_LOCK_INFORMATION;

    typedef struct _LPC_MESSAGE
    {
        USHORT  DataSize;
        USHORT  MessageSize;
        USHORT  MessageType;
        USHORT  VirtualRangesOffset;
        CLIENT_ID  ClientId;
        ULONG  MessageId;
        ULONG  SectionSize;
        UCHAR  Data[ANYSIZE_ARRAY];
    } LPC_MESSAGE, * PLPC_MESSAGE;

    typedef struct _LPC_SECTION_WRITE
    {
        ULONG  Length;
        HANDLE  SectionHandle;
        ULONG  SectionOffset;
        ULONG  ViewSize;
        PVOID  ViewBase;
        PVOID  TargetViewBase;
    } LPC_SECTION_WRITE, * PLPC_SECTION_WRITE;

    typedef struct _LPC_SECTION_READ
    {
        ULONG  Length;
        ULONG  ViewSize;
        PVOID  ViewBase;
    } LPC_SECTION_READ, * PLPC_SECTION_READ;

    typedef struct _KEY_LAST_WRITE_TIME_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
    } KEY_LAST_WRITE_TIME_INFORMATION, * PKEY_LAST_WRITE_TIME_INFORMATION;

    typedef struct _KEY_NAME_INFORMATION
    {
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_NAME_INFORMATION, * PKEY_NAME_INFORMATION;

    typedef struct _ATOM_BASIC_INFORMATION
    {
        USHORT  ReferenceCount;
        USHORT  Pinned;
        USHORT  NameLength;
        WCHAR  Name[1];
    } ATOM_BASIC_INFORMATION, * PATOM_BASIC_INFORMATION;

    typedef struct _ATOM_LIST_INFORMATION
    {
        ULONG  NumberOfAtoms;
        ATOM  Atoms[1];
    } ATOM_LIST_INFORMATION, * PATOM_LIST_INFORMATION;

    typedef struct _RTL_SPLAY_LINKS
    {
        struct _RTL_SPLAY_LINKS* Parent;
        struct _RTL_SPLAY_LINKS* LeftChild;
        struct _RTL_SPLAY_LINKS* RightChild;
    } RTL_SPLAY_LINKS, * PRTL_SPLAY_LINKS;

    typedef struct _RTL_HANDLE_TABLE_ENTRY
    {
        struct _RTL_HANDLE_TABLE_ENTRY* Next;
        PVOID  Object;
    } RTL_HANDLE_TABLE_ENTRY, * PRTL_HANDLE_TABLE_ENTRY;

    typedef struct _RTL_HANDLE_TABLE
    {
        ULONG MaximumNumberOfHandles;
        ULONG SizeOfHandleTableEntry;
        ULONG Unknown01;
        ULONG Unknown02;
        PRTL_HANDLE_TABLE_ENTRY FreeHandles;
        PRTL_HANDLE_TABLE_ENTRY CommittedHandles;
        PRTL_HANDLE_TABLE_ENTRY UnCommittedHandles;
        PRTL_HANDLE_TABLE_ENTRY MaxReservedHandles;
    } RTL_HANDLE_TABLE, * PRTL_HANDLE_TABLE;

    typedef struct _KEY_BASIC_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG   TitleIndex;
        ULONG   NameLength;
        WCHAR   Name[1];
    } KEY_BASIC_INFORMATION, * PKEY_BASIC_INFORMATION;

    typedef struct _KEY_NODE_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG   TitleIndex;
        ULONG   ClassOffset;
        ULONG   ClassLength;
        ULONG   NameLength;
        WCHAR   Name[1];
    } KEY_NODE_INFORMATION, * PKEY_NODE_INFORMATION;

    typedef struct _KEY_FULL_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG   TitleIndex;
        ULONG   ClassOffset;
        ULONG   ClassLength;
        ULONG   SubKeys;
        ULONG   MaxNameLen;
        ULONG   MaxClassLen;
        ULONG   Values;
        ULONG   MaxValueNameLen;
        ULONG   MaxValueDataLen;
        WCHAR   Class[1];
    } KEY_FULL_INFORMATION, * PKEY_FULL_INFORMATION;

    typedef struct _KEY_CACHED_INFORMATION
    {
        LARGE_INTEGER LastWriteTime;
        ULONG   TitleIndex;
        ULONG   SubKeys;
        ULONG   MaxNameLen;
        ULONG   Values;
        ULONG   MaxValueNameLen;
        ULONG   MaxValueDataLen;
        ULONG   NameLength;
        WCHAR   Name[1];
    } KEY_CACHED_INFORMATION, * PKEY_CACHED_INFORMATION;

    typedef struct _KEY_FLAGS_INFORMATION
    {
        ULONG   UserFlags;
    } KEY_FLAGS_INFORMATION, * PKEY_FLAGS_INFORMATION;

    typedef struct _KEY_VALUE_BASIC_INFORMATION
    {
        ULONG TitleIndex;
        ULONG Type;
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_VALUE_BASIC_INFORMATION, * PKEY_VALUE_BASIC_INFORMATION;

    typedef struct _KEY_VALUE_FULL_INFORMATION
    {
        ULONG TitleIndex;
        ULONG Type;
        ULONG DataOffset;
        ULONG DataLength;
        ULONG NameLength;
        WCHAR Name[1];
    } KEY_VALUE_FULL_INFORMATION, * PKEY_VALUE_FULL_INFORMATION;

    typedef struct _KEY_VALUE_PARTIAL_INFORMATION
    {
        ULONG   TitleIndex;
        ULONG   Type;
        ULONG   DataLength;
        UCHAR   Data[1];
    } KEY_VALUE_PARTIAL_INFORMATION, * PKEY_VALUE_PARTIAL_INFORMATION;

    typedef struct _SYSTEM_TIMEOFDAY_INFORMATION
    {
        LARGE_INTEGER BootTime;
        LARGE_INTEGER CurrentTime;
        LARGE_INTEGER TimeZoneBias;
        ULONG TimeZoneId;
        ULONG Reserved;
    } SYSTEM_TIMEOFDAY_INFORMATION, * PSYSTEM_TIMEOFDAY_INFORMATION;

    typedef struct _SYSTEM_PROCESS_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG NumberOfThreads;
        LARGE_INTEGER SpareLi1;
        LARGE_INTEGER SpareLi2;
        LARGE_INTEGER SpareLi3;
        LARGE_INTEGER CreateTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER KernelTime;
        UNICODE_STRING ImageName;
        KPRIORITY BasePriority;
        ULONG_PTR UniqueProcessId;
        ULONG_PTR InheritedFromUniqueProcessId;
        ULONG HandleCount;
    } SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

    typedef struct _SYSTEM_DEVICE_INFORMATION
    {
        ULONG NumberOfDisks;
        ULONG NumberOfFloppies;
        ULONG NumberOfCdRoms;
        ULONG NumberOfTapes;
        ULONG NumberOfSerialPorts;
        ULONG NumberOfParallelPorts;
    } SYSTEM_DEVICE_INFORMATION, * PSYSTEM_DEVICE_INFORMATION;

    typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
    {
        LARGE_INTEGER IdleTime;
        LARGE_INTEGER KernelTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER DpcTime;
        LARGE_INTEGER InterruptTime;
        ULONG InterruptCount;
    } SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, * PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

    typedef struct _SYSTEM_FLAGS_INFORMATION
    {
        ULONG GlobalFlag;
    } SYSTEM_FLAGS_INFORMATION, * PSYSTEM_FLAGS_INFORMATION;

    typedef struct _SYSTEM_VDM_INSTEMUL_INFO
    {
        ULONG SegmentNotPresent;
        ULONG VdmOpcode0F;
        ULONG OpcodeESPrefix;
        ULONG OpcodeCSPrefix;
        ULONG OpcodeSSPrefix;
        ULONG OpcodeDSPrefix;
        ULONG OpcodeFSPrefix;
        ULONG OpcodeGSPrefix;
        ULONG OpcodeOPER32Prefix;
        ULONG OpcodeADDR32Prefix;
        ULONG OpcodeINSB;
        ULONG OpcodeINSW;
        ULONG OpcodeOUTSB;
        ULONG OpcodeOUTSW;
        ULONG OpcodePUSHF;
        ULONG OpcodePOPF;
        ULONG OpcodeINTnn;
        ULONG OpcodeINTO;
        ULONG OpcodeIRET;
        ULONG OpcodeINBimm;
        ULONG OpcodeINWimm;
        ULONG OpcodeOUTBimm;
        ULONG OpcodeOUTWimm;
        ULONG OpcodeINB;
        ULONG OpcodeINW;
        ULONG OpcodeOUTB;
        ULONG OpcodeOUTW;
        ULONG OpcodeLOCKPrefix;
        ULONG OpcodeREPNEPrefix;
        ULONG OpcodeREPPrefix;
        ULONG OpcodeHLT;
        ULONG OpcodeCLI;
        ULONG OpcodeSTI;
        ULONG BopCount;
    } SYSTEM_VDM_INSTEMUL_INFO, * PSYSTEM_VDM_INSTEMUL_INFO;

    typedef struct _SYSTEM_QUERY_TIME_ADJUST_INFORMATION
    {
        ULONG TimeAdjustment;
        ULONG TimeIncrement;
        BOOLEAN Enable;
    } SYSTEM_QUERY_TIME_ADJUST_INFORMATION, * PSYSTEM_QUERY_TIME_ADJUST_INFORMATION;

    typedef struct _SYSTEM_SET_TIME_ADJUST_INFORMATION
    {
        ULONG TimeAdjustment;
        BOOLEAN Enable;
    } SYSTEM_SET_TIME_ADJUST_INFORMATION, * PSYSTEM_SET_TIME_ADJUST_INFORMATION;

    typedef struct _SYSTEM_THREAD_INFORMATION
    {
        LARGE_INTEGER KernelTime;
        LARGE_INTEGER UserTime;
        LARGE_INTEGER CreateTime;
        ULONG WaitTime;
        PVOID StartAddress;
        CLIENT_ID ClientId;
        KPRIORITY Priority;
        LONG BasePriority;
        ULONG ContextSwitches;
        ULONG ThreadState;
        ULONG WaitReason;
    } SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

    typedef struct _SYSTEM_MEMORY_INFO
    {
        PUCHAR StringOffset;
        USHORT ValidCount;
        USHORT TransitionCount;
        USHORT ModifiedCount;
        USHORT PageTableCount;
    } SYSTEM_MEMORY_INFO, * PSYSTEM_MEMORY_INFO;

    typedef struct _SYSTEM_MEMORY_INFORMATION
    {
        ULONG InfoSize;
        ULONG StringStart;
        SYSTEM_MEMORY_INFO Memory[1];
    } SYSTEM_MEMORY_INFORMATION, * PSYSTEM_MEMORY_INFORMATION;

    typedef struct _SYSTEM_CRASH_STATE_INFORMATION
    {
        ULONG ValidCrashDump;
    } SYSTEM_CRASH_STATE_INFORMATION, * PSYSTEM_CRASH_STATE_INFORMATION;

    typedef struct _SYSTEM_GDI_DRIVER_INFORMATION
    {
        UNICODE_STRING DriverName;
        PVOID ImageAddress;
        PVOID SectionPointer;
        PVOID EntryPoint;
        PIMAGE_EXPORT_DIRECTORY ExportSectionPointer;
        ULONG ImageLength;
    } SYSTEM_GDI_DRIVER_INFORMATION, * PSYSTEM_GDI_DRIVER_INFORMATION;

    typedef struct _SYSTEM_BOOT_ENVIRONMENT_INFORMATION
    {
        GUID  CurrentBootGuid;
        ULONG Unknown;
    } SYSTEM_BOOT_ENVIRONMENT_INFORMATION, * PSYSTEM_BOOT_ENVIRONMENT_INFORMATION;

    typedef struct _FILE_DIRECTORY_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG FileIndex;
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER EndOfFile;
        LARGE_INTEGER AllocationSize;
        ULONG FileAttributes;
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_DIRECTORY_INFORMATION, * PFILE_DIRECTORY_INFORMATION;

    typedef struct _FILE_FULL_DIR_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG FileIndex;
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER EndOfFile;
        LARGE_INTEGER AllocationSize;
        ULONG FileAttributes;
        ULONG FileNameLength;
        ULONG EaSize;
        WCHAR FileName[1];
    } FILE_FULL_DIR_INFORMATION, * PFILE_FULL_DIR_INFORMATION;

    typedef struct _FILE_BOTH_DIR_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG FileIndex;
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER EndOfFile;
        LARGE_INTEGER AllocationSize;
        ULONG FileAttributes;
        ULONG FileNameLength;
        ULONG EaSize;
        CCHAR ShortNameLength;
        WCHAR ShortName[12];
        WCHAR FileName[1];
    } FILE_BOTH_DIR_INFORMATION, * PFILE_BOTH_DIR_INFORMATION;

    typedef struct _FILE_BASIC_INFORMATION
    {
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        ULONG FileAttributes;
    } FILE_BASIC_INFORMATION, * PFILE_BASIC_INFORMATION;

    typedef struct _FILE_STANDARD_INFORMATION
    {
        LARGE_INTEGER AllocationSize;
        LARGE_INTEGER EndOfFile;
        ULONG NumberOfLinks;
        BOOLEAN DeletePending;
        BOOLEAN Directory;
    } FILE_STANDARD_INFORMATION, * PFILE_STANDARD_INFORMATION;

    typedef struct _FILE_INTERNAL_INFORMATION
    {
        LARGE_INTEGER IndexNumber;
    } FILE_INTERNAL_INFORMATION, * PFILE_INTERNAL_INFORMATION;

    typedef struct _FILE_EA_INFORMATION
    {
        ULONG EaSize;
    } FILE_EA_INFORMATION, * PFILE_EA_INFORMATION;

    typedef struct _FILE_ACCESS_INFORMATION
    {
        ACCESS_MASK AccessFlags;
    } FILE_ACCESS_INFORMATION, * PFILE_ACCESS_INFORMATION;

    typedef struct _FILE_NAME_INFORMATION
    {
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_NAME_INFORMATION, * PFILE_NAME_INFORMATION;

    typedef struct _FILE_RENAME_INFORMATION
    {
        BOOLEAN ReplaceIfExists;
        HANDLE RootDirectory;
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_RENAME_INFORMATION, * PFILE_RENAME_INFORMATION;

    typedef struct _FILE_NAMES_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG FileIndex;
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_NAMES_INFORMATION, * PFILE_NAMES_INFORMATION;

    typedef struct _FILE_DISPOSITION_INFORMATION
    {
        BOOLEAN DeleteFile;
    } FILE_DISPOSITION_INFORMATION, * PFILE_DISPOSITION_INFORMATION;

    typedef struct _FILE_POSITION_INFORMATION
    {
        LARGE_INTEGER CurrentByteOffset;
    } FILE_POSITION_INFORMATION, * PFILE_POSITION_INFORMATION;

    typedef struct _FILE_FULL_EA_INFORMATION
    {
        ULONG NextEntryOffset;
        UCHAR Flags;
        UCHAR EaNameLength;
        USHORT EaValueLength;
        CHAR EaName[1];
    } FILE_FULL_EA_INFORMATION, * PFILE_FULL_EA_INFORMATION;

    typedef struct _FILE_MODE_INFORMATION
    {
        ULONG Mode;
    } FILE_MODE_INFORMATION, * PFILE_MODE_INFORMATION;

    typedef struct _FILE_ALIGNMENT_INFORMATION
    {
        ULONG AlignmentRequirement;
    } FILE_ALIGNMENT_INFORMATION, * PFILE_ALIGNMENT_INFORMATION;

    typedef struct _FILE_ALL_INFORMATION
    {
        FILE_BASIC_INFORMATION BasicInformation;
        FILE_STANDARD_INFORMATION StandardInformation;
        FILE_INTERNAL_INFORMATION InternalInformation;
        FILE_EA_INFORMATION EaInformation;
        FILE_ACCESS_INFORMATION AccessInformation;
        FILE_POSITION_INFORMATION PositionInformation;
        FILE_MODE_INFORMATION ModeInformation;
        FILE_ALIGNMENT_INFORMATION AlignmentInformation;
        FILE_NAME_INFORMATION NameInformation;
    } FILE_ALL_INFORMATION, * PFILE_ALL_INFORMATION;

    typedef struct _FILE_ALLOCATION_INFORMATION
    {
        LARGE_INTEGER AllocationSize;
    } FILE_ALLOCATION_INFORMATION, * PFILE_ALLOCATION_INFORMATION;

    typedef struct _FILE_END_OF_FILE_INFORMATION
    {
        LARGE_INTEGER EndOfFile;
    } FILE_END_OF_FILE_INFORMATION, * PFILE_END_OF_FILE_INFORMATION;

    typedef struct _FILE_STREAM_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG StreamNameLength;
        LARGE_INTEGER StreamSize;
        LARGE_INTEGER StreamAllocationSize;
        WCHAR StreamName[1];
    } FILE_STREAM_INFORMATION, * PFILE_STREAM_INFORMATION;

    typedef struct _FILE_PIPE_INFORMATION
    {
        ULONG ReadMode;
        ULONG CompletionMode;
    } FILE_PIPE_INFORMATION, * PFILE_PIPE_INFORMATION;

    typedef struct _FILE_PIPE_LOCAL_INFORMATION
    {
        ULONG NamedPipeType;
        ULONG NamedPipeConfiguration;
        ULONG MaximumInstances;
        ULONG CurrentInstances;
        ULONG InboundQuota;
        ULONG ReadDataAvailable;
        ULONG OutboundQuota;
        ULONG WriteQuotaAvailable;
        ULONG NamedPipeState;
        ULONG NamedPipeEnd;
    } FILE_PIPE_LOCAL_INFORMATION, * PFILE_PIPE_LOCAL_INFORMATION;

    typedef struct _FILE_PIPE_REMOTE_INFORMATION
    {
        LARGE_INTEGER CollectDataTime;
        ULONG MaximumCollectionCount;
    } FILE_PIPE_REMOTE_INFORMATION, * PFILE_PIPE_REMOTE_INFORMATION;

    typedef struct _FILE_MAILSLOT_QUERY_INFORMATION
    {
        ULONG MaximumMessageSize;
        ULONG MailslotQuota;
        ULONG NextMessageSize;
        ULONG MessagesAvailable;
        LARGE_INTEGER ReadTimeout;
    } FILE_MAILSLOT_QUERY_INFORMATION, * PFILE_MAILSLOT_QUERY_INFORMATION;

    typedef struct _FILE_MAILSLOT_SET_INFORMATION
    {
        PLARGE_INTEGER ReadTimeout;
    } FILE_MAILSLOT_SET_INFORMATION, * PFILE_MAILSLOT_SET_INFORMATION;

    typedef struct _FILE_COMPRESSION_INFORMATION
    {
        LARGE_INTEGER CompressedFileSize;
        USHORT CompressionFormat;
        UCHAR CompressionUnitShift;
        UCHAR ChunkShift;
        UCHAR ClusterShift;
        UCHAR Reserved[3];
    } FILE_COMPRESSION_INFORMATION, * PFILE_COMPRESSION_INFORMATION;

    typedef struct _FILE_LINK_INFORMATION
    {
        BOOLEAN ReplaceIfExists;
        HANDLE RootDirectory;
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_LINK_INFORMATION, * PFILE_LINK_INFORMATION;

    typedef struct _FILE_OBJECTID_INFORMATION
    {
        LONGLONG FileReference;
        UCHAR ObjectId[16];
        union
        {
            struct
            {
                UCHAR BirthVolumeId[16];
                UCHAR BirthObjectId[16];
                UCHAR DomainId[16];
            };
            UCHAR ExtendedInfo[48];
        };
    } FILE_OBJECTID_INFORMATION, * PFILE_OBJECTID_INFORMATION;

    typedef struct _FILE_COMPLETION_INFORMATION
    {
        HANDLE Port;
        PVOID Key;
    } FILE_COMPLETION_INFORMATION, * PFILE_COMPLETION_INFORMATION;

    typedef struct _FILE_MOVE_CLUSTER_INFORMATION
    {
        ULONG ClusterCount;
        HANDLE RootDirectory;
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_MOVE_CLUSTER_INFORMATION, * PFILE_MOVE_CLUSTER_INFORMATION;

    typedef struct _FILE_ATTRIBUTE_TAG_INFORMATION
    {
        ULONG FileAttributes;
        ULONG ReparseTag;
    } FILE_ATTRIBUTE_TAG_INFORMATION, * PFILE_ATTRIBUTE_TAG_INFORMATION;

    typedef struct _FILE_TRACKING_INFORMATION
    {
        HANDLE DestinationFile;
        ULONG ObjectInformationLength;
        CHAR ObjectInformation[1];
    } FILE_TRACKING_INFORMATION, * PFILE_TRACKING_INFORMATION;

    typedef struct _FILE_REPARSE_POINT_INFORMATION
    {
        LONGLONG FileReference;
        ULONG Tag;
    } FILE_REPARSE_POINT_INFORMATION, * PFILE_REPARSE_POINT_INFORMATION;

    typedef struct _FILE_QUOTA_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG SidLength;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER QuotaUsed;
        LARGE_INTEGER QuotaThreshold;
        LARGE_INTEGER QuotaLimit;
        SID Sid;
    } FILE_QUOTA_INFORMATION, * PFILE_QUOTA_INFORMATION;

    typedef struct _FILE_ID_BOTH_DIR_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG FileIndex;
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER EndOfFile;
        LARGE_INTEGER AllocationSize;
        ULONG FileAttributes;
        ULONG FileNameLength;
        ULONG EaSize;
        CCHAR ShortNameLength;
        WCHAR ShortName[12];
        LARGE_INTEGER FileId;
        WCHAR FileName[1];
    } FILE_ID_BOTH_DIR_INFORMATION, * PFILE_ID_BOTH_DIR_INFORMATION;

    typedef struct _FILE_ID_FULL_DIR_INFORMATION
    {
        ULONG NextEntryOffset;
        ULONG FileIndex;
        LARGE_INTEGER CreationTime;
        LARGE_INTEGER LastAccessTime;
        LARGE_INTEGER LastWriteTime;
        LARGE_INTEGER ChangeTime;
        LARGE_INTEGER EndOfFile;
        LARGE_INTEGER AllocationSize;
        ULONG FileAttributes;
        ULONG FileNameLength;
        ULONG EaSize;
        LARGE_INTEGER FileId;
        WCHAR FileName[1];
    } FILE_ID_FULL_DIR_INFORMATION, * PFILE_ID_FULL_DIR_INFORMATION;

    typedef struct _FILE_VALID_DATA_LENGTH_INFORMATION
    {
        LARGE_INTEGER ValidDataLength;
    } FILE_VALID_DATA_LENGTH_INFORMATION, * PFILE_VALID_DATA_LENGTH_INFORMATION;

    typedef  struct _FILE_IO_COMPLETION_NOTIFICATION_INFORMATION
    {
        ULONG Flags;
    } FILE_IO_COMPLETION_NOTIFICATION_INFORMATION, * PFILE_IO_COMPLETION_NOTIFICATION_INFORMATION;

    typedef  struct _FILE_PROCESS_IDS_USING_FILE_INFORMATION
    {
        ULONG NumberOfProcessIdsInList;
        ULONG_PTR ProcessIdList[1];
    } FILE_PROCESS_IDS_USING_FILE_INFORMATION, * PFILE_PROCESS_IDS_USING_FILE_INFORMATION;

    typedef struct _FILE_IOSTATUSBLOCK_RANGE_INFORMATION
    {
        PUCHAR       IoStatusBlockRange;
        ULONG        Length;
    } FILE_IOSTATUSBLOCK_RANGE_INFORMATION, * PFILE_IOSTATUSBLOCK_RANGE_INFORMATION;

    typedef struct _FILE_IO_PRIORITY_HINT_INFORMATION
    {
        IO_PRIORITY_HINT   PriorityHint;
    } FILE_IO_PRIORITY_HINT_INFORMATION, * PFILE_IO_PRIORITY_HINT_INFORMATION;

    typedef struct _FILE_SFIO_RESERVE_INFORMATION
    {
        ULONG RequestsPerPeriod;
        ULONG Period;
        BOOLEAN RetryFailures;
        BOOLEAN Discardable;
        ULONG RequestSize;
        ULONG NumOutstandingRequests;
    } FILE_SFIO_RESERVE_INFORMATION, * PFILE_SFIO_RESERVE_INFORMATION;

    typedef struct _FILE_SFIO_VOLUME_INFORMATION
    {
        ULONG MaximumRequestsPerPeriod;
        ULONG MinimumPeriod;
        ULONG MinimumTransferSize;
    } FILE_SFIO_VOLUME_INFORMATION, * PFILE_SFIO_VOLUME_INFORMATION;

    typedef struct _FILE_LINK_ENTRY_INFORMATION
    {
        ULONG NextEntryOffset;
        LONGLONG ParentFileId;
        ULONG FileNameLength;
        WCHAR FileName[1];
    } FILE_LINK_ENTRY_INFORMATION, * PFILE_LINK_ENTRY_INFORMATION;

    typedef struct _FILE_LINKS_INFORMATION
    {
        ULONG BytesNeeded;
        ULONG EntriesReturned;
        FILE_LINK_ENTRY_INFORMATION Entry;
    } FILE_LINKS_INFORMATION, * PFILE_LINKS_INFORMATION;

    typedef struct _FILE_ID_GLOBAL_TX_DIR_INFORMATION
    {
        ULONG          NextEntryOffset;
        ULONG          FileIndex;
        LARGE_INTEGER  CreationTime;
        LARGE_INTEGER  LastAccessTime;
        LARGE_INTEGER  LastWriteTime;
        LARGE_INTEGER  ChangeTime;
        LARGE_INTEGER  EndOfFile;
        LARGE_INTEGER  AllocationSize;
        ULONG          FileAttributes;
        ULONG          FileNameLength;
        LARGE_INTEGER  FileId;
        GUID           LockingTransactionId;
        ULONG          TxInfoFlags;
        WCHAR          FileName[1];
    } FILE_ID_GLOBAL_TX_DIR_INFORMATION, * PFILE_ID_GLOBAL_TX_DIR_INFORMATION;

    typedef struct _FILE_IS_REMOTE_DEVICE_INFORMATION
    {
        BOOLEAN IsRemote;
    } FILE_IS_REMOTE_DEVICE_INFORMATION, * PFILE_IS_REMOTE_DEVICE_INFORMATION;

    typedef struct _FILE_NUMA_NODE_INFORMATION
    {
        USHORT NodeNumber;
    } FILE_NUMA_NODE_INFORMATION, * PFILE_NUMA_NODE_INFORMATION;

    typedef struct _FILE_FS_VOLUME_INFORMATION
    {
        LARGE_INTEGER VolumeCreationTime;
        ULONG VolumeSerialNumber;
        ULONG VolumeLabelLength;
        BOOLEAN SupportsObjects;
        WCHAR VolumeLabel[1];
    } FILE_FS_VOLUME_INFORMATION, * PFILE_FS_VOLUME_INFORMATION;

    typedef struct _FILE_FS_LABEL_INFORMATION
    {
        ULONG VolumeLabelLength;
        WCHAR VolumeLabel[1];
    } FILE_FS_LABEL_INFORMATION, * PFILE_FS_LABEL_INFORMATION;

    typedef struct _FILE_FS_SIZE_INFORMATION
    {
        LARGE_INTEGER TotalAllocationUnits;
        LARGE_INTEGER AvailableAllocationUnits;
        ULONG SectorsPerAllocationUnit;
        ULONG BytesPerSector;
    } FILE_FS_SIZE_INFORMATION, * PFILE_FS_SIZE_INFORMATION;

    typedef struct _FILE_FS_DEVICE_INFORMATION
    {
        DEVICE_TYPE DeviceType;
        ULONG Characteristics;
    } FILE_FS_DEVICE_INFORMATION, * PFILE_FS_DEVICE_INFORMATION;

    typedef struct _FILE_FS_ATTRIBUTE_INFORMATION
    {
        ULONG FileSystemAttributes;
        LONG MaximumComponentNameLength;
        ULONG FileSystemNameLength;
        WCHAR FileSystemName[1];
    } FILE_FS_ATTRIBUTE_INFORMATION, * PFILE_FS_ATTRIBUTE_INFORMATION;

    typedef struct _FILE_FS_CONTROL_INFORMATION
    {
        LARGE_INTEGER FreeSpaceStartFiltering;
        LARGE_INTEGER FreeSpaceThreshold;
        LARGE_INTEGER FreeSpaceStopFiltering;
        LARGE_INTEGER DefaultQuotaThreshold;
        LARGE_INTEGER DefaultQuotaLimit;
        ULONG FileSystemControlFlags;
    } FILE_FS_CONTROL_INFORMATION, * PFILE_FS_CONTROL_INFORMATION;

    typedef struct _FILE_FS_FULL_SIZE_INFORMATION
    {
        LARGE_INTEGER TotalAllocationUnits;
        LARGE_INTEGER CallerAvailableAllocationUnits;
        LARGE_INTEGER ActualAvailableAllocationUnits;
        ULONG SectorsPerAllocationUnit;
        ULONG BytesPerSector;
    } FILE_FS_FULL_SIZE_INFORMATION, * PFILE_FS_FULL_SIZE_INFORMATION;

    typedef struct _FILE_FS_OBJECTID_INFORMATION
    {
        UCHAR ObjectId[16];
        UCHAR ExtendedInfo[48];
    } FILE_FS_OBJECTID_INFORMATION, * PFILE_FS_OBJECTID_INFORMATION;

    typedef struct _FILE_FS_DRIVER_PATH_INFORMATION
    {
        BOOLEAN DriverInPath;
        ULONG   DriverNameLength;
        WCHAR   DriverName[1];
    } FILE_FS_DRIVER_PATH_INFORMATION, * PFILE_FS_DRIVER_PATH_INFORMATION;

    typedef struct _FILE_FS_VOLUME_FLAGS_INFORMATION
    {
        ULONG Flags;
    } FILE_FS_VOLUME_FLAGS_INFORMATION, * PFILE_FS_VOLUME_FLAGS_INFORMATION;

    typedef struct _SECTION_IMAGE_INFORMATION
    {
        PVOID TransferAddress;
        ULONG ZeroBits;
        ULONG_PTR MaximumStackSize;
        ULONG_PTR CommittedStackSize;
        ULONG SubSystemType;
        union _SECTION_IMAGE_INFORMATION_u0
        {
            struct _SECTION_IMAGE_INFORMATION_s0
            {
                USHORT SubSystemMinorVersion;
                USHORT SubSystemMajorVersion;
            };
            ULONG SubSystemVersion;
        };
        ULONG GpValue;
        USHORT ImageCharacteristics;
        USHORT DllCharacteristics;
        USHORT Machine;
        BOOLEAN ImageContainsCode;
        BOOLEAN Spare1;
        ULONG LoaderFlags;
        ULONG Reserved[2];
    } SECTION_IMAGE_INFORMATION, * PSECTION_IMAGE_INFORMATION;

    typedef struct _RTL_USER_PROCESS_INFORMATION
    {
        ULONG Length;
        HANDLE ProcessHandle;
        HANDLE ThreadHandle;
        CLIENT_ID ClientId;
        SECTION_IMAGE_INFORMATION ImageInformation;
    } RTL_USER_PROCESS_INFORMATION, * PRTL_USER_PROCESS_INFORMATION;

    typedef struct _LDR_DATA_TABLE_ENTRY
    {
        LIST_ENTRY InLoadOrderLinks;
        LIST_ENTRY InMemoryOrderLinks;
        LIST_ENTRY InInitializationOrderLinks;
        PVOID DllBase;
        PVOID EntryPoint;
        ULONG SizeOfImage;
        UNICODE_STRING FullDllName;
        UNICODE_STRING BaseDllName;
        ULONG  Flags;
        USHORT LoadCount;
        USHORT TlsIndex;
        LIST_ENTRY HashLinks;
        PVOID SectionPointer;
        ULONG CheckSum;
        ULONG TimeDateStamp;
        PVOID LoadedImports;
        PVOID EntryPointActivationContext;
        PVOID PatchInformation;
        PVOID Unknown1;
        PVOID Unknown2;
        PVOID Unknown3;
    } LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

    typedef struct _PORT_MESSAGE
    {
        union
        {
            struct
            {
                USHORT DataLength;
                USHORT TotalLength;
            } s1;
            ULONG Length;
        } u1;
        union
        {
            struct
            {
                USHORT Type;
                USHORT DataInfoOffset;
            } s2;
            ULONG ZeroInit;
        } u2;
        union
        {
            CLIENT_ID ClientId;
            double   DoNotUseThisField;
        };
        ULONG  MessageId;
        union
        {
            ULONG_PTR ClientViewSize;
            ULONG  CallbackId;
        };
    } PORT_MESSAGE, * PPORT_MESSAGE;

    typedef struct _PORT_VIEW
    {
        ULONG  Length;
        HANDLE SectionHandle;
        ULONG  SectionOffset;
        SIZE_T ViewSize;
        PVOID  ViewBase;
        PVOID  ViewRemoteBase;
    } PORT_VIEW, * PPORT_VIEW;

    typedef struct _REMOTE_PORT_VIEW
    {
        ULONG  Length;
        SIZE_T ViewSize;
        PVOID  ViewBase;
    } REMOTE_PORT_VIEW, * PREMOTE_PORT_VIEW;

    typedef struct RTL_HEAP_PARAMETERS
    {
        ULONG Length;
        ULONG SegmentReserve;
        ULONG SegmentCommit;
        ULONG DeCommitFreeBlockThreshold;
        ULONG DeCommitTotalFreeThreshold;
        ULONG MaximumAllocationSize;
        ULONG VirtualMemoryThreshold;
        ULONG InitialCommit;
        ULONG InitialReserve;
        PVOID CommitRoutine;
        ULONG Reserved;
    } RTL_HEAP_PARAMETERS, * PRTL_HEAP_PARAMETERS;

    typedef struct _EVENT_BASIC_INFORMATION
    {
        EVENT_TYPE EventType;
        LONG EventState;
    } EVENT_BASIC_INFORMATION, * PEVENT_BASIC_INFORMATION;

#pragma endregion

#pragma region TYPEDEF API

    typedef VOID(NTAPI* PKNORMAL_ROUTINE)(
        IN    PVOID  NormalContext,
        IN    PVOID  SystemArgument1,
        IN    PVOID  SystemArgument2
        );

    typedef VOID(NTAPI* PIO_APC_ROUTINE)(
        IN    PVOID ApcContext,
        IN    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG Reserved
        );

    typedef VOID(NTAPI* PIO_APC_ROUTINE)(
        IN    PVOID ApcContext,
        IN    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG Reserved
        );

    typedef VOID(NTAPI* PUSER_THREAD_START_ROUTINE)(
        IN    PVOID ApcArgument1
        );

#pragma endregion

#pragma region DEFINE API

#ifndef WIN64
#define NtCurrentProcess() ((HANDLE)0xFFFFFFFF)
#define NtCurrentThread() ((HANDLE)0xFFFFFFFE)
#else // WIN64
#define NtCurrentProcess() ((HANDLE)0xFFFFFFFFFFFFFFFF)
#define NtCurrentThread() ((HANDLE)0xFFFFFFFFFFFFFFFE)
#endif // WIN64

#define NtCurrentPeb()     (PPEB)(NtCurrentTeb()->ProcessEnvironmentBlock)

#define RtlProcessHeap() (HANDLE)(NtCurrentTeb()->ProcessEnvironmentBlock->ProcessHeap)

#define DECLARE_INTERNAL_OBJECT(x) struct _##x; typedef struct _##x *P##x;

#define DECLARE_INTERNAL_OBJECT2(x,y) struct _##x; typedef struct _##x *P##y;

#define InitializeObjectAttributes(p, n, a, r, s)    \
 {                                                    \
     (p)->Length = sizeof( OBJECT_ATTRIBUTES );        \
     (p)->RootDirectory = r;                            \
     (p)->Attributes = a;                            \
     (p)->ObjectName = n;                            \
     (p)->SecurityDescriptor = s;                    \
     (p)->SecurityQualityOfService = NULL;            \
 }

#define InitializeMessageHeader(ph, l, t)                            \
 {                                                                    \
     (ph)->u1.s1.TotalLength = (USHORT)(l);                            \
     (ph)->u1.s1.DataLength = (USHORT)(l - sizeof(PORT_MESSAGE));    \
     (ph)->u2.s2.Type = (USHORT)(t);                                    \
     (ph)->u2.s2.DataInfoOffset = 0;                                    \
     (ph)->ClientId.UniqueProcess = NULL;                            \
     (ph)->ClientId.UniqueThread = NULL;                                \
     (ph)->MessageId = 0;                                            \
     (ph)->ClientViewSize = 0;                                        \
 }

#define RtlInitEmptyUnicodeString(ucStr, buf, bufSize)    \
 {                                                        \
     (ucStr)->Buffer = (buf);                            \
     (ucStr)->Length = 0;                                \
     (ucStr)->MaximumLength = (USHORT)(bufSize);            \
 }

#define ABSOLUTE_INTERVAL(wait) (wait)

#define RELATIVE_INTERVAL(wait) (-(wait))

#define NANOSECONDS(nanos) (((signed __int64)(nanos)) / 100L)

#define MICROSECONDS(micros) (((signed __int64)(micros)) * NANOSECONDS(1000L))

#define MILISECONDS(mili) (((signed __int64)(mili)) * MICROSECONDS(1000L))

#define SECONDS(seconds) (((signed __int64)(seconds)) * MILISECONDS(1000L))

#pragma endregion

#pragma region REAL API

    BOOLEAN FORCEINLINE IsListEmpty(IN    const LIST_ENTRY* ListHead)
    {
        return (BOOLEAN)(ListHead->Flink == ListHead);
    }

    FORCEINLINE VOID InitializeListHead(IN    PLIST_ENTRY ListHead)
    {
        ListHead->Flink = ListHead->Blink = ListHead;
    }

    FORCEINLINE VOID InsertHeadList(IN OUT    PLIST_ENTRY ListHead, IN OUT    PLIST_ENTRY Entry)
    {
        PLIST_ENTRY Flink;
        Flink = ListHead->Flink;
        Entry->Flink = Flink;
        Entry->Blink = ListHead;
        Flink->Blink = Entry;
        ListHead->Flink = Entry;
    }

    FORCEINLINE VOID InsertTailList(IN OUT    PLIST_ENTRY ListHead, IN OUT    PLIST_ENTRY Entry)
    {
        PLIST_ENTRY Blink;
        Blink = ListHead->Blink;
        Entry->Flink = ListHead;
        Entry->Blink = Blink;
        Blink->Flink = Entry;
        ListHead->Blink = Entry;
    }

    FORCEINLINE BOOLEAN RemoveEntryList(IN    PLIST_ENTRY Entry)
    {
        PLIST_ENTRY Blink;
        PLIST_ENTRY Flink;
        Flink = Entry->Flink;
        Blink = Entry->Blink;
        Blink->Flink = Flink;
        Flink->Blink = Blink;
        return (BOOLEAN)(Flink == Blink);
    }

#pragma endregion

#pragma region NATIVE API

    NTSYSAPI NTSTATUS NTAPI NtAcceptConnectPort(
        OUT    PHANDLE PortHandle,
        IN    PVOID PortContext OPTIONAL,
        IN    PPORT_MESSAGE ConnectionRequest,
        IN    BOOLEAN AcceptConnection,
        IN OUT    PPORT_VIEW ServerView OPTIONAL,
        OUT    PREMOTE_PORT_VIEW ClientView OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheck(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    HANDLE TokenHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    PGENERIC_MAPPING GenericMapping,
        OUT    PPRIVILEGE_SET PrivilegeSet,
        IN    PULONG PrivilegeSetLength,
        OUT    PACCESS_MASK GrantedAccess,
        OUT    PBOOLEAN AccessStatus
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheckAndAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    PUNICODE_STRING ObjectTypeName,
        IN    PUNICODE_STRING ObjectName,
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    ACCESS_MASK DesiredAccess,
        IN    PGENERIC_MAPPING GenericMapping,
        IN    BOOLEAN ObjectCreation,
        OUT    PACCESS_MASK GrantedAccess,
        OUT    PBOOLEAN AccessStatus,
        OUT    PBOOLEAN GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheckByType(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    PSID PrincipalSelfSid,
        IN    HANDLE TokenHandle,
        IN    ULONG DesiredAccess,
        IN    POBJECT_TYPE_LIST ObjectTypeList,
        IN    ULONG ObjectTypeListLength,
        IN    PGENERIC_MAPPING GenericMapping,
        IN    PPRIVILEGE_SET PrivilegeSet,
        IN    PULONG PrivilegeSetLength,
        OUT    PACCESS_MASK GrantedAccess,
        OUT    PULONG AccessStatus
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheckByTypeAndAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    PUNICODE_STRING ObjectTypeName,
        IN    PUNICODE_STRING ObjectName,
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    PSID PrincipalSelfSid,
        IN    ACCESS_MASK DesiredAccess,
        IN    AUDIT_EVENT_TYPE AuditType,
        IN    ULONG Flags,
        IN    POBJECT_TYPE_LIST ObjectTypeList,
        IN    ULONG ObjectTypeListLength,
        IN    PGENERIC_MAPPING GenericMapping,
        IN    BOOLEAN ObjectCreation,
        OUT    PACCESS_MASK GrantedAccess,
        OUT    PULONG AccessStatus,
        OUT    PBOOLEAN GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheckByTypeResultList(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    PSID PrincipalSelfSid,
        IN    HANDLE TokenHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_TYPE_LIST ObjectTypeList,
        IN    ULONG ObjectTypeListLength,
        IN    PGENERIC_MAPPING GenericMapping,
        IN    PPRIVILEGE_SET PrivilegeSet,
        IN    PULONG PrivilegeSetLength,
        OUT    PACCESS_MASK GrantedAccessList,
        OUT    PULONG AccessStatusList
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheckByTypeResultListAndAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    PUNICODE_STRING ObjectTypeName,
        IN    PUNICODE_STRING ObjectName,
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    PSID PrincipalSelfSid,
        IN    ACCESS_MASK DesiredAccess,
        IN    AUDIT_EVENT_TYPE AuditType,
        IN    ULONG Flags,
        IN    POBJECT_TYPE_LIST ObjectTypeList,
        IN    ULONG ObjectTypeListLength,
        IN    PGENERIC_MAPPING GenericMapping,
        IN    BOOLEAN ObjectCreation,
        OUT    PACCESS_MASK GrantedAccessList,
        OUT    PULONG AccessStatusList,
        OUT    PULONG GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtAccessCheckByTypeResultListAndAuditAlarmByHandle(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    HANDLE TokenHandle,
        IN    PUNICODE_STRING ObjectTypeName,
        IN    PUNICODE_STRING ObjectName,
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    PSID PrincipalSelfSid,
        IN    ACCESS_MASK DesiredAccess,
        IN    AUDIT_EVENT_TYPE AuditType,
        IN    ULONG Flags,
        IN    POBJECT_TYPE_LIST ObjectTypeList,
        IN    ULONG ObjectTypeListLength,
        IN    PGENERIC_MAPPING GenericMapping,
        IN    BOOLEAN ObjectCreation,
        OUT    PACCESS_MASK GrantedAccessList,
        OUT    PULONG AccessStatusList,
        OUT    PULONG GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtAddAtom(
        IN    PWSTR String,
        IN    ULONG StringLength,
        OUT    PUSHORT Atom
    );

    NTSYSAPI NTSTATUS NTAPI NtAddBootEntry(
        IN    PUNICODE_STRING EntryName,
        IN    PUNICODE_STRING EntryValue
    );

    NTSYSAPI NTSTATUS NTAPI NtAddDriverEntry(
        IN    PUNICODE_STRING DriverName,
        IN    PUNICODE_STRING DriverPath
    );

    NTSYSAPI NTSTATUS NTAPI NtAdjustGroupsToken(
        IN    HANDLE TokenHandle,
        IN    BOOLEAN ResetToDefault,
        IN    PTOKEN_GROUPS NewState,
        IN    ULONG BufferLength,
        OUT    PTOKEN_GROUPS PreviousState OPTIONAL,
        OUT    PULONG ReturnLength
    );

    NTSYSAPI NTSTATUS NTAPI NtAdjustPrivilegesToken(
        IN    HANDLE TokenHandle,
        IN    BOOLEAN DisableAllPrivileges,
        IN    PTOKEN_PRIVILEGES NewState OPTIONAL,
        IN    ULONG BufferLength OPTIONAL,
        IN    PTOKEN_PRIVILEGES PreviousState OPTIONAL,
        OUT    PULONG ReturnLength
    );

    NTSYSAPI NTSTATUS NTAPI NtAlertResumeThread(
        IN    HANDLE ThreadHandle,
        OUT    PULONG PreviousSuspendCount OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtAllocateLocallyUniqueId(
        OUT    PLUID Luid
    );

    NTSYSAPI NTSTATUS NTAPI NtAllocateUserPhysicalPages(
        IN    HANDLE ProcessHandle,
        IN    PULONG NumberOfPages,
        OUT    PULONG PageFrameNumbers
    );

    NTSYSAPI NTSTATUS NTAPI NtAllocateUuids(
        OUT    PLARGE_INTEGER UuidLastTimeAllocated,
        OUT    PULONG UuidDeltaTime,
        OUT    PULONG UuidSequenceNumber,
        OUT    PUCHAR UuidSeed
    );
    NTSYSAPI NTSTATUS NTAPI NtAllocateVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN    ULONG ZeroBits,
        IN OUT    PULONG AllocationSize,
        IN    ULONG AllocationType,
        IN    ULONG Protect
    );

    NTSYSAPI NTSTATUS NTAPI NtAreMappedFilesTheSame(
        IN    PVOID Address1,
        IN    PVOID Address2
    );

    NTSYSAPI NTSTATUS NTAPI NtAssignProcessToJobObject(
        IN    HANDLE JobHandle,
        IN    HANDLE ProcessHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtCallbackReturn(
        IN    PVOID Result OPTIONAL,
        IN    ULONG ResultLength,
        IN    NTSTATUS Status
    );

    NTSYSAPI NTSTATUS NTAPI NtCancelDeviceWakeupRequest(
        IN    HANDLE DeviceHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtCancelIoFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock
    );

    NTSYSAPI NTSTATUS NTAPI NtCancelTimer(
        IN    HANDLE TimerHandle,
        OUT    PBOOLEAN PreviousState OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtClearEvent(
        IN    HANDLE EventHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtClose(
        IN    HANDLE Handle
    );

    NTSYSAPI NTSTATUS NTAPI NtCloseObjectAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    BOOLEAN GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtCompactKeys(
        IN    ULONG Length,
        IN    HANDLE Key
    );

    NTSYSAPI NTSTATUS NTAPI NtCompareTokens(
        IN    HANDLE FirstTokenHandle,
        IN    HANDLE SecondTokenHandle,
        OUT    PBOOLEAN IdenticalTokens
    );

    NTSYSAPI NTSTATUS NTAPI NtCompleteConnectPort(
        IN    HANDLE PortHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtCompressKey(
        IN    HANDLE Key
    );

    NTSYSAPI NTSTATUS NTAPI NtConnectPort(
        OUT    PHANDLE PortHandle,
        IN    PUNICODE_STRING PortName,
        IN    PSECURITY_QUALITY_OF_SERVICE SecurityQos,
        IN OUT    PLPC_SECTION_WRITE WriteSection OPTIONAL,
        IN OUT    PLPC_SECTION_READ ReadSection OPTIONAL,
        OUT    PULONG MaxMessageSize OPTIONAL,
        IN OUT    PVOID ConnectData OPTIONAL,
        IN OUT    PULONG ConnectDataLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtContinue(
        IN    PCONTEXT Context,
        IN    BOOLEAN TestAlert
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateDebugObject(
        OUT    PHANDLE DebugObject,
        IN    ULONG AccessRequired,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    BOOLEAN KillProcessOnExit
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateDirectoryObject(
        OUT    PHANDLE DirectoryHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateEvent(
        OUT    PHANDLE EventHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    EVENT_TYPE EventType,
        IN    BOOLEAN InitialState
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateEventPair(
        OUT    PHANDLE EventPairHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateFile(
        OUT    PHANDLE FileHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PLARGE_INTEGER AllocationSize OPTIONAL,
        IN    ULONG FileAttributes,
        IN    ULONG ShareAccess,
        IN    ULONG CreateDisposition,
        IN    ULONG CreateOptions,
        IN    PVOID EaBuffer OPTIONAL,
        IN    ULONG EaLength
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateIoCompletion(
        OUT    PHANDLE IoCompletionHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    ULONG NumberOfConcurrentThreads
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateJobObject(
        OUT    PHANDLE JobHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateJobSet(
        IN    ULONG Jobs,
        IN    PJOB_SET_ARRAY JobSet,
        IN    ULONG Reserved
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateKey(
        OUT    PHANDLE KeyHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    ULONG TitleIndex,
        IN    PUNICODE_STRING Class OPTIONAL,
        IN    ULONG CreateOptions,
        OUT    PULONG Disposition OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateKeyedEvent(
        OUT    PHANDLE KeyedEventHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    ULONG Reserved
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateMailslotFile(
        OUT    PHANDLE FileHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG CreateOptions,
        IN    ULONG InBufferSize,
        IN    ULONG MaxMessageSize,
        IN    PLARGE_INTEGER ReadTimeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateMutant(
        OUT    PHANDLE MutantHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
        IN    BOOLEAN InitialOwner
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateNamedPipeFile(
        OUT    PHANDLE FileHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG ShareAccess,
        IN    ULONG CreateDisposition,
        IN    ULONG CreateOptions,
        IN    BOOLEAN TypeMessage,
        IN    BOOLEAN ReadmodeMessage,
        IN    BOOLEAN Nonblocking,
        IN    ULONG MaxInstances,
        IN    ULONG InBufferSize,
        IN    ULONG OutBufferSize,
        IN    PLARGE_INTEGER DefaultTimeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtCreatePort(
        OUT    PHANDLE PortHandle,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    ULONG MaxConnectionInfoLength,
        IN    ULONG MaxMessageLength,
        IN    ULONG MaxPoolUsage
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateProcess(
        OUT    PHANDLE ProcessHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
        IN    HANDLE ParentProcess,
        IN    BOOLEAN InheritObjectTable,
        IN    HANDLE SectionHandle OPTIONAL,
        IN    HANDLE DebugPort OPTIONAL,
        IN    HANDLE ExceptionPort OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateProcessEx(
        OUT    PHANDLE ProcessHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    HANDLE InheritFromProcessHandle,
        IN    ULONG CreateFlags,
        IN    HANDLE SectionHandle OPTIONAL,
        IN    HANDLE DebugObject OPTIONAL,
        IN    HANDLE ExceptionPort OPTIONAL,
        IN    ULONG JobMemberLevel
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateProfile(
        OUT    PHANDLE ProfileHandle,
        IN    HANDLE ProcessHandle,
        IN    PVOID Base,
        IN    ULONG Size,
        IN    ULONG BucketShift,
        IN    PULONG Buffer,
        IN    ULONG BufferLength,
        IN    KPROFILE_SOURCE Source,
        IN    ULONG ProcessorMask
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateSection(
        OUT    PHANDLE SectionHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    PLARGE_INTEGER SectionSize OPTIONAL,
        IN    ULONG Protect,
        IN    ULONG Attributes,
        IN    HANDLE FileHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateSemaphore(
        OUT    PHANDLE SemaphoreHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
        IN    ULONG InitialCount,
        IN    ULONG MaximumCount
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateSymbolicLinkObject(
        OUT    PHANDLE SymbolicLinkHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    PUNICODE_STRING TargetName
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateThread(
        OUT    PHANDLE ThreadHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    HANDLE ProcessHandle,
        OUT    PCLIENT_ID ClientId,
        IN    PCONTEXT ThreadContext,
        IN    PUSER_STACK UserStack,
        IN    BOOLEAN CreateSuspended
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateToken(
        OUT    PHANDLE TokenHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    TOKEN_TYPE Type,
        IN    PLUID AuthenticationId,
        IN    PLARGE_INTEGER ExpirationTime,
        IN    PTOKEN_USER User,
        IN    PTOKEN_GROUPS Groups,
        IN    PTOKEN_PRIVILEGES Privileges,
        IN    PTOKEN_OWNER Owner,
        IN    PTOKEN_PRIMARY_GROUP PrimaryGroup,
        IN    PTOKEN_DEFAULT_DACL DefaultDacl,
        IN    PTOKEN_SOURCE Source
    );

    NTSYSAPI NTSTATUS NTAPI NtCreateWaitablePort(
        OUT    PHANDLE PortHandle,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    ULONG MaxConnectionInfoLength,
        IN    ULONG MaxMessageLength,
        IN    ULONG MaxPoolUsage
    );

    NTSYSAPI NTSTATUS NTAPI NtDebugActiveProcess(
        IN    HANDLE Process,
        IN    HANDLE DebugObject
    );

    NTSYSAPI NTSTATUS NTAPI NtDebugContinue(
        IN    HANDLE DebugObject,
        IN    PCLIENT_ID AppClientId,
        IN    NTSTATUS ContinueStatus
    );

    NTSYSAPI NTSTATUS NTAPI NtDelayExecution(
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER DelayInterval
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteAtom(
        IN    USHORT Atom
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteBootEntry(
        IN    PUNICODE_STRING EntryName,
        IN    PUNICODE_STRING EntryValue
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteDriverEntry(
        IN    PUNICODE_STRING DriverName,
        IN    PUNICODE_STRING DriverPath
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteFile(
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteKey(
        IN    HANDLE KeyHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteObjectAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    BOOLEAN GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtDeleteValueKey(
        IN    HANDLE KeyHandle,
        IN    PUNICODE_STRING ValueName
    );

    NTSYSAPI NTSTATUS NTAPI NtDeviceIoControlFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG IoControlCode,
        IN    PVOID InputBuffer OPTIONAL,
        IN    ULONG InputBufferLength,
        OUT    PVOID OutputBuffer OPTIONAL,
        IN    ULONG OutputBufferLength
    );

    NTSYSAPI NTSTATUS NTAPI NtDisplayString(
        IN    PUNICODE_STRING String
    );

    NTSYSAPI NTSTATUS NTAPI NtDuplicateObject(
        IN    HANDLE SourceProcessHandle,
        IN    HANDLE SourceHandle,
        IN    HANDLE TargetProcessHandle OPTIONAL,
        OUT    PHANDLE TargetHandle OPTIONAL,
        IN    ACCESS_MASK DesiredAccess,
        IN    ULONG HandleAttributes,
        IN    ULONG Options
    );

    NTSYSAPI NTSTATUS NTAPI NtDuplicateToken(
        IN    HANDLE ExistingTokenHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    BOOLEAN EffectiveOnly,
        IN    TOKEN_TYPE TokenType,
        OUT    PHANDLE NewTokenHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtEnumerateBootEntries(
        IN    ULONG Unknown1,
        IN    ULONG Unknown2
    );
    NTSYSAPI NTSTATUS NTAPI NtEnumerateKey(
        IN    HANDLE KeyHandle,
        IN    ULONG Index,
        IN    KEY_INFORMATION_CLASS KeyInformationClass,
        OUT    PVOID KeyInformation,
        IN    ULONG KeyInformationLength,
        OUT    PULONG ResultLength
    );

    NTSYSAPI NTSTATUS NTAPI NtEnumerateSystemEnvironmentValuesEx(
        IN    ULONG Unknown1,
        IN    ULONG Unknown2,
        IN    ULONG Unknown3
    );

    NTSYSAPI NTSTATUS NTAPI NtEnumerateValueKey(
        IN    HANDLE KeyHandle,
        IN    ULONG Index,
        IN    KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        OUT    PVOID KeyValueInformation,
        IN    ULONG KeyValueInformationLength,
        OUT    PULONG ResultLength
    );

    NTSYSAPI NTSTATUS NTAPI NtExtendSection(
        IN    HANDLE SectionHandle,
        IN    PLARGE_INTEGER SectionSize
    );

    NTSYSAPI NTSTATUS NTAPI NtFilterToken(
        IN    HANDLE ExistingTokenHandle,
        IN    ULONG Flags,
        IN    PTOKEN_GROUPS SidsToDisable,
        IN    PTOKEN_PRIVILEGES PrivilegesToDelete,
        IN    PTOKEN_GROUPS SidsToRestricted,
        OUT    PHANDLE NewTokenHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtFindAtom(
        IN    PWSTR String,
        IN    ULONG StringLength,
        OUT    PUSHORT Atom
    );

    NTSYSAPI NTSTATUS NTAPI NtFlushBuffersFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock
    );

    NTSYSAPI NTSTATUS NTAPI NtFlushInstructionCache(
        IN    HANDLE ProcessHandle,
        IN    PVOID BaseAddress OPTIONAL,
        IN    ULONG FlushSize
    );

    NTSYSAPI NTSTATUS NTAPI NtFlushKey(
        IN    HANDLE KeyHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtFlushVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN OUT    PULONG FlushSize,
        OUT    PIO_STATUS_BLOCK IoStatusBlock
    );

    NTSYSAPI NTSTATUS NTAPI NtFlushWriteBuffer(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtYieldExecution(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtWriteVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN    PVOID BaseAddress,
        IN    PVOID Buffer,
        IN    ULONG BufferLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWriteRequestData(
        IN    HANDLE PortHandle,
        IN    PPORT_MESSAGE Message,
        IN    ULONG Index,
        IN    PVOID Buffer,
        IN    ULONG BufferLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWriteFileGather(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PFILE_SEGMENT_ELEMENT Buffer,
        IN    ULONG Length,
        IN    PLARGE_INTEGER ByteOffset OPTIONAL,
        IN    PULONG Key OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWriteFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PVOID Buffer,
        IN    ULONG Length,
        IN    PLARGE_INTEGER ByteOffset OPTIONAL,
        IN    PULONG Key OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWaitLowEventPair(
        IN    HANDLE EventPairHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtWaitHighEventPair(
        IN    HANDLE EventPairHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtWaitForSingleObject(
        IN    HANDLE Handle,
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWaitForMultipleObjects32(
        IN    ULONG HandleCount,
        IN    PHANDLE Handles,
        IN    WAIT_TYPE WaitType,
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWaitForMultipleObjects(
        IN    ULONG HandleCount,
        IN    PHANDLE Handles,
        IN    WAIT_TYPE WaitType,
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtWaitForKeyedEvent(
        IN    HANDLE KeyedEventHandle,
        IN    PVOID Key,
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtUnmapViewOfSection(
        IN    HANDLE ProcessHandle,
        IN    PVOID BaseAddress
    );

    NTSYSAPI NTSTATUS NTAPI NtUnlockVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN OUT    PULONG LockSize,
        IN    ULONG LockType
    );

    NTSYSAPI NTSTATUS NTAPI NtUnlockFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PULARGE_INTEGER LockOffset,
        IN    PULARGE_INTEGER LockLength,
        IN    ULONG Key
    );

    NTSYSAPI NTSTATUS NTAPI NtUnloadKeyEx(
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes,
        IN    HANDLE EventHandle OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtUnloadKey2(
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes,
        IN    BOOLEAN ForceUnload
    );

    NTSYSAPI NTSTATUS NTAPI NtUnloadKey(
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtUnloadDriver(
        IN    PUNICODE_STRING DriverServiceName
    );

    NTSYSAPI NTSTATUS NTAPI NtTerminateThread(
        IN    HANDLE ThreadHandle OPTIONAL,
        IN    NTSTATUS ExitStatus
    );

    NTSYSAPI NTSTATUS NTAPI NtTerminateProcess(
        IN    HANDLE ProcessHandle OPTIONAL,
        IN    NTSTATUS ExitStatus
    );

    NTSYSAPI NTSTATUS NTAPI NtTerminateJobObject(
        IN    HANDLE JobHandle,
        IN    NTSTATUS ExitStatus
    );

    NTSYSAPI NTSTATUS NTAPI NtSystemDebugControl(
        IN    DEBUG_CONTROL_CODE ControlCode,
        IN    PVOID InputBuffer OPTIONAL,
        IN    ULONG InputBufferLength,
        OUT    PVOID OutputBuffer OPTIONAL,
        IN    ULONG OutputBufferLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtSuspendThread(
        IN    HANDLE ThreadHandle,
        OUT    PULONG PreviousSuspendCount OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtSuspendProcess(
        IN    HANDLE Process
    );

    NTSYSAPI NTSTATUS NTAPI NtStopProfile(
        IN    HANDLE ProfileHandle
    );
    NTSYSAPI NTSTATUS NTAPI NtStartProfile(
        IN    HANDLE ProfileHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSignalAndWaitForSingleObject(
        IN    HANDLE HandleToSignal,
        IN    HANDLE HandleToWait,
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtShutdownSystem(
        IN    SHUTDOWN_ACTION Action
    );

    NTSYSAPI NTSTATUS NTAPI NtSetVolumeInformationFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PVOID Buffer,
        IN    ULONG BufferLength,
        IN    FS_INFORMATION_CLASS VolumeInformationClass
    );

    NTSYSAPI NTSTATUS NTAPI NtSetValueKey(
        IN    HANDLE KeyHandle,
        IN    PUNICODE_STRING ValueName,
        IN    ULONG TitleIndex OPTIONAL,
        IN    ULONG Type,
        IN    PVOID Data,
        IN    ULONG DataSize
    );

    NTSYSAPI NTSTATUS NTAPI NtSetUuidSeed(
        IN    PUCHAR UuidSeed
    );

    NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(
        IN    ULONG RequestedResolution,
        IN    BOOLEAN Set,
        OUT    PULONG ActualResolution
    );

    NTSYSAPI NTSTATUS NTAPI NtSetThreadExecutionState(
        IN    EXECUTION_STATE ExecutionState,
        OUT    PEXECUTION_STATE PreviousExecutionState
    );

    NTSYSAPI NTSTATUS NTAPI NtSetSystemTime(
        IN    PLARGE_INTEGER NewTime,
        OUT    PLARGE_INTEGER OldTime OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtSetSystemPowerState(
        IN    POWER_ACTION SystemAction,
        IN    SYSTEM_POWER_STATE MinSystemState,
        IN    ULONG Flags
    );

    NTSYSAPI NTSTATUS NTAPI NtSetSystemInformation(
        IN    SYSTEM_INFORMATION_CLASS SystemInformationClass,
        IN OUT    PVOID SystemInformation,
        IN    ULONG SystemInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetSystemEnvironmentValue(
        IN    PUNICODE_STRING Name,
        IN    PUNICODE_STRING Value
    );

    NTSYSAPI NTSTATUS NTAPI NtSetSecurityObject(
        IN    HANDLE Handle,
        IN    SECURITY_INFORMATION SecurityInformation,
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor
    );

    NTSYSAPI NTSTATUS NTAPI NtSetLowWaitHighEventPair(
        IN    HANDLE EventPairHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetLowEventPair(
        IN    HANDLE EventPairHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetLdtEntries(
        IN    ULONG Selector1,
        IN    LDT_ENTRY LdtEntry1,
        IN    ULONG Selector2,
        IN    LDT_ENTRY LdtEntry2
    );

    NTSYSAPI NTSTATUS NTAPI NtSetIoCompletion(
        IN    HANDLE IoCompletionHandle,
        IN    ULONG CompletionKey,
        IN    ULONG CompletionValue,
        IN    NTSTATUS Status,
        IN    ULONG Information
    );

    NTSYSAPI NTSTATUS NTAPI NtSetIntervalProfile(
        IN    ULONG Interval,
        IN    KPROFILE_SOURCE Source
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationToken(
        IN    HANDLE TokenHandle,
        IN    TOKEN_INFORMATION_CLASS TokenInformationClass,
        IN    PVOID TokenInformation,
        IN    ULONG TokenInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationThread(
        IN    HANDLE ThreadHandle,
        IN    THREADINFOCLASS ThreadInformationClass,
        IN    PVOID ThreadInformation,
        IN    ULONG ThreadInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationProcess(
        IN    HANDLE ProcessHandle,
        IN    PROCESSINFOCLASS ProcessInformationClass,
        IN    PVOID ProcessInformation,
        IN    ULONG ProcessInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationObject(
        IN    HANDLE ObjectHandle,
        IN    OBJECT_INFORMATION_CLASS ObjectInformationClass,
        IN    PVOID ObjectInformation,
        IN    ULONG ObjectInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationKey(
        IN    HANDLE KeyHandle,
        IN    KEY_SET_INFORMATION_CLASS KeyInformationClass,
        IN    PVOID KeyInformation,
        IN    ULONG KeyInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationJobObject(
        IN    HANDLE JobHandle,
        IN    JOBOBJECTINFOCLASS JobInformationClass,
        IN    PVOID JobInformation,
        IN    ULONG JobInformationLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetInformationFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PVOID FileInformation,
        IN    ULONG FileInformationLength,
        IN    FILE_INFORMATION_CLASS FileInformationClass
    );

    NTSYSAPI NTSTATUS NTAPI NtSetHighWaitLowEventPair(
        IN    HANDLE EventPairHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetHighEventPair(
        IN    HANDLE EventPairHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetEventBoostPriority(
        IN    HANDLE EventHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetEvent(
        IN    HANDLE EventHandle,
        OUT    PULONG PreviousState OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtSetEaFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PFILE_FULL_EA_INFORMATION Buffer,
        IN    ULONG BufferLength
    );

    NTSYSAPI NTSTATUS NTAPI NtSetDefaultUILanguage(
        IN    LANGID LanguageId
    );

    NTSYSAPI NTSTATUS NTAPI NtSetDefaultLocale(
        IN    BOOLEAN ThreadOrSystem,
        IN    LCID Locale
    );

    NTSYSAPI NTSTATUS NTAPI NtSetDefaultHardErrorPort(
        IN    HANDLE PortHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetDebugFilterState(
        IN    ULONG ComponentId,
        IN    ULONG Level,
        IN    BOOLEAN Enable
    );

    NTSYSAPI NTSTATUS NTAPI NtSetContextThread(
        IN    HANDLE ThreadHandle,
        IN    PCONTEXT Context
    );

    NTSYSAPI NTSTATUS NTAPI NtSetContextChannel(
        IN    HANDLE CHannelHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSetBootEntryOrder(
        IN    ULONG Unknown1,
        IN    ULONG Unknown2
    );

    NTSYSAPI NTSTATUS NTAPI NtSecureConnectPort(
        OUT    PHANDLE PortHandle,
        IN    PUNICODE_STRING PortName,
        IN    PSECURITY_QUALITY_OF_SERVICE SecurityQos,
        IN OUT    PPORT_VIEW ClientView OPTIONAL,
        IN    PSID ServerSid OPTIONAL,
        OUT    PREMOTE_PORT_VIEW ServerView OPTIONAL,
        OUT    PULONG MaxMessageLength OPTIONAL,
        IN OUT    PVOID ConnectInformation OPTIONAL,
        IN OUT    PULONG ConnectInformationLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtSaveMergedKeys(
        IN    HANDLE KeyHandle1,
        IN    HANDLE KeyHandle2,
        IN    HANDLE FileHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtSaveKeyEx(
        IN    HANDLE KeyHandle,
        IN    HANDLE FileHandle,
        IN    ULONG Flags
    );

    NTSYSAPI NTSTATUS NTAPI NtSaveKey(
        IN    HANDLE KeyHandle,
        IN    HANDLE FileHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtResumeThread(
        IN    HANDLE ThreadHandle,
        OUT    PULONG PreviousSuspendCount OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtResumeProcess(
        IN    HANDLE Process
    );

    NTSYSAPI NTSTATUS NTAPI NtRestoreKey(
        IN    HANDLE KeyHandle,
        IN    HANDLE FileHandle,
        IN    ULONG Flags
    );

    NTSYSAPI NTSTATUS NTAPI NtResetWriteWatch(
        IN    HANDLE ProcessHandle,
        IN    PVOID BaseAddress,
        IN    ULONG RegionSize
    );

    NTSYSAPI NTSTATUS NTAPI NtResetEvent(
        IN    HANDLE EventHandle,
        OUT    PULONG PreviousState OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtRequestWakeupLatency(
        IN    LATENCY_TIME Latency
    );

    NTSYSAPI NTSTATUS NTAPI NtRequestWaitReplyPort(
        IN    HANDLE PortHandle,
        IN    PPORT_MESSAGE RequestMessage,
        OUT    PPORT_MESSAGE ReplyMessage
    );

    NTSYSAPI NTSTATUS NTAPI NtRequestPort(
        IN    HANDLE PortHandle,
        IN    PPORT_MESSAGE RequestMessage
    );

    NTSYSAPI NTSTATUS NTAPI NtRequestDeviceWakeup(
        IN    HANDLE DeviceHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtReplyWaitReplyPort(
        IN    HANDLE PortHandle,
        IN OUT    PPORT_MESSAGE ReplyMessage
    );

    NTSYSAPI NTSTATUS NTAPI NtReplyWaitReceivePortEx(
        IN    HANDLE PortHandle,
        OUT    PVOID* PortIdentifier OPTIONAL,
        IN    PPORT_MESSAGE ReplyMessage OPTIONAL,
        OUT    PPORT_MESSAGE Message,
        IN    PLARGE_INTEGER Timeout
    );

    NTSYSAPI NTSTATUS NTAPI NtReplyWaitReceivePort(
        IN    HANDLE PortHandle,
        OUT    PULONG PortIdentifier OPTIONAL,
        IN    PPORT_MESSAGE ReplyMessage OPTIONAL,
        OUT    PPORT_MESSAGE Message
    );

    NTSYSAPI NTSTATUS NTAPI NtReplyPort(
        IN    HANDLE PortHandle,
        IN    PPORT_MESSAGE ReplyMessage
    );

    NTSYSAPI NTSTATUS NTAPI NtReplaceKey(
        IN    POBJECT_ATTRIBUTES NewFileObjectAttributes,
        IN    HANDLE KeyHandle,
        IN    POBJECT_ATTRIBUTES OldFileObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtRenameKey(
        IN    HANDLE KeyHandle,
        IN    PUNICODE_STRING ReplacementName
    );

    NTSYSAPI NTSTATUS NTAPI NtRemoveProcessDebug(
        IN    HANDLE Process,
        IN    HANDLE DebugObject
    );

    NTSYSAPI NTSTATUS NTAPI NtRemoveIoCompletion(
        IN    HANDLE IoCompletionHandle,
        OUT    PULONG CompletionKey,
        OUT    PULONG CompletionValue,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtReleaseSemaphore(
        IN    HANDLE SemaphoreHandle,
        IN    LONG ReleaseCount,
        OUT    PLONG PreviousCount OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtReleaseMutant(
        IN    HANDLE MutantHandle,
        OUT    PULONG PreviousState
    );

    NTSYSAPI NTSTATUS NTAPI NtReleaseKeyedEvent(
        IN    HANDLE KeyedEventHandle,
        IN    PVOID Key,
        IN    BOOLEAN Alertable,
        IN    PLARGE_INTEGER Timeout OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtRegisterThreadTerminatePort(
        IN    HANDLE PortHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtReadVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN    PVOID BaseAddress,
        OUT    PVOID Buffer,
        IN    ULONG BufferLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtReadRequestData(
        IN    HANDLE PortHandle,
        IN    PPORT_MESSAGE Message,
        IN    ULONG Index,
        OUT    PVOID Buffer,
        IN    ULONG BufferLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtReadFileScatter(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PFILE_SEGMENT_ELEMENT Buffer,
        IN    ULONG Length,
        IN    PLARGE_INTEGER ByteOffset OPTIONAL,
        IN    PULONG Key OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtReadFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        OUT    PVOID Buffer,
        IN    ULONG Length,
        IN    PLARGE_INTEGER ByteOffset OPTIONAL,
        IN    PULONG Key OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtRaiseHardError(
        IN    NTSTATUS Status,
        IN    ULONG NumberOfArguments,
        IN    ULONG StringArgumentsMask,
        IN    PULONG_PTR Arguments,
        IN    HARDERROR_RESPONSE_OPTION ResponseOption,
        OUT    PHARDERROR_RESPONSE Response
    );

    NTSYSAPI NTSTATUS NTAPI NtRaiseException(
        IN    PEXCEPTION_RECORD ExceptionRecord,
        IN    PCONTEXT Context,
        IN    BOOLEAN SearchFrames
    );

    NTSYSAPI NTSTATUS NTAPI NtQueueApcThread(
        IN    HANDLE ThreadHandle,
        IN    PKNORMAL_ROUTINE ApcRoutine,
        IN    PVOID ApcContext OPTIONAL,
        IN    PVOID Argument1 OPTIONAL,
        IN    PVOID Argument2 OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryVolumeInformationFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        OUT    PVOID VolumeInformation,
        IN    ULONG VolumeInformationLength,
        IN    FS_INFORMATION_CLASS VolumeInformationClass
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN    PVOID BaseAddress,
        IN    MEMORY_INFORMATION_CLASS MemoryInformationClass,
        OUT    PVOID MemoryInformation,
        IN    ULONG MemoryInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryValueKey(
        IN    HANDLE KeyHandle,
        IN    PUNICODE_STRING ValueName,
        IN    KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        OUT    PVOID KeyValueInformation,
        IN    ULONG KeyValueInformationLength,
        OUT    PULONG ResultLength
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(
        OUT    PULONG CoarsestResolution,
        OUT    PULONG FinestResolution,
        OUT    PULONG ActualResolution
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySystemTime(
        OUT    PLARGE_INTEGER CurrentTime
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySystemInformation(
        IN    SYSTEM_INFORMATION_CLASS SystemInformationClass,
        IN OUT    PVOID SystemInformation,
        IN    ULONG SystemInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySystemEnvironmentValueEx(
        IN    ULONG Unknown1,
        IN    ULONG Unknown2,
        IN    ULONG Unknown3,
        IN    ULONG Unknown4,
        IN    ULONG Unknown5
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySystemEnvironmentValue(
        IN    PUNICODE_STRING Name,
        OUT    PVOID Value,
        IN    ULONG ValueLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySymbolicLinkObject(
        IN    HANDLE SymbolicLinkHandle,
        IN OUT    PUNICODE_STRING TargetName,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySecurityObject(
        IN    HANDLE ObjectHandle,
        IN    SECURITY_INFORMATION SecurityInformation,
        OUT    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    ULONG DescriptorLength,
        OUT    PULONG ReturnLength
    );

    NTSYSAPI NTSTATUS NTAPI NtQuerySection(
        IN    HANDLE SectionHandle,
        IN    SECTION_INFORMATION_CLASS SectionInformationClass,
        OUT    PVOID SectionInformation,
        IN    ULONG SectionInformationLength,
        OUT    PULONG ResultLength OPTIONAL
    );

    NTSYSAPI BOOLEAN NTAPI NtQueryPortInformationProcess(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryPerformanceCounter(
        OUT    PLARGE_INTEGER PerformanceCount,
        OUT    PLARGE_INTEGER PerformanceFrequency OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryOpenSubKeys(
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes,
        OUT    PULONG NumberOfKeys
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryObject(
        IN    HANDLE ObjectHandle,
        IN    OBJECT_INFORMATION_CLASS ObjectInformationClass,
        OUT    PVOID ObjectInformation,
        IN    ULONG ObjectInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryMultipleValueKey(
        IN    HANDLE KeyHandle,
        IN OUT    PKEY_VALUE_ENTRY ValueList,
        IN    ULONG NumberOfValues,
        OUT    PVOID Buffer,
        IN OUT    PULONG Length,
        OUT    PULONG ReturnLength
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryKey(
        IN    HANDLE KeyHandle,
        IN    KEY_INFORMATION_CLASS KeyInformationClass,
        OUT    PVOID KeyInformation,
        IN    ULONG KeyInformationLength,
        OUT    PULONG ResultLength
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryIntervalProfile(
        IN    KPROFILE_SOURCE Source,
        OUT    PULONG Interval
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInstallUILanguage(
        OUT    PLANGID LanguageId
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationToken(
        IN    HANDLE TokenHandle,
        IN    TOKEN_INFORMATION_CLASS TokenInformationClass,
        OUT    PVOID TokenInformation,
        IN    ULONG TokenInformationLength,
        OUT    PULONG ReturnLength
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationThread(
        IN    HANDLE ThreadHandle,
        IN    THREADINFOCLASS ThreadInformationClass,
        OUT    PVOID ThreadInformation,
        IN    ULONG ThreadInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationProcess(
        IN    HANDLE ProcessHandle,
        IN    PROCESSINFOCLASS ProcessInformationClass,
        OUT    PVOID ProcessInformation,
        IN    ULONG ProcessInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationPort(
        IN    HANDLE PortHandle,
        IN    PORT_INFORMATION_CLASS PortInformationClass,
        OUT    PVOID PortInformation,
        IN    ULONG PortInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationJobObject(
        IN    HANDLE JobHandle,
        IN    JOBOBJECTINFOCLASS JobInformationClass,
        OUT    PVOID JobInformation,
        IN    ULONG JobInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        OUT    PVOID FileInformation,
        IN    ULONG FileInformationLength,
        IN    FILE_INFORMATION_CLASS FileInformationClass
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryInformationAtom(
        IN    USHORT Atom,
        IN    ATOM_INFORMATION_CLASS AtomInformationClass,
        OUT    PVOID AtomInformation,
        IN    ULONG AtomInformationLength,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryFullAttributesFile(
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        OUT    PFILE_NETWORK_OPEN_INFORMATION FileInformation
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryEvent(
        IN    HANDLE EventHandle,
        IN    EVENT_INFORMATION_CLASS EventInformationClass,
        OUT    PVOID EventInformation,
        IN    ULONG EventInformationLength,
        OUT    PULONG ResultLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryDirectoryObject(
        IN    HANDLE DirectoryHandle,
        OUT    PVOID Buffer,
        IN    ULONG BufferLength,
        IN    BOOLEAN ReturnSingleEntry,
        IN    BOOLEAN RestartScan,
        IN OUT    PULONG Context,
        OUT    PULONG ReturnLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryDirectoryFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        OUT    PVOID FileInformation,
        IN    ULONG FileInformationLength,
        IN    FILE_INFORMATION_CLASS FileInformationClass,
        IN    BOOLEAN ReturnSingleEntry,
        IN    PUNICODE_STRING FileName OPTIONAL,
        IN    BOOLEAN RestartScan
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryDefaultUILanguage(
        OUT    PLANGID LanguageId
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryDefaultLocale(
        IN    BOOLEAN ThreadOrSystem,
        OUT    PLCID Locale
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryDebugFilterState(
        IN    ULONG ComponentId,
        IN    ULONG Level
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryBootOptions(
        IN    ULONG Unknown1,
        IN    ULONG Unknown2
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryBootEntryOrder(
        IN    ULONG Unknown1,
        IN    ULONG Unknown2
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryAttributesFile(
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        OUT    PFILE_BASIC_INFORMATION FileInformation
    );

    NTSYSAPI NTSTATUS NTAPI NtPulseEvent(
        IN    HANDLE EventHandle,
        OUT    PULONG PreviousState OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtProtectVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN OUT    PULONG ProtectSize,
        IN    ULONG NewProtect,
        OUT    PULONG OldProtect
    );

    NTSYSAPI NTSTATUS NTAPI NtPrivilegedServiceAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PUNICODE_STRING ServiceName,
        IN    HANDLE TokenHandle,
        IN    PPRIVILEGE_SET Privileges,
        IN    BOOLEAN AccessGranted
    );

    NTSYSAPI NTSTATUS NTAPI NtPrivilegeObjectAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID HandleId,
        IN    HANDLE TokenHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    PPRIVILEGE_SET Privileges,
        IN    BOOLEAN AccessGranted
    );

    NTSYSAPI NTSTATUS NTAPI NtPrivilegeCheck(
        IN    HANDLE TokenHandle,
        IN    PPRIVILEGE_SET RequiredPrivileges,
        OUT    PBOOLEAN Result
    );

    NTSYSAPI NTSTATUS NTAPI NtPowerInformation(
        IN    POWER_INFORMATION_LEVEL PowerInformationLevel,
        IN    PVOID InputBuffer OPTIONAL,
        IN    ULONG InputBufferLength,
        OUT    PVOID OutputBuffer OPTIONAL,
        IN    ULONG OutputBufferLength
    );

    NTSYSAPI NTSTATUS NTAPI NtPlugPlayControl(
        IN    ULONG ControlCode,
        IN OUT    PVOID Buffer,
        IN    ULONG BufferLength,
        IN    PVOID Unknown OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenTimer(
        OUT    PHANDLE TimerHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenThreadTokenEx(
        IN    HANDLE ThreadHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    BOOLEAN OpenAsSelf,
        IN    ULONG HandleAttributes,
        OUT    PHANDLE TokenHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenThreadToken(
        IN    HANDLE ThreadHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    BOOLEAN OpenAsSelf,
        OUT    PHANDLE TokenHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenThread(
        OUT    PHANDLE ThreadHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    PCLIENT_ID ClientId
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenSymbolicLinkObject(
        OUT    PHANDLE SymbolicLinkHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenSemaphore(
        OUT    PHANDLE SemaphoreHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenSection(
        OUT    PHANDLE SectionHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenProcessTokenEx(
        IN    HANDLE ProcessHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    ULONG HandleAttributes,
        OUT    PHANDLE TokenHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenProcessToken(
        IN    HANDLE ProcessHandle,
        IN    ACCESS_MASK DesiredAccess,
        OUT    PHANDLE TokenHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenProcess(
        OUT    PHANDLE ProcessHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        IN    PCLIENT_ID ClientId OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenObjectAuditAlarm(
        IN    PUNICODE_STRING SubsystemName,
        IN    PVOID* HandleId,
        IN    PUNICODE_STRING ObjectTypeName,
        IN    PUNICODE_STRING ObjectName,
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    HANDLE TokenHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    ACCESS_MASK GrantedAccess,
        IN    PPRIVILEGE_SET Privileges OPTIONAL,
        IN    BOOLEAN ObjectCreation,
        IN    BOOLEAN AccessGranted,
        OUT    PBOOLEAN GenerateOnClose
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenMutant(
        OUT    PHANDLE MutantHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenKeyedEvent(
        OUT    PHANDLE KeyedEventHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenKey(
        OUT    PHANDLE KeyHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenJobObject(
        OUT    PHANDLE JobHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenIoCompletion(
        OUT    PHANDLE IoCompletionHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenFile(
        OUT    PHANDLE FileHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG ShareAccess,
        IN    ULONG OpenOptions
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenEventPair(
        OUT    PHANDLE EventPairHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenEvent(
        OUT    PHANDLE EventHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtOpenDirectoryObject(
        OUT    PHANDLE DirectoryHandle,
        IN    ACCESS_MASK DesiredAccess,
        IN    POBJECT_ATTRIBUTES ObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtNotifyChangeMultipleKeys(
        IN    HANDLE KeyHandle,
        IN    ULONG Flags,
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes,
        IN    HANDLE EventHandle OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG NotifyFilter,
        IN    BOOLEAN WatchSubtree,
        IN    PVOID Buffer,
        IN    ULONG BufferLength,
        IN    BOOLEAN Asynchronous
    );

    NTSYSAPI NTSTATUS NTAPI NtNotifyChangeKey(
        IN    HANDLE KeyHandle,
        IN    HANDLE EventHandle OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG NotifyFilter,
        IN    BOOLEAN WatchSubtree,
        IN    PVOID Buffer,
        IN    ULONG BufferLength,
        IN    BOOLEAN Asynchronous
    );

    NTSYSAPI NTSTATUS NTAPI NtNotifyChangeDirectoryFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        OUT    PFILE_NOTIFY_INFORMATION Buffer,
        IN    ULONG BufferLength,
        IN    ULONG NotifyFilter,
        IN    BOOLEAN WatchSubtree
    );

    NTSYSAPI NTSTATUS NTAPI NtModifyDriverEntry(
        IN    PUNICODE_STRING DriverName,
        IN    PUNICODE_STRING DriverPath
    );

    NTSYSAPI NTSTATUS NTAPI NtModifyBootEntry(
        IN    PUNICODE_STRING EntryName,
        IN    PUNICODE_STRING EntryValue
    );

    NTSYSAPI NTSTATUS NTAPI NtMapViewOfSection(
        IN    HANDLE SectionHandle,
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN    ULONG ZeroBits,
        IN    ULONG CommitSize,
        IN OUT    PLARGE_INTEGER SectionOffset OPTIONAL,
        IN OUT    PULONG ViewSize,
        IN    SECTION_INHERIT InheritDisposition,
        IN    ULONG AllocationType,
        IN    ULONG Protect
    );

    NTSYSAPI NTSTATUS NTAPI NtMapUserPhysicalPagesScatter(
        IN    PVOID* BaseAddresses,
        IN    PULONG NumberOfPages,
        IN    PULONG PageFrameNumbers
    );

    NTSYSAPI NTSTATUS NTAPI NtMapUserPhysicalPages(
        IN    PVOID BaseAddress,
        IN    PULONG NumberOfPages,
        IN    PULONG PageFrameNumbers
    );

    NTSYSAPI NTSTATUS NTAPI NtMakeTemporaryObject(
        IN    HANDLE ObjectHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtMakePermanentObject(
        IN    HANDLE Object
    );

    NTSYSAPI NTSTATUS NTAPI NtLockVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN OUT    PULONG LockSize,
        IN    ULONG LockType
    );

    NTSYSAPI NTSTATUS NTAPI NtLockRegistryKey(
        IN    HANDLE Key
    );

    NTSYSAPI NTSTATUS NTAPI NtLockFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    PULARGE_INTEGER LockOffset,
        IN    PULARGE_INTEGER LockLength,
        IN    ULONG Key,
        IN    BOOLEAN FailImmediately,
        IN    BOOLEAN ExclusiveLock
    );

    NTSYSAPI NTSTATUS NTAPI NtLoadKey(
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes,
        IN    POBJECT_ATTRIBUTES FileObjectAttributes
    );

    NTSYSAPI NTSTATUS NTAPI NtLoadKey2(
        IN    POBJECT_ATTRIBUTES KeyObjectAttributes,
        IN    POBJECT_ATTRIBUTES FileObjectAttributes,
        IN    ULONG Flags
    );

    NTSYSAPI NTSTATUS NTAPI NtLoadDriver(
        IN    PUNICODE_STRING DriverServiceName
    );

    NTSYSAPI NTSTATUS NTAPI NtListenPort(
        IN    HANDLE PortHandle,
        OUT    PPORT_MESSAGE RequestMessage
    );

    NTSYSAPI NTSTATUS NTAPI NtFreeUserPhysicalPages(
        IN    HANDLE ProcessHandle,
        IN OUT    PULONG NumberOfPages,
        IN    PULONG PageFrameNumbers
    );

    NTSYSAPI NTSTATUS NTAPI NtFreeVirtualMemory(
        IN    HANDLE ProcessHandle,
        IN OUT    PVOID* BaseAddress,
        IN OUT    PULONG FreeSize,
        IN    ULONG FreeType
    );

    NTSYSAPI NTSTATUS NTAPI NtFsControlFile(
        IN    HANDLE FileHandle,
        IN    HANDLE Event OPTIONAL,
        IN    PIO_APC_ROUTINE ApcRoutine OPTIONAL,
        IN    PVOID ApcContext OPTIONAL,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        IN    ULONG FsControlCode,
        IN    PVOID InputBuffer OPTIONAL,
        IN    ULONG InputBufferLength,
        OUT    PVOID OutputBuffer OPTIONAL,
        IN    ULONG OutputBufferLength
    );

    NTSYSAPI NTSTATUS NTAPI NtGetContextThread(
        IN    HANDLE ThreadHandle,
        OUT    PCONTEXT Context
    );

    NTSYSAPI NTSTATUS NTAPI NtGetDevicePowerState(
        IN    HANDLE DeviceHandle,
        OUT    PDEVICE_POWER_STATE DevicePowerState
    );

    NTSYSAPI NTSTATUS NTAPI NtGetPlugPlayEvent(
        IN    ULONG Reserved1,
        IN    ULONG Reserved2,
        OUT    PVOID Buffer,
        IN    ULONG BufferLength
    );

    NTSYSAPI NTSTATUS NTAPI NtGetWriteWatch(
        IN    HANDLE ProcessHandle,
        IN    ULONG Flags,
        IN    PVOID BaseAddress,
        IN    ULONG RegionSize,
        OUT    PULONG Buffer,
        IN OUT    PULONG BufferEntries,
        OUT    PULONG Granularity
    );

    NTSYSAPI NTSTATUS NTAPI NtImpersonateAnonymousToken(
        IN    HANDLE ThreadHandle
    );

    NTSYSAPI NTSTATUS NTAPI NtImpersonateClientOfPort(
        IN    HANDLE PortHandle,
        IN    PPORT_MESSAGE Message
    );

    NTSYSAPI NTSTATUS NTAPI NtImpersonateThread(
        IN    HANDLE ThreadHandle,
        IN    HANDLE TargetThreadHandle,
        IN    PSECURITY_QUALITY_OF_SERVICE SecurityQos
    );

    NTSYSAPI NTSTATUS NTAPI NtInitializeRegistry(
        IN    BOOLEAN Setup
    );

    NTSYSAPI NTSTATUS NTAPI NtInitiatePowerAction(
        IN    POWER_ACTION SystemAction,
        IN    SYSTEM_POWER_STATE MinSystemState,
        IN    ULONG Flags,
        IN    BOOLEAN Asynchronous
    );

    NTSYSAPI NTSTATUS NTAPI NtIsProcessInJob(
        IN    HANDLE ProcessHandle,
        IN    HANDLE JobHandle OPTIONAL
    );

    NTSYSAPI BOOLEAN NTAPI NtIsSystemResumeAutomatic(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtTestAlert(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtAlertThread(
        IN    HANDLE ThreadHandle
    );

    NTSYSAPI ULONG NTAPI NtGetTickCount(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtW32Call(
        IN    ULONG RoutineIndex,
        IN    PVOID Argument,
        IN    ULONG ArgumentLength,
        OUT    PVOID* Result OPTIONAL,
        OUT    PULONG ResultLength OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtSetLowWaitHighThread(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtSetHighWaitLowThread(
        VOID
    );

    NTSYSAPI NTSTATUS NTAPI NtCreatePagingFile(
        IN    PUNICODE_STRING FileName,
        IN    PULARGE_INTEGER InitialSize,
        IN    PULARGE_INTEGER MaximumSize,
        IN    ULONG Priority OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI NtVdmControl(
        IN    ULONG ControlCode,
        IN    PVOID ControlData
    );

    NTSYSAPI NTSTATUS NTAPI NtQueryEaFile(
        IN    HANDLE FileHandle,
        OUT    PIO_STATUS_BLOCK IoStatusBlock,
        OUT    PVOID Buffer,
        IN    ULONG Length,
        IN    BOOLEAN ReturnSingleEntry,
        IN    PVOID EaList OPTIONAL,
        IN    ULONG EaListLength,
        IN    PULONG EaIndex OPTIONAL,
        IN    BOOLEAN RestartScan
    );

    NTSTATUS NTAPI RtlCreateProcessParameters(
        OUT    PRTL_USER_PROCESS_PARAMETERS* ProcessParameters,
        IN    PUNICODE_STRING ImageFile,
        IN    PUNICODE_STRING DllPath OPTIONAL,
        IN    PUNICODE_STRING CurrentDirectory OPTIONAL,
        IN    PUNICODE_STRING CommandLine OPTIONAL,
        IN    PWSTR Environment OPTIONAL,
        IN    PUNICODE_STRING WindowTitle OPTIONAL,
        IN    PUNICODE_STRING DesktopInfo OPTIONAL,
        IN    PUNICODE_STRING ShellInfo OPTIONAL,
        IN    PUNICODE_STRING RuntimeInfo OPTIONAL
    );

    NTSTATUS NTAPI RtlDestroyProcessParameters(
        IN    PRTL_USER_PROCESS_PARAMETERS ProcessParameters
    );

    PDEBUG_BUFFER NTAPI RtlCreateQueryDebugBuffer(
        IN    ULONG Size,
        IN    BOOLEAN EventPair
    );

    NTSTATUS NTAPI RtlQueryProcessDebugInformation(
        IN    ULONG ProcessId,
        IN    ULONG DebugInfoClassMask,
        IN OUT    PDEBUG_BUFFER DebugBuffer
    );

    NTSTATUS NTAPI RtlDestroyQueryDebugBuffer(
        IN    PDEBUG_BUFFER DebugBuffer
    );

    NTSYSAPI VOID NTAPI RtlInitUnicodeString(
        OUT    PUNICODE_STRING DestinationString,
        IN    PCWSTR SourceString
    );

    NTSYSAPI VOID NTAPI RtlInitString(
        PSTRING DestinationString,
        PCSTR SourceString
    );

    NTSYSAPI VOID NTAPI RtlInitAnsiString(
        OUT    PANSI_STRING DestinationString,
        IN    PCSTR SourceString
    );

    NTSYSAPI NTSTATUS NTAPI RtlAnsiStringToUnicodeString(
        OUT    PUNICODE_STRING DestinationString,
        IN    PANSI_STRING SourceString,
        IN    BOOLEAN AllocateDestinationString
    );

    NTSYSAPI NTSTATUS NTAPI RtlUnicodeStringToAnsiString(
        OUT    PANSI_STRING DestinationString,
        IN    PCUNICODE_STRING SourceString,
        IN    BOOLEAN AllocateDestinationString
    );

    NTSYSAPI LONG NTAPI RtlCompareUnicodeString(
        IN    PUNICODE_STRING String1,
        IN    PUNICODE_STRING String2,
        IN    BOOLEAN CaseInSensitive
    );

    NTSYSAPI BOOLEAN NTAPI RtlEqualUnicodeString(
        IN    PCUNICODE_STRING String1,
        IN    PCUNICODE_STRING String2,
        IN    BOOLEAN CaseInSensitive
    );

    NTSYSAPI NTSTATUS NTAPI RtlHashUnicodeString(
        IN    CONST UNICODE_STRING* String,
        IN    BOOLEAN CaseInSensitive,
        IN    ULONG HashAlgorithm,
        OUT    PULONG HashValue
    );

    NTSYSAPI VOID NTAPI RtlCopyUnicodeString(
        OUT    PUNICODE_STRING DestinationString,
        IN    PUNICODE_STRING SourceString
    );

    NTSYSAPI NTSTATUS NTAPI RtlAppendUnicodeStringToString(
        IN OUT    PUNICODE_STRING Destination,
        IN    PUNICODE_STRING Source
    );

    NTSYSAPI NTSTATUS NTAPI RtlAppendUnicodeToString(
        PUNICODE_STRING Destination,
        PCWSTR Source
    );

    NTSYSAPI VOID NTAPI RtlFreeUnicodeString(
        PUNICODE_STRING UnicodeString
    );

    NTSYSAPI VOID NTAPI RtlFreeAnsiString(
        PANSI_STRING AnsiString
    );

    NTSYSAPI ULONG NTAPI RtlxUnicodeStringToAnsiSize(
        PCUNICODE_STRING UnicodeString
    );

    NTSYSAPI DWORD NTAPI RtlNtStatusToDosError(
        IN    NTSTATUS status
    );

    NTSYSAPI NTSTATUS NTAPI RtlAdjustPrivilege(
        ULONG  Privilege,
        BOOLEAN Enable,
        BOOLEAN CurrentThread,
        PBOOLEAN Enabled
    );

    NTSYSAPI BOOLEAN NTAPI RtlCreateUnicodeString(
        OUT    PUNICODE_STRING DestinationString,
        IN    PCWSTR SourceString
    );

    NTSYSAPI BOOLEAN NTAPI RtlCreateUnicodeStringFromAsciiz(
        OUT    PUNICODE_STRING Destination,
        IN    PCSTR Source
    );

    NTSYSAPI BOOLEAN NTAPI RtlPrefixUnicodeString(
        IN    PUNICODE_STRING String1,
        IN    PUNICODE_STRING String2,
        IN    BOOLEAN CaseInSensitive
    );

    NTSYSAPI NTSTATUS NTAPI RtlDuplicateUnicodeString(
        IN    BOOLEAN AllocateNew,
        IN    PUNICODE_STRING SourceString,
        OUT    PUNICODE_STRING TargetString
    );

    NTSYSAPI NTSTATUS NTAPI RtlUnicodeStringToInteger(
        IN    PUNICODE_STRING String,
        IN    ULONG Base OPTIONAL,
        OUT    PULONG Value
    );

    NTSYSAPI NTSTATUS NTAPI RtlIntegerToUnicodeString(
        IN    ULONG Value,
        IN    ULONG Base OPTIONAL,
        IN OUT    PUNICODE_STRING String
    );

    NTSYSAPI NTSTATUS NTAPI RtlGUIDFromString(
        IN    PUNICODE_STRING GuidString,
        OUT    GUID* Guid
    );

    NTSYSAPI NTSTATUS NTAPI RtlUpcaseUnicodeString(
        OUT    PUNICODE_STRING DestinationString,
        IN    PUNICODE_STRING SourceString,
        IN    BOOLEAN AllocateDestinationString
    );

    NTSYSAPI NTSTATUS NTAPI RtlDowncaseUnicodeString(
        OUT    PUNICODE_STRING DestinationString,
        IN    PUNICODE_STRING SourceString,
        IN    BOOLEAN AllocateDestinationString
    );

    NTSYSAPI NTSTATUS NTAPI RtlFormatCurrentUserKeyPath(
        OUT    PUNICODE_STRING CurrentUserKeyPath
    );

    NTSYSAPI VOID NTAPI RtlRaiseStatus(
        IN    NTSTATUS Status
    );

    NTSYSAPI ULONG NTAPI RtlRandom(
        IN OUT    PULONG Seed
    );

    NTSYSAPI NTSTATUS NTAPI RtlInitializeCriticalSection(
        IN    PRTL_CRITICAL_SECTION CriticalSection
    );

    NTSYSAPI BOOL NTAPI RtlTryEnterCriticalSection(
        IN    PRTL_CRITICAL_SECTION CriticalSection
    );

    NTSYSAPI NTSTATUS NTAPI RtlEnterCriticalSection(
        IN    PRTL_CRITICAL_SECTION CriticalSection
    );

    NTSYSAPI NTSTATUS NTAPI RtlLeaveCriticalSection(
        IN    PRTL_CRITICAL_SECTION CriticalSection
    );

    NTSYSAPI NTSTATUS NTAPI RtlDeleteCriticalSection(
        IN    PRTL_CRITICAL_SECTION CriticalSection
    );

    NTSYSAPI NTSTATUS NTAPI RtlCompressBuffer(
        IN    USHORT CompressionFormatAndEngine,
        IN    PUCHAR UncompressedBuffer,
        IN    ULONG UncompressedBufferSize,
        OUT    PUCHAR CompressedBuffer,
        IN    ULONG CompressedBufferSize,
        IN    ULONG UncompressedChunkSize,
        OUT    PULONG FinalCompressedSize,
        IN    PVOID WorkSpace
    );

    NTSYSAPI NTSTATUS NTAPI RtlDecompressBuffer(
        IN    USHORT CompressionFormat,
        OUT    PUCHAR UncompressedBuffer,
        IN    ULONG UncompressedBufferSize,
        IN    PUCHAR CompressedBuffer,
        IN    ULONG CompressedBufferSize,
        OUT    PULONG FinalUncompressedSize
    );

    NTSYSAPI VOID NTAPI RtlInitializeHandleTable(
        IN    ULONG MaximumNumberOfHandles,
        IN    ULONG SizeOfHandleTableEntry,
        OUT    PRTL_HANDLE_TABLE HandleTable
    );

    NTSYSAPI PRTL_HANDLE_TABLE_ENTRY NTAPI RtlAllocateHandle(
        IN    PRTL_HANDLE_TABLE HandleTable,
        OUT    PULONG HandleIndex OPTIONAL
    );

    NTSYSAPI BOOLEAN NTAPI RtlFreeHandle(
        IN    PRTL_HANDLE_TABLE HandleTable,
        IN    PRTL_HANDLE_TABLE_ENTRY Handle
    );

    NTSYSAPI BOOLEAN NTAPI RtlIsValidIndexHandle(
        IN    PRTL_HANDLE_TABLE HandleTable,
        IN    ULONG HandleIndex,
        OUT    PRTL_HANDLE_TABLE_ENTRY* Handle
    );

    NTSYSAPI NTSTATUS NTAPI RtlOpenCurrentUser(
        IN    ULONG DesiredAccess,
        OUT    PHANDLE CurrentUserKey
    );

    NTSYSAPI NTSTATUS NTAPI RtlCreateEnvironment(
        BOOLEAN CloneCurrentEnvironment,
        PVOID* Environment
    );

    NTSYSAPI NTSTATUS NTAPI RtlQueryEnvironmentVariable_U(
        PVOID Environment,
        PUNICODE_STRING Name,
        PUNICODE_STRING Value
    );

    NTSYSAPI NTSTATUS NTAPI RtlSetEnvironmentVariable(
        PVOID* Environment,
        PUNICODE_STRING Name,
        PUNICODE_STRING Value
    );

    NTSYSAPI NTSTATUS NTAPI RtlDestroyEnvironment(
        PVOID Environment
    );

    NTSYSAPI BOOLEAN NTAPI RtlDosPathNameToNtPathName_U(
        IN    PWSTR DosPathName,
        OUT    PUNICODE_STRING NtPathName,
        OUT    PWSTR* NtFileNamePart OPTIONAL,
        OUT    PCURDIR DirectoryInfo OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI RtlCreateUserProcess(
        PUNICODE_STRING NtImagePathName,
        ULONG Attributes,
        PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
        PSECURITY_DESCRIPTOR ProcessSecurityDescriptor,
        PSECURITY_DESCRIPTOR ThreadSecurityDescriptor,
        HANDLE ParentProcess,
        BOOLEAN InheritHandles,
        HANDLE DebugPort,
        HANDLE ExceptionPort,
        PRTL_USER_PROCESS_INFORMATION ProcessInformation
    );

    NTSYSAPI NTSTATUS NTAPI RtlCreateUserThread(
        IN    HANDLE Process,
        IN    PSECURITY_DESCRIPTOR ThreadSecurityDescriptor OPTIONAL,
        IN    BOOLEAN CreateSuspended,
        IN    ULONG_PTR ZeroBits OPTIONAL,
        IN    SIZE_T MaximumStackSize OPTIONAL,
        IN    SIZE_T CommittedStackSize OPTIONAL,
        IN    PUSER_THREAD_START_ROUTINE StartAddress,
        IN    PVOID Parameter OPTIONAL,
        OUT    PHANDLE Thread OPTIONAL,
        OUT    PCLIENT_ID ClientId OPTIONAL
    );

    NTSYSAPI HANDLE NTAPI RtlCreateHeap(
        IN    ULONG Flags,
        IN    PVOID BaseAddress OPTIONAL,
        IN    ULONG SizeToReserve,
        IN    ULONG SizeToCommit,
        IN    BOOLEAN Lock OPTIONAL,
        IN    PRTL_HEAP_PARAMETERS Definition OPTIONAL
    );

    NTSYSAPI ULONG NTAPI RtlDestroyHeap(
        IN    HANDLE HeapHandle
    );

    NTSYSAPI PVOID NTAPI RtlAllocateHeap(
        IN    HANDLE HeapHandle,
        IN    ULONG Flags,
        IN    SIZE_T Size
    );

    NTSYSAPI PVOID NTAPI RtlReAllocateHeap(
        IN    HANDLE HeapHandle,
        IN    ULONG Flags,
        IN    LPVOID Address,
        IN    SIZE_T Size
    );

    NTSYSAPI BOOLEAN NTAPI RtlFreeHeap(
        IN    HANDLE HeapHandle,
        IN    ULONG Flags,
        IN    PVOID Address
    );

    NTSYSAPI ULONG NTAPI RtlCompactHeap(
        IN    HANDLE HeapHandle,
        IN    ULONG Flags
    );

    NTSYSAPI BOOLEAN NTAPI RtlLockHeap(
        IN    HANDLE HeapHandle
    );

    NTSYSAPI BOOLEAN NTAPI RtlUnlockHeap(
        IN    HANDLE HeapHandle
    );

    NTSYSAPI ULONG NTAPI RtlSizeHeap(
        IN    HANDLE HeapHandle,
        IN    ULONG Flags,
        IN    PVOID Address
    );

    NTSYSAPI BOOLEAN NTAPI RtlValidateHeap(
        IN    HANDLE HeapHandle,
        IN    ULONG Flags,
        IN    PVOID Address OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI RtlCreateSecurityDescriptor(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    ULONG Revision
    );

    NTSYSAPI NTSTATUS NTAPI RtlGetDaclSecurityDescriptor(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        OUT    PBOOLEAN DaclPresent,
        OUT    PACL* Dacl,
        OUT    PBOOLEAN DaclDefaulted
    );

    NTSYSAPI NTSTATUS NTAPI RtlSetDaclSecurityDescriptor(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    BOOLEAN DaclPresent,
        IN    PACL Dacl OPTIONAL,
        IN    BOOLEAN DaclDefaulted OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI RtlSetOwnerSecurityDescriptor(
        IN    PSECURITY_DESCRIPTOR SecurityDescriptor,
        IN    PSID Owner OPTIONAL,
        IN    BOOLEAN OwnerDefaulted OPTIONAL
    );

    NTSYSAPI NTSTATUS NTAPI RtlAllocateAndInitializeSid(
        IN    PSID_IDENTIFIER_AUTHORITY IdentifierAuthority,
        IN    UCHAR SubAuthorityCount,
        IN    ULONG SubAuthority0,
        IN    ULONG SubAuthority1,
        IN    ULONG SubAuthority2,
        IN    ULONG SubAuthority3,
        IN    ULONG SubAuthority4,
        IN    ULONG SubAuthority5,
        IN    ULONG SubAuthority6,
        IN    ULONG SubAuthority7,
        OUT    PSID* Sid
    );

    NTSYSAPI ULONG NTAPI RtlLengthSid(
        IN    PSID Sid
    );

    NTSYSAPI BOOLEAN NTAPI RtlEqualSid(
        IN    PSID Sid1,
        IN    PSID Sid2
    );

    NTSYSAPI PVOID NTAPI RtlFreeSid(
        IN    PSID Sid
    );

    NTSYSAPI NTSTATUS NTAPI RtlCreateAcl(
        IN    PACL Acl,
        IN    ULONG AclLength,
        IN    ULONG AclRevision
    );

    NTSYSAPI NTSTATUS NTAPI RtlGetAce(
        IN    PACL Acl,
        IN    ULONG AceIndex,
        OUT    PVOID* Ace
    );

    NTSYSAPI NTSTATUS NTAPI RtlAddAccessAllowedAce(
        IN OUT    PACL Acl,
        IN    ULONG AceRevision,
        IN    ACCESS_MASK AccessMask,
        IN    PSID Sid
    );

    NTSYSAPI NTSTATUS NTAPI RtlAddAccessAllowedAceEx(
        IN OUT    PACL Acl,
        IN    ULONG AceRevision,
        IN    ULONG AceFlags,
        IN    ULONG AccessMask,
        IN    PSID Sid
    );

    NTSYSAPI ULONG NTAPI RtlNtStatusToDosErrorNoTeb(
        NTSTATUS Status
    );

    NTSYSAPI NTSTATUS NTAPI RtlGetLastNtStatus(
    );

    NTSYSAPI ULONG NTAPI RtlGetLastWin32Error(
    );

    NTSYSAPI VOID NTAPI RtlSetLastWin32Error(
        ULONG WinError
    );

    NTSYSAPI VOID NTAPI RtlSetLastWin32ErrorAndNtStatusFromNtStatus(
        NTSTATUS Status
    );

    NTSYSAPI VOID NTAPI DbgBreakPoint(
        VOID
    );

    NTSYSAPI ULONG _cdecl DbgPrint(
        PCH Format,
        ...
    );

    NTSYSAPI NTSTATUS NTAPI LdrLoadDll(
        IN    PWSTR DllPath OPTIONAL,
        IN    PULONG DllCharacteristics OPTIONAL,
        IN    PUNICODE_STRING DllName,
        OUT    PVOID* DllHandle
    );

    NTSYSAPI NTSTATUS NTAPI LdrGetDllHandle(
        IN    PWSTR DllPath OPTIONAL,
        IN    PULONG DllCharacteristics OPTIONAL,
        IN    PUNICODE_STRING DllName,
        OUT    PVOID* DllHandle
    );

    NTSYSAPI NTSTATUS NTAPI LdrUnloadDll(
        IN    PVOID DllHandle
    );

    NTSYSAPI NTSTATUS NTAPI LdrGetProcedureAddress(
        IN    PVOID DllHandle,
        IN    PANSI_STRING ProcedureName OPTIONAL,
        IN    ULONG ProcedureNumber OPTIONAL,
        OUT    PVOID* ProcedureAddress
    );

#pragma endregion

#pragma region TAIL

#ifdef __cplusplus
} // extern "C"
#endif

#pragma endregion

#endif // __NTDLL_H__