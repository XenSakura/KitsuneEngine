// module;
// #include <mpscq.h>
// export module LogHandler;
// import std;
//
// enum class LogLevel
// {
//     INFO,
//     DEBUG,
//     WARNING,
//     ERROR
// };
//
// export struct LogMessage
// {
//     LogLevel level;
//     const char* system;
//     char * message[512];
// };
// namespace FileIO
// {
//     /**
//      * Gets appdata on windows, or program directory on Linux
//      * @param appName 
//      * @return 
//      */
//     export inline std::string get_log_directory(const std::string& appName)
//     {
//         std::string basePath;
// #ifdef _WIN32
//         
//         const char* localAppData = std::getenv("LOCALAPPDATA");
//         if (localAppData)
//         {
//             basePath = std::string(localAppData) + "\\" + appName + "\\logs\\";
//             return basePath;
//         }
// #elif defined(__APPLE__)
//         //TODO: Handle on apple
// #else
//         //TODO: Linux
//         const char* home_path = std::getenv("HOME");
//         if (home_path)
//         {
//             basePath = std::string(home_path) + "/" + appName + "/logs/";
//             return basePath;
//         }
// #endif
//         return basePath;
//     }
//     
//     export inline std::string get_crash_log_directory(const std::string& appName)
//     {
//         std::string basePath;
// #ifdef _WIN32
//         const char* localAppData = std::getenv("LOCALAPPDATA");
//         if (localAppData)
//         {
//             basePath = std::string(localAppData) + "\\" + appName + "\\crash_logs\\";
//             return basePath;
//         }
// #elif defined(__APPLE__)
//         //TODO: Handle on apple
// #else
//         //TODO: Linux
//         const char* home_path = std::getenv("HOME");
//         if (home_path)
//         {
//             basePath = std::string(home_path) + "/" + appName + "/crash_logs/";
//             return basePath;
//         }
// #endif
//         return basePath;
//     }
// }
//
//
// export class LogHandler
// {
// private:
//     static inline std::atomic<bool> inUse = false;
//     static inline mpscq* Logs = mpscq_create(nullptr, 100);
//     static inline std::ofstream logFile;
// public:
//     static void initialize()
//     {
//         
//         std::string logPath = FileIO::get_log_directory("AngelBase") + "AngelBase.log";
//         logFile.open(logPath, std::ios::app);
//         
//     }
//
//     static inline void LOG_ERROR()
//     {
//         
//     };
//
//     static inline void LOG_WARNING()
//     {
//         
//     };
//
//     static inline void LOG_INFO()
//     {
//         
//     };
//
//     static inline void LOG_DEBUG()
//     {
//         
//     };
//
//     static inline void close_file()
//     {
//         auto start_time = std::chrono::steady_clock::now();
//         bool expected = false;
//         //wait until no one else is using this 
//         while (!inUse.compare_exchange_strong(expected, true))
//         {
//             if (std::chrono::steady_clock::now() - start_time > std::chrono::seconds(1))
//             {
//                 //catastrophic-- Exit out and force close
//                 std::cerr << " [Error] [LogHandler] : Catastrophic failure -- forcing file closure. " <<std::endl;
//                 break;
//             }
//             std::this_thread::sleep_for(std::chrono::milliseconds(10));
//             expected = false;
//         }
//
//         LogMessage* message;
//         while ((message = static_cast<LogMessage*>(mpscq_dequeue(Logs))) != nullptr)
//         {
//             //Print message here
//         }
//
//         if (logFile.is_open())
//         {
//             logFile.flush();
//             logFile.close();
//         }
//
//         inUse.store(false, std::memory_order_release);
//     }
// };