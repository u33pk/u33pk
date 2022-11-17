

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

        void U33pk::DumpDexFile(ArtMethod *method)
        {
            string self_name = urzpk::U3conf::getSelfProcessName();
            if (self_name.length() > 1)
            {
                const DexFile *dex_file = method->GetDexFile();
                const uint8_t *dex_begin = dex_file->Begin();
                size_t dex_size = dex_file->Size();
//                LOG(INFO) << to_string(dex_size);
                stringstream program_dex_path;
                program_dex_path << "/data/data/" << self_name << "/u33pk/";
//                LOG(INFO) << "out dex: " << program_dex_path.str();
                if (access(program_dex_path.str().c_str(), F_OK) != 0)
                {
                    mkdir(program_dex_path.str().c_str(), 0777);
                }
                stringstream tk_dex_name;
                tk_dex_name << program_dex_path.str() << "dex_" << to_string(dex_size) << ".dex";
                WriteToFile(tk_dex_name.str(), (char *)dex_begin, dex_size);
                urzlog::info(DEFAULT_TAG, "U33pk::DumpDexFile", tk_dex_name.str());
            }
            return;
        }

        void U33pk::DumpArtMethod(ArtMethod *method){
            string self_name = urzpk::U3conf::getSelfProcessName();
            if(self_name.size() > 1) {
                const DexFile *dex_file = method->GetDexFile();
                size_t dex_size = dex_file->Size();
                stringstream program_dex_path;
                program_dex_path << "/data/data/" << self_name << "/u33pk/";
                if (access(program_dex_path.str().c_str(), F_OK) != 0)
                {
                    mkdir(program_dex_path.str().c_str(), 0777);
                }
                stringstream tk_item_name;
                tk_item_name << program_dex_path.str() << "item_" << to_string(dex_size) << ".item";
                const dex::CodeItem *item = method->GetCodeItem();
                const StandardDexFile::CodeItem *stand_code_item = reinterpret_cast<const StandardDexFile::CodeItem*>(item);
                uint32_t item_len = 0;
                if(stand_code_item != nullptr){
                    item_len = dex_file->GetCodeItemSize(*stand_code_item);
                }
                if(item_len <= 0) return;
                uint32_t method_idx_ = method->GetDexMethodIndex();
//                method->GetDexMethodIndex();
                stringstream method_count_stream;
                long encode_len;
                method_count_stream << to_string(method_idx_) << " : " << U33pk::Base64Encode((char*)item, item_len, &encode_len) << "\n";
                stringstream tk_item_name_;
                tk_item_name_ << "/data/data/" << self_name << "/u33pk/" << "dex_" << to_string(dex_size) << ".item";
                U33pk::WriteToFile(tk_item_name_.str(), method_count_stream.str());
                urzlog::info(DEFAULT_TAG, "U33pk::DumpArtMethod", tk_item_name_.str());
            }
        }

        void U33pk::WriteToFile(string tk_dex_name, char *begen, size_t _sz)
        {
            ofstream file_out;
            file_out.open(tk_dex_name, ios::out | ios::binary);
            file_out.write(begen, _sz);
            file_out.flush();
            file_out.close();
        }

        void U33pk::WriteToFile(string tk_item_name, string method_count) {
            ofstream file_out;
            file_out.open(tk_item_name, ios::app);
            file_out.write(method_count.c_str(), method_count.size());
            file_out.flush();
            file_out.close();
        }

        U33pk::U33pk(/* args */)
        {
        }

        U33pk::~U33pk()
        {
        }
    } // namespace urzpk

} // namespace art
