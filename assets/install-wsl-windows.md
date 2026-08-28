# Installing WSL (Windows Subsystem for Linux) on Windows

## Quick Install (Windows 10 version 2004+ and Windows 11)

Open **PowerShell** or **Command Prompt as Administrator** and run:

```powershell
wsl --install
```

This single command enables the required features, installs the WSL kernel, and installs Ubuntu as the default Linux distribution. Restart your computer when prompted.

After restarting, a terminal window opens and prompts you to create a **username** and **password** for your Linux environment.

## Install a Specific Distribution

See available distributions:

```powershell
wsl --list --online
```

Install a specific one (e.g., Debian):

```powershell
wsl --install -d Debian
```

## Common Commands

```powershell
wsl --list --verbose          # list installed distros and their WSL version
wsl --set-default-version 2   # set WSL 2 as default
wsl --update                  # update the WSL kernel
wsl --shutdown                # shut down all running distros
wsl -d Ubuntu                 # launch a specific distro
```

## Manual Install (older Windows 10 builds)

If `wsl --install` isn't available, enable the features manually in an Administrator PowerShell:

```powershell
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
```

Restart, then download and run the [WSL2 kernel update package](https://learn.microsoft.com/windows/wsl/install-manual), set WSL 2 as default (`wsl --set-default-version 2`), and install a distro from the Microsoft Store.

## Requirements

- Windows 10 version 2004 or higher (Build 19041+), or Windows 11
- Virtualization enabled in BIOS/UEFI (needed for WSL 2)

## Verify

```powershell
wsl --version
```

Once installed, launch it anytime by typing `wsl` in a terminal or opening your distro (e.g., "Ubuntu") from the Start menu.
