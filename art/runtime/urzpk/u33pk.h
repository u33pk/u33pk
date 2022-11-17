#ifndef ART_RUNTIME_U33PK_H_
#define ART_RUNTIME_U33PK_H_

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>

#include "art_method.h"
#include "dex/dex_file.h"
#include "art_method.h"
#include "art_method-inl.h"
#include "dex/standard_dex_file.h"

#include "u3conf.h"
#include "urzlog.h"

namespace art
{
    class DexFile;
    class ArtMethod;
    namespace urzpk
    {
        using namespace std;

        class U33pk
        {
//        private:
            /* data */
        public:
            U33pk(/* args */);
            ~U33pk();

            static char* Base64Encode(char *str, long str_len, long *outlen);
            static void DumpDexFile(ArtMethod* method) REQUIRES_SHARED(Locks::mutator_lock_);
            static void DumpArtMethod(ArtMethod *method) REQUIRES_SHARED(Locks::mutator_lock_);
            static void WriteToFile(string tk_dex_name, char* begen, size_t _sz) REQUIRES_SHARED(Locks::mutator_lock_);
            static void WriteToFile(string tk_item_name, string method_count) REQUIRES_SHARED(Locks::mutator_lock_);
        };

        
        
    } // namespace urzpk
    
} // namespace runtime
#endif