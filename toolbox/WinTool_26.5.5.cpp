// Downloader.cpp - 简单下载器
// 编译：VS2019 新建 Windows 桌面应用程序，替换此文件内容

#include <windows.h>
#include <commctrl.h>
#include <urlmon.h>
#include <stdio.h>
#include <stdint.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// 控件 ID
#define ID_LIST_PROGRAMS       1001
#define BTN_DOWNLOAD           1002
#define BTN_DOWNLOAD_RUN       1003
#define IDC_STATUS             1004
#define IDC_TAB_MAIN           1005
#define BTN_UPDATE_SERVER      1006
#define IDC_SEARCH             1007
#define BTN_INTERRUPT          1008
#define IDC_CHKDSK_DRIVE       1009
#define IDC_CHKDSK_OUTPUT      1010
#define BTN_CHKDSK_RUN         1011
#define IDC_CHKDSK_FIX         1012
#define IDC_CHKDSK_REPAIR      1013
#define IDC_CHKDSK_FORCE       1014
#define WM_CHKDSK_APPEND       (WM_APP + 1)

// 程序信息结构
typedef struct {
    wchar_t name[64];
    wchar_t url[2048];
} ProgramInfo;
// KNOWN BUG:
// HTTPS in HTTP only website will stuck

const wchar_t* g_version = L"26.5.5";

