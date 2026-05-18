// WinTool.cpp -- Windows Toolbox
// Visual Studio 2019, Windows C++ Desktop Project

#include <windows.h>
#include <commctrl.h>
#include <urlmon.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdint.h>
#include <wctype.h>
#include <vector>
#include <string>
#include <set>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

const wchar_t* g_version = L"26.5.6";

// 控件 ID
#define ID_LIST_PROGRAMS       1001
#define BTN_DOWNLOAD           1002
#define BTN_DOWNLOAD_RUN       1003
#define IDC_STATUS             1004
#define IDC_TAB_MAIN           1005
#define BTN_UPDATE_SERVER      1006
#define IDC_SEARCH             1007
#define BTN_INTERRUPT          1008
#define BTN_DOWNLOADS          1024
#define IDC_CHKDSK_DRIVE       1009
#define IDC_CHKDSK_OUTPUT      1010
#define BTN_CHKDSK_RUN         1011
#define IDC_CHKDSK_FIX         1012
#define IDC_CHKDSK_REPAIR      1013
#define IDC_CHKDSK_FORCE       1014
#define IDC_CHKDSK_PROGRESS    1015
#define WM_CHKDSK_APPEND       (WM_APP + 1)

#define IDC_APPS_LIST          1017
#define BTN_APPS_REFRESH       1018
#define BTN_APPS_BASIC_UNINST  1019
#define BTN_APPS_FORCE_UNINST  1020
#define IDC_APPS_FORCE_OUTPUT  1021
#define IDC_APPS_FORCE_PROGRESS 1022
#define IDC_APPS_SEARCH        1023
#define IDC_INFO_OUTPUT        1030
#define BTN_INFO_REFRESH       1031
#define IDC_STARTUP_LIST       1032
#define BTN_STARTUP_REFRESH    1033
#define BTN_STARTUP_DISABLE    1034
#define BTN_STARTUP_ENABLE     1035
#define IDC_ACTIVATION_PORTAL_BTN  1036
#define IDC_ACTIVATION_KEYLIST     1037
#define IDC_ACTIVATION_INSTALL_BTN 1038
#define IDC_ACTIVATION_ACTIVATE_BTN 1039

#define WM_APPS_FORCE_APPEND   (WM_APP + 2)
#define WM_APPS_FORCE_PROGRESS (WM_APP + 3)
#define WM_APPS_FORCE_DONE     (WM_APP + 4)
#define WM_APPS_FORCE_STATUS   (WM_APP + 5)

// 程序信息结构
typedef struct {
    wchar_t name[260];
    wchar_t url[2048];
} ProgramInfo;

// 程序出Bug了？
// 　　　∩∩
// 　　（´･ω･）
// 　 ＿|　⊃／(＿＿_
// 　／ └-(＿＿＿／
// 　￣￣￣￣￣￣￣
// 算了反正不是我写的
// 　　 ⊂⌒／ヽ-、＿
// 　／⊂_/＿＿＿＿ ／
// 　￣￣￣￣￣￣￣
// 万一是我写的呢
// 　　　∩∩
// 　　（´･ω･）
// 　 ＿|　⊃／(＿＿_
// 　／ └-(＿＿＿／
// 　￣￣￣￣￣￣￣
// 算了反正改了一个又出三个
// 　　 ⊂⌒／ヽ-、＿
// 　／⊂_/＿＿＿＿ ／
// 　￣￣￣￣￣￣￣

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
    { L"Kiva MIDI Player 1.1.15", L"https://github.com/arduano/Kiva/releases/download/v1.1.15/KivaInstaller.exe"},
    { L"Kiva MIDI Player 1.1.15 Portable", L"https://github.com/arduano/Kiva/releases/download/v1.1.15/KivaPortable.zip"},
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
    { L"PCL2 (Plain Craft Launcher) x64 (MD5:92886839e678a82edf4d0eb44de79e7b)", L"http://45.126.125.60/internal/toolbox/PlainCraftLauncher2.exe"},
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
    { L"Self: Windows Toolbox 26.5.3 Packaged", L"http://45.126.125.60/internal/toolbox/versions/pkg/toolbox26.5.3.zip" },
    { L"Self: Windows Toolbox 26.5.4 Packaged", L"http://45.126.125.60/internal/toolbox/versions/pkg/toolbox26.5.4.zip" },
    { L"Self: Windows Toolbox 26.5.5 Packaged", L"http://45.126.125.60/internal/toolbox/versions/pkg/toolbox26.5.5.zip" },
    { L"Self: Windows Toolbox 26.5.0 Core", L"http://45.126.125.60/internal/toolbox/versions/core/toolbox26.5.0.exe" },
    { L"Self: Windows Toolbox 26.5.1 Core", L"http://45.126.125.60/internal/toolbox/versions/core/toolbox26.5.1.exe" },
    { L"Self: Windows Toolbox 26.5.2 Core", L"http://45.126.125.60/internal/toolbox/versions/core/toolbox26.5.2.exe" },
    { L"Self: Windows Toolbox 26.5.3 Core", L"http://45.126.125.60/internal/toolbox/versions/core/toolbox26.5.3.exe" },
    { L"Self: Windows Toolbox 26.5.4 Core", L"http://45.126.125.60/internal/toolbox/versions/core/toolbox26.5.4.exe" },
    { L"Self: Windows Toolbox 26.5.5 Core", L"http://45.126.125.60/internal/toolbox/versions/core/toolbox26.5.5.exe" },


    { L"Debug: The 16383 GB File", L"http://45.126.125.60/public/speedtest16t.bin"},
    { L"Debug: The Batch File", L"http://45.126.125.60/public/injection.bat"},
    { L"Debug: Something that doesn't exist", L"http://45.126.125.60/public/imnotexist"},
    { L"Debug: Call the stupid function", L"Call the stupid function lol"},
    { L"Debug: Invalid pointer", L"fucking stupid invalid pointer"},
    // 可以继续添加更多程序
    // { L"另一个程序", L"https://example.com/setup.exe" },
};
int g_programCount = sizeof(g_programs) / sizeof(g_programs[0]);

int you_cannot_notice_me_until_you_seek_deeper = 114514;
int in_this_such_trash_source_code = 1919810;

ProgramInfo someStupidWordsForWhoseSeekDeeply[] = {
    { L"Hello, thanks for looking deeper" },
    { L"But what I said are useless"},
    { L"It will be more meaningful if you", L"parse this with strings or inside memory"},
    { L"Its surprising that you saw this"},
    { L"GitHub @lqx211" },
    { L"Copyleft 2026-2099, all rights not reserved."}
};


// 全局变量
HINSTANCE g_hInst;
HWND g_hTab;
HWND g_hDownloadTitle;
HWND g_hSettingsTitle;
HWND g_hChkDskTitle;
HWND g_hAppsTitle;
HWND g_hInfoTitle;
HWND g_hStartupTitle;
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
HWND g_hDownloadsBtn; // Open downloads folder button
HWND g_hChkDskDriveLabel;
HWND g_hChkDskDrive;
HWND g_hChkDskFix;
HWND g_hChkDskRepair;
HWND g_hChkDskForce;
HWND g_hChkDskRunBtn;
HWND g_hChkDskProgress;
HWND g_hChkDskOutput;

HWND g_hAppsList;
HWND g_hAppsSearch;
HWND g_hAppsRefreshBtn;
HWND g_hAppsBasicUninstBtn;
HWND g_hAppsForceUninstBtn;
HWND g_hAppsForceOutput;
HWND g_hAppsForceProgress;
HWND g_hInfoOutput;
HWND g_hInfoRefreshBtn;
HWND g_hStartupList;
HWND g_hStartupRefreshBtn;
HWND g_hStartupDisableBtn;
HWND g_hStartupEnableBtn;
HWND g_hActivationTitle;
HWND g_hActivationPortalBtn;
HWND g_hActivationKeyList;
HWND g_hActivationInstallBtn;
HWND g_hActivationActivateBtn;

volatile BOOL g_bChkDskRunning = FALSE;
int g_chkDskProgressValue = 0;
volatile BOOL g_bAppsForceRunning = FALSE;
int  g_filteredIndices[512]; // maps listbox pos -> g_programs index
volatile LONG g_activeDownloads = 0; // active download thread count
volatile BOOL g_bInterruptDownload = FALSE; // set to TRUE to abort
HBRUSH g_hWhiteBrush = NULL; // white background brush for static controls

typedef struct {
    wchar_t name[260];
    wchar_t key[64];
} WindowsKey;

const int WINDOWS_KEYS_COUNT = 8;
WindowsKey g_windowsKeys[WINDOWS_KEYS_COUNT] = {
    { L"Windows 7 x64 Ultimate", L"W47TC-J777J-GQ9XC-87JH6-DGP3C" },
    { L"Windows 8.1 Pro / Enterprise", L"C8D8Y-QF8XC-7YFWM-YQ9BB-4FXN3" },
    { L"Windows 10/11 Pro", L"2WY6N-JR8BB-RJCKC-HGDKV-CYT6Y" },
    { L"Windows 11 Pro Workstation (1)", L"98QTC-N2B7D-MXQMY-V2H8G-YHBWG" },
    { L"Windows 11 Pro Workstation (2)", L"7N7JY-RFFFY-HW9FQ-2B24K-FRH79" },
    { L"Windows 11 Pro Workstation (3)", L"G3TFB-R3PCG-V6X9H-9RWXW-TN2RK" },
    { L"Windows 10/11 Pro Workstation", L"FPTW7-GXN6B-JMFD9-WC8FG-D3XYX" },
    { L"Windows 11 Enterprise LTSC", L"QGNCF-C4DD9-R3C9J-6H4PD-RJV6H" },
};


wchar_t g_downloadPath[MAX_PATH];
wchar_t g_downloadsFolderPath[MAX_PATH];

typedef struct {
    wchar_t name[260];
    wchar_t uninstallCmd[1024];
    wchar_t installLocation[MAX_PATH];
} AppEntry;

