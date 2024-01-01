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
        static constexpr char TRACE_TAG[] = "trace_smali";
        static constexpr char TRACE_JNI[] = "trace_jni";
        static constexpr char UPK_FUNC_TAG[] = "u33pk_func";
        static constexpr char FRIDAPK_TAG[] = "frida_pkg";
        #define ITEM_VALUE(ITEM_NAME) _line.substr(strlen(ITEM_NAME) + 3)
        #define FIND_ITEM(ITEM_NAME) _line.find(ITEM_NAME) != string::npos
        class U3conf
        {
        private:
            string conf_pkg;
            string conf_func;
            string conf_trace_smali;
            string conf_trace_jni;

        public:
            U3conf();
            ~U3conf();
            bool shouldUnpk();
            bool shouldUnpk(string current_pkg);
            bool shouldUnpkMethod(string method_name);
            U3conf* getInstence();
            bool ShouldTraceSmali(string method_name);
            bool shouldTraceJni(string method_name);
            static string getSelfProcessName();
            string getConfPkg();
        };
        
    } // namespace urzpk
    
    
} // namespace art


#endif // _ART_RUNTIME_U33PK_U3CONF_H_