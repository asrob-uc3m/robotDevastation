#include "RdRobotManager.hpp"

#include <vector>
#include <string>

#include <dlfcn.h>

using namespace rd;

int main()
{
    //-- Store a list of shared libraries available
    std::vector<std::string> shared_libs;
    shared_libs.push_back("../lib/libRdTextualRobot.so");

    //-- Dynamically load the shared libraries
    void *lib_handle;

    lib_handle = dlopen(shared_libs[0].c_str(), RTLD_LAZY);
    if (!lib_handle)
    {
       RD_ERROR("%s\n", dlerror());
       return 1;
    }

    //-- List all the available plugins
    std::vector<std::string> available_managers = RdRobotManager::listAll();
    for (int i = 0; i < available_managers.size(); i++)
        RD_SUCCESS("Loaded plugin \"%s\" succesfully!\n", available_managers[i].c_str());

    dlclose(lib_handle);
    return 0;
}