const int APP_ENTRY_CAPACITY = 8192;
AppEntry g_apps[APP_ENTRY_CAPACITY];
int g_appsFilteredIndices[APP_ENTRY_CAPACITY];
int g_appCount = 0;

enum StartupSourceType {
    STARTUP_REG_HKLM = 1,
    STARTUP_REG_HKCU = 2,
    STARTUP_FILE_USER = 3,
    STARTUP_FILE_COMMON = 4
};

typedef struct {
    wchar_t name[260];
    wchar_t command[1024];
    wchar_t location[160];
    BOOL enabled;
    int sourceType;
    HKEY regRoot;
    wchar_t regSubKey[260];
    wchar_t filePath[MAX_PATH];
    wchar_t originalPath[MAX_PATH];
} StartupItem;

const int STARTUP_ITEM_CAPACITY = 4096;
StartupItem g_startupItems[STARTUP_ITEM_CAPACITY];
int g_startupItemCount = 0;

// 前向声明
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddProgramsToList(HWND hList);
void FilterProgramList(HWND hList, const wchar_t* query);
int GetSelectedProgramIndex(HWND hList);
void EnsureDownloadsFolder();
void DownloadFile(HWND hWnd, int programIndex, BOOL runAfterDownload);
DWORD WINAPI DownloadThread(LPVOID lpParam);
int GetAllSelectedIndices(HWND hList, int* indices, int maxCount);
void SwitchTab(int tabIndex);
DWORD WINAPI ChkDskThread(LPVOID lpParam);
int DetectChkDskPercent(const wchar_t* text, int currentValue);
void LoadInstalledApps(HWND hList);
void FilterAppsList(HWND hList, const wchar_t* query);
int GetSelectedAppIndices(HWND hList, int* indices, int maxCount);
static void ScanUnknownAppFolders(const wchar_t* rootPath, const wchar_t* sourceTag);
static BOOL RemoveAppRegistryEntries(const AppEntry* app);
void RunBasicUninstall(HWND hWnd);
void StartForceUninstall(HWND hWnd);
DWORD WINAPI ForceUninstallThread(LPVOID lpParam);
static void UpdateAppsForceModeUI(BOOL running);
void RefreshSystemInfo();
void LoadStartupItems(HWND hList);
void DisableSelectedStartupItem(HWND hWnd);
void EnableSelectedStartupItem(HWND hWnd);
static BOOL EnsureDirectoryRecursive(const wchar_t* path);
static BOOL GetStartupFolderPath(BOOL commonFolder, wchar_t* outPath, DWORD outChars);
static BOOL GetStartupBackupPath(BOOL commonFolder, wchar_t* outPath, DWORD outChars);
static BOOL MoveRegistryStartupItem(const StartupItem* item, BOOL enable);
static BOOL MoveStartupFileItem(const StartupItem* item, BOOL enable);
static const wchar_t* CpuArchitectureToText(WORD arch);
static int GetPhysicalCoreCount();
static std::wstring FormatBytesULongLong(ULONGLONG bytes);
static BOOL GetWindowsVersionText(wchar_t* outBuf, DWORD outChars);

typedef struct {
    HWND hWnd;
    int indices[64];
    int count;
} ForceUninstallParam;

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