// program list
ProgramInfo g_programs[] = {
    { L"360zip", L"https://dl.360scdn.com/360zip_setup.exe"},
    { L"360zip International", L"https://free.360totalsecurity.com/360zip/360zip_setup_1.0.0.1041.exe"},
    { L"Adobe AIR", L"https://airsdk.harman.com/assets/downloads/51.3.1.2/AdobeAIR.exe" },
    { L"AirServer 5.7.2 x64", L"https://dl.airserver.com/pc32/AirServer-5.7.2-x64.msi"},
    { L"AirServer 5.7.2 x86", L"https://dl.airserver.com/pc32/AirServer-5.7.2-x86.msi"},
    { L"Android Studio 2025.3.4.7", L"https://edgedl.me.gvt1.com/android/studio/install/2025.3.4.7/android-studio-panda4-patch1-windows.exe"},
    { L"Bilibili x64", L"https://dl.hdslb.com/mobile/fixed/bili_win/bili_win-install.exe?v=1.17.6-3&spm_id_from=333.47.b_646f776e6c6f61642d6c696e6b.9"},
    { L"Blender 5.1.1 x64", L"https://ftp.halifax.rwth-aachen.de/blender/release/Blender5.1/blender-5.1.1-windows-x64.msi"},
    { L"Clash 1.7.7 x64 (>=Win7)",L"https://github.com/clash-verge-rev/clash-verge-rev/releases/download/v1.7.7/Clash.Verge_1.7.7_x64-setup.exe"},
    { L"Clash 1.7.7 x86 (>=Win7)", L"https://github.com/clash-verge-rev/clash-verge-rev/releases/download/v1.7.7/Clash.Verge_1.7.7_x86-setup.exe"},
    { L"Clash 2.4.7 x64 (>Win7)", L"https://github.com/clash-verge-rev/clash-verge-rev/releases/download/v2.4.7/Clash.Verge_2.4.7_x64-setup.exe"},
    { L"Clash 2.4.7 x64 (Integrated Webview2, OS>Win7)", L"https://github.com/clash-verge-rev/clash-verge-rev/releases/download/v2.4.7/Clash.Verge_2.4.7_x64_fixed_webview2-setup.exe"},
    { L"Cloudflare WARP 2026.3.851.0", L"https://downloads.cloudflareclient.com/v1/download/windows/version/2026.3.851.0" },
    { L"Core Temp 1.20.0.145", L"https://www.alcpu.com/CoreTemp/Core-Temp-setup-v1.20.0.145.exe" },
    { L"Discord", L"https://stable.dl2.discordapp.net/distro/app/stable/win/x64/1.0.9236/DiscordSetup.exe"},
    { L"DiskGenius 5.5.0.1488 Pro x64", L"https://dl.naixi.net/tools/diskgenius/Diskgenius_Pro_x64_5.5.0.1488_cn.exe"},
    { L"DiskGenius 6.0.0.1631 Pro x64", L"https://dl.naixi.net/tools/diskgenius/DiskGenius_6.0.0.1631_x64.exe"},
    { L"DiskGenius 6.0.0.1631 Pro x86", L"https://dl.naixi.net/tools/diskgenius/DiskGenius_6.0.0.1631_x86.exe"},
    { L"Dism++ 10.1.1002.2", L"https://github.com/Chuyu-Team/Dism-Multi-language/releases/download/v10.1.1002.2/Dism++10.1.1002.1B.zip"},
    { L"Docker Desktop x64", L"https://desktop.docker.com/win/main/amd64/Docker%20Desktop%20Installer.exe?utm_source=docker&amp;utm_medium=webreferral&amp;utm_campaign=docs-driven-download-win-amd64"},
    { L"Epic Games Launcher 19.2.3", L"https://epicgames-download1.akamaized.net/Builds/UnrealEngineLauncher/Installers/Windows/EpicInstaller-19.2.3.exe?launcherfilename=EpicInstaller-19.2.3.exe"},
    { L"Everything 1.4.1.1032 x64", L"https://www.voidtools.com/Everything-1.4.1.1032.x64-Setup.exe"},
    { L"Everything 1.4.1.1032 x86", L"https://www.voidtools.com/Everything-1.4.1.1032.x86-Setup.exe"},
    { L"Everything Portable 1.4.1.1032 x64", L"https://www.voidtools.com/Everything-1.4.1.1032.x64.zip"},
    { L"FinalShell", L"https://dl.hostbuf.com/finalshell3/finalshell_windows_x64.exe" },
    { L"Firefox 39.0", L"https://ftp.mozilla.org/pub/firefox/releases/39.0/win32/zh-CN/Firefox%20Setup%2039.0.exe" },
    { L"Firefox 56.0", L"https://ftp.mozilla.org/pub/firefox/releases/56.0/win64/zh-CN/Firefox%20Setup%2056.0.exe" },
    { L"Firefox 60.0", L"https://ftp.mozilla.org/pub/firefox/releases/60.0/win64/zh-CN/Firefox%20Setup%2060.0.exe" },
    { L"Geek Uninstaller", L"https://geekuninstaller.com/geek.zip"},
    { L"Genshin Impact 20260302", L"https://download-porter.hoyoverse.com/download-porter/2026/03/05/GenshinImpact_install_20260302_hoyoverse.exe?trace_key=GenshinImpact_install_ua_8ea830101ad4"},
    { L"Git 2.54.0 x64", L"https://github.com/git-for-windows/git/releases/download/v2.54.0.windows.1/Git-2.54.0-64-bit.exe"},
    { L"GitHub Desktop 3.5.8 x64", L"https://desktop.githubusercontent.com/releases/3.5.8-b1d863ab/GitHubDesktopSetup-x64.exe"},
    { L"Hex Editor (HxD)", L"https://mh-nexus.de/downloads/HxDSetup.zip"},
    { L"Huorong 6.0.9.5 x64 (火绒安全软件)", L"https://down-tencent.huorong.cn/sysdiag-all-x64-6.0.9.5-2026.05.08.2.exe"},
    { L"Hyper (Terminal) 3.4.1", L"https://github.com/vercel/hyper/releases/download/v3.4.1/Hyper-Setup-3.4.1.exe"},
    { L"IDA 9.0 Pro x64", L"https://dl.naixi.net/tools/ida/9.0/idapro_90_x64win.exe" },
    { L"IDA 9.0 Pro Cracker", L"https://dl.naixi.net/tools/ida/9.0/crack_ida90_beta.py"},
    { L"Jump Desktop (Client)", L"https://mirror.jumpdesktop.com/downloads/jdwin/JumpDesktop.exe"},
    { L"Jump Desktop Connect (Server)", L"https://mirror.jumpdesktop.com/downloads/connect/JumpDesktopConnect.exe"},
    { L"Ldap Admin", L"https://dl.naixi.net/tools/ldap/LdapAdmin.exe"},
    { L"LibreOffice 26.2.3", L"https://mirror-hk.koddos.net/tdf/libreoffice/stable/26.2.3/win/x86_64/LibreOffice_26.2.3_Win_x86-64.msi"},
    { L"MuseScore 3.6.2 x64", L"https://ftp.osuosl.org/pub/musescore-nightlies/windows/3x/stable/MuseScore-3.6.2.548021803-x86_64.msi"},
    { L"MuseScore 3.6.2 x64 Linux", L"https://ftp.osuosl.org/pub/musescore-nightlies/linux/3x/stable/MuseScore-3.6.2.548021370-x86_64.AppImage"},
    { L"Node.js 22.12.0 x64", L"https://nodejs.org/dist/v22.12.0/node-v22.12.0-x64.msi"},
    { L"Notepad++ 8.9.4 x64", L"https://github.com/notepad-plus-plus/notepad-plus-plus/releases/download/v8.9.4/npp.8.9.4.Installer.x64.exe"},
    { L"Notepad++ 8.9.4 x64 Portable", L"https://github.com/notepad-plus-plus/notepad-plus-plus/releases/download/v8.9.4/npp.8.9.4.portable.x64.zip"},
    { L"Notepad++ 8.9.4 x86", L"https://github.com/notepad-plus-plus/notepad-plus-plus/releases/download/v8.9.4/npp.8.9.4.Installer.exe"},
    { L"Notepad++ 8.9.4 x86 Portable", L"https://github.com/notepad-plus-plus/notepad-plus-plus/releases/download/v8.9.4/npp.8.9.4.portable.zip"},
    { L"Nyarc x64", L"https://dl.naixi.net/temp/nyarc-windows-amd64.exe" },
    { L"OBS Studio 32.1.2 x64", L"https://cdn-fastly.obsproject.com/downloads/OBS-Studio-32.1.2-Windows-x64-Installer.exe"},
    { L"Opera GX", L"https://net.geo.opera.com/opera_gx/stable/edition/std-2?utm_source=PWNgames&utm_medium=pa&utm_campaign=PWN_HK_SVR_9481_DD_3657&utm_id=00a5388b92024ccd87528081ffa3c5bc"},
    { L"PotPlayer x64", L"https://t1.daumcdn.net/potplayer/PotPlayer/Version/Latest/PotPlayerSetup64.exe"},
    { L"PotPlayer x86", L"https://t1.daumcdn.net/potplayer/PotPlayer/Version/Latest/PotPlayerSetup.exe"},
    { L"PowerShell 7.6.1 x64", L"https://github.com/PowerShell/PowerShell/releases/download/v7.6.1/PowerShell-7.6.1-win-x64.msi"},
    { L"PowerShell 7.6.1 x86", L"https://github.com/PowerShell/PowerShell/releases/download/v7.6.1/PowerShell-7.6.1-win-x86.msi"},
    { L"PowerToys 0.99.1 x64 System", L"https://github.com/microsoft/PowerToys/releases/download/v0.99.1/PowerToysSetup-0.99.1-x64.exe"},
    { L"PowerToys 0.99.1 x64 User", L"https://github.com/microsoft/PowerToys/releases/download/v0.99.1/PowerToysUserSetup-0.99.1-x64.exe"},
    { L"PyCharm 2026.1.1", L"https://download-cdn.jetbrains.com/python/pycharm-2026.1.1.exe"},
    { L"PyCharm 2026.1.1 Arm64", L"https://download-cdn.jetbrains.com/python/pycharm-2026.1.1-aarch64.exe"},
    { L"Python 3.13.13 x64", L"https://www.python.org/ftp/python/3.13.13/python-3.13.13-amd64.exe"},
    { L"Python 3.14.4 x64", L"https://www.python.org/ftp/python/3.14.4/python-3.14.4-amd64.exe"},
    { L"QEMU 20260422 x64", L"https://qemu.weilnetz.de/w64/qemu-w64-setup-20260422.exe"},
    { L"QQ/Weixin RevokeMsg Patcher 2.1 (GitHub)", L"https://github.com/huiyadanli/RevokeMsgPatcher/releases/download/2.1/RevokeMsgPatcher.v2.1.zip"},
    { L"QQ 9.7.25.29417", L"https://dldir1v6.qq.com/qqfile/qq/PCQQ9.7.25/QQ9.7.25.29417.exe"},
    { L"QQ 9.9.29 x64", L"https://dldir1v6.qq.com/qqfile/qq/QQNT/Windows/QQ_9.9.29_260401_x64_01.exe"},
    { L"Roblox Player Client", L"https://www.roblox.com/download/client?os=win"},
    { L"Roblox Studio", L"https://setup.rbxcdn.com/RobloxStudioInstaller.exe"},
    { L"Rufus 1.1.7 with FreeDOS", L"https://github.com/pbatard/rufus/releases/download/v1.1.7/rufus-1.1.7f.exe"},
    { L"Rufus 1.4.12", L"https://github.com/pbatard/rufus/releases/download/v1.4.12/rufus-1.4.12.exe"},
    { L"Rufus 2.18 (>=WinXP)", L"https://github.com/pbatard/rufus/releases/download/v2.18/rufus-2.18.exe"},
    { L"Rufus 2.18 Portable (>=WinXP)", L"https://github.com/pbatard/rufus/releases/download/v2.18/rufus-2.18p.exe"},
    { L"Rufus 3.22 (>=Win7)", L"https://github.com/pbatard/rufus/releases/download/v3.22/rufus-3.22.exe"},
    { L"Rufus 3.22 Portable (>=Win7)", L"https://github.com/pbatard/rufus/releases/download/v3.22/rufus-3.22p.exe"},
    { L"Rufus 4.14 x64 (>Win7)", L"https://github.com/pbatard/rufus/releases/download/v4.14/rufus-4.14.exe"},
    { L"Rufus 4.14 x64 Portable (>Win7)", L"https://github.com/pbatard/rufus/releases/download/v4.14/rufus-4.14p.exe"},
    { L"Rufus 4.14 x86 (>Win7)", L"https://github.com/pbatard/rufus/releases/download/v4.14/rufus-4.14_x86.exe"},
    { L"SourceTree 3.4.30", L"https://product-downloads.atlassian.com/software/sourcetree/windows/ga/SourceTreeSetup-3.4.30.exe"},
    { L"Steam", L"https://cdn.fastly.steamstatic.com/client/installer/SteamSetup.exe"},
    { L"Sumatra PDF Reader 3.6.1 x64", L"https://files2.sumatrapdfreader.org/software/sumatrapdf/rel/3.6.1/SumatraPDF-3.6.1-64-install.exe"},
    { L"Sumatra PDF Reader 3.6.1 x86", L"https://files2.sumatrapdfreader.org/software/sumatrapdf/rel/3.6.1/SumatraPDF-3.6.1-install.exe"},
    { L"SP Flash Tool", L"https://drive.usercontent.google.com/download?id=1IJGCWb55lRd9r4EBjq2uPLTZjlfDlcYY&export=download&authuser=0&confirm=t&uuid=8ba4ca13-21b1-41c0-b13d-0761620c1d97&at=ALBwUglWRf14-LHrc0-ZcNvkO6RY%3A1778150524183" },
    { L"UniGetUI 2026.1.9.0 x64 [CDN]", L"https://cdn.devolutions.net/download/Devolutions.UniGetUI.win-x64.2026.1.9.0.exe"},
    { L"UniGetUI 2026.1.9.0 x64 [GitHub]", L"https://github.com/Devolutions/UniGetUI/releases/download/v2026.1.9/UniGetUI.Installer.x64.exe"},
    { L"UniGetUI 2026.1.9.0 x86 [GitHub]", L"https://github.com/Devolutions/UniGetUI/releases/download/v2026.1.9/UniGetUI.Installer.exe"},
    { L"Ventoy 1.1.12", L"https://github.com/ventoy/Ventoy/releases/download/v1.1.12/ventoy-1.1.12-windows.zip"},
    { L"Ventoy 1.1.12 LiveCD ISO", L"https://github.com/ventoy/Ventoy/releases/download/v1.1.12/ventoy-1.1.12-livecd.iso"},
    { L"Visual Studio 2017 Community", L"https://aka.ms/vs/15/release/vs_community.exe"},
    { L"Visual Studio 2017 Enterprise", L"https://aka.ms/vs/15/release/vs_enterprise.exe"},
    { L"Visual Studio 2017 Professional", L"https://aka.ms/vs/15/release/vs_professional.exe"},
    { L"Visual Studio 2017 WD Express", L"https://aka.ms/vs/15/release/vs_WDExpress.exe"},
    { L"Visual Studio 2019 Community", L"https://aka.ms/vs/16/release/vs_community.exe"},
    { L"Visual Studio 2019 Enterprise", L"https://aka.ms/vs/16/release/vs_enterprise.exe"},
    { L"Visual Studio 2019 Professional", L"https://aka.ms/vs/16/release/vs_professional.exe"},
    { L"Visual Studio 2022 Community", L"https://aka.ms/vs/17/release/vs_community.exe"},
    { L"Visual Studio 2022 Enterprise", L"https://aka.ms/vs/17/release/vs_enterprise.exe"},
    { L"Visual Studio 2022 Professional", L"https://aka.ms/vs/17/release/vs_professional.exe"},
    { L"Visual Studio 2026 Community", L"https://aka.ms/vs/18/Stable/vs_community.exe"},
    { L"Visual Studio 2026 Enterprise", L"https://aka.ms/vs/18/Stable/vs_enterprise.exe"},
    { L"Visual Studio 2026 Professional", L"https://aka.ms/vs/18/Stable/vs_professional.exe"},
    { L"Visual Studio Code 1.117.0 System", L"https://vscode.download.prss.microsoft.com/dbazure/download/stable/10c8e557c8b9f9ed0a87f61f1c9a44bde731c409/VSCodeSetup-x64-1.117.0.exe"},
    { L"Visual Studio Code 1.117.0 User", L"https://vscode.download.prss.microsoft.com/dbazure/download/stable/10c8e557c8b9f9ed0a87f61f1c9a44bde731c409/VSCodeUserSetup-x64-1.117.0.exe"},
    { L"VLC Media Player 3.0.23 x86 (Hong Kong Mirror)", L"https://mirror-hk.koddos.net/videolan/vlc/3.0.23/win32/vlc-3.0.23-win32.exe"},
    { L"Weixin 2.6.2.31", L"https://web.archive.org/web/20160303054217if_/http://dldir1.qq.com/weixin/Windows/WeChatSetup.exe"},
    { L"VMware Workstation 10.0.7-2844087", L"https://dn710001.ca.archive.org/0/items/vmwareworkstationarchive/10.x/VMware-workstation-full-10.0.7-2844087.exe"},
    { L"VMware Workstation 14.1.8-14921873", L"https://dn710001.ca.archive.org/0/items/vmwareworkstationarchive/14.x/VMware-workstation-full-14.1.8-14921873.exe"},
    { L"Windows Desktop Runtime 3.1.10 x64", L"https://download.visualstudio.microsoft.com/download/pr/513acf37-8da2-497d-bdaa-84d6e33c1fee/eb7b010350df712c752f4ec4b615f89d/windowsdesktop-runtime-3.1.10-win-x64.exe" },
    { L"Windows Vista explorer.exe x64", L"https://www.upload.ee/download/564972/1863f7ca6625227c4c47/explorer.exe" },
    { L"WinRAR 722 x64, Chinese Simp.", L"https://www.win-rar.com/fileadmin/winrar-versions/winrar/winrar-x64-722sc.exe"},
    { L"WinRAR 722 x64, Chinese Trad.", L"https://www.win-rar.com/fileadmin/winrar-versions/winrar-x64-722tc.exe"},
    { L"WinRAR 722 x64, English", L"https://www.win-rar.com/fileadmin/winrar-versions/winrar/winrar-x64-722.exe"},
    { L"Wireshark 4.6.5 x64", L"https://2.na.dl.wireshark.org/win64/Wireshark-4.6.5-x64.exe"},
    { L"Xmanager PowerSuite 8.0.0001r", L"https://dl.naixi.net/tools/NetSarang/XmanagerPowerSuite-8.0.0001r.exe"},
    { L"Debug: The 16383 GB File", L"http://45.126.125.60/public/speedtest16t.bin"},
    { L"Debug: Something that doesn't exist", L"http://45.126.125.60/public/imnotexist"},
    { L"Debug: Call the stupid function", L"Call the stupid function lol"},
    { L"Debug: Invalid pointer", L"fucking stupid invalid pointer"}
    // 可以继续添加更多程序
    // { L"另一个程序", L"https://example.com/setup.exe" },
};
int g_programCount = sizeof(g_programs) / sizeof(g_programs[0]);

