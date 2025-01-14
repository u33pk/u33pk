

#include "u33pk.h"

namespace art
{
    class DexFile;
    class ArtMethod;
    namespace urzpk
    {
        char *U33pk::Base64Encode(char *str, long str_len, long *outlen)
        {
            long len;
            char *res;
            int i, j;
            const char *base64_table =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
            if (str_len % 3 == 0)
                len = str_len / 3 * 4;
            else
                len = (str_len / 3 + 1) * 4;

            res = (char *)malloc(sizeof(char) * (len + 1));
            res[len] = '\0';
            *outlen = len;
            for (i = 0, j = 0; i < len - 2; j += 3, i += 4)
            {
                res[i] = base64_table[str[j] >> 2];
                res[i + 1] = base64_table[(str[j] & 0x3) << 4 | (str[j + 1] >> 4)];
                res[i + 2] = base64_table[(str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6)];
                res[i + 3] = base64_table[str[j + 2] & 0x3f];
            }

            switch (str_len % 3)
            {
            case 1:
                res[i - 2] = '=';
                res[i - 1] = '=';
                break;
            case 2:
                res[i - 1] = '=';
                break;
            }

            return res;
        }

        // void U33pk::DumpDexFile(string self_name, ArtMethod *method)
        void U33pk::U33pkDEX(string self_name, ArtMethod *method)
        {
            // string self_name = urzpk::U3conf::getSelfProcessName();
            // string self_name = conf.getConfPkg();
            if (self_name.length() > 1)
            {
                
                const DexFile *dex_file = method->GetDexFile();
                
                const string& dex_location = dex_file->GetLocation();
                if (dex_location.find("/system/") != std::string::npos || dex_location.find("/apex/") != std::string::npos ) {
                    // 如果是framework就不dump
                    return;
                }
                const uint8_t *dex_begin = dex_file->Begin();
                size_t dex_size = dex_file->Size();
                //                LOG(INFO) << to_string(dex_size);
                stringstream program_dex_path;
                // program_dex_path << "/data/data/" << self_name << "/u33pk/";
                program_dex_path << "/storage/emulated/0/Android/data/" << self_name << "/files/u33pk/";
                // program_dex_path << "/sdcard/u33pk/";
                //                LOG(INFO) << "out dex: " << program_dex_path.str();
                if (access(program_dex_path.str().c_str(), F_OK) != 0)
                {
                    mkdir(program_dex_path.str().c_str(), 0777);
                }
                stringstream tk_dex_name;
                tk_dex_name << program_dex_path.str() << "dex_" << to_string(dex_size) << ".dex";
                WriteToFile(tk_dex_name.str(), (char *)dex_begin, dex_size);
                // urzlog::info(DEFAULT_TAG, "DumpDexFile", tk_dex_name.str());
            }
            return;
        }

        // void U33pk::DumpArtMethod(string self_name, ArtMethod *method)
        void U33pk::U33pkMTD(string self_name, ArtMethod *method)
        {
            // string self_name = urzpk::U3conf::getSelfProcessName();
            // string self_name = conf.getConfPkg();
            if (self_name.size() > 1)
            {
                // urzlog::info(DEFAULT_TAG, "DumpArtMethod", self_name);
                const DexFile *dex_file = method->GetDexFile();
                const string& dex_location = dex_file->GetLocation();
                if (dex_location.find("/system/") != std::string::npos || dex_location.find("/apex/") != std::string::npos ){
                    // 如果是framework就不dump
                    return;
                }
                size_t dex_size = dex_file->Size();
                stringstream program_dex_path;
                uint32_t method_off = 0;
                // program_dex_path << "/data/data/" << self_name << "/u33pk/";
                // /storage/emulated/0/Android/data/cn.com.spdb.mobilebank.per/files
                program_dex_path << "/storage/emulated/0/Android/data/" << self_name << "/files/u33pk/";
                // program_dex_path << "/sdcard/u33pk/";
                if (access(program_dex_path.str().c_str(), F_OK) != 0)
                {
                    mkdir(program_dex_path.str().c_str(), 0777);
                }
                stringstream tk_item_name;
                tk_item_name << program_dex_path.str() << "item_" << to_string(dex_size) << ".item";
                const dex::CodeItem *item = method->GetCodeItem();
                CodeItemDataAccessor accessor(*dex_file, item);
                if(accessor.HasCodeItem()){
                    uint32_t item_len = 0;
                    uint32_t ins_size = accessor.InsnsSizeInCodeUnits();
                    method_off = (uint64_t)item - (uint64_t)(dex_file->Begin());

                    item_len = 16 + ins_size * 2;
                    uint32_t method_idx_ = method->GetDexMethodIndex();
                    stringstream method_count_stream;
                    long encode_len;
                    method_count_stream << to_string(method_idx_) << " : " << to_string(method_off) << " : " << U33pk::Base64Encode((char*)item, item_len, &encode_len) << "\n";
                    U33pk::WriteToFile(tk_item_name.str(), method_count_stream.str());
                }
            }
        }

