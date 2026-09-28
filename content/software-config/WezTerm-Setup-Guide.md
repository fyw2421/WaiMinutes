---
title: "WezTerm 安装与配置指南"
weight: 7
description: "Windows 上 Cmder 风格的单实例多标签终端"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/WezTerm-Setup-Guide.svg"
---

> 目标：在 Windows 上实现 Cmder 风格的单实例多标签终端体验——右键文件夹在同一个 WezTerm 窗口新建标签，双击 `.bat` 文件在 WezTerm 中执行。

***

# 一、目录结构

    F:\ProgramFilesNoMove\WezTerm\
    ├── wezterm.exe              # 主程序（CLI + GUI）
    ├── wezterm-gui.exe          # GUI 客户端
    ├── wezterm-mux-server.exe   # 多路复用后台服务
    ├── wezterm-spawn.cmd        # 右键菜单入口（批处理包装）
    └── wezterm-spawn.ps1        # 核心生成脚本（PowerShell）

    C:\Users\ShenLan-Wai\
    └── .wezterm.lua             # WezTerm 用户配置

***

## (一) 架构原理

    ┌────────────────────────────────────────────┐
    │           WezTerm GUI 窗口（唯一）           │
    │  ┌──────┬──────┬──────┬──────┐             │
    │  │ 标签1 │ 标签2 │ 标签3 │ 标签4 │             │
    │  └──────┴──────┴──────┴──────┘             │
    │                  ▲                          │
    └──────────────────┼──────────────────────────┘
                       │ connect unix
               ┌───────┴───────┐
               │  Mux Server   │  ← 后台进程，管理所有标签
               └───────┬───────┘
                       ▲
         ┌─────────────┼─────────────┐
         │             │             │
      右键文件夹A   右键文件夹B   双击 .bat

*   **Mux Server**（`wezterm-mux-server.exe`）：后台进程，管理所有标签页和面板
*   **GUI Client**（`wezterm-gui.exe`）：连接到 Mux Server 显示窗口
*   **Spawn Script**（`wezterm-spawn.ps1`）：通过 `wezterm cli spawn` 在 Mux 中创建标签

关键配置：

*   `unix_domains` — 启用多路复用，所有实例共享同一个 Mux Server
*   `default_domain = 'unix'` — 默认连接到 Mux 域，避免产生独立的本地域窗口

***

## (二) 安装步骤

### 1. 下载 WezTerm