// 全局变量
HINSTANCE g_hInst;
HWND g_hTab;
HWND g_hList;
HWND g_hStatus;
HWND g_hDownloadBtn;
HWND g_hDownloadRunBtn;
HWND g_hProgress; // i added a progress bar
HWND g_hStats;       // New handle for the ETA/Speed line
HWND g_hUpdateBtn;
HWND g_hFooterCopy;
HWND g_hFooterLeft;  // Fixed version label (left ~10%)
HWND g_hFooterInfo;  // Status text (right ~90%)
HWND g_hSearch;      // Search bar
HWND g_hInterruptBtn; // Interrupt download button
HWND g_hChkDskDriveLabel;
HWND g_hChkDskDrive;
HWND g_hChkDskFix;
HWND g_hChkDskRepair;
HWND g_hChkDskForce;
HWND g_hChkDskRunBtn;
HWND g_hChkDskOutput;
volatile BOOL g_bChkDskRunning = FALSE;
int  g_filteredIndices[512]; // maps listbox pos -> g_programs index
volatile LONG g_activeDownloads = 0; // active download thread count
volatile BOOL g_bInterruptDownload = FALSE; // set to TRUE to abort
HBRUSH g_hWhiteBrush = NULL; // white background brush for static controls


wchar_t g_downloadPath[MAX_PATH];

// 前向声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddProgramsToList(HWND hList);
void FilterProgramList(HWND hList, const wchar_t* query);
int GetSelectedProgramIndex(HWND hList);
void DownloadFile(HWND hWnd, int programIndex, BOOL runAfterDownload);
DWORD WINAPI DownloadThread(LPVOID lpParam);
int GetAllSelectedIndices(HWND hList, int* indices, int maxCount);
void SwitchTab(int tabIndex);
DWORD WINAPI ChkDskThread(LPVOID lpParam);