        void U33pk::WriteToFile(string tk_dex_name, char *begen, size_t _sz)
        {
            
            ofstream file_out;
            int dex_fd;
            struct stat info;
            // void* dex_mem;
            if(access(tk_dex_name.c_str(), F_OK) != 0){
                dex_fd = open(tk_dex_name.c_str(), O_RDWR | O_CREAT, 0666);
                urzlog::info("orz", "begen", begen);
                int w_sz = write(dex_fd, begen, _sz);
                // int w_sz = write(dex_fd, "123456789", 10);
                w_sz += 1;
                close(dex_fd);
            } else {
                stat(tk_dex_name.c_str(), &info);
                if((unsigned long)info.st_size != _sz) {
                    dex_fd = open(tk_dex_name.c_str(), O_RDWR | O_CREAT, 0666);
                    int w_sz = write(dex_fd, begen, _sz);
                    // int w_sz = write(dex_fd, "123456789", 10);
                    w_sz += 1;
                    close(dex_fd);
                }
            }
            
        }

        void U33pk::WriteToFile(string tk_item_name, string _count)
        {
            
            int item_fd = open(tk_item_name.c_str(), O_RDWR | O_APPEND | O_CREAT, 0666);
            ssize_t write_len = write(item_fd, _count.c_str(), _count.size());
            write_len += 1;
            close(item_fd);
            // urzlog::info("orz", "WriteToFile", _count);
            
        }

        ofstream U33pk::GetTraceSmaliStream(string current_pkg, ArtMethod *method)
        {
            ofstream trace_smali_out;
            stringstream tk_item_name_;
            tk_item_name_ << "/storage/emulated/0/Android/data/" << current_pkg << "/files/u33pk/smali.txt";
            trace_smali_out.open(tk_item_name_.str(), ios::app);
            trace_smali_out << "---------- " << method->PrettyMethod() << " ----------\n";
            return trace_smali_out;
        }