从 [WezTerm Releases](https://github.com/wezterm/wezterm/releases) 下载 Windows nightly 版本，解压到 `F:\ProgramFilesNoMove\WezTerm\`。

### 2. 部署配置文件

将 `.wezterm.lua` 放到 `C:\Users\ShenLan-Wai\`（用户主目录）。

将 `wezterm-spawn.cmd` 和 `wezterm-spawn.ps1` 放到 WezTerm 安装目录。

### 3. 注册右键菜单

以**管理员**身份运行 PowerShell，执行以下命令：

```powershell
$weztermDir = "F:\ProgramFilesNoMove\WezTerm"

# 文件夹右键菜单
New-Item -Path "Registry::HKEY_CLASSES_ROOT\Directory\shell\Open WezTerm here\command" -Force | Out-Null
Set-ItemProperty -Path "Registry::HKEY_CLASSES_ROOT\Directory\shell\Open WezTerm here\command" -Name "(default)" -Value "\"$weztermDir\wezterm-spawn.cmd\" --cwd `"%V`""

# 文件夹空白处右键菜单
New-Item -Path "Registry::HKEY_CLASSES_ROOT\Directory\Background\shell\Open WezTerm here\command" -Force | Out-Null
Set-ItemProperty -Path "Registry::HKEY_CLASSES_ROOT\Directory\Background\shell\Open WezTerm here\command" -Name "(default)" -Value "\"$weztermDir\wezterm-spawn.cmd\" --cwd `"%V`""
```

### 4. 关联 .bat 文件

```powershell
# .bat 文件双击 → 在 WezTerm 中执行
New-Item -Path "Registry::HKEY_CURRENT_USER\Software\Classes\batfile\shell\open\command" -Force | Out-Null
Set-ItemProperty -Path "Registry::HKEY_CURRENT_USER\Software\Classes\batfile\shell\open\command" -Name "(default)" -Value "\"$weztermDir\wezterm-spawn.cmd\" --bat `"%1`" %*"
```

### 5. 当前注册表状态

| 注册表路径                                                       | 值                                          |
| ----------------------------------------------------------- | ------------------------------------------ |
| `HKCR\Directory\shell\Open WezTerm here\command`            | `"F:\...\wezterm-spawn.cmd" --cwd "%V"`    |
| `HKCR\Directory\Background\shell\Open WezTerm here\command` | `"F:\...\wezterm-spawn.cmd" --cwd "%V"`    |
| `HKCU\Software\Classes\batfile\shell\open\command`          | `"F:\...\wezterm-spawn.cmd" --bat "%1" %*` |

***

## (三) 核心脚本流程

    用户操作（右键文件夹 / 双击 .bat）
            │
            ▼
      wezterm-spawn.cmd
            │
            ├── --cwd → powershell ... -TargetDir "路径"
            └── --bat → powershell ... -BatFile  "文件"
                    │
                    ▼
            wezterm-spawn.ps1
                    │
            ┌───────┴────────┐
            ▼                ▼
        Mux 未运行        Mux 已运行
            │                │
      启动 Mux Server    检测 GUI 状态
      (重试等待就绪)          │
            │         ┌──────┴──────┐
            ▼         ▼             ▼
       查找窗口 ID   无GUI         有GUI
            │         │             │
            └────┬────┘       spawn 到现有窗口
                 ▼                 │
        spawn 新标签到窗口      激活新面板
                 │                 │
          ┌──────┴──────┐          │
          ▼             ▼          │
       首次启动     已有GUI        │
          │             │          │
      kill 默认标签   激活新面板    │
      启动 GUI            │        │
      激活标签            └────┬───┘
          │                   ▼
          └─────────── 激活 WezTerm 窗口（前台）

### 1. 关键设计决策

| 问题                 | 解决方案                                             |
| ------------------ | ------------------------------------------------ |
| Mux 启动时序不稳定        | 重试循环（最多 5 秒），轮询 `cli list` 直到就绪                  |
| Mux 运行但窗口全关（空 mux） | 杀掉 mux 重启，重新获取窗口 ID                              |
| 首次启动时出现默认标签        | Spawn 目标标签后 `kill-pane --pane-id 0` 删除默认标签       |
| 窗口未激活              | `WScript.Shell.AppActivate` 将 WezTerm 拉到前台       |
| 新标签未聚焦             | `wezterm cli activate-pane --pane-id <id>` 激活新面板 |

***

## (四) 快捷键速查

| 快捷键                   | 功能                   |
| --------------------- | -------------------- |
| `Ctrl+Shift+\` / `\|` | 垂直分屏                 |
| `Ctrl+Shift+-` / `_`  | 水平分屏                 |
| `Ctrl+Shift+方向键`      | 调整面板大小               |
| `Ctrl+Shift+Z`        | 面板最大化/还原             |
| `Ctrl+Shift+W`        | 关闭当前面板               |
| `Ctrl+Shift+T`        | 新建标签                 |
| `Ctrl+Tab`            | 下一标签                 |
| `Ctrl+Shift+Tab`      | 上一标签                 |
| `Ctrl+Shift+P`        | 命令面板                 |
| `Ctrl+Shift+L`        | 启动器                  |
| `Ctrl+Shift+M` / `m`  | 新标签菜单                |
| `Ctrl+Shift+1` / `!`  | 新建 PowerShell 标签     |
| `Ctrl+Shift+2` / `@`  | 新建 Git Bash 标签       |
| `Ctrl+Shift+3` / `#`  | 新建 CMD 标签            |
| `Ctrl+Shift+C`        | 复制                   |
| `Ctrl+Shift+V`        | 粘贴                   |
| `Ctrl+V` / `Ctrl+v`   | 粘贴（GUI 层拦截，类似 Cmder） |
| `Ctrl+单击链接`           | 打开链接                 |

***

## (五) 配置文件

### 1. `.wezterm.lua` — 主配置

路径：`C:\Users\ShenLan-Wai\.wezterm.lua`

```lua
local wezterm = require 'wezterm'
local act = wezterm.action
local config = {}

-- 启动 Shell
config.default_prog = { 'powershell.exe' }

-- 外观
config.color_scheme = 'Tokyo Night'
config.font = wezterm.font_with_fallback({
    { family = 'Consolas', weight = 'Regular' },
    'Microsoft YaHei',
})
config.font_size = 13.0
config.window_background_opacity = 0.95
config.window_padding = { left = 10, right = 10, top = 10, bottom = 10 }

-- 性能
config.front_end = 'OpenGL'
config.max_fps = 60
config.window_decorations = 'TITLE | RESIZE'

-- 禁用字体连字，防止重叠
config.harfbuzz_features = { 'calt=0', 'liga=0' }

-- 回滚
config.scrollback_lines = 10000

-- 光标
config.default_cursor_style = 'BlinkingBar'
config.cursor_blink_rate = 500

-- 标签栏
config.use_fancy_tab_bar = false
config.tab_bar_at_bottom = true
config.hide_tab_bar_if_only_one_tab = false

-- 快捷键
config.keys = {
  -- 分屏 (Ctrl+Shift+\ → 横分, Ctrl+Shift+- → 竖分)
  { key = '\\', mods = 'CTRL|SHIFT', action = act.SplitPane { direction = 'Right' } },
  { key = '|', mods = 'CTRL|SHIFT', action = act.SplitPane { direction = 'Right' } },
  { key = '-', mods = 'CTRL|SHIFT', action = act.SplitPane { direction = 'Down' } },
  { key = '_', mods = 'CTRL|SHIFT', action = act.SplitPane { direction = 'Down' } },

  -- 调整面板大小
  { key = 'LeftArrow', mods = 'CTRL|SHIFT', action = act.AdjustPaneSize { 'Left', 5 } },
  { key = 'RightArrow', mods = 'CTRL|SHIFT', action = act.AdjustPaneSize { 'Right', 5 } },
  { key = 'UpArrow', mods = 'CTRL|SHIFT', action = act.AdjustPaneSize { 'Up', 5 } },
  { key = 'DownArrow', mods = 'CTRL|SHIFT', action = act.AdjustPaneSize { 'Down', 5 } },

  -- 放大/还原面板
  { key = 'Z', mods = 'CTRL|SHIFT', action = act.TogglePaneZoomState },

  -- 关闭面板
  { key = 'W', mods = 'CTRL|SHIFT', action = act.CloseCurrentPane { confirm = false } },

  -- 标签操作
  { key = 'T', mods = 'CTRL|SHIFT', action = act.SpawnTab 'CurrentPaneDomain' },
  { key = 'Tab', mods = 'CTRL', action = act.ActivateTabRelative(1) },
  { key = 'Tab', mods = 'CTRL|SHIFT', action = act.ActivateTabRelative(-1) },

  -- 命令面板
  { key = 'P', mods = 'CTRL|SHIFT', action = act.ActivateCommandPalette },
  -- 启动器
  { key = 'L', mods = 'CTRL|SHIFT', action = act.ShowLauncher },
  -- 新标签菜单
  { key = 'M', mods = 'CTRL|SHIFT', action = act.ShowLauncherArgs { flags = 'LAUNCH_MENU_ITEMS' } },
  { key = 'm', mods = 'CTRL|SHIFT', action = act.ShowLauncherArgs { flags = 'LAUNCH_MENU_ITEMS' } },

  -- 切换 Shell
  { key = '1', mods = 'CTRL|SHIFT', action = act.SpawnCommandInNewTab { args = { 'powershell.exe' } } },
  { key = '!', mods = 'CTRL|SHIFT', action = act.SpawnCommandInNewTab { args = { 'powershell.exe' } } },
  { key = '2', mods = 'CTRL|SHIFT', action = act.SpawnCommandInNewTab { args = { 'F:\\ProgramFilesNoMove\\git-for-windows\\bin\\bash.exe', '--login' } } },
  { key = '@', mods = 'CTRL|SHIFT', action = act.SpawnCommandInNewTab { args = { 'F:\\ProgramFilesNoMove\\git-for-windows\\bin\\bash.exe', '--login' } } },
  { key = '3', mods = 'CTRL|SHIFT', action = act.SpawnCommandInNewTab { args = { 'cmd.exe' } } },
  { key = '#', mods = 'CTRL|SHIFT', action = act.SpawnCommandInNewTab { args = { 'cmd.exe' } } },

  -- 复制粘贴
  { key = 'C', mods = 'CTRL|SHIFT', action = act.CopyTo 'Clipboard' },
  { key = 'V', mods = 'CTRL|SHIFT', action = act.PasteFrom 'Clipboard' },
  -- Ctrl+V 粘贴（类似 Cmder，GUI 层拦截不分发给终端程序）
  { key = 'V', mods = 'CTRL', action = act.PasteFrom 'Clipboard' },
  { key = 'v', mods = 'CTRL', action = act.PasteFrom 'Clipboard' },
}

-- 鼠标绑定
config.mouse_bindings = {
  {
    event = { Down = { streak = 1, button = 'Right' } },
    mods = 'NONE',
    action = act.ExtendSelectionToMouseCursor 'Cell',
  },
  {
    event = { Up = { streak = 1, button = 'Left' } },
    mods = 'CTRL',
    action = act.OpenLinkAtMouseCursor,
  },
}

-- 新标签菜单
config.launch_menu = {
  { label = 'PowerShell', args = { 'powershell.exe' } },
  { label = 'Bash (Git)',  args = { 'F:\\ProgramFilesNoMove\\git-for-windows\\bin\\bash.exe', '--login' } },
  { label = 'CMD',         args = { 'cmd.exe' } },
}

-- 单实例多标签（类似 Cmder）：所有窗口/标签统一走 mux
config.unix_domains = {
  { name = 'unix' },
}
config.default_domain = 'unix'

-- 命令面板：添加配色方案选择
wezterm.on('augment-command-palette', function(window, pane)
  local schemes = {}
  for name, _ in pairs(wezterm.color.get_builtin_schemes()) do
    table.insert(schemes, name)
  end
  table.sort(schemes)

  local entries = {}
  for _, name in ipairs(schemes) do
    table.insert(entries, {
      brief = string.format('Scheme: %s', name),
      icon = 'md_palette',
      action = wezterm.action_callback(function(win, p)
        win:set_config_overrides({ color_scheme = name })
      end),
    })
  end
  return entries
end)

-- 窗口大小
config.initial_cols = 120
config.initial_rows = 30

return config
```

### 2. `wezterm-spawn.cmd` — 批处理入口

路径：`F:\ProgramFilesNoMove\WezTerm\wezterm-spawn.cmd`

```batch
@echo off
if "%~1"=="--bat" (
    powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0wezterm-spawn.ps1" -BatFile "%~2"
) else (
    powershell.exe -NoLogo -NoProfile -ExecutionPolicy Bypass -File "%~dp0wezterm-spawn.ps1" -TargetDir "%~2"
)
```

### 3. `wezterm-spawn.ps1` — 核心生成脚本

路径：`F:\ProgramFilesNoMove\WezTerm\wezterm-spawn.ps1`

```powershell
param([string]$TargetDir, [string]$BatFile)

$weztermDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$mux = Join-Path $weztermDir "wezterm-mux-server.exe"
$cli = Join-Path $weztermDir "wezterm.exe"
$gui = Join-Path $weztermDir "wezterm-gui.exe"

# ---- 0. Bat file mode ----
$spawnArgs = @()
if ($BatFile) {
    $TargetDir = Split-Path -Parent $BatFile
    $spawnArgs = @("--", "cmd.exe", "/k", "`"$BatFile`"")
}

