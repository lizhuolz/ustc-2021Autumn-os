#include "io.h"
#include "myPrintk.h"
#include "uart.h"
#include "vga.h"
#include "i8253.h"
#include "i8259A.h"
#include "tick.h"
#include "wallClock.h"
//maybe bug
int strcmp(const char* src, const char* dest)
{
    while (*src && *dest && (*src == *dest))
    {
        src++;
        dest++;
    }
    return (int)(*src - *dest);
}
int strlen(const char* str)
{
    int len = 0;
    while (str[len])
        len++;
    return len;
}

int strsplt(const char* str, char(*str_clips)[8]) {
    int clip_num = 0;
    int i = 0;
    while (*str++ == ' '); 
    str--;
    while (*str) {
        if (*str == ' ') {
            while (*str++ == ' '); 
            if (*--str) { 
                str_clips[clip_num++][i] = '\0';
                i = 0;
            }
        }else
            str_clips[clip_num][i++] = *str++;
    }
    str_clips[clip_num++][i] = '\0';
    return clip_num;
}

typedef struct myCommand {
    char name[80];
    char help_content[200];
    int (*func)(int argc, char (*argv)[8]);
}myCommand; 

int func_cmd(int argc, char(*argv)[8]);
const myCommand cmd = { "cmd\0","List all command\n\0",func_cmd };
int func_help(int argc, char(*argv)[8]);
const myCommand help = { "help\0","Usage: help [command]\n\0Display info about [command]\n\0",func_help };
#define num_of_cmds 2
myCommand cmds[num_of_cmds] = { { "cmd\0","print hello world\n\0",func_cmd }, { "help\0","Usage: help [command]\nDisplay info about [command]\n\0",func_help } };

int func_cmd(int argc, char (*argv)[8]){
	//你需要填写这里
    myPrintk(0x07, "cmd: hello world pb19000064\n\0");
    return 0;
} 

int func_help(int argc, char (*argv)[8]){
	//你需要填写这里
    if (argc == 1) {
        myPrintk(0x7, "help [command] : Display the information of [command].\n\0");
        return 0;
    }

    for (int i = 0; i< num_of_cmds; i++) {
        if (!strcmp(argv[1], cmds[i].name)) {
            myPrintk(0x7, cmds[i].help_content);
            return 0;
        }
    }

    myPrintk(0x7, "Command \"%s\" not found.\n\0", argv[1]);
    return 1;
}



void solve(int argc, char (*argv)[8]) {

    if (!strlen(argv[0])) 
        return;
    for (int i = 0; i < num_of_cmds; i++) {
        if (!strcmp(argv[0], cmds[i].name)) {
            cmds[i].func(argc, argv);
            return;
        }
    }

    myPrintk(0x7, "Command \"%s\" not found.\n\0", argv[0]);
}

void startShell(void){
	//我们通过串口来实现数据的输入
	char BUF[256]; //输入缓存区
	int BUF_len=0;	//输入缓存区的长度
    
	int argc;
    char argv[8][8];

    do{
        BUF_len=0; 
        myPrintk(0x07,"Student>>\0");
        while((BUF[BUF_len]=uart_get_char())!='\r'){
            uart_put_char(BUF[BUF_len]);//将串口输入的数存入BUF数组中
            BUF_len++;  //BUF数组的长度加
        }
	BUF[BUF_len] = '\0';
        uart_put_chars(" -pseudo_terminal\0");
	uart_put_char('\n');
	
	//BUF[strlen(BUF)-1] = '\0';
	//myPrintk(0x7, BUF);
	//myPrintk(0x7, "%d",BUF[3]);
	//myPrintk(0x7, "%d",strlen(BUF));
	
	argc = strsplt(BUF, argv);
	//myPrintk(0x7, argv[0]);
        solve(argc, argv);

    }while(1);
        
}

