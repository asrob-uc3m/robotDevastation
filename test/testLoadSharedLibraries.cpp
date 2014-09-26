#include "RdRobotManager.hpp"

#include <vector>
#include <string>
#include <stdlib.h>

#include <yarp/os/SharedLibrary.h>

using namespace rd;

int main()
{
    //- -Set the shared libraries folder:
    putenv("LD_LIBRARY_PATH=/home/def/Repositories/robotDevastation/build/lib/");

    //-- Store a list of shared libraries available
    std::vector<std::string> shared_lib_files;
    shared_lib_files.push_back("RdTextualRobot");

    //-- Dynamically load the shared libraries
    std::vector<yarp::os::SharedLibrary*> shared_libs;

    for (int i = 0; i < shared_lib_files.size(); i++)
    {
        yarp::os::SharedLibrary * new_shared_lib = new yarp::os::SharedLibrary(shared_lib_files[i].c_str());
        if (new_shared_lib->isValid())
        {
            RD_SUCCESS("Loaded shared lib \"%s\" successfully!\n", shared_lib_files[i].c_str());
            shared_libs.push_back(new_shared_lib);
        }
        else
        {
            RD_ERROR("Error loading lib \"%s\"\n", shared_lib_files[i].c_str());
        }

        if ( shared_libs.size() > 0 && shared_libs.back() != NULL)
        {
//            bool (*fn)(void) = (bool (*)(void))(shared_libs.back()->getSymbol("load"));
//            if (fn==NULL)
//            {
//                RD_ERROR("Could not load library!\n");
//                return 0;
//            }

//            if ( !(*fn)() )
//            {
//                RD_ERROR("Could not load library!\n");
//            }
                void * fn = shared_libs.back()->getSymbol("_Z9somethingv");
                if (fn==NULL)
                {
                    RD_ERROR("Could not load symbol!\n");
                    return 0;
                }
                bool (*func)(void);
                func = (bool(*)())(fn);
                func();

        }

    }


    //-- List all the available plugins
    std::vector<std::string> available_managers = RdRobotManager::listAll();
    for (int i = 0; i < available_managers.size(); i++)
        RD_SUCCESS("Loaded plugin \"%s\" succesfully!\n", available_managers[i].c_str());

    for (int i = 0; i < shared_libs.size(); i++)
    {
        shared_libs[i]->close();
        delete shared_libs[i];
        shared_libs[i] = NULL;
    }
    return 0;
}