# ---- 1. Ensure mux server is running ----
$muxProcs = Get-Process -Name "wezterm-mux-server" -ErrorAction SilentlyContinue
$muxRunning = ($muxProcs | Where-Object { -not $_.HasExited }).Count -gt 0

if (-not $muxRunning) {
    Start-Process -WindowStyle Hidden -FilePath $mux
    # Wait for mux to be ready (retry up to 5s)
    for ($i = 0; $i -lt 10; $i++) {
        Start-Sleep -Milliseconds 500
        $testOutput = & $cli cli --prefer-mux list 2>$null
        if ($LASTEXITCODE -eq 0 -and $testOutput) { break }
    }
}

# ---- 2. Check if GUI is already connected ----
$clients = & $cli cli --prefer-mux list-clients 2>$null
if ($LASTEXITCODE -ne 0) { $clients = @() }
$guiConnected = ($clients | Measure-Object).Count -gt 1

# ---- 3. Find window ID; restart mux if empty ----
$listOutput = & $cli cli --prefer-mux list 2>$null
if ($LASTEXITCODE -ne 0) { $listOutput = @() }

$winId = 0
$winLine = ($listOutput | Select-Object -Skip 1 -First 1)
if ($winLine -and $winLine -match '^\s*(\d+)') {
    $winId = $Matches[1]
} else {
    # Mux running but empty (all tabs closed): restart it
    $muxProcs | Stop-Process -Force
    Start-Sleep -Milliseconds 500
    Start-Process -WindowStyle Hidden -FilePath $mux
    for ($i = 0; $i -lt 10; $i++) {
        Start-Sleep -Milliseconds 500
        $testOutput = & $cli cli --prefer-mux list 2>$null
        if ($LASTEXITCODE -eq 0) {
            $line = ($testOutput | Select-Object -Skip 1 -First 1)
            if ($line -and $line -match '^\s*(\d+)') { $winId = $Matches[1]; break }
        }
    }
}

