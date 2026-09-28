---
title: "CDP 别名配置 CLI"
weight: 5
description: "cdp 目录别名工具的 CMD 与 PowerShell 实现"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/cdp别名配置cli.svg"
---

`cdp` 是一个目录别名工具，用一个短单词代替完整路径。例如输入 `cdp dev` 即可跳转到项目目录，无需记忆和输入完整路径。

# 一、文件清单

| 文件            | 路径                                      | 作用               |
| ------------- | --------------------------------------- | ---------------- |
| CMD 实现        | `F:\ProgramFilesNoMove\WezTerm\cdp.bat` | CMD 环境下使用        |
| PowerShell 实现 | `$PROFILE`                              | PowerShell 环境下使用 |

# 二、CMD 实现：cdp.bat

**路径：** `F:\ProgramFilesNoMove\WezTerm\cdp.bat`

```batch
@echo off
if "%~1"=="dev" cd /d "E:\Projects\devenv4miniprogram.pc\Frgsys.wechat.dev" & goto :eof
if "%~1"=="classes" cd /d "E:\Projects\devenv4app.pc\Frgsys.v2\Frgsys64\Classes" & goto :eof
if "%~1"=="desktop" cd /d "C:\Users\ShenLan-Wai\Desktop" & goto :eof
echo Usage: cdp ^<alias^>
echo Aliases: dev classes desktop
```

## (一) 结构说明

    if "%~1"=="别名" cd /d "目标路径" & goto :eof
                 │          │              │
                 │          │              └── 跳转成功后退出，不再匹配后续别名
                 │          └── /d 支持跨盘符跳转
                 └── %~1 去掉引号后的第一个参数

## (二) 使用方式

```cmd
cdp dev           # → E:\Projects\devenv4miniprogram.pc\Frgsys.wechat.dev
cdp classes       # → E:\Projects\devenv4app.pc\Frgsys.v2\Frgsys64\Classes
cdp desktop       # → C:\Users\ShenLan-Wai\Desktop
```

## (三) 注册为全局命令

将 `F:\ProgramFilesNoMove\WezTerm` 添加到系统 PATH 环境变量后，在任意目录下均可直接使用 `cdp` 别名。或将 `cdp.bat` 复制到已在 PATH 中的目录。

## (四) 添加新别名

按格式在 `goto :eof` 行之前插入：

```batch
if "%~1"=="新别名" cd /d "目标路径" & goto :eof
```

# 三、PowerShell 实现：Profile 函数

**路径：** `C:\Users\ShenLan-Wai\Documents\WindowsPowerShell\Microsoft.PowerShell_profile.ps1`（PowerShell 5）
或 `C:\Users\ShenLan-Wai\Documents\PowerShell\Microsoft.PowerShell_profile.ps1`（PowerShell 7）

```powershell
# cdp — 快捷进入目的目录（别名跳转）
function cdp {
    switch ($args[0]) {
        'dev'     { Set-Location 'E:\Projects\devenv4miniprogram.pc\Frgsys.wechat.dev' }
        'classes' { Set-Location 'E:\Projects\devenv4app.pc\Frgsys.v2\Frgsys64\Classes' }
        'desktop' { Set-Location 'C:\Users\ShenLan-Wai\Desktop' }
        default   { Write-Host "Usage: cdp <alias>"; Write-Host "Aliases: dev classes desktop" }
    }
}
```

## (一) 首次配置 Profile

```powershell
# 创建 Profile（如果不存在）
if (-not (Test-Path $PROFILE)) { New-Item -Path $PROFILE -Force }

# 编辑 Profile
notepad $PROFILE

# 粘贴上述 cdp 函数后保存，然后重新加载
. $PROFILE
```

## (二) 使用方式

与 CMD 完全一致：

```powershell
cdp dev           # → E:\Projects\devenv4miniprogram.pc\Frgsys.wechat.dev
cdp classes       # → E:\Projects\devenv4app.pc\Frgsys.v2\Frgsys64\Classes
cdp desktop       # → C:\Users\ShenLan-Wai\Desktop
```

## (三) 添加新别名

在 switch 语句中添加：

```powershell
'新别名' { Set-Location '目标路径' }
```

# 四、别名与路径对照

| 别名        | 目标路径                                                   |
| --------- | ------------------------------------------------------ |
| `dev`     | `E:\Projects\devenv4miniprogram.pc\Frgsys.wechat.dev`  |
| `classes` | `E:\Projects\devenv4app.pc\Frgsys.v2\Frgsys64\Classes` |
| `desktop` | `C:\Users\ShenLan-Wai\Desktop`                         |

> 新增项目目录时，同时在 `cdp.bat` 和 PowerShell Profile 中添加对应的别名即可。
