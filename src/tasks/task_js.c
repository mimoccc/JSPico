//-----------------------------------------------------------------------------
// Javascript handling task
//-----------------------------------------------------------------------------

#include <pico/stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "board.h"
#include "log/log.h"
//#include "boot/boot.h"
//#include "tusb.h"
//#include "msc/msc_disk.h"
#include "hw/led.h"
#include "js.h"
#include "fatfs/ff.h"
#include "js_modules.h"
#include "main.h"

//-----------------------------------------------------------------------------

// loop method
char *JS_INIT_METHOD = "main_init();";
char *JS_LOOP_METHOD = "main_loop();";

//-----------------------------------------------------------------------------

extern struct js *js;
extern char *js_buff;

//-----------------------------------------------------------------------------

// Initialize js environment
void init_js(int js_buff_size) {
    ilog("Initializing javascript model with %d B size.", js_buff_size);
    // alloc memory
    js_buff = malloc(js_buff_size);
    // initialize javascript interpreter
    js = js_create(js_buff, js_buff_size);
}

//-----------------------------------------------------------------------------

// Prepare all modules
// Create interfaces to javascript
void prepare_js_modules() {
    // create global interface instance
    jsval_t global = js_glob(js);
    // get modules count
    int modules_cnt = sizeof(JS_MODULES) / sizeof(JS_MODULE *);
    ilog("Traversing modules, modules: %d.", modules_cnt);
    // for all modules
    for (int i = 0; i < modules_cnt; i++) {
        JS_MODULE *jsm = JS_MODULES[i];
        slog("Attaching module: %s.", jsm->name);
        // if no mount point defined, use global
        if (jsm->mount_point == NULL) {
            jsval_t mp;
            // if mount point is no defined
            if (jsm->name == NULL) {
                // use global mount point
                mp = global;
            } else {
                // create new mount point
                mp = js_mkobj(js);
                js_set(js, global, jsm->name, mp);
            }
            // create functions interface
            int fnc_count = jsm->fnc_count;
            ilog("Module have %d functions.", fnc_count);
            for (int j = 0; j < fnc_count; j++) {
                uintptr_t fn_pointer = jsm->functions[j].fn_pointer;
                char *fn_name = jsm->functions[j].name;
                char *fn_params = jsm->functions[j].params;
                char fnc_code[255] = {};
                // write to log, for debugging
                if (jsm->name == NULL) {
                    sprintf(fnc_code, "%s(%s)", jsm->functions[j].name, jsm->functions[j].params);
                } else {
                    sprintf(fnc_code, "%s.%s(%s)", jsm->name, jsm->functions[j].name, jsm->functions[j].params);
                }
                slog("Creating function: %s.", fnc_code);
                // install it
                js_set(js, mp, fn_name, js_import(js, fn_pointer, fn_params));
            }
        } else {
            // todo create mount point
        }
    }
}

//-----------------------------------------------------------------------------

// Load js file
// todo: move to js.c, and made id withouth memry allocation, sekventially
jsval_t load_js_file(char *js_file_name) {
    char *js_code;
    // file result
    FRESULT f_res;
    // file handle
    FIL f;
    // javascript result
    jsval_t js_res;
    // file read result
    UINT bt_read;
    slog("Loading js file: %s.", js_file_name);
    // open js file
    f_res = f_open(&f, js_file_name, FA_READ);
    // if file open ok
    if (f_res == FR_OK) {
        // get size of file
        int js_size = f_size(&f);
        ilog("Javascript file size: %d. allocating memory.", js_size);
        // allocate buffer
        js_code = malloc(js_size);
        // read file content
        tlog("Reading file content.");
        f_res = f_read(&f, js_code, js_size, &bt_read);
        // if read ok
        if ((f_res == FR_OK) && (bt_read == js_size)) {
            ilog("Reading file content [%d B] - ok.", bt_read);
            //tlog("Code:");
            //tlogb(js_code, js_size);
            tlog("Starting code evaluation.");
            // run code
            js_res = js_eval_buff(js, js_code, js_size);
            // if ok, initialize
            if (js_res != T_ERR) {
                tlog("Code evaluation - ok.");
                slog("Starting method %s.", JS_INIT_METHOD);
                js_res = js_eval(js, JS_INIT_METHOD);
                if (js_res == T_ERR) {
                    slog("Starting method %s - error.", JS_INIT_METHOD);
                } else {
                    slog("Starting method %s - ok.", JS_INIT_METHOD);
                }
            } else {
                tlog("Code evaluation - err.");
            }
        } else {
            tlog("Reading file content - err.");
            // error
            js_res = T_ERR;
        }
        // un-alloc memory
        tlog("Freeing memory.");
        free(js_code);
    } else {
        // error
        tlog("Reading file content - err.");
        js_res = T_ERR;
    }
    return js_res;
}

//-----------------------------------------------------------------------------

void log_js_error(jsval_t err_result) {
    ilog("Javascript error: %d", err_result);
}

void task_js_init() {
    jsval_t res;
    tlog("JS task init start.");
    init_js(8192);
    prepare_js_modules();
    res = load_js_file("index.js");
    // show state by led
    if (res == T_ERR) {
        log_js_error(res);
        set_led_blink_interval(BLINK_ERROR);
    } else {
        set_led_blink_interval(BLINK_NONE);
    }
    tlog("JS task initialized.");
}

//-----------------------------------------------------------------------------

void task_js_task() {
//    tlog("JS task cycle called.");
    jsval_t res = js_eval(js, JS_LOOP_METHOD);
    if (res == T_ERR) {
        slog("Method %s - error.", JS_LOOP_METHOD);
        set_led_blink_interval(BLINK_ERROR);
    } else {
        set_led_blink_interval(BLINK_NONE);
    }
}

//-----------------------------------------------------------------------------