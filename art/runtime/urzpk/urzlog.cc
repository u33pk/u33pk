#include "urzlog.h"

namespace art{
    namespace urzpk {
        using namespace std;
        void urzlog::info(const string tag, const string local,const string message){
            LOG(INFO) << FILTER_TAG << " [" << tag << "-" << local << "] "<< message;
        }
    }
}