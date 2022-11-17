#include "u3conf.h"

namespace art
{
    namespace urzpk
    {
        static U3conf *U3CONF_INSTENCE = NULL;
        U3conf::U3conf()
        {
            fstream conf_fs(conf_path, ios::in);
            if (conf_fs.good())
            {
                string _line;
                getline(conf_fs, _line, '\n');
                if (_line.find(UPK_TAG) != string::npos)
                {
                    this->conf_pkg = _line.substr(strlen(UPK_TAG) + 3);
//                    LOG(INFO) << this->conf_pkg;
                    urzlog::info(UPK_TAG, "U3conf::U3conf", this->conf_pkg);
                } else if (_line.find(UPK_FUNC_TAG) != string::npos) {
                    this->conf_func = _line.substr(strlen(UPK_FUNC_TAG) + 3);
                    urzlog::info(UPK_FUNC_TAG, "U3conf::U3conf", this->conf_func);
                }
            }
        }
        U3conf::~U3conf()
        {
        }

        bool U3conf::shouldUnpk()
        {
            string current_pkg = U3conf::getSelfProcessName();
            return shouldUnpk(current_pkg);
        }
        
        bool U3conf::shouldUnpk(string current_pkg){

            if (this->conf_pkg.size() > 0 && current_pkg.find(this->conf_pkg) != string::npos)
            {
//                LOG(INFO) << "[u33pk] " << this->conf_pkg;
                urzlog::info(DEFAULT_TAG, "U3conf::shouldUnpk", current_pkg);
                return true;
            }
            else
            {
                return false;
            }
        }

        bool U3conf::shouldUnpkMethod(string method_name) {
            if(this->conf_func.size() > 0 && method_name.find(this->conf_func) != string::npos){
                urzlog::info(DEFAULT_TAG, "U3conf::shouldUnpkMethod", method_name);
                return true;
            }
            return false;
        }

        U3conf *U3conf::getInstence()
        {
            if (U3CONF_INSTENCE == NULL)
            {
                U3CONF_INSTENCE = new U3conf();
            }
            return U3CONF_INSTENCE;
        }

        string U3conf::getSelfProcessName()
        {
            int self_pid = getpid();
            ifstream file_in;
            stringstream _cmdline;
            _cmdline << "/proc/" << to_string(self_pid) << "/cmdline";
            file_in.open(_cmdline.str(), ios::in);
            if (!file_in.is_open())
            {
//                LOG(INFO) << "{orz} not found " << _cmdline.str();
                return "";
            }
            string self_name;
            getline(file_in, self_name);
            file_in.close();

            // .eg: com.test.xxx:remote
            size_t _pos = self_name.find(':');
            if (_pos != string::npos)
            {
                self_name = self_name.substr(0, _pos);
            }
            return self_name;
        }

    } // namespace urzpk

} // namespace art