//    进入入口函数之前:
//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//                      *  *  *
//                      |  |  |
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永无BUG
//
//     “ 话说女孩就不能做程序员吗?
//        怎么同学都觉得这么奇怪?
//        ( 无语 )

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
        // 创建下载目录
        EnsureDownloadsFolder();

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
        tie.pszText = const_cast<LPWSTR>(L"Apps");
        TabCtrl_InsertItem(g_hTab, 3, &tie);
        tie.pszText = const_cast<LPWSTR>(L"Info");
        TabCtrl_InsertItem(g_hTab, 4, &tie);
        tie.pszText = const_cast<LPWSTR>(L"Startup");
        TabCtrl_InsertItem(g_hTab, 5, &tie);
        tie.pszText = const_cast<LPWSTR>(L"Activation");
        TabCtrl_InsertItem(g_hTab, 6, &tie);

        g_hDownloadTitle = CreateWindowW(L"STATIC", L"Select to download.",
            WS_CHILD,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hSettingsTitle = CreateWindowW(L"STATIC", L"Settings:",
            WS_CHILD,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hChkDskTitle = CreateWindowW(L"STATIC", L"ChkDsk:",
            WS_CHILD,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hAppsTitle = CreateWindowW(L"STATIC", L"Apps:",
            WS_CHILD,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hInfoTitle = CreateWindowW(L"STATIC", L"Info:",
            WS_CHILD,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hStartupTitle = CreateWindowW(L"STATIC", L"Startup:",
            WS_CHILD,
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
            158, 333, 100, 28, hWnd, (HMENU)BTN_DOWNLOAD_RUN, g_hInst, NULL);

        g_hInterruptBtn = CreateWindowW(L"BUTTON", L"Interrupt",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            264, 333, 100, 28, hWnd, (HMENU)BTN_INTERRUPT, g_hInst, NULL);
        EnableWindow(g_hInterruptBtn, FALSE);

        g_hDownloadsBtn = CreateWindowW(L"BUTTON", L"Downloads",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            370, 333, 106, 28, hWnd, (HMENU)BTN_DOWNLOADS, g_hInst, NULL);

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

        g_hChkDskProgress = CreateWindowExW(0, PROGRESS_CLASS, NULL,
            WS_CHILD | PBS_SMOOTH,
            24, 98, 452, 18, hWnd, (HMENU)IDC_CHKDSK_PROGRESS, g_hInst, NULL);

        g_hChkDskOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
            24, 120, 452, 233, hWnd, (HMENU)IDC_CHKDSK_OUTPUT, g_hInst, NULL);

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

        g_hAppsSearch = CreateWindowW(L"EDIT", L"",
            WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
            24, 74, 452, 22, hWnd, (HMENU)IDC_APPS_SEARCH, g_hInst, NULL);
        SendMessageW(g_hAppsSearch, EM_SETCUEBANNER, TRUE, (LPARAM)L"Search installed apps...");

        g_hAppsList = CreateWindowW(L"LISTBOX", NULL,
            WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_EXTENDEDSEL,
            24, 100, 452, 190, hWnd, (HMENU)IDC_APPS_LIST, g_hInst, NULL);

        g_hAppsRefreshBtn = CreateWindowW(L"BUTTON", L"Refresh",
            WS_CHILD | BS_PUSHBUTTON,
            24, 300, 120, 28, hWnd, (HMENU)BTN_APPS_REFRESH, g_hInst, NULL);

        g_hAppsBasicUninstBtn = CreateWindowW(L"BUTTON", L"Basic Uninstall",
            WS_CHILD | BS_PUSHBUTTON,
            152, 300, 140, 28, hWnd, (HMENU)BTN_APPS_BASIC_UNINST, g_hInst, NULL);

        g_hAppsForceUninstBtn = CreateWindowW(L"BUTTON", L"Force Uninstall",
            WS_CHILD | BS_PUSHBUTTON,
            300, 300, 140, 28, hWnd, (HMENU)BTN_APPS_FORCE_UNINST, g_hInst, NULL);

        g_hAppsForceProgress = CreateWindowExW(0, PROGRESS_CLASS, NULL,
            WS_CHILD | PBS_SMOOTH,
            24, 338, 452, 18, hWnd, (HMENU)IDC_APPS_FORCE_PROGRESS, g_hInst, NULL);

        g_hAppsForceOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
            24, 364, 452, 34, hWnd, (HMENU)IDC_APPS_FORCE_OUTPUT, g_hInst, NULL);

        SendMessage(g_hAppsForceProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(g_hAppsForceProgress, PBM_SETPOS, 0, 0);
        ShowWindow(g_hAppsForceProgress, SW_HIDE);
        ShowWindow(g_hAppsForceOutput, SW_HIDE);
        // ──────────────────────────────────────────────────────────────────

        g_hInfoRefreshBtn = CreateWindowW(L"BUTTON", L"Refresh",
            WS_CHILD | BS_PUSHBUTTON,
            24, 66, 120, 28, hWnd, (HMENU)BTN_INFO_REFRESH, g_hInst, NULL);

        g_hInfoOutput = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
            24, 100, 452, 258, hWnd, (HMENU)IDC_INFO_OUTPUT, g_hInst, NULL);

        g_hStartupRefreshBtn = CreateWindowW(L"BUTTON", L"Refresh",
            WS_CHILD | BS_PUSHBUTTON,
            24, 66, 110, 28, hWnd, (HMENU)BTN_STARTUP_REFRESH, g_hInst, NULL);

        g_hStartupDisableBtn = CreateWindowW(L"BUTTON", L"Disable",
            WS_CHILD | BS_PUSHBUTTON,
            140, 66, 110, 28, hWnd, (HMENU)BTN_STARTUP_DISABLE, g_hInst, NULL);

        g_hStartupEnableBtn = CreateWindowW(L"BUTTON", L"Enable",
            WS_CHILD | BS_PUSHBUTTON,
            256, 66, 110, 28, hWnd, (HMENU)BTN_STARTUP_ENABLE, g_hInst, NULL);

        g_hStartupList = CreateWindowW(L"LISTBOX", NULL,
            WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            24, 100, 452, 258, hWnd, (HMENU)IDC_STARTUP_LIST, g_hInst, NULL);

        g_hActivationTitle = CreateWindowW(L"STATIC", L"Activation:",
            WS_CHILD,
            24, 42, 200, 25, hWnd, NULL, g_hInst, NULL);

        g_hActivationPortalBtn = CreateWindowW(L"BUTTON", L"Activation Portal",
            WS_CHILD | BS_PUSHBUTTON,
            24, 74, 180, 28, hWnd, (HMENU)IDC_ACTIVATION_PORTAL_BTN, g_hInst, NULL);

        CreateWindowW(L"STATIC", L"Windows Keys:",
            WS_CHILD,
            24, 110, 100, 18, hWnd, NULL, g_hInst, NULL);

        g_hActivationKeyList = CreateWindowW(L"LISTBOX", NULL,
            WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            24, 130, 452, 120, hWnd, (HMENU)IDC_ACTIVATION_KEYLIST, g_hInst, NULL);
        for (int k = 0; k < WINDOWS_KEYS_COUNT; k++) {
            SendMessageW(g_hActivationKeyList, LB_ADDSTRING, 0, (LPARAM)g_windowsKeys[k].name);
        }
        SendMessage(g_hActivationKeyList, LB_SETCURSEL, 0, 0);

        g_hActivationInstallBtn = CreateWindowW(L"BUTTON", L"Install Key",
            WS_CHILD | BS_PUSHBUTTON,
            24, 258, 150, 28, hWnd, (HMENU)IDC_ACTIVATION_INSTALL_BTN, g_hInst, NULL);

        g_hActivationActivateBtn = CreateWindowW(L"BUTTON", L"Activate",
            WS_CHILD | BS_PUSHBUTTON,
            182, 258, 150, 28, hWnd, (HMENU)IDC_ACTIVATION_ACTIVATE_BTN, g_hInst, NULL);

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
        SendMessage(g_hDownloadTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hSettingsTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hInfoTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hStartupTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hSearch, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hDownloadBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hDownloadRunBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hInterruptBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hDownloadsBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hUpdateBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskDriveLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskDrive, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskFix, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskRepair, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskForce, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskRunBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hChkDskProgress, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsSearch, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsList, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsRefreshBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsBasicUninstBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsForceUninstBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hAppsForceProgress, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hInfoRefreshBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hInfoOutput, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hStartupRefreshBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hStartupDisableBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hStartupEnableBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hStartupList, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hActivationTitle, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hActivationPortalBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hActivationKeyList, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hActivationInstallBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(g_hActivationActivateBtn, WM_SETFONT, (WPARAM)hFont, TRUE);

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
        SendMessage(g_hChkDskProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(g_hChkDskProgress, PBM_SETPOS, 0, 0);
        SendMessage(g_hAppsForceOutput, WM_SETFONT, (WPARAM)hFont, TRUE);

        LoadInstalledApps(g_hAppsList);
        RefreshSystemInfo();
        LoadStartupItems(g_hStartupList);
        ShowWindow(g_hDownloadTitle, SW_SHOW);
        ShowWindow(g_hSettingsTitle, SW_HIDE);
        ShowWindow(g_hChkDskTitle, SW_HIDE);
        ShowWindow(g_hAppsTitle, SW_HIDE);
        ShowWindow(g_hInfoTitle, SW_HIDE);
        ShowWindow(g_hStartupTitle, SW_HIDE);

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

        if (wmId == BTN_DOWNLOADS)
        {
            // 打开下载文件夹
            ShellExecuteW(NULL, L"open", g_downloadsFolderPath, NULL, NULL, SW_SHOW);
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
            g_chkDskProgressValue = 0;
            SendMessage(g_hChkDskProgress, PBM_SETPOS, 0, 0);
            CreateThread(NULL, 1024 * 1024, ChkDskThread, p, 0, NULL);
            return 0;
        }

        if (wmId == BTN_APPS_REFRESH)
        {
            SetWindowTextW(g_hAppsSearch, L"");
            LoadInstalledApps(g_hAppsList);
            SetWindowTextW(g_hFooterInfo, L"Apps list refreshed");
            return 0;
        }

        if (wmId == IDC_APPS_SEARCH && HIWORD(wParam) == EN_CHANGE)
        {
            wchar_t query[260] = {};
            GetWindowTextW(g_hAppsSearch, query, 259);
            FilterAppsList(g_hAppsList, query);
            return 0;
        }

        if (wmId == BTN_APPS_BASIC_UNINST)
        {
            RunBasicUninstall(hWnd);
            return 0;
        }

        if (wmId == BTN_APPS_FORCE_UNINST)
        {
            StartForceUninstall(hWnd);
            return 0;
        }

        if (wmId == BTN_INFO_REFRESH)
        {
            RefreshSystemInfo();
            SetWindowTextW(g_hFooterInfo, L"Info refreshed");
            return 0;
        }

        if (wmId == BTN_STARTUP_REFRESH)
        {
            LoadStartupItems(g_hStartupList);
            SetWindowTextW(g_hFooterInfo, L"Startup list refreshed");
            return 0;
        }

        if (wmId == BTN_STARTUP_DISABLE)
        {
            DisableSelectedStartupItem(hWnd);
            return 0;
        }

        if (wmId == BTN_STARTUP_ENABLE)
        {
            EnableSelectedStartupItem(hWnd);
            return 0;
        }

        if (wmId == IDC_ACTIVATION_PORTAL_BTN)
        {
            ShellExecuteW(NULL, L"open", L"https://visualsupport.microsoft.com/", NULL, NULL, SW_SHOW);
            return 0;
        }

        if (wmId == IDC_ACTIVATION_INSTALL_BTN)
        {
            int sel = (int)SendMessageW(g_hActivationKeyList, LB_GETCURSEL, 0, 0);
            if (sel < 0 || sel >= WINDOWS_KEYS_COUNT) {
                MessageBoxW(hWnd, L"Please select a Windows key.", L"Activation", MB_OK);
                return 0;
            }
            wchar_t cmdLine[512] = {};
            swprintf_s(cmdLine, L"C:\\\\Windows\\\\System32\\\\slmgr.vbs -ipk %s", g_windowsKeys[sel].key);
            STARTUPINFOW si = {};
            PROCESS_INFORMATION pi = {};
            si.cb = sizeof(si);
            if (!CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                MessageBoxW(hWnd, L"Failed to run slmgr.vbs.", L"Activation", MB_OK | MB_ICONERROR);
                return 0;
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            SetWindowTextW(g_hFooterInfo, L"Windows key installation initiated");
            return 0;
        }

        if (wmId == IDC_ACTIVATION_ACTIVATE_BTN)
        {
            ShellExecuteW(NULL, L"open", L"C:\\Windows\\System32\\slui.exe", L"4", NULL, SW_SHOW);
            SetWindowTextW(g_hFooterInfo, L"Activation window launched");
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
            int tab = TabCtrl_GetCurSel(g_hTab);
            SwitchTab(tab);
            if (tab == 4)
                RefreshSystemInfo();
            else if (tab == 5)
                LoadStartupItems(g_hStartupList);
            return 0;
        }
        break;
    }

    case WM_ACTIVATE:
    {
        if (LOWORD(wParam) != WA_INACTIVE) {
            int tab = TabCtrl_GetCurSel(g_hTab);
            if (tab == 3 && !g_bAppsForceRunning) {
                LoadInstalledApps(g_hAppsList);
                SetWindowTextW(g_hFooterInfo, L"Apps list refreshed");
            }
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

            g_chkDskProgressValue = DetectChkDskPercent(text, g_chkDskProgressValue);
            SendMessage(g_hChkDskProgress, PBM_SETPOS, g_chkDskProgressValue, 0);
            if (wcsstr(text, L"[ChkDsk finished") != NULL && wcsstr(text, L"exit code 0") != NULL) {
                g_chkDskProgressValue = 100;
                SendMessage(g_hChkDskProgress, PBM_SETPOS, 100, 0);
            }

            free(text);
        }
        return 0;
    }

    case WM_APPS_FORCE_APPEND:
    {
        wchar_t* text = (wchar_t*)lParam;
        if (text) {
            int len = GetWindowTextLengthW(g_hAppsForceOutput);
            SendMessage(g_hAppsForceOutput, EM_SETSEL, (WPARAM)len, (LPARAM)len);
            SendMessage(g_hAppsForceOutput, EM_REPLACESEL, FALSE, (LPARAM)text);
            SendMessage(g_hAppsForceOutput, EM_SCROLLCARET, 0, 0);
            free(text);
        }
        return 0;
    }

    case WM_APPS_FORCE_PROGRESS:
        SendMessage(g_hAppsForceProgress, PBM_SETPOS, (WPARAM)wParam, 0);
        return 0;

    case WM_APPS_FORCE_STATUS:
    {
        wchar_t* text = (wchar_t*)lParam;
        if (text) {
            SetWindowTextW(g_hFooterInfo, text);
            free(text);
        }
        return 0;
    }

    case WM_APPS_FORCE_DONE:
        g_bAppsForceRunning = FALSE;
        UpdateAppsForceModeUI(FALSE);
        SetWindowTextW(g_hFooterInfo, L"Force uninstall completed");
        LoadInstalledApps(g_hAppsList);
        return 0;

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
        wchar_t lowerName[512];
        wcsncpy_s(lowerName, g_programs[i].name, _TRUNCATE);
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
    BOOL showApps = (tabIndex == 3);
    BOOL showInfo = (tabIndex == 4);
    BOOL showStartup = (tabIndex == 5);
    BOOL showActivation = (tabIndex == 6);

    ShowWindow(g_hSearch, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hList, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hProgress, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hDownloadBtn, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hDownloadRunBtn, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hInterruptBtn, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hDownloadsBtn, showDownload ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hDownloadTitle, showDownload ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hSettingsTitle, showSettings ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskTitle, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsTitle, showApps ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hInfoTitle, showInfo ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hStartupTitle, showStartup ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hUpdateBtn, showSettings ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hChkDskDriveLabel, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskDrive, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskFix, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskRepair, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskForce, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskRunBtn, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskProgress, showChkDsk ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hChkDskOutput, showChkDsk ? SW_SHOW : SW_HIDE);

    BOOL showAppsNormal = showApps && !g_bAppsForceRunning;
    BOOL showAppsForce = showApps && g_bAppsForceRunning;
    ShowWindow(g_hAppsSearch, showAppsNormal ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsList, showAppsNormal ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsRefreshBtn, showAppsNormal ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsBasicUninstBtn, showAppsNormal ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsForceUninstBtn, showAppsNormal ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsForceProgress, showAppsForce ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hAppsForceOutput, showAppsForce ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hInfoRefreshBtn, showInfo ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hInfoOutput, showInfo ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hStartupRefreshBtn, showStartup ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hStartupDisableBtn, showStartup ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hStartupEnableBtn, showStartup ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hStartupList, showStartup ? SW_SHOW : SW_HIDE);

    ShowWindow(g_hActivationTitle, showActivation ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hActivationPortalBtn, showActivation ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hActivationKeyList, showActivation ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hActivationInstallBtn, showActivation ? SW_SHOW : SW_HIDE);
    ShowWindow(g_hActivationActivateBtn, showActivation ? SW_SHOW : SW_HIDE);

    if (showDownload)
        SetWindowTextW(g_hFooterInfo, L"Ready");
    else if (showSettings)
        SetWindowTextW(g_hFooterInfo, L"Settings");
    else if (showApps)
        SetWindowTextW(g_hFooterInfo, L"Apps");
    else if (showInfo)
        SetWindowTextW(g_hFooterInfo, L"Info");
    else if (showStartup)
        SetWindowTextW(g_hFooterInfo, L"Startup");
    else if (showActivation)
        SetWindowTextW(g_hFooterInfo, L"Activation");
    else
        SetWindowTextW(g_hFooterInfo, L"ChkDsk");
}

static const wchar_t* CpuArchitectureToText(WORD arch)
{
    switch (arch) {
    case PROCESSOR_ARCHITECTURE_AMD64: return L"x64";
    case PROCESSOR_ARCHITECTURE_INTEL: return L"x86";
    case PROCESSOR_ARCHITECTURE_ARM64: return L"ARM64";
    case PROCESSOR_ARCHITECTURE_ARM: return L"ARM";
    default: return L"Unknown";
    }
}

static int GetPhysicalCoreCount()
{
    DWORD len = 0;
    GetLogicalProcessorInformation(NULL, &len);
    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER || len == 0)
        return 0;

    std::vector<BYTE> buf(len);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION info = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)buf.data();
    if (!GetLogicalProcessorInformation(info, &len))
        return 0;

    int count = 0;
    DWORD n = len / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    for (DWORD i = 0; i < n; i++) {
        if (info[i].Relationship == RelationProcessorCore)
            count++;
    }
    return count;
}

static std::wstring FormatBytesULongLong(ULONGLONG bytes)
{
    wchar_t buf[64] = {};
    if (bytes >= (1024ULL * 1024ULL * 1024ULL)) {
        double gb = (double)bytes / (1024.0 * 1024.0 * 1024.0);
        swprintf_s(buf, L"%.2f GB", gb);
    }
    else if (bytes >= (1024ULL * 1024ULL)) {
        double mb = (double)bytes / (1024.0 * 1024.0);
        swprintf_s(buf, L"%.2f MB", mb);
    }
    else if (bytes >= 1024ULL) {
        double kb = (double)bytes / 1024.0;
        swprintf_s(buf, L"%.2f KB", kb);
    }
    else {
        swprintf_s(buf, L"%llu B", bytes);
    }
    return std::wstring(buf);
}

static BOOL GetWindowsVersionText(wchar_t* outBuf, DWORD outChars)
{
    if (!outBuf || outChars == 0) return FALSE;
    outBuf[0] = L'\0';

    HKEY hKey = NULL;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
        0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS) {
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
            L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
            0, KEY_READ, &hKey) != ERROR_SUCCESS) {
            return FALSE;
        }
    }

    wchar_t productName[128] = L"Windows";
    wchar_t buildStr[64] = L"";
    DWORD ubr = 0;
    DWORD type = 0;
    DWORD cb = sizeof(productName);
    RegQueryValueExW(hKey, L"ProductName", NULL, &type, (LPBYTE)productName, &cb);

    type = 0;
    cb = sizeof(buildStr);
    RegQueryValueExW(hKey, L"CurrentBuildNumber", NULL, &type, (LPBYTE)buildStr, &cb);

    type = 0;
    cb = sizeof(ubr);
    RegQueryValueExW(hKey, L"UBR", NULL, &type, (LPBYTE)&ubr, &cb);
    RegCloseKey(hKey);

    SYSTEM_INFO si = {};
    GetNativeSystemInfo(&si);
    swprintf_s(outBuf, outChars, L"%s (Build %s.%lu, %s)",
        productName, buildStr[0] ? buildStr : L"?", ubr, CpuArchitectureToText(si.wProcessorArchitecture));
    return TRUE;
}

