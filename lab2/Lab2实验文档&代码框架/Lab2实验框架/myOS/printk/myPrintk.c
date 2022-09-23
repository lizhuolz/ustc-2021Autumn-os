#include "vga.h"
#include "uart.h"
#include "vsprintf.h"

/* 内核 print 函数
 * 调用已经完成的 vga 和 串口 输出接口，补全此函数
 * 禁止修改此函数接口 */
char kBuf[400];
int myPrintk(int color, const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(kBuf, format, args);
    append2screen(kBuf, color);
    uart_put_chars(kBuf);   //串口输出
    va_end(args);
    
    /* todo */
    
    return cnt;
}

/* 用户 print 函数
 * 调用已经完成的 vga 和 串口 输出接口，补全此函数
 * 禁止修改此函数接口 */
char uBuf[400];
int myPrintf(int color, const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(uBuf, format, args);

    append2screen(uBuf, color);
    uart_put_chars(uBuf);   //串口输出
    va_end(args);

    /* todo */
    
    return cnt;
}
