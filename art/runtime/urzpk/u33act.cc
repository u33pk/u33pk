#include "u33act.h"

namespace art {
    class DexFile;
    class ArtMethod;
    class Thread;
    class JavaVMExt;

    namespace urzpk {
        
        void U33act::U3Invoke(const DexFile& dex_file){
            Thread* const self = Thread::Current();
            ObjPtr<mirror::ClassLoader> clazz_loader = getClassLoader();
            StackHandleScope<2> shs(self);
            ClassLinker* class_linker = Runtime::Current()->GetClassLinker();
            ObjPtr<mirror::DexCache> dex_cache = class_linker->FindDexCache(self, dex_file);
            Handle<mirror::DexCache> dex_cache_handle(shs.NewHandle(dex_cache));
            Handle<mirror::ClassLoader> clazz_loader_handle(shs.NewHandle(clazz_loader));
            urzlog::info("orz", "U33act::U3Invoke", dex_file.GetLocation());
            for(uint16_t clazz_idx = 0; clazz_idx < dex_file.NumClassDefs(); clazz_idx++){
                ObjPtr<mirror::Class> klazz = class_linker->ResolveType(dex_file.GetClassDef(clazz_idx).class_idx_, dex_cache_handle, clazz_loader_handle);
                if(klazz == nullptr) {
                    DCHECK(self->IsExceptionPending());
                    self->ClearException();
                    continue;
                }

                PointerSize pointer_size = class_linker->GetImagePointerSize();
                ArraySlice<ArtMethod> methods = klazz->GetDeclaredMethods(pointer_size);
                for(ArtMethod& md: methods){
                    FakeInvoke(&md, klazz);
                }
            }
        }

        ObjPtr<mirror::ClassLoader> U33act::getClassLoader() {
            Thread* const self = Thread::Current();
            ScopedObjectAccessUnchecked soa(self);
            JNIEnv* env = self->GetJniEnv();
            jclass cls_ActivityThread = env->FindClass("android/app/ActivityThread");
            jmethodID mid_currentActivityThread = env->GetStaticMethodID(cls_ActivityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
            jobject obj_ActivityThread = env->CallStaticObjectMethod(cls_ActivityThread, mid_currentActivityThread);
            jfieldID fid_mInitialApplication = env->GetFieldID(cls_ActivityThread, "mInitialApplication", "Landroid/app/Application;");
            jobject obj_mInitialApplication = env->GetObjectField(obj_ActivityThread, fid_mInitialApplication);
            jclass cls_Context = env->FindClass("android/content/Context");
            jmethodID mid_getClassLoader = env->GetMethodID(cls_Context, "getClassLoader", "()Ljava/lang/ClassLoader;");
            jobject obj_classLoader = env->CallObjectMethod(obj_mInitialApplication, mid_getClassLoader);
            return soa.Decode<mirror::ClassLoader>(obj_classLoader);
        }

        std::list<const DexFile*> U33act::getDexFiles() {
            std::list<const DexFile*> dex_files;
            Thread* const self = Thread::Current();
            ClassLinker* class_linker = Runtime::Current()->GetClassLinker();
            ReaderMutexLock mu(self, *Locks::dex_lock_);
            const std::list<ClassLinker::DexCacheData>& dex_caches = class_linker->GetDexCachesData();
            for (auto it = dex_caches.begin(); it != dex_caches.end(); ++it) {
                ClassLinker::DexCacheData data = *it;
                const DexFile* dex_file = data.dex_file;
                const std::string& dex_location = dex_file->GetLocation();
                urzlog::info("orz", "U33act::getDexFiles", dex_location);
                if (dex_location.find("/data") == 0) {
                    dex_files.push_back(dex_file);
                } else {
                    continue;
                }
                
            }
            return dex_files;
        }

        void U33act::FakeInvoke(ArtMethod *method, ObjPtr<mirror::Class> klazz) {
            if(method->IsNative() || method->IsAbstract() || method->IsProxyMethod() || !method->IsInvokable()){
                // urzlog::info(DEFAULT_TAG, "U33act::FakeInvoke-not_invoke", method->PrettyMethod());
                return;
            }
            else {
                std::string method_name = method->PrettyMethod();
                uint32_t args_size = (uint32_t)ArtMethod::NumArgRegisters(method->GetShorty());
                if (!method->IsStatic()) {
                    args_size += 1;
                }
                
                JValue result;
                Thread *self=Thread::Current();
                self->SetThreadName("114514");
                std::vector<uint32_t> args(args_size, 0);
                urzlog::info(DEFAULT_TAG, "U33act::FakeInvoke", method_name);
                // urzlog::info(DEFAULT_TAG, "U33act::FakeInvoke", std::to_string(args_size));
                result.SetI(114514);

                if (!method->IsStatic()) {
                    ObjPtr<mirror::Object> thiz = klazz->AllocObject(self);
                    args[0] = StackReference<mirror::Object>::FromMirrorPtr(thiz.Ptr()).AsVRegValue();
                }
                method->Invoke(self, args.data(), args_size, &result, method->GetShorty());
            }
        }

        bool U33act::IsFakeInvoke(Thread* self) {
            std::string name;
            self->GetThreadName(name);
            if(name.find("114514") == 0){
                return true;
            } else {
                return false;
            }
        }

    }
}