// 线程参数结构
typedef struct {
    HWND hWnd;
    int programIndex;
    BOOL runAfter;
} ThreadParam;

typedef struct {
    HWND hWnd;
    wchar_t drive[8];
    BOOL doFix;
    BOOL doRepair;
    BOOL doForce;
} ChkDskParam;

// ==================== 入口函数 ====================
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化公共控件
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS | ICC_TAB_CLASSES;
    InitCommonControlsEx(&icex);

    // 注册窗口类
    WNDCLASSEX wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"DownloaderClass";
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return FALSE;

    // 创建窗口
    HWND hWnd = CreateWindowEx(0, L"DownloaderClass", L"Windows Toolbox",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 517,
        NULL, NULL, hInstance, NULL);

    if (!hWnd)
        return FALSE;

    g_hInst = hInstance;
    g_hWhiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// ==================== 窗口过程 ====================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        HWND hLabel;
        TCITEMW tie;

        g_hTab = CreateWindowW(WC_TABCONTROLW, L"",
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
            10, 10, 480, 362, hWnd, (HMENU)IDC_TAB_MAIN, g_hInst, NULL);

        tie.mask = TCIF_TEXT;
        tie.pszText = const_cast<LPWSTR>(L"Download");
        TabCtrl_InsertItem(g_hTab, 0, &tie);
        tie.pszText = const_cast<LPWSTR>(L"Settings");
        TabCtrl_InsertItem(g_hTab, 1, &tie);
        tie.pszText = const_cast<LPWSTR>(L"ChkDsk");
        TabCtrl_InsertItem(g_hTab, 2, &tie);

        hLabel = CreateWindowW(L"STATIC", L"Select to download.",
            WS_CHILD | WS_VISIBLE,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hSearch = CreateWindowW(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            24, 66, 452, 22, hWnd, (HMENU)IDC_SEARCH, g_hInst, NULL);
        SendMessageW(g_hSearch, EM_SETCUEBANNER, TRUE, (LPARAM)L"Search programs...");

        g_hList = CreateWindowW(L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_EXTENDEDSEL | LBS_NOTIFY,
            24, 90, 452, 211, hWnd, (HMENU)ID_LIST_PROGRAMS, g_hInst, NULL);
        AddProgramsToList(g_hList);
        SendMessage(g_hList, LB_SETCURSEL, 0, 0);

        g_hProgress = CreateWindowEx(0, PROGRESS_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
            24, 308, 452, 18, hWnd, (HMENU)IDC_STATUS, g_hInst, NULL);

        g_hDownloadBtn = CreateWindowW(L"BUTTON", L"Download",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            24, 333, 130, 28, hWnd, (HMENU)BTN_DOWNLOAD, g_hInst, NULL);

        g_hDownloadRunBtn = CreateWindowW(L"BUTTON", L"Run",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            158, 333, 155, 28, hWnd, (HMENU)BTN_DOWNLOAD_RUN, g_hInst, NULL);

        g_hInterruptBtn = CreateWindowW(L"BUTTON", L"Interrupt",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            317, 333, 159, 28, hWnd, (HMENU)BTN_INTERRUPT, g_hInst, NULL);
        EnableWindow(g_hInterruptBtn, FALSE);

        g_hUpdateBtn = CreateWindowW(L"BUTTON", L"Update from server",
            WS_CHILD | BS_PUSHBUTTON,
            24, 66, 140, 28, hWnd, (HMENU)BTN_UPDATE_SERVER, g_hInst, NULL);

        // ── ChkDsk tab controls ────────────────────────────────────────────
        g_hChkDskDriveLabel = CreateWindowW(L"STATIC", L"Drive:",
            WS_CHILD, 24, 45, 44, 20, hWnd, NULL, g_hInst, NULL);

        g_hChkDskDrive = CreateWindowW(L"COMBOBOX", NULL,
            WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            70, 42, 110, 150, hWnd, (HMENU)IDC_CHKDSK_DRIVE, g_hInst, NULL);

        g_hChkDskFix = CreateWindowW(L"BUTTON", L"/F  Fix errors",
            WS_CHILD | BS_AUTOCHECKBOX,
            190, 44, 120, 20, hWnd, (HMENU)IDC_CHKDSK_FIX, g_hInst, NULL);

        g_hChkDskRepair = CreateWindowW(L"BUTTON", L"/R  Locate bad sectors",
            WS_CHILD | BS_AUTOCHECKBOX,
            190, 68, 155, 20, hWnd, (HMENU)IDC_CHKDSK_REPAIR, g_hInst, NULL);

        g_hChkDskForce = CreateWindowW(L"BUTTON", L"/X  Force dismount",
            WS_CHILD | BS_AUTOCHECKBOX,
            350, 44, 140, 20, hWnd, (HMENU)IDC_CHKDSK_FORCE, g_hInst, NULL);

        g_hChkDskRunBtn = CreateWindowW(L"BUTTON", L"Run ChkDsk",
            WS_CHILD | BS_PUSHBUTTON,
            352, 63, 138, 28, hWnd, (HMENU)BTN_CHKDSK_RUN, g_hInst, NULL);

        g_hChkDskOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
            24, 98, 452, 255, hWnd, (HMENU)IDC_CHKDSK_OUTPUT, g_hInst, NULL);

        // Populate logical drives into combo
        {
            DWORD drivesMask = GetLogicalDrives();
            for (int d = 0; d < 26; d++) {
                if (drivesMask & (1 << d)) {
                    wchar_t drv[4] = { (wchar_t)(L'A' + d), L':', L'\0' };
                    SendMessageW(g_hChkDskDrive, CB_ADDSTRING, 0, (LPARAM)drv);
                }
            }
            SendMessage(g_hChkDskDrive, CB_SETCURSEL, 0, 0);
        }
        // ──────────────────────────────────────────────────────────────────

        g_hFooterCopy = CreateWindowW(L"STATIC",
            L"Copyleft 2026-2099, all rights not reserved.",
            WS_CHILD | WS_VISIBLE,
            10, 402, 480, 18, hWnd, NULL, g_hInst, NULL);

        CreateWindowW(L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_ETCHEDHORZ,
            10, 425, 480, 8, hWnd, NULL, g_hInst, NULL);

        wchar_t initVersion[32];
        wsprintfW(initVersion, L"Toolbox v%s", g_version);
        g_hFooterLeft = CreateWindowW(L"STATIC", initVersion,
            WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER,
            10, 437, 120, 18, hWnd, NULL, g_hInst, NULL);

        g_hFooterInfo = CreateWindowW(L"STATIC", L"Ready",
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            132, 437, 358, 18, hWnd, NULL, g_hInst, NULL);

        g_hStatus = g_hFooterInfo;
        g_hStats = g_hFooterInfo;

        // self added: setup font
        HFONT hFont = CreateFont(
            -12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");

        // Set font for controls
        SendMessage(g_hTab, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hSearch, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hDownloadBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hDownloadRunBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hInterruptBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hUpdateBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskDriveLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskDrive, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskFix, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskRepair, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskForce, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskRunBtn, WM_SETFONT, (WPARAM)hFont, TRUE);

        // Force SimSun (宋体) with raster-oriented settings for ChkDsk output
        HFONT hMonoFont = CreateFontW(
            -16, 0, 0, 0,
            FW_NORMAL,
            FALSE, FALSE, FALSE,
            GB2312_CHARSET,
            OUT_RASTER_PRECIS,
            CLIP_DEFAULT_PRECIS,
            NONANTIALIASED_QUALITY,
            FIXED_PITCH | FF_MODERN,
            L"宋体");
        SendMessage(g_hChkDskOutput, WM_SETFONT, (WPARAM)hMonoFont, TRUE);
        SendMessage(g_hFooterCopy, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hFooterLeft, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hFooterInfo, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hList, WM_SETFONT, (WPARAM)hFont, TRUE);

        // reset the progress bar to 0%
        SendMessage(g_hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

        SwitchTab(0);


        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        if (wmId == IDC_SEARCH && HIWORD(wParam) == EN_CHANGE)
        {
            wchar_t query[128] = {};
            GetWindowTextW(g_hSearch, query, 127);
            FilterProgramList(g_hList, query);
            return 0;
        }

        if (wmId == BTN_INTERRUPT)
        {
            g_bInterruptDownload = TRUE;
            EnableWindow(g_hInterruptBtn, FALSE);
            SetWindowTextW(g_hStatus, L"Interrupting...");
            return 0;
        }

        if (wmId == BTN_UPDATE_SERVER)
        {
            MessageBoxW(hWnd, L"Settings tab is empty for now.", L"Settings", MB_OK);
            return 0;
        }

        if (wmId == BTN_CHKDSK_RUN)
        {
            if (g_bChkDskRunning) return 0;
            wchar_t drive[8] = {};
            GetWindowTextW(g_hChkDskDrive, drive, 7);
            if (drive[0] == L'\0') {
                MessageBoxW(hWnd, L"Select a drive first.", L"ChkDsk", MB_OK);
                return 0;
            }
            BOOL doFix = (SendMessage(g_hChkDskFix, BM_GETCHECK, 0, 0) == BST_CHECKED);
            BOOL doRepair = (SendMessage(g_hChkDskRepair, BM_GETCHECK, 0, 0) == BST_CHECKED);
            BOOL doForce = (SendMessage(g_hChkDskForce, BM_GETCHECK, 0, 0) == BST_CHECKED);
            if ((doFix || doRepair || doForce) &&
                MessageBoxW(hWnd,
                    L"Fix / Repair / Force dismount options require administrator privileges and may lock the volume.\r\n\r\nProceed?",
                    L"ChkDsk Warning", MB_YESNO | MB_ICONWARNING) != IDYES)
                return 0;
            ChkDskParam* p = (ChkDskParam*)malloc(sizeof(ChkDskParam));
            if (!p) return 0;
            p->hWnd = hWnd;
            wcscpy_s(p->drive, drive);
            p->doFix = doFix;
            p->doRepair = doRepair;
            p->doForce = doForce;
            g_bChkDskRunning = TRUE;
            EnableWindow(g_hChkDskRunBtn, FALSE);
            SetWindowTextW(g_hChkDskOutput, L"");
            CreateThread(NULL, 1024 * 1024, ChkDskThread, p, 0, NULL);
            return 0;
        }

        if (wmId == BTN_DOWNLOAD || wmId == BTN_DOWNLOAD_RUN)
        {
            BOOL runAfter = (wmId == BTN_DOWNLOAD_RUN);
            int indices[64];
            int count = GetAllSelectedIndices(g_hList, indices, 64);
            if (count > 0)
                for (int i = 0; i < count; i++)
                    DownloadFile(hWnd, indices[i], runAfter);
            else
                MessageBoxW(hWnd, L"Please select.", L"Hint", MB_OK);
            return 0;
        }
        break;
    }

    case WM_NOTIFY:
    {
        LPNMHDR hdr = (LPNMHDR)lParam;
        if (hdr->hwndFrom == g_hTab && hdr->code == TCN_SELCHANGE)
        {
            SwitchTab(TabCtrl_GetCurSel(g_hTab));
            return 0;
        }
        break;
    }

    case WM_CTLCOLORSTATIC:
        SetBkColor((HDC)wParam, RGB(255, 255, 255));
        return (LRESULT)g_hWhiteBrush;

    case WM_CHKDSK_APPEND:
    {
        wchar_t* text = (wchar_t*)lParam;
        if (text) {
            int len = GetWindowTextLengthW(g_hChkDskOutput);
            SendMessage(g_hChkDskOutput, EM_SETSEL, (WPARAM)len, (LPARAM)len);
            SendMessage(g_hChkDskOutput, EM_REPLACESEL, FALSE, (LPARAM)text);
            SendMessage(g_hChkDskOutput, EM_SCROLLCARET, 0, 0);

            // While on ChkDsk tab, mirror the latest output line to footer status.
            if (TabCtrl_GetCurSel(g_hTab) == 2) {
                int lineCount = (int)SendMessageW(g_hChkDskOutput, EM_GETLINECOUNT, 0, 0);
                for (int line = lineCount - 1; line >= 0; --line) {
                    wchar_t lineBuf[256] = {};
                    *((WORD*)lineBuf) = 255; // EM_GETLINE: max wchar count in first WORD
                    int copied = (int)SendMessageW(g_hChkDskOutput, EM_GETLINE, (WPARAM)line, (LPARAM)lineBuf);
                    if (copied > 0) {
                        lineBuf[copied] = L'\0';
                        SetWindowTextW(g_hFooterInfo, lineBuf);
                        break;
                    }
                }
            }

            free(text);
        }
        return 0;
    }

    case WM_DESTROY:
        DeleteObject(g_hWhiteBrush);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ==================== 辅助函数 ====================
void AddProgramsToList(HWND hList)
{
    for (int i = 0; i < g_programCount; i++)
    {
        g_filteredIndices[i] = i;
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)g_programs[i].name);
    }
}