# ---- 4. Spawn new tab ----
if ($spawnArgs.Count -gt 0) {
    $newPaneId = & $cli cli --prefer-mux spawn --cwd $TargetDir --window-id $winId @spawnArgs 2>$null
} else {
    $newPaneId = & $cli cli --prefer-mux spawn --cwd $TargetDir --window-id $winId 2>$null
}

# ---- 5. Handle first launch: kill default tab, start GUI ----
if (-not $guiConnected) {
    & $cli cli --prefer-mux kill-pane --pane-id 0 2>$null

    Start-Process -WindowStyle Normal -FilePath $gui -ArgumentList "connect", "unix"
    Start-Sleep -Seconds 3

    # Activate the only remaining tab
    $listOutput = & $cli cli --prefer-mux list 2>$null
    if ($LASTEXITCODE -ne 0) { $listOutput = @() }
    foreach ($line in $listOutput) {
        if ($line -match '^\s*\d+\s+\d+\s+(\d+)') {
            & $cli cli --prefer-mux activate-pane --pane-id $Matches[1] 2>$null
            break
        }
    }
} else {
    # Activate the new pane in existing GUI
    if ($newPaneId -and $newPaneId -match '^\d+') {
        Start-Sleep -Milliseconds 100
        & $cli cli --prefer-mux activate-pane --pane-id $newPaneId.Trim() 2>$null
    }
}

