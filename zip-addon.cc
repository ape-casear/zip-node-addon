#include <napi.h>
#include <stdio.h>
#include <string>
#include <memory.h>
#include "promiseWrapper.h"
#include "zip.h"
#include "zip-addon.h"

using namespace Napi;
using namespace ZipAddon;

/**
 * 从buffer解压文件
 * @param info
 * @return Napi::Promise
 */
Napi::Promise unzipStream(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
                              [&info](resolveFunc resolve, rejectFunc reject)
                              {
                                  if (info.Length() < 2)
                                  {
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
                                  const char *buf = reinterpret_cast<char *>(data.Data());

                                  // 因为node那边的buffer会持有buf这个块内存，所以这里传递指针给子线程不怕未定义行为（我不能确定）
                                  std::function<void(const char *, size_t, std::string)> fun = [resolve, reject](const char *buf, size_t len, std::string dest)
                                  {
                                      int arg = 2;
                                      ZipAddon::on_extract fun1 = [](const char *filename, void *arg)
                                      { return 0; };

                                      int code = zip_stream_extract(buf, len, dest.c_str(), fun1, &arg);
                                      if (code != 0)
                                      {
                                          reject("Error: unzip fail,code:" + std::to_string(code));
                                          return;
                                      }
                                      zip_t *zip = zip_stream_open(buf, len, 0, 'r');
                                      std::string str = getAllEntries(zip);
                                      zip_stream_close(zip);
                                      resolve(str);
                                  };
                                  std::thread task1(fun, buf, len, dest);
                                  task1.detach();
                              });
}
/**
 * 从指定文件解压文件
 * @param info
 * @return Napi::Promise
 */
Napi::Promise unzipFile(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
                              [&info](resolveFunc resolve, rejectFunc reject)
                              {
                                  if (info.Length() < 2)
                                  {
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

                                  std::function<void(const std::string, const std::string)> fun = [resolve, reject](const std::string file, const std::string dest)
                                  {
                                      int arg = 2;
                                      ZipAddon::on_extract fun1 = [](const char *filename, void *arg)
                                      { return 0; };
                                      int code = zip_extract(file.c_str(), dest.c_str(), fun1, &arg);
                                      if (code != 0)
                                      {
                                          reject("Error: unzip fail,code:" + std::to_string(code));
                                          return;
                                      }
                                      zip_t *zip = zip_open(file.c_str(), 0, 'r');
                                      std::string str = getAllEntries(zip);
                                      zip_close(zip);
                                      resolve(str);
                                  };
                                  std::thread task1(fun, file, dest);
                                  task1.detach();
                              });
}

/**
 * 从buffer列出所有文件
 * @param info
 * @return Napi::Promise
 */
Napi::Promise listAllEntriesFromBuffer(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
                              [&info](resolveFunc resolve, rejectFunc reject)
                              {
                                  if (info.Length() < 1)
                                  {
                                      reject("Wrong arguments length");
                                      return;
                                  }
                                  if (!info[0].IsTypedArray())
                                  {
                                      reject("params is wrong");
                                      return;
                                  }
                                  Napi::Buffer<uint8_t> data = info[0].As<Buffer<uint8_t> >();
                                  size_t len = data.ElementLength();
                                  const char *buf = reinterpret_cast<char *>(data.Data());

                                  std::function<void(const char *, size_t)> fun = [resolve, reject](const char *buf, size_t len)
                                  {
                                      zip_t *zip = zip_stream_open(buf, len, 0, 'r');
                                      if (!zip)
                                      {
                                          reject("create zip object fail:check input");
                                          return;
                                      }
                                      std::string str = getAllEntries(zip);
                                      zip_stream_close(zip);
                                      if (str.compare("") == 0)
                                      {
                                          reject("get entry fail:check input");
                                          return;
                                      }
                                      resolve(str);
                                  };
                                  std::thread task1(fun, buf, len);
                                  task1.detach();
                              });
}
/**
 * 从zip文件列出所有文件
 * @param info
 * @return Napi::Promise
 */
Napi::Promise listAllEntriesFromFile(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
                              [&info](resolveFunc resolve, rejectFunc reject)
                              {
                                  if (info.Length() < 1)
                                  {
                                      reject("Wrong arguments length");
                                      return;
                                  }
                                  if (!info[0].IsString())
                                  {
                                      reject("params is wrong");
                                      return;
                                  }
                                  std::string file = info[0].As<String>();

                                  std::function<void(const std::string)> fun = [resolve, reject](const std::string file)
                                  {
                                      zip_t *zip = zip_open(file.c_str(), 0, 'r');
                                      if (!zip)
                                      {
                                          reject("create zip object fail:check input");
                                          return;
                                      }
                                      std::string str = getAllEntries(zip);
                                      zip_close(zip);
                                      if (str.compare("") == 0)
                                      {
                                          reject("get entry fail:check input");
                                          return;
                                      }
                                      resolve(str);
                                  };
                                  std::thread task1(fun, file);
                                  task1.detach();
                              });
}
/**
 * 压缩文件
 * @param info
 * @return Napi::Promise
 */
