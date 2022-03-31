#ifndef ZIP_NODE_ADDON_H
#define ZIP_NODE_ADDON_H

namespace ZipAddon {
    std::string getAllEntries(zip_t*);
    struct buffer_t {
        char *data;
        size_t size;
    };
    struct input_data {
        std::string name;
        const char *content;
        size_t size;
    };
    struct tfsnContext {
        Napi::Promise::Deferred deffered;
        buffer_t data;
        bool resolve;
        bool called;
        Napi::ThreadSafeFunction tsfn;
        std::vector<input_data> inputData;
        tfsnContext(Napi::Env env)
            : deffered{Napi::Promise::Deferred::New(env)},
            data{NULL, 0},
            resolve{false},
            called{false} {};
    };
}
#endif