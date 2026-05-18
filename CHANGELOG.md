# Changelog

All notable changes to Windows Toolbox will be documented in this file.

---

## [26.5.6] - 2026-05-10 16:41

### Added
- **Apps Manager** tab - List installed applications from registry
- **Basic Uninstall** - Launch standard uninstaller
- **Force Uninstall** - Aggressive removal (kill processes + delete files/folders + remove registry)
- **System Info** tab - OS, CPU, memory, disk, uptime, identity
- **Startup Manager** tab - Enable/disable startup items
- **Startup backup** - Disabled items moved to `%ProgramData%\Toolbox\StartupDisabled\`
- **Activation Helper** tab - Windows product key installer
- 8 pre-configured Windows keys (Windows 7 through 11)
- Search/filter functionality for Apps tab
- Program folder scanning for unknown applications (Program Files, ProgramData, WindowsApps)

### Changed
- Version increment from 26.5.5 to 26.5.6

---

## [26.5.5] - 2026-05-09 18:29

### Added
- **ChkDsk GUI** tab - Graphical frontend for Windows Check Disk
- `/F`, `/R`, `/X` checkbox options
- Real-time output capture via anonymous pipe
- Automatic Y/N response handling (`Y\r\n`)
- Progress percentage detection from console output
- Exit code reporting

---

## [26.5.4] - 2026-05-09 02:15

### Added
- **Interrupt button** - Cancel active downloads
- Window size adjustments for better visibility

### Changed
- Window dimensions optimized for content

---

## [26.5.3] - 2026-05-08 18:20

### Fixed
- Switched from Debug to Release build configuration
- No longer requires Visual Studio Debugger Runtime DLLs

---

## [26.5.2] - 2026-05-08 18:18

### Changed
- Version numbering scheme: `YY.MM.PATCH` (26.5.2 = May 2026, patch 2)
- Previously: 1.0 / 1.1

---

## [26.5.1] - 2026-05-08 17:05

### Added
- Tab control interface
- Progress bar with download tracking
- ETA and speed display
- Settings tab (placeholder)
- Download statistics (KB/s, MB/s, ETA)

### Changed
- UI layout restructured for tabs

---

## [26.5.0] - 2026-05-07 18:34

### Added
- Initial release: "Tool Downloader"
- Single tool: FinalShell
- Download and Download & Execute modes
- Basic program list

---

[26.5.6]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.6
[26.5.5]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.5
[26.5.4]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.4
[26.5.3]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.3
[26.5.2]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.2
[26.5.1]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.1
[26.5.0]: https://github.com/lqx211/WindowsToolbox/releases/tag/26.5.0
```

---

## Quick checklist for your repo:

- [ ] Create `README.md` with above content
- [ ] Create `CHANGELOG.md` with above content
- [ ] Create `LICENSE` file (or add license statement to README)
- [ ] Add screenshots (replace placeholder text)
- [ ] Attach `WindowsToolbox.exe` to the release (drag into binaries section)
