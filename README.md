# 🪟 Windows Toolbox

**A comprehensive Windows system utility - Download Manager, ChkDsk GUI, Apps Manager, Startup Manager, System Info & Activation Helper**

![Version](https://img.shields.io/badge/version-26.5.6-blue)
![Windows](https://img.shields.io/badge/Windows-7%2B-brightgreen)
![Platform](https://img.shields.io/badge/platform-x86%20%7C%20x64-blue)
![Visual Studio](https://img.shields.io/badge/VS-2019-purple)

---

## 📋 Table of Contents

- [Features](#-features)
- [Screenshots](#-screenshots)
- [Download](#-download)
- [System Requirements](#-system-requirements)
- [Building from Source](#-building-from-source)
- [Version History](#-version-history)
- [License](#-license)
- [Disclaimer](#-disclaimer)

---

## ✨ Features

| Tab | Module | Description |
|-----|--------|-------------|
| 📥 | **Download Manager** | Download tools from curated catalog (130+ entries) with real-time progress, speed display, ETA calculation, and interrupt support |
| ⚙️ | **Settings** | (Placeholder for future features) |
| 🔧 | **ChkDsk GUI** | Graphical interface for Windows Check Disk with `/F`, `/R`, `/X` options and real-time output capture |
| 📦 | **Apps Manager** | List installed applications with basic uninstall and **force uninstall** (kills processes + deletes folders + removes registry) |
| ℹ️ | **System Info** | Display OS version, CPU architecture, physical cores, memory stats, disk usage, uptime, and user/computer identity |
| 🚀 | **Startup Manager** | Enable/disable startup items (registry + Startup Folder) with automatic backup support |
| 🔑 | **Activation Helper** | Windows product key installer with 8 pre-configured keys (Windows 7-11) |

---

## 📸 Screenshots

*I will add screenshots soon... Sorry*

| Download Tab | Apps Tab | Info Tab |
|--------------|----------|----------|
| *[screenshot]* | *[screenshot]* | *[screenshot]* |

---

## 📥 Download

### Latest Release: **v26.5.6**

Get the executable from the [Releases page](https://github.com/lqx211/WindowsToolbox/releases/latest).

**Direct Download:** [WindowsToolbox.exe](https://github.com/lqx211/WindowsToolbox/releases/download/26.5.6/WindowsToolbox.exe)

> ⚠️ **Note:** Administrator privileges recommended for full functionality (ChkDsk, Force Uninstall, Startup Management).

---

## 🖥️ System Requirements

| Component | Requirement |
|-----------|-------------|
| **OS** | Windows 7 or later |
| **Architecture** | x86 or x64 |
| **Runtime** | No additional runtime required (static linking) |
| **Disk Space** | ~2 MB |
| **Privileges** | Standard user for download; Admin for advanced features |

---

## 🛠️ Building from Source

### Prerequisites

- **Visual Studio 2019** (or newer)
- **Windows SDK** (included with VS)
- **C++17** support

### Build Steps

```bash
# Clone the repository
git clone https://github.com/lqx211/WindowsToolbox.git
cd WindowsToolbox

# Open in Visual Studio
start WindowsToolbox.sln

# Build → Build Solution (Ctrl+Shift+B)
# Output: Win32/Release/WindowsToolbox.exe
```

### Dependencies (all included or Windows-native)

| Library | Purpose |
|---------|---------|
| `windows.h` | Win32 API |
| `commctrl.h` | Common controls (tab, progress bar) |
| `urlmon.h` | URL download (`URLDownloadToFileW`) |
| `shlobj.h` | Shell folders (Startup paths) |
| `comctl32.lib` | Common controls library |
| `urlmon.lib` | URL moniker library |
| `shell32.lib` | Shell operations |

---

## 📜 Version History

| Version | Date | Changes |
|---------|------|---------|
| **26.5.6** | May 2026 | Added Apps Manager, System Info, Startup Manager, Activation Helper; Force uninstall support |
| 26.5.5 | May 2026 | Added ChkDsk GUI with real-time output |
| 26.5.4 | May 2026 | Window resize; Interrupt button for downloads |
| 26.5.3 | May 2026 | Release build; No debug runtime required |
| 26.5.2 | May 2026 | Switched to YY.MM.PATCH versioning |
| 26.5.1 | May 2026 | Menus, progress bar, ETA display, Settings tab |
| 26.5.0 | May 2026 | Initial release (Tool Downloader) |

Full changelog: [CHANGELOG.md](CHANGELOG.md)
( Does it even exist? )

---

## 📦 Download Catalog (130+ entries)

Includes popular tools across categories:

- **System Utilities**: Rufus, Ventoy, Dism++, Everything, Core Temp
- **Development**: Visual Studio (2017-2026), VS Code, Git, Node.js, Python, PyCharm
- **Communication**: Discord, QQ, Weixin
- **Media**: VLC, PotPlayer, OBS Studio, MuseScore
- **Security**: Wireshark, Huorong Antivirus
- **Virtualization**: VMware Workstation, Docker Desktop, QEMU
- **Remote Access**: Jump Desktop, Xmanager, FinalShell

*See source code for complete list.*

---

## ⚠️ Disclaimer

**THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED.**

- **Force Uninstall** can permanently delete data - use at your own risk
- The author assumes **no responsibility** for misuse, data loss, or system damage
- Administrator privileges are required for some features; understand what you're doing

---

## 🙏 Acknowledgments

- Original "Tool Downloader" concept (source code lost to history)
- All open-source tools included in the download catalog
- Windows SDK documentation

---

## 📞 Support & Contact

- **GitHub Issues**: [Report bugs or request features](https://github.com/lqx211/WindowsToolbox/issues)
- **Author**: [@lqx211](https://github.com/lqx211)

---

**Built with ❤️ and C++17 on Visual Studio 2019**