void RefreshSystemInfo()
{
    if (!g_hInfoOutput) return;

    std::wstring text;

    wchar_t osText[256] = L"Unknown";
    if (!GetWindowsVersionText(osText, 256))
        wcscpy_s(osText, L"Unknown");
    text += L"OS\r\n";
    text += L"  Version: ";
    text += osText;
    text += L"\r\n\r\n";

    SYSTEM_INFO si = {};
    GetNativeSystemInfo(&si);
    wchar_t cpuName[256] = L"Unknown";
    HKEY hCpu = NULL;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE,
        L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hCpu) == ERROR_SUCCESS) {
        DWORD type = 0;
        DWORD cb = sizeof(cpuName);
        RegQueryValueExW(hCpu, L"ProcessorNameString", NULL, &type, (LPBYTE)cpuName, &cb);
        RegCloseKey(hCpu);
    }
    int coreCount = GetPhysicalCoreCount();
    text += L"CPU\r\n";
    text += L"  Name: ";
    text += cpuName;
    text += L"\r\n";
    {
        wchar_t line[128] = {};
        swprintf_s(line, L"  Cores: %d\r\n", coreCount > 0 ? coreCount : si.dwNumberOfProcessors);
        text += line;
    }
    {
        wchar_t line[128] = {};
        swprintf_s(line, L"  Logical processors: %lu\r\n", si.dwNumberOfProcessors);
        text += line;
    }
    text += L"  Architecture: ";
    text += CpuArchitectureToText(si.wProcessorArchitecture);
    text += L"\r\n\r\n";

    MEMORYSTATUSEX mem = {};
    mem.dwLength = sizeof(mem);
    if (GlobalMemoryStatusEx(&mem)) {
        ULONGLONG used = mem.ullTotalPhys - mem.ullAvailPhys;
        text += L"RAM\r\n";
        text += L"  Total: ";
        text += FormatBytesULongLong(mem.ullTotalPhys);
        text += L"\r\n";
        text += L"  Used: ";
        text += FormatBytesULongLong(used);
        text += L"\r\n";
        text += L"  Free: ";
        text += FormatBytesULongLong(mem.ullAvailPhys);
        text += L"\r\n";
        {
            wchar_t line[128] = {};
            swprintf_s(line, L"  Usage: %lu%%\r\n\r\n", mem.dwMemoryLoad);
            text += line;
        }
    }

    text += L"Disk\r\n";
    UINT oldErrMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
    wchar_t drives[512] = {};
    DWORD n = GetLogicalDriveStringsW(512, drives);
    if (n > 0 && n < 512) {
        for (wchar_t* p = drives; *p; p += wcslen(p) + 1) {
            UINT driveType = GetDriveTypeW(p);
            if (driveType == DRIVE_NO_ROOT_DIR || driveType == DRIVE_UNKNOWN)
                continue;
            // Skip media-dependent drives to avoid "No disk" popups on empty readers.
            if (driveType == DRIVE_REMOVABLE || driveType == DRIVE_CDROM)
                continue;

            ULARGE_INTEGER freeBytes = {}, totalBytes = {};
            if (!GetDiskFreeSpaceExW(p, NULL, &totalBytes, &freeBytes))
                continue;

            ULONGLONG used = totalBytes.QuadPart - freeBytes.QuadPart;
            int usedPct = (totalBytes.QuadPart > 0)
                ? (int)((used * 100ULL) / totalBytes.QuadPart)
                : 0;

            text += L"  ";
            text += p;
            text += L"  Total: ";
            text += FormatBytesULongLong(totalBytes.QuadPart);
            text += L"  Used: ";
            text += FormatBytesULongLong(used);
            text += L"  Free: ";
            text += FormatBytesULongLong(freeBytes.QuadPart);
            {
                wchar_t line[32] = {};
                swprintf_s(line, L"  (%d%%)\r\n", usedPct);
                text += line;
            }
        }
    }
    SetErrorMode(oldErrMode);
    text += L"\r\n";

    ULONGLONG uptimeSec = GetTickCount64() / 1000ULL;
    ULONGLONG days = uptimeSec / 86400ULL;
    ULONGLONG hours = (uptimeSec % 86400ULL) / 3600ULL;
    ULONGLONG mins = (uptimeSec % 3600ULL) / 60ULL;
    ULONGLONG secs = uptimeSec % 60ULL;
    {
        wchar_t line[256] = {};
        swprintf_s(line, L"Uptime\r\n  %llud %lluh %llum %llus\r\n\r\n", days, hours, mins, secs);
        text += line;
    }

    wchar_t computer[128] = L"";
    DWORD cchComputer = 128;
    GetComputerNameW(computer, &cchComputer);

    wchar_t user[128] = L"";
    DWORD cchUser = 128;
    GetUserNameW(user, &cchUser);

    text += L"Identity\r\n";
    text += L"  Computer: ";
    text += computer;
    text += L"\r\n";
    text += L"  User: ";
    text += user;
    text += L"\r\n";

    SetWindowTextW(g_hInfoOutput, text.c_str());
}

static BOOL EnsureDirectoryRecursive(const wchar_t* path)
{
    if (!path || !path[0]) return FALSE;

    wchar_t tmp[MAX_PATH] = {};
    wcsncpy_s(tmp, path, _TRUNCATE);
    for (wchar_t* p = tmp; *p; ++p) {
        if (*p == L'/') *p = L'\\';
    }

    for (wchar_t* p = tmp + 3; *p; ++p) {
        if (*p == L'\\') {
            wchar_t ch = *p;
            *p = L'\0';
            CreateDirectoryW(tmp, NULL);
            *p = ch;
        }
    }

    if (CreateDirectoryW(tmp, NULL)) return TRUE;
    return GetLastError() == ERROR_ALREADY_EXISTS;
}