# ---- 6. Bring window to foreground ----
Start-Sleep -Milliseconds 200
$guiProcs = Get-Process -Name "wezterm-gui" -ErrorAction SilentlyContinue
if ($guiProcs) {
    $wshell = New-Object -ComObject WScript.Shell
    foreach ($proc in $guiProcs) {
        if ($proc.MainWindowHandle -ne 0) {
            $wshell.AppActivate($proc.Id) | Out-Null
            break
        }
    }
}
```

***

## (六) 故障排除

| 现象                   | 可能原因                   | 解决                                                        |
| -------------------- | ---------------------- | --------------------------------------------------------- |
| 右键打开出现两个标签（默认+目标）    | `unix_domains` 未正确配置   | 确认 `.wezterm.lua` 中 `unix_domains` 和 `default_domain` 已设置 |
| 冷启动时出现默认标签，无目标标签     | Mux 启动时序问题             | 脚本已内置重试等待逻辑                                               |
| 双击 bat 未在 WezTerm 打开 | 注册表未更新                 | 重新执行（二）.4 节注册命令                                           |
| 多个 WezTerm 窗口        | 从开始菜单启动了 `wezterm.exe` | 关闭多余窗口，使用右键或 `wezterm-gui connect unix` 打开                |
| 窗口未激活到前台             | AppActivate 时序         | 脚本已内置 200ms 延迟 + 激活                                       |

### 1. 手动杀进程重来

```powershell
Get-Process -Name "wezterm*" -ErrorAction SilentlyContinue | Stop-Process -Force
```

***

## (七) 参考

*   [WezTerm 官方文档](https://wezterm.org/)
*   [WezTerm CLI 参考](https://wezterm.org/cli/)
*   [WezTerm Mux 多路复用](https://wezterm.org/multiplexing/)
