#include <Windows.h>
#include "tools/DirtyLogger.h"
#include "BDSMod.h"
#include "TrapdoorMod.h"


/*
 * 设置所有输出为utf8,设置支持彩色输出
 */
void initConsole() {
    system("chcp 65001");
    //enable colorful output
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    GetConsoleMode(hOutput, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; //NOLINT
    SetConsoleMode(hOutput, dwMode);
}


trapdoor::BDSMod *createBDSModInstance() {
    return new mod::TrapdoorMod();
}

//dll注入初始化
void mod_init() {
    initConsole();
    trapdoor::initLogger("trapdoor.log"); //初始化日志
    // trapdoor::setDevMode(true);
    mod::TrapdoorMod::printCopyRightInfo(); //打印日志
    auto *mod = createBDSModInstance();
    mod->asInstance<mod::TrapdoorMod>()->readConfigFile("trapdoor-config.json"); //读取配置文件
    trapdoor::initializeMod(mod);
}

void mod_exit() {}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    switch (ul_reason_for_call) {  //NOLINT
        case DLL_PROCESS_ATTACH:
            mod_init();
            break;
        case DLL_THREAD_ATTACH: //NOLINT
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            mod_exit();
            break;
    }
    return TRUE;
}
