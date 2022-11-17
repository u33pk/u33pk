#ifndef _ART_RUNTIME_U33PK_U3CONF_H_
#define _ART_RUNTIME_U33PK_U3CONF_H_

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<string.h>
#include <android-base/logging.h>

#include "urzlog.h"

using namespace std;

namespace art
{
    namespace urzpk
    {
        static constexpr char conf_path[] = "/data/local/tmp/u33pk.conf";
        static constexpr char UPK_TAG[] = "u33pk_pkg";
        static constexpr char TRACE_TAG[] = "trace_pkg";
        static constexpr char UPK_FUNC_TAG[] = "u33pk_func";
        static constexpr char FRIDAPK_TAG[] = "frida_pkg";
        class U3conf
        {
        private:
            string conf_pkg;
            string conf_func;

        public:
            U3conf();
            ~U3conf();
            bool shouldUnpk();
            bool shouldUnpk(string current_pkg);
            bool shouldUnpkMethod(string method_name);
            U3conf* getInstence();

            static string getSelfProcessName();
        };
        
    } // namespace urzpk
    
    
} // namespace art


#endif // _ART_RUNTIME_U33PK_U3CONF_H_