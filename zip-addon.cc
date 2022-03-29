#include <napi.h>
#include <stdio.h>
#include <string>
#include "promiseWrapper.h"
#include "zip.h"
#include "zip-addon.h"

using namespace Napi;

Napi::Promise unzipStream(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
    [&info](resolveFunc resolve, rejectFunc reject){
        
        if (info.Length() < 2) {
            reject("Wrong arguments length");
            return;
        }
        if (!info[0].IsTypedArray() || !info[1].IsString())
        {
            reject("params is wrong");
            return;
        }
        Napi::Buffer<uint8_t> data = info[0].As<Buffer<uint8_t> >();
        std::string dest = info[1].As<String>();
        size_t len = data.ElementLength();
        const char* buf = reinterpret_cast<char*>(data.Data());

        auto fun = [resolve, reject](const char* buf, size_t len, std::string dest) {
            int arg = 2;
            auto fun1 = [](const char* filename, void* arg) { return 0; };
          
            int code = zip_stream_extract(buf, len, dest.c_str(), fun1, &arg);
            if (code != 0) {
                reject("Error: unzip fail,code:" + std::to_string(code));
                return;
            }
            zip_t* zip = zip_stream_open(buf, len, 0, 'r');
            std::string str = getAllEntries(zip);
            resolve(str);
        };
        std::thread task1(fun, buf, len, dest);
        task1.detach();
    });
}

Napi::Promise unzipFile(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
        [&info](resolveFunc resolve, rejectFunc reject){
            
            if (info.Length() < 2) {
                reject("Wrong arguments length");
                return;
            }
            if (!info[0].IsString() || !info[1].IsString())
            {
                reject("params is wrong");
                return;
            }
            std::string file = info[0].As<String>();
            std::string dest = info[1].As<String>();

            auto fun = [resolve, reject](const std::string file, const std::string dest) {
                int arg = 2;
                auto fun1 = [](const char* filename, void* arg) { return 0; };
                auto code = zip_extract(file.c_str(), dest.c_str(), fun1, &arg );
                if (code != 0) {
                    reject("Error: unzip fail,code:" + std::to_string(code));
                    return;
                }
                zip_t* zip = zip_open(file.c_str(), 0, 'r');
                std::string str = getAllEntries(zip);
                resolve(str);
            };
            std::thread task1(fun, file, dest);
            task1.detach();
        });
}



std::string getAllEntries(zip_t* zip)
{
    std::string str = "[";
    int i, n = zip_entries_total(zip);
    for (i = 0; i < n; ++i) {
        zip_entry_openbyindex(zip, i);
        {
            const char* name = zip_entry_name(zip);
            str.append("\"");
            str.append(name);
            if (i==n - 1) {
                str.append("\"");
            } else {
                str.append("\",");
            }
        }
        zip_entry_close(zip);
    }
    str.append("]");
    return str;
}

// hello world
Napi::String HellowMethod(const Napi::CallbackInfo &info)
{
        Napi::Env env = info.Env();
        return Napi::String::New(env, "hello world");
}
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
        exports.Set(String::New(env, "hello"), Function::New(env, HellowMethod));
        exports.Set(String::New(env, "unzipStream"), Function::New(env, unzipStream));
        exports.Set(String::New(env, "unzipFile"), Function::New(env, unzipFile));
        return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)