Napi::Promise zipBuffer(const CallbackInfo &info)
{
    return promiseFuncWrapper(info.Env(),
                              [&info](resolveFunc resolve, rejectFunc reject)
                              {
                                  if (info.Length() < 1)
                                  {
                                      reject("Wrong arguments length");
                                      return;
                                  }
                                  if (!info[0].IsObject())
                                  {
                                      reject("params is wrong");
                                      return;
                                  }
                                  // 检查参数
                                  Napi::Object zipObj = info[0].As<Object>();
                                  Napi::Array keys = zipObj.GetPropertyNames();
                                  for (uint32_t i = 0; i < keys.Length(); i++)
                                  {
                                      Napi::Value ele = keys[i];
                                      if (ele.IsString())
                                      {
                                          std::string key = ele.As<Napi::String>();
                                          Napi::Value valueEle = zipObj.Get(key);
                                          if (!valueEle.IsTypedArray())
                                          {
                                              reject("param " + key + " is not buffer");
                                              return;
                                          }
                                      }
                                  }

                                  std::function<void(const std::string)> fun = [resolve, reject](const std::string file)
                                  {
                                      resolve("ok");
                                  };
                                  std::thread task1(fun, "ok");
                                  task1.detach();
                              });
}
/**
 * 压缩文件
 * @param info
 * @return Napi::Promise
 */
Napi::Promise zipBuffer2(const CallbackInfo &info)
{
    Napi::Env env = info.Env();
    std::shared_ptr<ZipAddon::tfsnContext> context = std::make_shared<ZipAddon::tfsnContext>(env);
    if (info.Length() < 1)
    {
        context->deffered.Reject(Napi::Error::New(env, "params length wrong").Value());
        return context->deffered.Promise();
    }
    if (!info[0].IsObject())
    {
        context->deffered.Reject(Napi::Error::New(env, Napi::String::New(env, "params is wrong")).Value());
        return context->deffered.Promise();
    }
    // 检查参数
    Napi::Object zipObj = info[0].As<Object>();
    Napi::Array keys = zipObj.GetPropertyNames();

    size_t totalContent = 0;
    for (uint32_t i = 0; i < keys.Length(); i++)
    {
        Napi::Value ele = keys[i];
        if (ele.IsString())
        {
            std::string key = ele.As<Napi::String>().Utf8Value();
            Napi::Value valueEle = zipObj.Get(key);
            if (!valueEle.IsTypedArray())
            {
                context->deffered.Reject(Napi::Error::New(env, Napi::String::New(env, "param " + key + " is not buffer")).Value());
                return context->deffered.Promise();
            }
            Buffer<uint8_t> content = valueEle.As<Buffer<uint8_t> >();
            size_t contextSize = content.ElementLength();
            totalContent += contextSize;
            context->inputData.push_back({key, reinterpret_cast<const char *>(content.Data()), contextSize});
        }
    }
    context->data.size = totalContent;
    context->tsfn = ThreadSafeFunction::New(
        env,
        Napi::Function::New(env, [](const Napi::CallbackInfo &info) {}),
        "zipBuffer2",              // Name
        0,                         // Unlimited queue
        1,                         // Only one thread will use this initially
        [context](Napi::Env env) { // Finalizer used to clean threads up
            if (context->resolve)
            {
                context->deffered.Resolve(
                    Napi::ArrayBuffer::New(env, context->data.data, context->data.size));
            }
            else
            {
                context->deffered.Reject(Napi::Error::New(env, context->data.data).Value());
            }
        });

    // 子线程里面不能使用 Napi相关对象 https://github.com/nodejs/node-addon-api/blob/main/doc/threadsafe.md
    std::function<void()> fun = [context]()
    {
        // const char* key = context -> inputData.begin() -> name.c_str();
        // printf("keys[0]: %s,  content:%s\n", key, context -> inputData.begin() -> content);

        context->data.data = new char[context->data.size];
        struct zip_t *zip = zip_stream_open(NULL, 0, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
        char *outbuf = NULL;
        size_t outbufsize = 0;
        for (ZipAddon::input_data ele : context->inputData)
        {
            // printf("process file:%s, size:%zu \n", ele.name.c_str(), ele.size);
            {
                zip_entry_open(zip, ele.name.c_str());
                {
                    zip_entry_write(zip, ele.content, ele.size);
                }
                zip_entry_close(zip);
            }
        }
        /* copy compressed stream into outbuf */
        zip_stream_copy(zip, (void **)&outbuf, &outbufsize);
        zip_stream_close(zip);
        memcpy(context->data.data, outbuf, outbufsize);
        free(outbuf);

        if (!context->data.data)
        {
            context->resolve = false;
        }
        else
        {
            context->resolve = true;
        }
        context->called = true;
        context->tsfn.Release();
    };
    std::thread task(fun);
    task.detach();

    return context->deffered.Promise();
}

std::string ZipAddon::getAllEntries(zip_t *zip)
{
    std::string str = "[";
    int i, n = zip_entries_total(zip);
    if (n == -1)
    {
        return "";
    }
    for (i = 0; i < n; ++i)
    {
        zip_entry_openbyindex(zip, i);
        {
            const char *name = zip_entry_name(zip);
            str.append("\"");
            str.append(name);
            if (i == n - 1)
            {
                str.append("\"");
            }
            else
            {
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
    exports.Set(String::New(env, "listAllEntriesFromBuffer"), Function::New(env, listAllEntriesFromBuffer));
    exports.Set(String::New(env, "listAllEntriesFromFile"), Function::New(env, listAllEntriesFromFile));
    exports.Set(String::New(env, "zipBuffer"), Function::New(env, zipBuffer2));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)