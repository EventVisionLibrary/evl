#ifndef EVL_CORE_SHUTDOWN_hpp_
#define EVL_CORE_SHUTDOWN_hpp_

#include <atomic>

class Shutdown
{
public:
    static std::atomic_bool globalShutdown;

    static void globalShutdownSignalHandler(int signal);

    static void usbShutdownHandler(void *ptr);

    int setSigHandler();
};

#endif // EVL_CORE_SHUTDOWN_hpp_