// Filter the list by query (case-insensitive substring match on name)
void FilterProgramList(HWND hList, const wchar_t* query)
{
    SendMessage(hList, WM_SETREDRAW, FALSE, 0);
    SendMessage(hList, LB_RESETCONTENT, 0, 0);

    int filteredCount = 0;
    wchar_t lowerQuery[128];
    wcsncpy_s(lowerQuery, query, 127);
    CharLowerW(lowerQuery);

    for (int i = 0; i < g_programCount; i++)
    {
        wchar_t lowerName[64];
        wcsncpy_s(lowerName, g_programs[i].name, 63);
        CharLowerW(lowerName);

        if (lowerQuery[0] == L'\0' || wcsstr(lowerName, lowerQuery) != NULL)
        {
            g_filteredIndices[filteredCount++] = i;
            SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)g_programs[i].name);
        }
    }

    SendMessage(hList, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hList, NULL, TRUE);
}

int GetSelectedProgramIndex(HWND hList)
{
    int index = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
    return index;
}

int GetAllSelectedIndices(HWND hList, int* indices, int maxCount)
{
    int count = (int)SendMessage(hList, LB_GETSELCOUNT, 0, 0);
    if (count <= 0) return 0;
    if (count > maxCount) count = maxCount;
    int listboxIndices[64];
    SendMessage(hList, LB_GETSELITEMS, count, (LPARAM)listboxIndices);
    // Map filtered listbox positions back to g_programs indices
    for (int i = 0; i < count; i++)
        indices[i] = g_filteredIndices[listboxIndices[i]];
    return count;
}

