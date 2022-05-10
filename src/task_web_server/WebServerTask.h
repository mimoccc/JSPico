#ifndef JS_PICO_WEB_SERVER_H_1
#define JS_PICO_WEB_SERVER_H_1

#include "Task.h"

class WebServerTask : public Task {
public:
    explicit WebServerTask();

    ~WebServerTask();

    void init() override;

    void proc() override;
};

#endif //JS_PICO_WEB_SERVER_H_1