static BOOL GetStartupFolderPath(BOOL commonFolder, wchar_t* outPath, DWORD outChars)
{
    if (!outPath || outChars == 0) return FALSE;
    outPath[0] = L'\0';
    int csidl = commonFolder ? CSIDL_COMMON_STARTUP : CSIDL_STARTUP;
    return SUCCEEDED(SHGetFolderPathW(NULL, csidl, NULL, SHGFP_TYPE_CURRENT, outPath));
}

static BOOL GetStartupBackupPath(BOOL commonFolder, wchar_t* outPath, DWORD outChars)
{
    if (!outPath || outChars == 0) return FALSE;
    wchar_t programData[MAX_PATH] = {};
    DWORD len = GetEnvironmentVariableW(L"ProgramData", programData, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) return FALSE;

    swprintf_s(outPath, outChars, L"%s\\Toolbox\\StartupDisabled\\%s",
        programData, commonFolder ? L"Common" : L"User");
    return EnsureDirectoryRecursive(outPath);
}

static void AddStartupItem(const wchar_t* name, const wchar_t* command, const wchar_t* location,
    BOOL enabled, int sourceType, HKEY regRoot, const wchar_t* regSubKey,
    const wchar_t* filePath, const wchar_t* originalPath)
{
    if (g_startupItemCount >= STARTUP_ITEM_CAPACITY) return;
    StartupItem& item = g_startupItems[g_startupItemCount++];
    wcsncpy_s(item.name, name ? name : L"", _TRUNCATE);
    wcsncpy_s(item.command, command ? command : L"", _TRUNCATE);
    wcsncpy_s(item.location, location ? location : L"", _TRUNCATE);
    item.enabled = enabled;
    item.sourceType = sourceType;
    item.regRoot = regRoot;
    wcsncpy_s(item.regSubKey, regSubKey ? regSubKey : L"", _TRUNCATE);
    wcsncpy_s(item.filePath, filePath ? filePath : L"", _TRUNCATE);
    wcsncpy_s(item.originalPath, originalPath ? originalPath : L"", _TRUNCATE);
}