void SwitchTab(int tabIndex)
{
    BOOL showDownload = (tabIndex == 0);
    BOOL showSettings = (tabIndex == 1);
    BOOL showChkDsk = (tabIndex == 2);

    ShowWindow(g_hSearch, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hList, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hProgress, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hDownloadBtn, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hDownloadRunBtn, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hInterruptBtn, showDownload ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hUpdateBtn, showSettings ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hChkDskDriveLabel, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskDrive, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskFix, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskRepair, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskForce, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskRunBtn, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskOutput, showChkDsk ? SW_SHOW : SW_HIDE);

    if (showDownload)
        SetWindowTextW(g_hFooterInfo, L"Ready");
    else if (showSettings)
        SetWindowTextW(g_hFooterInfo, L"Settings");
    else
        SetWindowTextW(g_hFooterInfo, L"ChkDsk");
}

// 获取临时下载路径
void GetTempDownloadPath(const wchar_t* filename, wchar_t* outPath, int outSize)
{
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);

    // 从 URL 中提取文件名，如果没有就用默认
    const wchar_t* lastSlash = wcsrchr(filename, L'/');
    if (lastSlash && *(lastSlash + 1))
        wsprintfW(outPath, L"%s%s", tempPath, lastSlash + 1);
    else
        wsprintfW(outPath, L"%sdownload.exe", tempPath);
}

// ima handle the progress correctly before DownloadThread
class CDownloadProgress : public IBindStatusCallback {
public:
    HWND m_hProgress;
    HWND m_hStats;
    DWORD m_startTime;
    DWORD m_lastSpeedTick;
    ULONG m_lastProgress;    // last raw downloaded amount (32-bit callback value)
    ULONG m_lastProgressMax; // last raw total amount (32-bit callback value)
    uint64_t m_progressBase; // high part for reconstructed downloaded bytes
    uint64_t m_totalBase;    // high part for reconstructed total bytes
    uint64_t m_progress64;   // reconstructed downloaded bytes
    uint64_t m_total64;      // reconstructed total bytes
    uint64_t m_lastSpeedProgress64;
    double m_lastSpeedBps;
    bool  m_forcedMarquee;   // true once we detect an increasing total size

    CDownloadProgress(HWND hProg, HWND hStats)
        : m_hProgress(hProg), m_hStats(hStats), m_startTime(GetTickCount()),
        m_lastSpeedTick(m_startTime),
        m_lastProgress(0), m_lastProgressMax(0),
        m_progressBase(0), m_totalBase(0), m_progress64(0), m_total64(0),
        m_lastSpeedProgress64(0), m_lastSpeedBps(0.0),
        m_forcedMarquee(false) {}

    // The most important method: called by Windows as data arrives
    STDMETHOD(OnProgress)(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText) {
        if (g_bInterruptDownload)
            return E_ABORT;

        ULONG prevProgress = m_lastProgress;
        ULONG prevProgressMax = m_lastProgressMax;

        // Reconstruct monotonic 64-bit counters from 32-bit callback values.
        // Only treat a decrease as wrap-around when it looks like a real 32-bit rollover.
        if (ulProgress < prevProgress) {
            ULONG drop = prevProgress - ulProgress;
            if (prevProgress > 0xF0000000UL && ulProgress < 0x0FFFFFFFUL && drop > 0x70000000UL) {
                m_progressBase += (1ULL << 32);
            }
            else {
                // Non-wrap regression (redirect/retry/internal reset): restart reconstruction.
                m_progressBase = 0;
            }
        }
        m_progress64 = m_progressBase + (uint64_t)ulProgress;

        if (ulProgressMax > 0) {
            if (prevProgressMax > 0 && ulProgressMax < prevProgressMax) {
                ULONG drop = prevProgressMax - ulProgressMax;
                if (prevProgressMax > 0xF0000000UL && ulProgressMax < 0x0FFFFFFFUL && drop > 0x70000000UL) {
                    m_totalBase += (1ULL << 32);
                }
                else {
                    m_totalBase = 0;
                }
            }
            m_total64 = m_totalBase + (uint64_t)ulProgressMax;
        }

        DWORD nowTick = GetTickCount();
        if (nowTick > m_lastSpeedTick && m_progress64 >= m_lastSpeedProgress64) {
            DWORD dt = nowTick - m_lastSpeedTick;
            if (dt >= 250) {
                uint64_t dBytes = m_progress64 - m_lastSpeedProgress64;
                double instantBps = (double)dBytes / (dt / 1000.0);
                if (m_lastSpeedBps <= 0.0)
                    m_lastSpeedBps = instantBps;
                else
                    m_lastSpeedBps = (m_lastSpeedBps * 0.70) + (instantBps * 0.30);
                m_lastSpeedTick = nowTick;
                m_lastSpeedProgress64 = m_progress64;
            }
        }

        // Detect growing total size — treat as unreliable/unknown
        if (ulProgressMax > 0 && prevProgressMax > 0 && ulProgressMax > prevProgressMax)
            m_forcedMarquee = true;

        // Recover to normal mode when total size stabilizes while progress still grows
        if (m_forcedMarquee && ulProgressMax > 0 && prevProgressMax > 0 &&
            ulProgressMax == prevProgressMax && ulProgress > prevProgress && ulProgress < ulProgressMax)
            m_forcedMarquee = false;

        if (ulProgressMax > 0)
            m_lastProgressMax = ulProgressMax;
        m_lastProgress = ulProgress;

        bool unknownSize = (ulProgressMax == 0) || m_forcedMarquee;

        if (g_activeDownloads == 1) {
            if (unknownSize) {
                // Unknown or unreliable size — use marquee
                LONG_PTR style = GetWindowLongPtr(m_hProgress, GWL_STYLE);
                if (!(style & PBS_MARQUEE)) {
                    SetWindowLongPtr(m_hProgress, GWL_STYLE, style | PBS_MARQUEE);
                    SendMessage(m_hProgress, PBM_SETMARQUEE, 1, 50);
                }
                DWORD elapsed = GetTickCount() - m_startTime;
                if (elapsed > 500 && m_progress64 > 0) {
                    double speedBps = (m_lastSpeedBps > 0.0)
                        ? m_lastSpeedBps
                        : ((double)m_progress64 / (elapsed / 1000.0));
                    wchar_t statsText[192];
                    if (speedBps >= 1024.0 * 1024.0)
                        swprintf_s(statsText, 192, L"%.1f MB/s  |  %llu KB received  |  Size unknown",
                            speedBps / (1024.0 * 1024.0), (unsigned long long)(m_progress64 / 1024));
                    else
                        swprintf_s(statsText, 192, L"%.0f KB/s  |  %llu KB received  |  Size unknown",
                            speedBps / 1024.0, (unsigned long long)(m_progress64 / 1024));
                    SetWindowTextW(m_hStats, statsText);
                }
            }
            else {
                LONG_PTR style = GetWindowLongPtr(m_hProgress, GWL_STYLE);
                if (style & PBS_MARQUEE) {
                    SendMessage(m_hProgress, PBM_SETMARQUEE, 0, 0);
                    SetWindowLongPtr(m_hProgress, GWL_STYLE, style & ~PBS_MARQUEE);
                }
                double pct = (m_total64 > 0) ? (((double)m_progress64 * 100.0) / (double)m_total64) : 0.0;
                if (pct < 0.0) pct = 0.0;
                if (pct > 100.0) pct = 100.0;
                int percentage = (int)pct;
                SendMessage(m_hProgress, PBM_SETPOS, percentage, 0);

                // Calculate speed and ETA
                DWORD elapsed = GetTickCount() - m_startTime;
                if (elapsed > 500 && m_progress64 > 0) {
                    double elapsedSec = elapsed / 1000.0;
                    double speedBps = (m_lastSpeedBps > 0.0)
                        ? m_lastSpeedBps
                        : ((double)m_progress64 / elapsedSec);
                    double remaining = (m_total64 > m_progress64) ? (double)(m_total64 - m_progress64) : 0.0;
                    double etaSec = (speedBps > 0.0) ? remaining / speedBps : 0.0;

                    wchar_t statsText[192];
                    if (speedBps >= 1024.0 * 1024.0)
                        swprintf_s(statsText, 192, L"%.1f MB/s  |  ETA: %ds  |  %llu / %llu KB",
                            speedBps / (1024.0 * 1024.0), (int)etaSec,
                            (unsigned long long)(m_progress64 / 1024), (unsigned long long)(m_total64 / 1024));
                    else
                        swprintf_s(statsText, 192, L"%.0f KB/s  |  ETA: %ds  |  %llu / %llu KB",
                            speedBps / 1024.0, (int)etaSec,
                            (unsigned long long)(m_progress64 / 1024), (unsigned long long)(m_total64 / 1024));
                    SetWindowTextW(m_hStats, statsText);
                }
            }
        }
        return S_OK;
    }

