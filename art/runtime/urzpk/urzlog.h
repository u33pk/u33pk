#ifndef ART_RUNTIME_URZLOG_H_
#define ART_RUNTIME_URZLOG_H_

#include <android-base/logging.h>
#include <iostream>


namespace art{

    namespace urzpk {
        using namespace std;
        static constexpr char DEFAULT_TAG[] = "orz";
        static constexpr char FILTER_TAG[] = "orz";
        class urzlog {
        public:
            static void info(const string tag, const string local, const string message);
        };
    }
}
#endif