        void U33pk::BeforJNITrace(Thread *thread, ArtMethod *current_method)
        {
            ArtMethod *caller_method = nullptr;
            stringstream tk_item_name_;
            stringstream jni_trace_count;
            U3conf conf;
            string current_pkg = U3conf::getSelfProcessName();
            string self_pkg = conf.getConfPkg();

            if(!conf.shouldUnpk(current_pkg)){
                return ;
            }

            const ManagedStack *managedStack = thread->GetManagedStack();
            if (managedStack != nullptr)
            {
                // ArtMethod **top_frame = managedStack->PopShadowFrame();
                ArtMethod **temp_frame = managedStack->GetTopQuickFrame();
                if (temp_frame != nullptr)
                {
                    caller_method = *temp_frame;
                    // urzlog::info(DEFAULT_TAG, "U33pk::BeforJNITrace", "tmp_fram find it");
                }
                // managedStack->PushShadowFrame()
            }
            
            if (caller_method != nullptr)
            {
                if (!conf.shouldTraceJni(caller_method->PrettyMethod())){
                // urzlog::info(DEFAULT_TAG, "U33pk::BeforJNITrace", "not trace");
                    return ;
                }
                tk_item_name_ << "/storage/emulated/0/Android/data/" << self_pkg << "/files/u33pk/jni.txt";
                // trace_smali_out.open(tk_item_name_.str(), ios::app);
                jni_trace_count << "[JNI]" << caller_method->PrettyMethod() << " <-> " << current_method->PrettyMethod() << "\n";
                // jni_trace_count << "[JNI]" << current_method->PrettyMethod();
                // urzlog::info(DEFAULT_TAG, "U33pk::BeforJNITrace", jni_trace_count.str());
                U33pk::WriteToFile(tk_item_name_.str(), jni_trace_count.str());
                return ;
            }
            else
            {
                // urzlog::info(DEFAULT_TAG, "U33pk::BeforJNITrace", "caller_method == nullptr");
                return ;
            }
        }

        // void U33pk::AfterJNITrace(ArtMethod *caller_method, ArtMethod *current_method, U3conf conf)
        // {
        //     stringstream tk_item_name_;
        //     stringstream jni_trace_count;
        //     string current_pkg = conf.getConfPkg();
        //     if (caller_method != nullptr)
        //     {
        //         tk_item_name_ << "/storage/emulated/0/Android/data/" << current_pkg << "/files/u33pk/jni.txt";
        //         // trace_smali_out.open(tk_item_name_.str(), ios::app);
        //         jni_trace_count << "[JNI]" << caller_method->PrettyMethod() << " |-| " << current_method->PrettyMethod() << "\n";
        //         U33pk::WriteToFile(tk_item_name_.str(), jni_trace_count.str());
        //     }
        // }

        void U33pk::DumpJNIRegister(string class_name, const char* name, const char* sig, const void* fnPtr) {
            stringstream tk_item_name_;
            stringstream jni_register_count;
            U3conf conf;
            string current_pkg = U3conf::getSelfProcessName();
            string self_pkg = conf.getConfPkg();
            if(!conf.shouldUnpk(current_pkg)){
                return ;
            }
            tk_item_name_ << "/storage/emulated/0/Android/data/" << self_pkg << "/files/u33pk/jni.txt";
            jni_register_count << class_name << "." << name << " " << sig << " register in addr : " << fnPtr << "\n";
            U33pk::WriteToFile(tk_item_name_.str(), jni_register_count.str());
        }

        

        U33pk::U33pk(/* args */)
        {
        }

        U33pk::~U33pk()
        {
        }

        static void U33pk_fakeInvoke(JNIEnv*, jclass) REQUIRES_SHARED(Locks::mutator_lock_){
            // Thread* self = Thread::Current();
            // U33act *act = U33act::getInstance(0, self);
            urzlog::info(DEFAULT_TAG, "U33pk_fakeInvoke", "fake invoke start");
            for(auto dex_file: U33act::getDexFiles()){
                // U33act::U3Invoke(*dex_file);
                const string& dex_location = dex_file->GetLocation();
                if (dex_location.find("/system/") == std::string::npos || dex_location.find("/apex/") == std::string::npos ){
                    U33act::U3Invoke(*dex_file);
                }
                
            }
        }

        static JNINativeMethod gMethods[] = {
            // NATIVE_METHOD(U33pk, fakeInvoke, "()V");
            {"fakeInvoke", "()V", reinterpret_cast<void*>(U33pk_fakeInvoke)}
        };

        void U33pk::register_orz_u33pk_U33pk(JNIEnv* env) {
            // REGISTER_NATIVE_METHODS("orz/u33pk/U33pk");
            RegisterNativeMethodsInternal(env, "orz/u33pk/U33pk", gMethods, 1);
            urzlog::info("orz", "register", "register fake invoke");
        }
    } // namespace urzpk

} // namespace art