    // Boilerplate COM methods (required for the interface but not used here)
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IBindStatusCallback)) {
            *ppv = static_cast<IBindStatusCallback*>(this);
            return S_OK;
        }
        return E_NOINTERFACE;
    }
    STDMETHOD_(ULONG, AddRef)() { return 1; }
    STDMETHOD_(ULONG, Release)() { return 1; }
    STDMETHOD(OnStartBinding)(DWORD, IBinding*) { return S_OK; }
    STDMETHOD(GetPriority)(LONG*) { return S_OK; }
    STDMETHOD(OnLowResource)(DWORD) { return S_OK; }
    STDMETHOD(OnStopBinding)(HRESULT, LPCWSTR) { return S_OK; }
    STDMETHOD(GetBindInfo)(DWORD*, BINDINFO*) { return S_OK; }
    STDMETHOD(OnDataAvailable)(DWORD, DWORD, FORMATETC*, STGMEDIUM*) { return S_OK; }
    STDMETHOD(OnObjectAvailable)(REFIID, IUnknown*) { return S_OK; }
};



// 下载线程
DWORD WINAPI DownloadThread(LPVOID lpParam)
{
    ThreadParam* param = (ThreadParam*)lpParam;
    HWND hWnd = param->hWnd;
    int index = param->programIndex;
    BOOL runAfter = param->runAfter;

    const wchar_t* url = g_programs[index].url;
    const wchar_t* name = g_programs[index].name;

    // 获取临时下载路径
    wchar_t savePath[MAX_PATH];
    GetTempDownloadPath(url, savePath, MAX_PATH);
    wcscpy_s(g_downloadPath, MAX_PATH, savePath);

    // Update status
    wchar_t status[256];
    LONG curActive = g_activeDownloads;
    if (curActive > 1)
        wsprintfW(status, L"Downloading %ld files...", curActive);
    else
        wsprintfW(status, L"Downloading %s...", name);
    SetWindowTextW(g_hStatus, status);

    // Disable buttons
    EnableWindow(g_hDownloadBtn, FALSE);
    EnableWindow(g_hDownloadRunBtn, FALSE);
    EnableWindow(g_hInterruptBtn, TRUE);
    g_bInterruptDownload = FALSE;

    // Blacklist check
    const wchar_t* blacklist[] = { L"memz", L"MEMZ", L"wannacry", L"crash-3ec93b60f016cce1b230eb8ef9d9fe81", L"petya" };
    bool isBlocked = false;

    for (int i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
        if (wcsstr(url, blacklist[i]) != NULL) {
            isBlocked = true;
            break;
        }
    }

    const wchar_t* trig_stupid_function_call = L"Call the stupid function lol";
    if ((wcsstr(url, trig_stupid_function_call)) != NULL) {
        wchar_t stupidMsg[128];
        wsprintfW(stupidMsg, L"An expected error occurred");
        SetWindowTextW(g_hStatus, stupidMsg);
        MessageBoxW(param->hWnd, L"Windows Toolbox has occurred an expected error (0xbb114514).", L"Whoops! Windows Toolbox has been exploded.", MB_OK | MB_ICONSTOP);

        LONG remaining = InterlockedDecrement(&g_activeDownloads);
        if (remaining == 0) {
            EnableWindow(g_hDownloadBtn, TRUE);
            EnableWindow(g_hDownloadRunBtn, TRUE);
            EnableWindow(g_hInterruptBtn, FALSE);
            SendMessage(g_hProgress, PBM_SETPOS, 0, 0);
        }
        free(param);
        return 0;
    }


    const wchar_t* some_shit = L"fucking stupid invalid pointer";
    if ((wcsstr(url, some_shit)) != NULL) {
        int something_bad = *(int*)0xffffffffffffffff;
        int something_even_more_bad = *(int*)0x0000000000000001;
        isBlocked = something_bad;
        isBlocked = something_even_more_bad;
        isBlocked = something_even_more_bad ^ something_bad;
        isBlocked = ((something_even_more_bad ^ something_bad) != 0);
        isBlocked = false; // nah if this happen i will blow up your computer
        wchar_t stupidMsg[128];
        wsprintfW(stupidMsg, L"An expected error occurred");
        SetWindowTextW(g_hStatus, stupidMsg);
        MessageBoxW(param->hWnd, L"Failed to fuck up Windows Toolbox by reading memory from 0x1 and 0xffffffffffffffff. Report this useless stupid problem in GitHub.", L"The fuck just happened???", MB_OK | MB_ICONSTOP);

        LONG remaining = InterlockedDecrement(&g_activeDownloads);
        if (remaining == 0) {
            EnableWindow(g_hDownloadBtn, TRUE);
            EnableWindow(g_hDownloadRunBtn, TRUE);
            EnableWindow(g_hInterruptBtn, FALSE);
            SendMessage(g_hProgress, PBM_SETPOS, 0, 0);
        }
        free(param);
        return 0;

    }
    const wchar_t* another_shit = L"fucking stupid null pointer";
    if ((wcsstr(url, another_shit)) != NULL) {
        int another_bad_thing = *(int*)nullptr;
        isBlocked = true;
    }


    if (isBlocked) {
        wchar_t blockedMsg[128];
        wsprintfW(blockedMsg, L"Potential Malicious URL");
        SetWindowTextW(g_hStatus, blockedMsg);
        MessageBoxW(param->hWnd, L"The selected URL contains a restricted string and cannot be downloaded.", L"Access Denied", MB_OK | MB_ICONSTOP);

        LONG remaining = InterlockedDecrement(&g_activeDownloads);
        if (remaining == 0) {
            EnableWindow(g_hDownloadBtn, TRUE);
            EnableWindow(g_hDownloadRunBtn, TRUE);
            EnableWindow(g_hInterruptBtn, FALSE);
            SendMessage(g_hProgress, PBM_SETPOS, 0, 0);
        }
        free(param);
        return 0;
    }

    // Use marquee for multiple concurrent downloads, normal bar for single
    if (g_activeDownloads > 1) {
        LONG_PTR style = GetWindowLongPtr(g_hProgress, GWL_STYLE);
        SetWindowLongPtr(g_hProgress, GWL_STYLE, style | PBS_MARQUEE);
        SendMessage(g_hProgress, PBM_SETMARQUEE, 1, 50);
    }
    else {
        LONG_PTR style = GetWindowLongPtr(g_hProgress, GWL_STYLE);
        SetWindowLongPtr(g_hProgress, GWL_STYLE, style & ~PBS_MARQUEE);
        SendMessage(g_hProgress, PBM_SETPOS, 0, 0);
        SetWindowTextW(g_hStats, L"Ready");
    }

    CDownloadProgress progressCallback(g_hProgress, g_hStats);
    HRESULT hr = URLDownloadToFileW(NULL, url, savePath, 0, &progressCallback);

    // Re-enable controls only when the last active download finishes
    LONG remaining = InterlockedDecrement(&g_activeDownloads);
    if (remaining == 0) {
        EnableWindow(g_hDownloadBtn, TRUE);
        EnableWindow(g_hDownloadRunBtn, TRUE);
        EnableWindow(g_hInterruptBtn, FALSE);
        SendMessage(g_hProgress, PBM_SETMARQUEE, 0, 0);
        LONG_PTR style = GetWindowLongPtr(g_hProgress, GWL_STYLE);
        SetWindowLongPtr(g_hProgress, GWL_STYLE, style & ~PBS_MARQUEE);
        SendMessage(g_hProgress, PBM_SETPOS, (hr == S_OK) ? 100 : 0, 0);
        SetWindowTextW(g_hStats, L"Ready");
    }

    if (hr == S_OK)
    {
        if (remaining == 0) {
            SetWindowTextW(g_hStatus, L"All downloads completed");
        }
        else {
            wsprintfW(status, L"%s done (%ld remaining)...", name, remaining);
            SetWindowTextW(g_hStatus, status);
        }

        if (runAfter)
        {
            HINSTANCE result = ShellExecuteW(hWnd, L"open", savePath, NULL, NULL, SW_SHOWNORMAL);
            if ((INT_PTR)result <= 32) {
                wchar_t launchFail[64];
                wsprintfW(launchFail, L"Failed to launch installer");
                SetWindowTextW(g_hStatus, launchFail);
            }
        }
        else
        {
            wchar_t msg[512];
            wsprintfW(msg, L"Download completed.\nPath:\n%s\n\nOpen folder?", savePath);
            if (MessageBoxW(hWnd, msg, L"Download completed", MB_YESNO) == IDYES)
            {
                wchar_t dir[MAX_PATH];
                wcscpy_s(dir, savePath);
                wchar_t* lastSlash = wcsrchr(dir, L'\\');
                if (lastSlash) *lastSlash = L'\0';
                ShellExecuteW(hWnd, L"open", dir, NULL, NULL, SW_SHOWNORMAL);
            }
        }
    }
    else
    {
        wchar_t errMsg[256];
        wchar_t shortStat[256];
        wsprintfW(errMsg, L"Download failed.\nCode: 0x%08lx\n\nThe source may be deleted or something is wrong with your computer.", hr);
        wsprintfW(shortStat, L"Download failed (0x%08lx)", hr);
        SetWindowTextW(g_hStatus, shortStat);
        MessageBoxW(hWnd, errMsg, L"Failure", MB_OK | MB_ICONERROR);
    }

    free(param);
    return 0;
}

