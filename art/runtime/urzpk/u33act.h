#ifndef ART_RUNTIME_U33ACT_H_
#define ART_RUNTIME_U33ACT_H_

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

#include "art_method.h"
#include "dex/dex_file.h"
#include "art_method.h"
#include "art_method-inl.h"
#include "mirror/class-alloc-inl.h"
#include "dex/standard_dex_file.h"
#include "thread.h"
#include "managed_stack.h"
#include "mirror/class.h"
#include "jni.h"
#include "jni/jni_internal.h"
#include "dex/dex_instruction.h"
// #include "jni_env_ext.h"
#include <list>

#include "u3conf.h"
#include "urzlog.h"

namespace art {
    class DexFile;
    class ArtMethod;
    class Thread;
    class JavaVMExt;
    namespace urzpk {
        class U33act {

        public:
            static void U3Invoke(const DexFile& dex_file) REQUIRES_SHARED(Locks::mutator_lock_);
            static ObjPtr<mirror::ClassLoader> getClassLoader() REQUIRES_SHARED(Locks::mutator_lock_);
            static void FakeInvoke(ArtMethod *method , ObjPtr<mirror::Class> klazz) REQUIRES_SHARED(Locks::mutator_lock_);
            static std::list<const DexFile*> getDexFiles() REQUIRES_SHARED(Locks::mutator_lock_);
            static U33act* getInstance(int mod, Thread* act_thread) REQUIRES_SHARED(Locks::mutator_lock_);
            static bool IsFakeInvoke(Thread* self);
        };
        // static void U33act_U3Invoke(JNIEnv*, jclass) REQUIRES_SHARED(Locks::mutator_lock_);
    }
    
}

#endif