static void EnumStartupRegistry(HKEY root, const wchar_t* subKey, BOOL enabled, const wchar_t* location, int sourceType)
{
    HKEY hKey = NULL;
    if (RegOpenKeyExW(root, subKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS) {
        if (RegOpenKeyExW(root, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
            return;
    }

    DWORD idx = 0;
    while (1) {
        wchar_t valueName[260] = {};
        DWORD valueNameLen = 260;
        BYTE data[2048] = {};
        DWORD dataSize = sizeof(data);
        DWORD type = 0;

        LONG rc = RegEnumValueW(hKey, idx, valueName, &valueNameLen, NULL, &type, data, &dataSize);
        if (rc != ERROR_SUCCESS)
            break;

        if ((type == REG_SZ || type == REG_EXPAND_SZ) && valueName[0]) {
            wchar_t command[1024] = {};
            wcsncpy_s(command, (wchar_t*)data, _TRUNCATE);
            if (type == REG_EXPAND_SZ) {
                wchar_t expanded[1024] = {};
                ExpandEnvironmentStringsW(command, expanded, 1024);
                wcsncpy_s(command, expanded, _TRUNCATE);
            }
            AddStartupItem(valueName, command, location, enabled, sourceType, root, subKey, L"", L"");
        }
        idx++;
    }

    RegCloseKey(hKey);
}

static void EnumStartupFolder(const wchar_t* folderPath, BOOL enabled, const wchar_t* location,
    int sourceType, const wchar_t* originalBase)
{
    if (!folderPath || !folderPath[0]) return;

    wchar_t pattern[MAX_PATH] = {};
    swprintf_s(pattern, L"%s\\*", folderPath);

    WIN32_FIND_DATAW fd = {};
    HANDLE hFind = FindFirstFileW(pattern, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;

        wchar_t fullPath[MAX_PATH] = {};
        swprintf_s(fullPath, L"%s\\%s", folderPath, fd.cFileName);

        wchar_t originalPath[MAX_PATH] = {};
        if (enabled) {
            wcsncpy_s(originalPath, fullPath, _TRUNCATE);
        }
        else if (originalBase && originalBase[0]) {
            swprintf_s(originalPath, L"%s\\%s", originalBase, fd.cFileName);
        }

        AddStartupItem(fd.cFileName, fullPath, location, enabled, sourceType,
            NULL, L"", fullPath, originalPath);
    } while (FindNextFileW(hFind, &fd));

    FindClose(hFind);
}

void LoadStartupItems(HWND hList)
{
    if (!hList) return;

    g_startupItemCount = 0;
    SendMessageW(hList, WM_SETREDRAW, FALSE, 0);
    SendMessageW(hList, LB_RESETCONTENT, 0, 0);

    const wchar_t* runKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t* disabledKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run-ToolboxDisabled";

    EnumStartupRegistry(HKEY_LOCAL_MACHINE, runKey, TRUE, L"Registry HKLM Run", STARTUP_REG_HKLM);
    EnumStartupRegistry(HKEY_CURRENT_USER, runKey, TRUE, L"Registry HKCU Run", STARTUP_REG_HKCU);
    EnumStartupRegistry(HKEY_LOCAL_MACHINE, disabledKey, FALSE, L"Registry HKLM Run-ToolboxDisabled", STARTUP_REG_HKLM);
    EnumStartupRegistry(HKEY_CURRENT_USER, disabledKey, FALSE, L"Registry HKCU Run-ToolboxDisabled", STARTUP_REG_HKCU);

    wchar_t userStartup[MAX_PATH] = {};
    wchar_t commonStartup[MAX_PATH] = {};
    wchar_t userBackup[MAX_PATH] = {};
    wchar_t commonBackup[MAX_PATH] = {};

    if (GetStartupFolderPath(FALSE, userStartup, MAX_PATH)) {
        EnumStartupFolder(userStartup, TRUE, L"Startup Folder (User)", STARTUP_FILE_USER, userStartup);
    }
    if (GetStartupFolderPath(TRUE, commonStartup, MAX_PATH)) {
        EnumStartupFolder(commonStartup, TRUE, L"Startup Folder (Common)", STARTUP_FILE_COMMON, commonStartup);
    }
    if (GetStartupBackupPath(FALSE, userBackup, MAX_PATH) && userStartup[0]) {
        EnumStartupFolder(userBackup, FALSE, L"Startup Folder Disabled (User)", STARTUP_FILE_USER, userStartup);
    }
    if (GetStartupBackupPath(TRUE, commonBackup, MAX_PATH) && commonStartup[0]) {
        EnumStartupFolder(commonBackup, FALSE, L"Startup Folder Disabled (Common)", STARTUP_FILE_COMMON, commonStartup);
    }

    for (int i = 0; i < g_startupItemCount; i++) {
        wchar_t line[1700] = {};
        swprintf_s(line, L"[%s] %s | %s | %s",
            g_startupItems[i].enabled ? L"Enabled" : L"Disabled",
            g_startupItems[i].name,
            g_startupItems[i].command,
            g_startupItems[i].location);
        SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)line);
    }

    SendMessageW(hList, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hList, NULL, TRUE);
}

static BOOL MoveRegistryStartupItem(const StartupItem* item, BOOL enable)
{
    if (!item || (item->sourceType != STARTUP_REG_HKLM && item->sourceType != STARTUP_REG_HKCU))
        return FALSE;

    const wchar_t* runKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t* disabledKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run-ToolboxDisabled";
    const wchar_t* srcSubKey = item->regSubKey[0] ? item->regSubKey : (enable ? disabledKey : runKey);
    const wchar_t* dstSubKey = enable ? runKey : disabledKey;

    HKEY hSrc = NULL;
    if (RegOpenKeyExW(item->regRoot, srcSubKey, 0, KEY_READ | KEY_WRITE | KEY_WOW64_64KEY, &hSrc) != ERROR_SUCCESS) {
        if (RegOpenKeyExW(item->regRoot, srcSubKey, 0, KEY_READ | KEY_WRITE, &hSrc) != ERROR_SUCCESS)
            return FALSE;
    }

    wchar_t valueData[2048] = {};
    DWORD type = REG_SZ;
    DWORD cb = sizeof(valueData);
    LONG rc = RegQueryValueExW(hSrc, item->name, NULL, &type, (LPBYTE)valueData, &cb);
    if (rc != ERROR_SUCCESS) {
        RegCloseKey(hSrc);
        return FALSE;
    }

    HKEY hDst = NULL;
    DWORD disp = 0;
    if (RegCreateKeyExW(item->regRoot, dstSubKey, 0, NULL, 0, KEY_WRITE | KEY_WOW64_64KEY, NULL, &hDst, &disp) != ERROR_SUCCESS) {
        if (RegCreateKeyExW(item->regRoot, dstSubKey, 0, NULL, 0, KEY_WRITE, NULL, &hDst, &disp) != ERROR_SUCCESS) {
            RegCloseKey(hSrc);
            return FALSE;
        }
    }

    rc = RegSetValueExW(hDst, item->name, 0, type, (const BYTE*)valueData, cb);
    if (rc == ERROR_SUCCESS)
        RegDeleteValueW(hSrc, item->name);

    RegCloseKey(hDst);
    RegCloseKey(hSrc);
    return (rc == ERROR_SUCCESS);
}

static BOOL MoveStartupFileItem(const StartupItem* item, BOOL enable)
{
    if (!item || (item->sourceType != STARTUP_FILE_USER && item->sourceType != STARTUP_FILE_COMMON))
        return FALSE;

    wchar_t dst[MAX_PATH] = {};
    if (enable) {
        if (!item->originalPath[0]) return FALSE;
        wcsncpy_s(dst, item->originalPath, _TRUNCATE);
    }
    else {
        wchar_t backupDir[MAX_PATH] = {};
        if (!GetStartupBackupPath(item->sourceType == STARTUP_FILE_COMMON, backupDir, MAX_PATH))
            return FALSE;

        const wchar_t* fileName = wcsrchr(item->filePath, L'\\');
        fileName = fileName ? (fileName + 1) : item->filePath;
        swprintf_s(dst, L"%s\\%s", backupDir, fileName);
    }

    return MoveFileExW(item->filePath, dst, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
}

void DisableSelectedStartupItem(HWND hWnd)
{
    int sel = (int)SendMessageW(g_hStartupList, LB_GETCURSEL, 0, 0);
    if (sel < 0 || sel >= g_startupItemCount) {
        MessageBoxW(hWnd, L"Select one startup item first.", L"Startup", MB_OK);
        return;
    }

    const StartupItem* item = &g_startupItems[sel];
    if (!item->enabled) {
        MessageBoxW(hWnd, L"This startup item is already disabled.", L"Startup", MB_OK);
        return;
    }

    BOOL ok = (item->sourceType == STARTUP_REG_HKLM || item->sourceType == STARTUP_REG_HKCU)
        ? MoveRegistryStartupItem(item, FALSE)
        : MoveStartupFileItem(item, FALSE);

    if (!ok) {
        MessageBoxW(hWnd, L"Failed to disable startup item.", L"Startup", MB_OK | MB_ICONERROR);
        return;
    }

    LoadStartupItems(g_hStartupList);
    SetWindowTextW(g_hFooterInfo, L"Startup item disabled");
}

void EnableSelectedStartupItem(HWND hWnd)
{
    int sel = (int)SendMessageW(g_hStartupList, LB_GETCURSEL, 0, 0);
    if (sel < 0 || sel >= g_startupItemCount) {
        MessageBoxW(hWnd, L"Select one startup item first.", L"Startup", MB_OK);
        return;
    }

    const StartupItem* item = &g_startupItems[sel];
    if (item->enabled) {
        MessageBoxW(hWnd, L"This startup item is already enabled.", L"Startup", MB_OK);
        return;
    }

    BOOL ok = (item->sourceType == STARTUP_REG_HKLM || item->sourceType == STARTUP_REG_HKCU)
        ? MoveRegistryStartupItem(item, TRUE)
        : MoveStartupFileItem(item, TRUE);

    if (!ok) {
        MessageBoxW(hWnd, L"Failed to enable startup item.", L"Startup", MB_OK | MB_ICONERROR);
        return;
    }

    LoadStartupItems(g_hStartupList);
    SetWindowTextW(g_hFooterInfo, L"Startup item enabled");
}

int DetectChkDskPercent(const wchar_t* text, int currentValue)
{
    if (!text) return currentValue;

    int best = currentValue;
    const wchar_t* p = text;

    while (*p) {
        while (*p && !iswdigit(*p)) p++;
        if (!*p) break;

        int value = 0;
        while (iswdigit(*p)) {
            value = value * 10 + (*p - L'0');
            if (value > 1000) break;
            p++;
        }

        while (*p == L' ' || *p == L'\t') p++;

        BOOL isPercentToken = FALSE;
        if (*p == L'%') {
            isPercentToken = TRUE;
        }
        else if (towlower(p[0]) == L'p' && towlower(p[1]) == L'e' &&
            towlower(p[2]) == L'r' && towlower(p[3]) == L'c' &&
            towlower(p[4]) == L'e' && towlower(p[5]) == L'n' &&
            towlower(p[6]) == L't') {
            isPercentToken = TRUE;
        }

        if (isPercentToken) {
            if (value > 100) value = 100;
            if (value > best) best = value;
        }
    }

    return best;
}

static BOOL QueryRegStringValue(HKEY hKey, const wchar_t* valueName, wchar_t* outBuf, DWORD outChars)
{
    if (!outBuf || outChars == 0) return FALSE;
    outBuf[0] = L'\0';
    DWORD type = 0;
    DWORD cb = outChars * sizeof(wchar_t);
    LONG rc = RegQueryValueExW(hKey, valueName, NULL, &type, (LPBYTE)outBuf, &cb);
    if (rc != ERROR_SUCCESS || (type != REG_SZ && type != REG_EXPAND_SZ)) return FALSE;
    outBuf[outChars - 1] = L'\0';
    if (type == REG_EXPAND_SZ) {
        wchar_t expanded[2048] = {};
        ExpandEnvironmentStringsW(outBuf, expanded, 2048);
        wcsncpy_s(outBuf, outChars, expanded, _TRUNCATE);
    }
    return TRUE;
}

static void AddAppEntry(const wchar_t* name, const wchar_t* uninstallCmd, const wchar_t* installLocation)
{
    if (!name || name[0] == L'\0') return;
    for (int i = 0; i < g_appCount; i++) {
        if (_wcsicmp(g_apps[i].name, name) == 0 &&
            _wcsicmp(g_apps[i].uninstallCmd, uninstallCmd ? uninstallCmd : L"") == 0 &&
            _wcsicmp(g_apps[i].installLocation, installLocation ? installLocation : L"") == 0)
            return;
    }
    if (g_appCount >= (int)(sizeof(g_apps) / sizeof(g_apps[0]))) return;
    wcsncpy_s(g_apps[g_appCount].name, name, _TRUNCATE);
    wcsncpy_s(g_apps[g_appCount].uninstallCmd, uninstallCmd ? uninstallCmd : L"", _TRUNCATE);
    wcsncpy_s(g_apps[g_appCount].installLocation, installLocation ? installLocation : L"", _TRUNCATE);
    g_appCount++;
}

static BOOL IsNoiseAppFolder(const wchar_t* folderName)
{
    if (!folderName || !folderName[0]) return TRUE;
    // Do not hide known top-level folders; user wants full Program Files visibility.
    return FALSE;
}

static BOOL IsKnownInstallPath(const wchar_t* path)
{
    if (!path || !path[0]) return FALSE;
    for (int i = 0; i < g_appCount; i++) {
        if (g_apps[i].installLocation[0] && _wcsicmp(g_apps[i].installLocation, path) == 0)
            return TRUE;
    }
    return FALSE;
}

static void ScanUnknownAppFolders(const wchar_t* rootPath, const wchar_t* sourceTag)
{
    if (!rootPath || !rootPath[0]) return;

    wchar_t pattern[MAX_PATH] = {};
    wsprintfW(pattern, L"%s\\*", rootPath);

    WIN32_FIND_DATAW fd = {};
    HANDLE hFind = FindFirstFileW(pattern, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) continue;
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;
        if (IsNoiseAppFolder(fd.cFileName)) continue;

        wchar_t fullPath[MAX_PATH] = {};
        wsprintfW(fullPath, L"%s\\%s", rootPath, fd.cFileName);
        if (IsKnownInstallPath(fullPath)) continue;

        wchar_t unknownName[320] = {};
        const wchar_t* tag = (sourceTag && sourceTag[0]) ? sourceTag : L"PrgmFolder";
        wsprintfW(unknownName, L"[%s] %s", tag, fd.cFileName);
        AddAppEntry(unknownName, L"", fullPath);
    } while (FindNextFileW(hFind, &fd));

    FindClose(hFind);
}

void FilterAppsList(HWND hList, const wchar_t* query)
{
    SendMessageW(hList, WM_SETREDRAW, FALSE, 0);
    SendMessageW(hList, LB_RESETCONTENT, 0, 0);

    wchar_t lowerQuery[260] = {};
    if (query) {
        wcsncpy_s(lowerQuery, query, _TRUNCATE);
        CharLowerW(lowerQuery);
    }

    int filteredCount = 0;
    for (int i = 0; i < g_appCount && filteredCount < (int)(sizeof(g_appsFilteredIndices) / sizeof(g_appsFilteredIndices[0])); i++) {
        wchar_t lowerName[260] = {};
        wcsncpy_s(lowerName, g_apps[i].name, _TRUNCATE);
        CharLowerW(lowerName);

        if (lowerQuery[0] == L'\0' || wcsstr(lowerName, lowerQuery) != NULL) {
            g_appsFilteredIndices[filteredCount] = i;
            filteredCount++;
            SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)g_apps[i].name);
        }
    }

    SendMessageW(hList, WM_SETREDRAW, TRUE, 0);
    InvalidateRect(hList, NULL, TRUE);
}

int GetSelectedAppIndices(HWND hList, int* indices, int maxCount)
{
    if (!indices || maxCount <= 0) return 0;

    int selCount = (int)SendMessageW(hList, LB_GETSELCOUNT, 0, 0);
    if (selCount <= 0) return 0;
    if (selCount > maxCount) selCount = maxCount;

    int listIndices[64] = {};
    SendMessageW(hList, LB_GETSELITEMS, selCount, (LPARAM)listIndices);
    for (int i = 0; i < selCount; i++)
        indices[i] = g_appsFilteredIndices[listIndices[i]];
    return selCount;
}