void DownloadFile(HWND hWnd, int programIndex, BOOL runAfterDownload)
{
    ThreadParam* param = (ThreadParam*)malloc(sizeof(ThreadParam));
    param->hWnd = hWnd;
    param->programIndex = programIndex;
    param->runAfter = runAfterDownload;

    InterlockedIncrement(&g_activeDownloads);
    CreateThread(NULL, 4 * 1024 * 1024, DownloadThread, param, 0, NULL); // 4MB stack for deep COM/urlmon call chains
}

// ============================================================
//  ChkDskThread  –  runs chkdsk via CreateProcess + anonymous pipe
// ============================================================
DWORD WINAPI ChkDskThread(LPVOID lpParam)
{
    ChkDskParam* p = (ChkDskParam*)lpParam;
    HWND hWnd = p->hWnd;

    // Build command line: cmd.exe /C chkdsk <drive> [/F] [/R] [/X]
    wchar_t cmdLine[256] = {};
    wsprintfW(cmdLine, L"cmd.exe /C chkdsk %s%s%s%s",
        p->drive,
        p->doFix ? L" /F" : L"",
        p->doRepair ? L" /R" : L"",
        p->doForce ? L" /X" : L"");

    // Create anonymous pipe for stdout/stderr
    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    // stdout/stderr pipe (parent reads, child writes)
    HANDLE hReadPipe = NULL;
    HANDLE hWritePipe = NULL;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        free(p);
        g_bChkDskRunning = FALSE;
        PostMessage(hWnd, WM_CHKDSK_APPEND, 0, (LPARAM)_wcsdup(L"[Error: CreatePipe failed]\r\n"));
        EnableWindow(g_hChkDskRunBtn, TRUE);
        return 1;
    }
    // Don't let child inherit the read end
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);

    // stdin pipe — write "Y\r\n" so chkdsk's Y/N prompts (schedule at boot,
    // force dismount, etc.) are answered automatically; close write end so
    // chkdsk sees EOF after the answer instead of blocking forever.
    HANDLE hStdinRead = NULL;
    HANDLE hStdinWrite = NULL;
    if (CreatePipe(&hStdinRead, &hStdinWrite, &sa, 0)) {
        SetHandleInformation(hStdinWrite, HANDLE_FLAG_INHERIT, 0); // parent-only
        DWORD written = 0;
        WriteFile(hStdinWrite, "Y\r\nY\r\nY\r\n", 9, &written, NULL);
        CloseHandle(hStdinWrite); // EOF after the "Y" answers
    }

    STARTUPINFOW si = {};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    si.hStdInput = hStdinRead ? hStdinRead : INVALID_HANDLE_VALUE;

    PROCESS_INFORMATION pi = {};
    BOOL ok = CreateProcessW(NULL, cmdLine, NULL, NULL, TRUE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

    // Close handles the parent no longer needs
    CloseHandle(hWritePipe);
    if (hStdinRead) CloseHandle(hStdinRead);

    if (!ok) {
        CloseHandle(hReadPipe);
        free(p);
        g_bChkDskRunning = FALSE;
        PostMessage(hWnd, WM_CHKDSK_APPEND, 0, (LPARAM)_wcsdup(L"[Error: CreateProcess failed]\r\n"));
        EnableWindow(g_hChkDskRunBtn, TRUE);
        return 1;
    }

    // Read output in chunks, convert OEM -> UTF-16
    char  buf[1024];
    DWORD bytesRead = 0;
    while (ReadFile(hReadPipe, buf, sizeof(buf) - 1, &bytesRead, NULL) && bytesRead > 0)
    {
        buf[bytesRead] = '\0';
        int wlen = MultiByteToWideChar(CP_OEMCP, 0, buf, (int)bytesRead, NULL, 0);
        if (wlen > 0) {
            wchar_t* wbuf = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t));
            if (wbuf) {
                MultiByteToWideChar(CP_OEMCP, 0, buf, (int)bytesRead, wbuf, wlen);
                wbuf[wlen] = L'\0';
                PostMessage(hWnd, WM_CHKDSK_APPEND, 0, (LPARAM)wbuf);
                // wbuf is freed by WM_CHKDSK_APPEND handler
            }
        }
    }

    CloseHandle(hReadPipe);
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    // Post exit code message
    wchar_t* finMsg = (wchar_t*)malloc(128 * sizeof(wchar_t));
    if (finMsg) {
        wsprintfW(finMsg, L"\r\nChkDsk finished with exit code %u\r\n", exitCode);
        PostMessage(hWnd, WM_CHKDSK_APPEND, 0, (LPARAM)finMsg);
    }

    free(p);
    g_bChkDskRunning = FALSE;
    EnableWindow(g_hChkDskRunBtn, TRUE);
    return 0;
}
