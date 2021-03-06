#ifndef HDCS_C_AIOREQUESTCOMPLETION_H
#define HDCS_C_AIOREQUESTCOMPLETION_H

#include "common/AioCompletion.h"
#include <condition_variable>
#include <chrono>
#include <mutex>
#include <atomic>
#include "common/Log.h"

namespace hdcs{
typedef void (*callback_t)(void* comp, void *arg);

class C_AioRequestCompletion : public AioCompletion {
private:
    callback_t complete_cb;
    void *complete_arg;
    ssize_t ret;
    std::atomic<bool> completed;
    std::condition_variable m_cond;
    std::mutex cond_lock;

public:
    C_AioRequestCompletion( void *cb_arg, callback_t cb ):complete_arg(cb_arg),complete_cb(cb){
        assert(complete_cb != NULL);
        assert(complete_arg != NULL);
        ret = -1;
        completed = false;
    }
    ~C_AioRequestCompletion(){
    }

    void release() {
      delete this;
    }
    void complete(ssize_t r){
      ret = r;
      if (complete_cb) {
        completed = true;
        m_cond.notify_all();
        complete_cb((void*)this, complete_arg);
      }
    }

    ssize_t get_return_value() {
      return ret;
    }

    void wait_for_complete() {
      std::unique_lock<std::mutex> unique_lock(cond_lock);
      m_cond.wait(unique_lock, [&]{return completed == true;});
    }

    void set_reserved_ptr(void* ptr){}
};

}// hdcs
#endif