static BOOL RemoveAppRegistryEntries(const AppEntry* app)
{
    if (!app) return FALSE;

    const wchar_t* uninstallPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    HKEY roots[] = { HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER };
    REGSAM views[] = { KEY_WOW64_64KEY, KEY_WOW64_32KEY };
    BOOL removedAny = FALSE;

    for (int r = 0; r < 2; r++) {
        for (int v = 0; v < 2; v++) {
            HKEY hUninstall = NULL;
            if (RegOpenKeyExW(roots[r], uninstallPath, 0, KEY_READ | KEY_WRITE | views[v], &hUninstall) != ERROR_SUCCESS)
                continue;

            std::vector<std::wstring> keysToDelete;
            DWORD idx = 0;
            wchar_t subKeyName[260] = {};
            DWORD subKeyLen = 260;

            while (RegEnumKeyExW(hUninstall, idx, subKeyName, &subKeyLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
                HKEY hApp = NULL;
                if (RegOpenKeyExW(hUninstall, subKeyName, 0, KEY_READ | views[v], &hApp) == ERROR_SUCCESS) {
                    wchar_t displayName[260] = {};
                    wchar_t uninstallCmd[1024] = {};
                    wchar_t installLoc[MAX_PATH] = {};

                    QueryRegStringValue(hApp, L"DisplayName", displayName, 260);
                    QueryRegStringValue(hApp, L"UninstallString", uninstallCmd, 1024);
                    QueryRegStringValue(hApp, L"InstallLocation", installLoc, MAX_PATH);
                    RegCloseKey(hApp);

                    BOOL nameMatch = FALSE;
                    if (app->name[0] && wcsncmp(app->name, L"[Unknown] ", 10) != 0)
                        nameMatch = (_wcsicmp(displayName, app->name) == 0);

                    BOOL uninstallMatch = FALSE;
                    if (app->uninstallCmd[0])
                        uninstallMatch = (_wcsicmp(uninstallCmd, app->uninstallCmd) == 0);

                    BOOL locationMatch = FALSE;
                    if (app->installLocation[0] && installLoc[0])
                        locationMatch = (_wcsicmp(installLoc, app->installLocation) == 0);

                    if (nameMatch || uninstallMatch || locationMatch)
                        keysToDelete.push_back(subKeyName);
                }

                idx++;
                subKeyLen = 260;
            }

            for (size_t i = 0; i < keysToDelete.size(); i++) {
                if (RegDeleteTreeW(hUninstall, keysToDelete[i].c_str()) == ERROR_SUCCESS)
                    removedAny = TRUE;
            }

            RegCloseKey(hUninstall);
        }
    }

    return removedAny;
}

void LoadInstalledApps(HWND hList)
{
    g_appCount = 0;
    SendMessageW(hList, LB_RESETCONTENT, 0, 0);

    const wchar_t* uninstallPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
    HKEY roots[] = { HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER };
    REGSAM views[] = { KEY_WOW64_64KEY, KEY_WOW64_32KEY };

    for (int r = 0; r < 2; r++) {
        for (int v = 0; v < 2; v++) {
            HKEY hUninstall = NULL;
            if (RegOpenKeyExW(roots[r], uninstallPath, 0, KEY_READ | views[v], &hUninstall) != ERROR_SUCCESS)
                continue;

            DWORD idx = 0;
            wchar_t subKeyName[260];
            DWORD subKeyLen = 260;
            while (RegEnumKeyExW(hUninstall, idx, subKeyName, &subKeyLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
                HKEY hApp = NULL;
                if (RegOpenKeyExW(hUninstall, subKeyName, 0, KEY_READ | views[v], &hApp) == ERROR_SUCCESS) {
                    wchar_t displayName[260] = {};
                    wchar_t uninstallCmd[1024] = {};
                    wchar_t installLoc[MAX_PATH] = {};
                    BOOL hasName = QueryRegStringValue(hApp, L"DisplayName", displayName, 260);
                    QueryRegStringValue(hApp, L"UninstallString", uninstallCmd, 1024);
                    QueryRegStringValue(hApp, L"InstallLocation", installLoc, MAX_PATH);
                    if (hasName) {
                        AddAppEntry(displayName, uninstallCmd, installLoc);
                    }
                    RegCloseKey(hApp);
                }
                idx++;
                subKeyLen = 260;
            }
            RegCloseKey(hUninstall);
        }
    }

    wchar_t programFiles[MAX_PATH] = {};
    wchar_t programFilesX86[MAX_PATH] = {};
    wchar_t programData[MAX_PATH] = {};

    DWORD len = GetEnvironmentVariableW(L"ProgramFiles", programFiles, MAX_PATH);
    if (len > 0 && len < MAX_PATH)
        ScanUnknownAppFolders(programFiles, L"Prgm64");

    len = GetEnvironmentVariableW(L"ProgramFiles(x86)", programFilesX86, MAX_PATH);
    if (len > 0 && len < MAX_PATH)
        ScanUnknownAppFolders(programFilesX86, L"Prgm32");

    len = GetEnvironmentVariableW(L"ProgramData", programData, MAX_PATH);
    if (len > 0 && len < MAX_PATH)
        ScanUnknownAppFolders(programData, L"PrgmData");

    if (programFiles[0]) {
        wchar_t windowsAppsPath[MAX_PATH] = {};
        wsprintfW(windowsAppsPath, L"%s\\WindowsApps", programFiles);
        ScanUnknownAppFolders(windowsAppsPath, L"WindowsApps");
    }

    wchar_t currentQuery[260] = {};
    if (g_hAppsSearch)
        GetWindowTextW(g_hAppsSearch, currentQuery, 259);
    FilterAppsList(hList, currentQuery);
}

void RunBasicUninstall(HWND hWnd)
{
    int sel = (int)SendMessageW(g_hAppsList, LB_GETCURSEL, 0, 0);
    if (sel < 0) {
        MessageBoxW(hWnd, L"Select an app first.", L"Apps", MB_OK);
        return;
    }

    // Map listbox position through filtered indices
    int appIndex = g_appsFilteredIndices[sel];
    if (appIndex < 0 || appIndex >= g_appCount) {
        MessageBoxW(hWnd, L"Select an app first.", L"Apps", MB_OK);
        return;
    }
    if (!g_apps[appIndex].uninstallCmd[0]) {
        MessageBoxW(hWnd, L"This app has no uninstall command in registry.", L"Apps", MB_OK | MB_ICONWARNING);
        return;
    }

    wchar_t cmd[2048] = {};
    wcsncpy_s(cmd, g_apps[appIndex].uninstallCmd, _TRUNCATE);
    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    if (!CreateProcessW(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        wchar_t viaCmd[2300] = {};
        wsprintfW(viaCmd, L"cmd.exe /C %s", g_apps[appIndex].uninstallCmd);
        if (!CreateProcessW(NULL, viaCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            MessageBoxW(hWnd, L"Failed to launch uninstaller.", L"Apps", MB_OK | MB_ICONERROR);
            return;
        }
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    SetWindowTextW(g_hFooterInfo, L"Uninstaller launched");
}

static std::wstring ExtractExePathFromCommand(const wchar_t* cmd)
{
    if (!cmd || !cmd[0]) return L"";
    const wchar_t* p = cmd;
    while (*p == L' ' || *p == L'\t') p++;
    if (*p == L'"') {
        p++;
        const wchar_t* q = wcschr(p, L'"');
        if (q) return std::wstring(p, q - p);
    }
    const wchar_t* q = p;
    while (*q && *q != L' ' && *q != L'\t') q++;
    return std::wstring(p, q - p);
}

static void PostAppsForceLine(HWND hWnd, const std::wstring& line)
{
    size_t n = line.size() + 3;
    wchar_t* buf = (wchar_t*)malloc(n * sizeof(wchar_t));
    if (!buf) return;
    wcscpy_s(buf, n, line.c_str());
    wcscat_s(buf, n, L"\r\n");
    PostMessage(hWnd, WM_APPS_FORCE_APPEND, 0, (LPARAM)buf);
}

static void PostAppsForceStatus(HWND hWnd, const std::wstring& text)
{
    size_t n = text.size() + 1;
    wchar_t* buf = (wchar_t*)malloc(n * sizeof(wchar_t));
    if (!buf) return;
    wcscpy_s(buf, n, text.c_str());
    PostMessage(hWnd, WM_APPS_FORCE_STATUS, 0, (LPARAM)buf);
}

static void UpdateAppsForceModeUI(BOOL running)
{
    if (!g_hAppsList || !g_hAppsSearch || !g_hAppsForceOutput || !g_hAppsForceProgress) return;

    if (running) {
        ShowWindow(g_hAppsSearch, SW_HIDE);
        ShowWindow(g_hAppsList, SW_HIDE);
        ShowWindow(g_hAppsRefreshBtn, SW_HIDE);
        ShowWindow(g_hAppsBasicUninstBtn, SW_HIDE);
        ShowWindow(g_hAppsForceUninstBtn, SW_HIDE);

        // Replace Apps list area with a large progress log view.
        MoveWindow(g_hAppsForceOutput, 24, 74, 452, 250, TRUE);
        MoveWindow(g_hAppsForceProgress, 24, 332, 452, 18, TRUE);
        ShowWindow(g_hAppsForceOutput, SW_SHOW);
        ShowWindow(g_hAppsForceProgress, SW_SHOW);
    }
    else {
        // Restore default layout.
        MoveWindow(g_hAppsForceProgress, 24, 338, 452, 18, TRUE);
        MoveWindow(g_hAppsForceOutput, 24, 364, 452, 34, TRUE);
        ShowWindow(g_hAppsForceOutput, SW_HIDE);
        ShowWindow(g_hAppsForceProgress, SW_HIDE);

        if (TabCtrl_GetCurSel(g_hTab) == 3) {
            ShowWindow(g_hAppsSearch, SW_SHOW);
            ShowWindow(g_hAppsList, SW_SHOW);
            ShowWindow(g_hAppsRefreshBtn, SW_SHOW);
            ShowWindow(g_hAppsBasicUninstBtn, SW_SHOW);
            ShowWindow(g_hAppsForceUninstBtn, SW_SHOW);
        }
        EnableWindow(g_hAppsForceUninstBtn, TRUE);
    }
}

static void RunHiddenTaskkill(const std::wstring& exeName)
{
    wchar_t cmdLine[1024] = {};
    wsprintfW(cmdLine, L"cmd.exe /C taskkill /f /im \"%s\" >nul 2>&1", exeName.c_str());
    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    if (CreateProcessW(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, 5000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

static void CollectFilesAndDirs(const std::wstring& dir,
    std::vector<std::wstring>& files,
    std::vector<std::wstring>& dirs,
    std::set<std::wstring>& exeNames)
{
    std::wstring pattern = dir + L"\\*";
    WIN32_FIND_DATAW fd = {};
    HANDLE hFind = FindFirstFileW(pattern.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;
        std::wstring full = dir + L"\\" + fd.cFileName;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CollectFilesAndDirs(full, files, dirs, exeNames);
            dirs.push_back(full);
        }
        else {
            files.push_back(full);
            const wchar_t* ext = wcsrchr(fd.cFileName, L'.');
            if (ext && _wcsicmp(ext, L".exe") == 0)
                exeNames.insert(fd.cFileName);
        }
    } while (FindNextFileW(hFind, &fd));

    FindClose(hFind);
}

void StartForceUninstall(HWND hWnd)
{
    if (g_bAppsForceRunning) return;

    int indices[64] = {};
    int count = GetSelectedAppIndices(g_hAppsList, indices, 64);
    if (count <= 0) {
        MessageBoxW(hWnd, L"Select one or more apps first.", L"Apps", MB_OK);
        return;
    }

    if (MessageBoxW(hWnd,
        L"Force remove will attempt to kill EXEs under each selected app folder and delete files/folders recursively.\r\n\r\nContinue?",
        L"Force Uninstall Warning", MB_YESNO | MB_ICONWARNING) != IDYES)
        return;

    ForceUninstallParam* p = (ForceUninstallParam*)malloc(sizeof(ForceUninstallParam));
    if (!p) return;
    p->hWnd = hWnd;
    p->count = count;
    for (int i = 0; i < count; i++)
        p->indices[i] = indices[i];

    g_bAppsForceRunning = TRUE;
    UpdateAppsForceModeUI(TRUE);
    SetWindowTextW(g_hAppsForceOutput, L"");
    SendMessage(g_hAppsForceProgress, PBM_SETPOS, 0, 0);
    PostAppsForceLine(hWnd, L"Force uninstall started.");
    PostAppsForceStatus(hWnd, L"Preparing force uninstall...");
    CreateThread(NULL, 1024 * 1024, ForceUninstallThread, p, 0, NULL);
}

DWORD WINAPI ForceUninstallThread(LPVOID lpParam)
{
    ForceUninstallParam* p = (ForceUninstallParam*)lpParam;
    HWND hWnd = p->hWnd;
    int totalApps = p->count;
    int registryDone = 0;
    unsigned long long totalFilesDiscovered = 0;
    unsigned long long deletedFiles = 0;

    for (int appIndex = 0; appIndex < p->count; appIndex++) {
        int sel = p->indices[appIndex];
        if (sel < 0 || sel >= g_appCount) continue;

        const AppEntry& app = g_apps[sel];
        {
            wchar_t regPhase[128] = {};
            swprintf_s(regPhase, L"(Cleaning Registry %d of %d)", registryDone, totalApps);
            PostAppsForceLine(hWnd, regPhase);
            PostAppsForceStatus(hWnd, regPhase);
        }

        if (RemoveAppRegistryEntries(&app)) {
            PostAppsForceLine(hWnd, std::wstring(L"Clean Registry: ") + app.name + L" -> removed matching entries");
        }
        else {
            PostAppsForceLine(hWnd, std::wstring(L"Clean Registry: ") + app.name + L" -> no matching uninstall registry entry");
        }

        registryDone++;
        {
            wchar_t regPhase[128] = {};
            swprintf_s(regPhase, L"(Cleaning Registry %d of %d)", registryDone, totalApps);
            PostAppsForceLine(hWnd, regPhase);
            PostAppsForceStatus(hWnd, regPhase);
        }

        std::wstring baseDir;
        if (app.installLocation[0])
            baseDir = app.installLocation;
        if (baseDir.empty()) {
            std::wstring exePath = ExtractExePathFromCommand(app.uninstallCmd);
            size_t pos = exePath.find_last_of(L"\\/");
            if (pos != std::wstring::npos)
                baseDir = exePath.substr(0, pos);
        }

        if (baseDir.empty()) {
            PostAppsForceLine(hWnd, std::wstring(L"[") + app.name + L"] Cannot determine install directory.");
            continue;
        }

        DWORD attr = GetFileAttributesW(baseDir.c_str());
        if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
            PostAppsForceLine(hWnd, std::wstring(L"[") + app.name + L"] Install directory does not exist.");
            continue;
        }

        PostAppsForceLine(hWnd, std::wstring(L"Target: ") + app.name + L" -> " + baseDir);
        PostAppsForceLine(hWnd, L"Scanning files...");

        std::vector<std::wstring> files;
        std::vector<std::wstring> dirs;
        std::set<std::wstring> exeNames;
        CollectFilesAndDirs(baseDir, files, dirs, exeNames);
        totalFilesDiscovered += (unsigned long long)files.size();

        wchar_t scanMsg[256] = {};
        wsprintfW(scanMsg, L"Found %u files, %u folders, %u executables.",
            (unsigned)files.size(), (unsigned)dirs.size(), (unsigned)exeNames.size());
        PostAppsForceLine(hWnd, scanMsg);

        for (std::set<std::wstring>::const_iterator it = exeNames.begin(); it != exeNames.end(); ++it) {
            PostAppsForceLine(hWnd, std::wstring(L"taskkill /f /im ") + *it);
            RunHiddenTaskkill(*it);
        }

        for (size_t i = 0; i < files.size(); i++) {
            SetFileAttributesW(files[i].c_str(), FILE_ATTRIBUTE_NORMAL);
            if (DeleteFileW(files[i].c_str())) {
                deletedFiles++;
                PostAppsForceLine(hWnd, std::wstring(L"Deleted: ") + files[i]);
            }
            else {
                PostAppsForceLine(hWnd, std::wstring(L"Delete failed: ") + files[i]);
            }

            int pct = 0;
            if (totalFilesDiscovered > 0) {
                unsigned long long raw = (deletedFiles * 100ULL) / totalFilesDiscovered;
                if (raw > 100ULL) raw = 100ULL;
                pct = (int)raw;
            }
            PostMessage(hWnd, WM_APPS_FORCE_PROGRESS, (WPARAM)pct, 0);

            wchar_t deletingStatus[160] = {};
            swprintf_s(deletingStatus, L"Deleting... %llu of %llu completed",
                deletedFiles, totalFilesDiscovered);
            PostAppsForceStatus(hWnd, deletingStatus);
        }

        for (size_t i = 0; i < dirs.size(); i++) {
            SetFileAttributesW(dirs[i].c_str(), FILE_ATTRIBUTE_NORMAL);
            RemoveDirectoryW(dirs[i].c_str());
        }
        RemoveDirectoryW(baseDir.c_str());
    }

    PostMessage(hWnd, WM_APPS_FORCE_PROGRESS, 100, 0);
    {
        wchar_t doneStatus[160] = {};
        swprintf_s(doneStatus, L"Deleting... %llu of %llu completed", deletedFiles, totalFilesDiscovered);
        PostAppsForceStatus(hWnd, doneStatus);
    }
    PostAppsForceLine(hWnd, L"Force remove completed.");
    PostMessage(hWnd, WM_APPS_FORCE_DONE, 0, 0);

    free(p);
    return 0;
}

// 创建 %temp%\Toolbox\Downloads 目录结构
void EnsureDownloadsFolder()
{
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);

    wchar_t toolboxPath[MAX_PATH];
    wsprintfW(toolboxPath, L"%sToolbox", tempPath);

    // 创建 Toolbox 目录
    CreateDirectoryW(toolboxPath, NULL);

    // 创建 Toolbox\Downloads 目录
    wsprintfW(g_downloadsFolderPath, L"%s\\Downloads", toolboxPath);
    CreateDirectoryW(g_downloadsFolderPath, NULL);

    // 写入清理下载目录的批处理文件
    wchar_t batPath[MAX_PATH] = {};
    wsprintfW(batPath, L"%s\\[Option] Delete all downloads.bat", g_downloadsFolderPath);

    const char deleteScript[] =
        "@echo off\r\n"
        "@title Deleting executables\r\n"
        "@echo Deleting executables.\r\n"
        "@del *.exe /f 2>nul 1>nul\r\n"
        "@title Deleting archives\r\n"
        "@echo Deleting archives.\r\n"
        "@del *.zip /f 2>nul 1>nul\r\n"
        "@title Deleting installers\r\n"
        "@echo Deleting installers.\r\n"
        "@del *.msi /f 2>nul 1>nul\r\n"
        "@echo Operation completed.\r\n"
        "@echo.\r\n"
        "@pause\r\n";

    HANDLE hBat = CreateFileW(
        batPath,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hBat != INVALID_HANDLE_VALUE) {
        DWORD written = 0;
        WriteFile(hBat, deleteScript, (DWORD)(sizeof(deleteScript) - 1), &written, NULL);
        CloseHandle(hBat);
    }
}

// 获取临时下载路径
void GetTempDownloadPath(const wchar_t* filename, wchar_t* outPath, int outSize)
{
    // 从 URL 中提取文件名，如果没有就用默认
    const wchar_t* lastSlash = wcsrchr(filename, L'/');
    if (lastSlash && *(lastSlash + 1))
        wsprintfW(outPath, L"%s\\%s", g_downloadsFolderPath, lastSlash + 1);
    else
        wsprintfW(outPath, L"%s\\download.exe", g_downloadsFolderPath);
}

// ima handle the progress correctly before DownloadThread
class CDownloadProgress : public IBindStatusCallback {
public:
    HWND m_hProgress;
    HWND m_hStats;
    ULONGLONG m_startTime;
    ULONGLONG m_lastSpeedTick;
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
        : m_hProgress(hProg), m_hStats(hStats), m_startTime(GetTickCount64()),
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

        ULONGLONG nowTick = GetTickCount64();
        if (nowTick > m_lastSpeedTick && m_progress64 >= m_lastSpeedProgress64) {
            ULONGLONG dt = nowTick - m_lastSpeedTick;
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
                ULONGLONG elapsed = GetTickCount64() - m_startTime;
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
                ULONGLONG elapsed = GetTickCount64() - m_startTime;
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

