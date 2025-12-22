// module;
// #pragma once
// // Platform detection macros
// #if defined(_WIN32) || defined(_WIN64)
// #define PLATFORM_WINDOWS
// #include <Windows.h>
// #include <DbgHelp.h>
// #pragma comment(lib, "dbghelp.lib")
// #elif defined(__APPLE__)
// #define PLATFORM_MACOS
// #include <TargetConditionals.h>
// #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
// #define PLATFORM_IOS
// #endif
// #include <execinfo.h>
// #include <signal.h>
// #include <unistd.h>
// #elif defined(__ANDROID__)
// #define PLATFORM_ANDROID
// #include <android/log.h>
// #include <unwind.h>
// #include <signal.h>
// #elif defined(__linux__)
// #define PLATFORM_LINUX
// #include <execinfo.h>
// #include <signal.h>
// #include <unistd.h>
// #include <libunwind.h>
// #elif defined(__unix__)
// #define PLATFORM_UNIX
// #include <execinfo.h>
// #include <signal.h>
// #include <unistd.h>
// #else
// #error "Unsupported platform"
// #endif
//
// // Architecture detection
// #if defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
// #define ARCH_X64
// #elif defined(_M_IX86) || defined(__i386__)
// #define ARCH_X86
// #elif defined(_M_ARM64) || defined(__aarch64__)
// #define ARCH_ARM64
// #elif defined(_M_ARM) || defined(__arm__)
// #define ARCH_ARM
// #endif
//
// // Compiler detection
// #if defined(_MSC_VER)
// #define COMPILER_MSVC
// #elif defined(__clang__)
// #define COMPILER_CLANG
// #elif defined(__GNUC__)
// #define COMPILER_GCC
// #endif
//
//
// export module ExceptionHandler;
// import LogHandler;
// import std;
//
// namespace AngelBase::Core::Debug
// {
//     class Exception : public std::exception
//     {
//     public:
//         std::source_location loc;
//         std::string message;
//         Exception(std::string msg, 
//                               std::source_location l = std::source_location::current())
//                 : loc(l), message(std::move(msg)) {}
//     
//         const char* what() const noexcept override { return message.c_str(); }
//         const std::source_location& where() const noexcept { return loc; }
//         
//     };
//         
//     /**
//      * Used for capturing information about crashes
//      */
//     struct CrashContext
//     {
//         std::string timeStamp;
//         std::string stackTrace;
//         std::string sourceLocation;
//         std::string threadInfo;
//         std::string systemInfo;
//         std::string environmentInfo;
//         std::string memoryInfo;
//
//         /**
//          * creates crash information with std library
//          * @param loc location of the crash
//          * @return returns struct of crash context
//          */
//         static CrashContext createCrashContext(const std::source_location& loc = std::source_location())
//         {
//             CrashContext ctx;
//
//             // 1. Current Timestamps
//             auto now = std::chrono::system_clock::now();
//             auto nowTime = std::chrono::system_clock::to_time_t(now);
//             auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
//                 now.time_since_epoch()) % 1000;
//             
//             std::ostringstream tsSs;
//             tsSs << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H:%M:%S")
//                  << '.' << std::setfill('0') << std::setw(3) << ms.count();
//             ctx.timeStamp = tsSs.str();
//
//             //2. capture stack trace
//             auto trace = std::stacktrace::current();
//             std::ostringstream oss;
//             oss << trace;
//             ctx.stackTrace = oss.str();
//             
//             std::ostringstream locSs;
//             locSs << "File: " << loc.file_name() << "\n"
//                   << "Line: " << loc.line() << "\n"
//                   << "Column: " << loc.column() << "\n"
//                   << "Function: " << loc.function_name();
//             ctx.sourceLocation = locSs.str();
//
//             // 3. Capture thread info
//             std::ostringstream threadSs;
//             threadSs << "Thread ID: " << std::this_thread::get_id();
//             ctx.threadInfo = threadSs.str();
//
//             // 4. Capture relevant memory 
//             std::ostringstream memSs;
//             
//             // Current working directory
//             try 
//             {
//                 memSs << "Working Directory: " << std::filesystem::current_path() << "\n";
//             }
//             catch (...) 
//             {
//                 memSs << "Working Directory: Unable to retrieve\n";
//             }
//             
//             // Temp directory
//             try 
//             {
//                 memSs << "Temp Directory: " << std::filesystem::temp_directory_path() << "\n";
//             }
//             catch (...) 
//             {
//                 memSs << "Temp Directory: Unable to retrieve\n";
//             }
//             
//             // Disk space
//             try 
//             {
//                 auto space = std::filesystem::space(std::filesystem::current_path());
//                 memSs << "Disk Space Available: " 
//                       << (space.available / 1024 / 1024) << " MB\n";
//                 memSs << "Disk Space Capacity: " 
//                       << (space.capacity / 1024 / 1024) << " MB\n";
//             }
//             catch (...) 
//             {
//                 memSs << "Disk Space: Unable to retrieve\n";
//             }
//             
//             ctx.memoryInfo = memSs.str();
//
//             // 5. Capture relevant environment information
//             std::ostringstream envSs;
//             
//             // Get some common environment variables
//             const char* envVars[] = {
//                 "PATH", "HOME", "USER", "TEMP", "TMP", 
//                 "COMPUTERNAME", "USERNAME", "PROCESSOR_ARCHITECTURE"
//             };
//             
//             for (const char* var : envVars)
//             {
//                 if (const char* val = std::getenv(var))
//                 {
//                     envSs << var << ": " << val << "\n";
//                 }
//             }
//
//             ctx.environmentInfo = envSs.str();
//             
//             // 6. Capture relevant system information
//             std::ostringstream sysSs;
//             
//             // Hardware concurrency
//             sysSs << "Hardware Threads: " 
//                   << std::thread::hardware_concurrency() << "\n";
//             
//             // Compiler and standard info
//             sysSs << "C++ Standard: " << __cplusplus << "\n";
//             
//     #ifdef __VERSION__
//             sysSs << "Compiler: " << __VERSION__ << "\n";
//     #endif
//             
//     #ifdef _MSC_VER
//             sysSs << "MSVC Version: " << _MSC_VER << "\n";
//     #endif
//             
//     #ifdef __GNUC__
//             sysSs << "GCC Version: " << __GNUC__ << "." 
//                   << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__ << "\n";
//     #endif
//             
//     #ifdef __clang__
//             sysSs << "Clang Version: " << __clang_major__ << "." 
//                   << __clang_minor__ << "." << __clang_patchlevel__ << "\n";
//     #endif
//             
//             // Build configuration
//     #ifdef NDEBUG
//             sysSs << "Build Type: Release\n";
//     #else
//             sysSs << "Build Type: Debug\n";
//     #endif
//             
//             ctx.systemInfo = sysSs.str();
//             
//             return ctx;
//         }
//     };
//
//
//
//     /**
//      * Static class-- must be able to be called anywhere and handle all relevant functions
//      */
//     export class ExceptionHandler
//     {
//     public:
//         /**
//          * Initializes crash handler instance
//          * @param app_name name of program crashing
//          */
//         static void Initialize(const std::string& app_name)
//         {
//             std::set_terminate([]()
//             {
//                 std::exception_ptr ex = std::current_exception();
//                 if (ex)
//                 {
//                     try
//                     {
//                         std::rethrow_exception(ex);
//                     }
//                     catch (const Exception& e)
//                     {
//                         auto ctx = CrashContext::createCrashContext(e.loc);
//             
//                         std::cerr << "FATAL: C++ exception caught: " << e.what() << '\n';
//                         std::cerr << "Timestamp: " << ctx.timeStamp << "\n\n";
//                         std::cerr << ctx.sourceLocation << "\n\n";
//                         std::cerr << ctx.threadInfo << "\n";
//                         std::cerr << ctx.systemInfo << "\n";
//                         std::cerr << ctx.memoryInfo << "\n";
//                         std::cerr << "Stack Trace:\n" << ctx.stackTrace << "\n";
//                     }
//                     catch (const std::exception& e)
//                     {
//                         auto ctx = CrashContext::createCrashContext();
//                         std::cerr << "ERROR: Don't use std runtime errors-- Use AngelBase::Core::Debug::Exception instead." << std::endl;
//                         std::cerr << "FATAL: C++ exception caught: " << e.what() << '\n';
//                         std::cerr << "Timestamp: " << ctx.timeStamp << "\n\n";
//                         std::cerr << ctx.sourceLocation << "\n\n";
//                         std::cerr << ctx.threadInfo << "\n";
//                         std::cerr << ctx.systemInfo << "\n";
//                         std::cerr << ctx.memoryInfo << "\n";
//                         std::cerr << "Stack Trace:\n" << ctx.stackTrace << "\n";
//                         
//                     }
//                     catch (...)
//                     {
//                         auto ctx = CrashContext::createCrashContext();
//                         std::cerr << "Unknown C++ exception caught\n";
//                         std::cerr << "Timestamp: " << ctx.timeStamp << "\n\n";
//                         std::cerr << ctx.sourceLocation << "\n\n";
//                         std::cerr << ctx.threadInfo << "\n";
//                         std::cerr << ctx.systemInfo << "\n";
//                         std::cerr << ctx.memoryInfo << "\n";
//                         std::cerr << "Stack Trace:\n" << ctx.stackTrace << "\n";
//                     }
//                     //todo: close log file
//
//                     //todo: see if we can handle regular mini dumps too.
//                 }
//                 else
//                 {
//                     // terminate called without an exception
//                     std::cerr << "std::terminate called without active exception\n";
//                     std::abort();
//                 }
//             });
//             appName = app_name;
//             //On windows, we can also grab the crash dump
//     #ifdef PLATFORM_WINDOWS
//             {
//     #ifdef _DEBUG
//                 ULONG page = 16 * 1024;
//     #else
//                 ULONG page = 8 * 1024;            
//     #endif
//                 SetThreadStackGuarantee(&page);
//             }
//     #endif
//             
//         }
//
//     #ifdef PLATFORM_WINDOWS
//         /**
//          * Creates a crash dump
//          * @param pException -- pointers to the exception
//          * @param context crash context-- includes timestamps, thread info, etc.
//          * @return returns a LONG to tell WinAPI how to next handle the exception
//          */
//         [[nodiscard]] static LONG WINAPI WriteDump(EXCEPTION_POINTERS* pException, const CrashContext& context)
//         {
//             bool expected = false;
//             while (inUse.exchange(true, std::memory_order_acquire))
//             {
//                 std::this_thread::sleep_for(std::chrono::milliseconds(100));
//             }
//             auto flags = MINIDUMP_TYPE(MiniDumpNormal | MiniDumpWithDataSegs | MiniDumpWithCodeSegs | MiniDumpWithIndirectlyReferencedMemory | MiniDumpWithUnloadedModules | MiniDumpWithFullMemory);
//             std::string file_name = appName + "_" + context.timeStamp + "_" + "crash.dmp";
//
//             std::string file_path = FileIO::get_crash_log_directory("AngelBase") + file_name;
//             HANDLE file = CreateFileA(file_path.c_str(), GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//             MINIDUMP_EXCEPTION_INFORMATION dmp;
//             dmp.ThreadId = GetCurrentThreadId();
//             dmp.ExceptionPointers = pException;
//             dmp.ClientPointers = FALSE;
//
//             BOOL dmp_success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file,flags, &dmp, nullptr, nullptr);
//
//             CloseHandle(file);
//
//             if (dmp_success)
//             {
//                 std::cout << "Dump file created successfully. " << std::endl;
//             }
//             else
//             {
//                 std::cerr << "Dump file creation failed. " << std::endl;
//             }
//             
//             inUse.store(false, std::memory_order_release);
//
//             return EXCEPTION_CONTINUE_SEARCH;
//         }
//     #endif
//     private:
//         inline static std::string appName;
//         inline static std::atomic<bool> inUse = false; 
//     };
//
// }
//
//
//
//
//
//    