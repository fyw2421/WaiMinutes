---
title: "Claude Code 安装使用教程"
weight: 1
description: "Claude Code 的安装、环境变量配置与常用命令"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/Claude Code安装使用教程.svg"
---

# 一、安装

安装 Claude Code：

```bash
npm install -g @anthropic-ai/claude-code
```

# 二、配置

## (一) 设置环境变量

API Key 通过环境变量 `ANTHROPIC_AUTH_TOKEN` 提供，避免密钥明文写入配置文件。

**方式一：系统环境变量（推荐）**

1.  右键「此电脑」→「属性」→「高级系统设置」→「环境变量」
2.  在「用户变量」中新增：
    - **变量名：** `ANTHROPIC_AUTH_TOKEN`
    - **变量值：** 你的 DeepSeek API Key（`sk-xxx`）
3.  点击「确定」保存，重启终端后生效

**方式二：PowerShell 配置文件（永久有效）**

在 PowerShell 配置文件中添加环境变量，每次打开 PowerShell 时自动加载：

```powershell
[System.Environment]::SetEnvironmentVariable("ANTHROPIC_AUTH_TOKEN", "sk-xxx", "User")
```

执行后重启终端即可生效，仅对当前用户有效。

## (二) 本教程提供两种配置方式

**方式一：CC-Switch 配置**（推荐）— 通过 CC-Switch 图形界面添加供应商，自动写入配置文件，无需手动编辑。

**方式二：直接修改配置文件** — 如需手动调整，可编辑 `C:\Users\ShenLan-Wai\.claude\settings.json`，按上述配置 JSON 写入即可。

无论使用哪种方式，`ANTHROPIC_AUTH_TOKEN` 都通过环境变量提供，不落盘到配置文件中。

### 1. CC-Switch 配置（接入 DeepSeek）

#### (1) 安装 CC-Switch

CC-Switch 是一个配置管理工具，用于切换 Claude Code、Codex、OpenCode 等 AI 工具的供应商配置。

**下载地址：** <https://github.com/anthropics/cc-switch/releases>

**安装步骤：**

1.  下载最新版本的安装包
2.  运行安装程序
3.  安装完成后，CC-Switch 会出现在系统托盘中

#### (2) 打开 CC-Switch 设置

*   工具栏选择 **Claude Code**
*   点击 **添加新供应商**

#### (3) 供应商配置

| 配置项     | 值                                    |
| ------- | ------------------------------------ |
| 官网链接    | <https://platform.deepseek.com>      |
| API Key | `{env:"ANTHROPIC_AUTH_TOKEN"}`       |
| 请求地址    | <https://api.deepseek.com/anthropic> |
| API 格式  | Anthropic Messages（原生）               |
| 认证字段    | ANTHROPIC\_AUTH\_TOKEN               |
| 模型映射    | 点击获取模型列表                             |

#### (4) 配置 JSON

```json
{
  "effortLevel": "medium",
  "enabledPlugins": {
    "frontend-design@claude-plugins-official": true,
    "superpowers@claude-plugins-official": true,
    "wechat-miniprogram-design@banjinbell": true
  },
  "env": {
    "ANTHROPIC_BASE_URL": "https://api.deepseek.com/anthropic",
    "ANTHROPIC_DEFAULT_FABLE_MODEL": "deepseek-v4-pro",
    "ANTHROPIC_DEFAULT_FABLE_MODEL_NAME": "deepseek-v4-pro",
    "ANTHROPIC_DEFAULT_HAIKU_MODEL": "deepseek-v4-flash",
    "ANTHROPIC_DEFAULT_HAIKU_MODEL_NAME": "deepseek-v4-flash",
    "ANTHROPIC_DEFAULT_OPUS_MODEL": "deepseek-v4-pro",
    "ANTHROPIC_DEFAULT_OPUS_MODEL_NAME": "deepseek-v4-pro",
    "ANTHROPIC_DEFAULT_SONNET_MODEL": "deepseek-v4-pro",
    "ANTHROPIC_DEFAULT_SONNET_MODEL_NAME": "deepseek-v4-pro",
    "ANTHROPIC_MODEL": "deepseek-v4-flash[1M]",
    "CLAUDE_CODE_SUBAGENT_MODEL": "deepseek-v4-flash"
  },
  "extraKnownMarketplaces": {
    "banjinbell": {
      "autoUpdate": true,
      "source": {
        "source": "git",
        "url": "https://github.com/banjinbell/wechat-miniprogram-design.git"
      }
    }
  },
  "language": "English",
  "model": "sonnet",
  "permissions": {
    "allow": [
      "WebSearch",
      "WebFetch(domain:youzan.github.io)"
    ]
  },
  "statusLine": {
    "command": "powershell -NoProfile -File \"C:\\Users\\ShenLan-Wai\\.claude\\statusline.ps1\"",
    "type": "command"
  },
  "theme": "dark-ansi",
  "useAutoModeDuringPlan": true,
  "verbose": true
}
```

**说明：** `ANTHROPIC_AUTH_TOKEN` 不再写入配置文件，而是通过环境变量提供，避免密钥明文落盘。

#### (5) 配置文件目录

配置文件路径：`C:\Users\ShenLan-Wai\.claude\settings.json`

CC-Switch 会自动将配置写入此文件，无需手动修改。

### 2. 直接修改配置文件

如需手动配置，可直接编辑 `C:\Users\ShenLan-Wai\.claude\settings.json`，将上述配置 JSON 写入文件。`ANTHROPIC_AUTH_TOKEN` 仍需通过环境变量提供，不要写入该文件。

# 三、使用

安装完成后，在终端输入 `claude` 即可启动 Claude Code。

# 四、快速进入项目目录（cdp）

`cdp` 是一个目录别名工具，用一个短单词代替完整路径。需要先配置 cdp 脚本才能使用，详见 [cdp 配置指南](cdp%20快捷进入目的目录配置指南.md)。

## (一) 使用方式

```cmd
cdp dev           # → E:\Projects\devenv4miniprogram.pc\Frgsys.wechat.dev
cdp classes       # → E:\Projects\devenv4app.pc\Frgsys.v2\Frgsys64\Classes
cdp forest        # → E:\Projects\devenv4app.pc\frgsys.v2\Frgsys64\Classes\ui_fairygui\forest
cdp ynote         # → C:\Users\ShenLan-Wai\Desktop\tmp\ynote
cdp tmp           # → C:\Users\ShenLan-Wai\Desktop\tmp
cdp apifox        # → C:\Users\ShenLan-Wai\Desktop\tmp\apifox
```

## (二) 在 Claude Code 中使用

```bash
# 先用 cdp 进入项目目录
cdp dev

# 然后启动 Claude Code
claude
```

# 五、配置文件目录汇总

| 工具               | 配置文件路径                                  |
| ---------------- | --------------------------------------- |
| Claude Code      | `C:\Users\ShenLan-Wai\.claude`          |
| cdp (CMD)        | `F:\ProgramFilesNoMove\WezTerm\cdp.bat` |
| cdp (PowerShell) | `$PROFILE`                              |

# 六、常用命令

```bash
claude                    # 启动 Claude Code
claude update             # 更新 Claude Code 到最新版本
claude --continue         # 继续上一次会话
claude resume <session>   # 恢复指定会话
claude --help             # 查看帮助
cdp dev                   # 快速进入 dev 项目目录
cdp classes               # 快速进入 classes 项目目录
cdp forest                # 快速进入 forest 项目目录
cdp ynote                 # 快速进入 ynote 目录
cdp tmp                   # 快速进入 tmp 目录
cdp apifox                # 快速进入 apifox 目录
```

# 七、状态栏配置（statusLine）

Claude Code 支持自定义底部状态栏，实时显示模型、目录、Git 分支、上下文用量等信息。

## (一) 配置方式

在 `C:\Users\ShenLan-Wai\.claude\settings.json` 中添加 `statusLine` 字段：

```json
{
  "statusLine": {
    "type": "command",
    "command": "powershell -NoProfile -File \"C:\\Users\\ShenLan-Wai\\.claude\\statusline.ps1\""
  }
}
```

- **type:** `"command"` — 表示通过执行外部命令生成状态栏文本
- **command:** 要执行的命令。Windows 下需要显式调用 `powershell -NoProfile -File` 来运行 `.ps1` 脚本，因为 Claude Code 内部使用 `cmd.exe` 执行命令

## (二) 脚本文件

脚本文件位于 `C:\Users\ShenLan-Wai\.claude\statusline.ps1`，内容如下：

```powershell
$o = $input | ConvertFrom-Json

$e = [char]27
$cyan = "${e}[36m"; $yellow = "${e}[33m"; $green = "${e}[32m"
$red = "${e}[31m"; $dim = "${e}[2m"; $reset = "${e}[0m"

$parts = @()

$parts += "${cyan}$($o.model.display_name)${reset}"

$dir = Split-Path -Leaf $o.workspace.current_dir -ErrorAction SilentlyContinue
if ($dir) { $parts += "${dim}${dir}${reset}" }

try {
  $b = git --no-optional-locks branch --show-current 2>$null
  if ($b) {
    $changes = git status --porcelain 2>$null
    $count = ($changes -split "`n" | Where-Object { $_ -ne "" }).Count
    $suffix = if ($count -gt 0) { " +$count" } else { "" }
    $parts += "${yellow}${b}${suffix}${reset}"
  }
} catch {}

if ($o.context_window.context_window_size) {
  $used = $o.context_window.total_input_tokens + $o.context_window.total_output_tokens
  $pct = [math]::Round(($used / $o.context_window.context_window_size) * 100, 1)
  $clr = if ($pct -ge 80) { $red } elseif ($pct -ge 50) { $yellow } else { $green }
  $parts += "${clr}${pct}%${reset}"

  $unit = if ($used -ge 1000) { "$([math]::Round($used/1000, 1))k" } else { "$used" }
  $parts += "${dim}${unit}tokens${reset}"
}

$parts -join " | "
```

## (三) 工作原理

1. Claude Code 每次更新状态栏时，都会执行 `command` 中配置的命令
2. 标准输入（stdin）会传入一个 JSON 对象，包含当前会话的元数据（模型、工作目录、上下文用量、Git 信息等）
3. 脚本通过 `$input | ConvertFrom-Json` 解析 JSON
4. 脚本输出一行纯文本（含 ANSI 转义码），Claude Code 将其渲染在底部状态栏

## (四) 状态栏各段说明

| 段位 | 颜色 | 数据来源 | 说明 |
|------|------|----------|------|
| 模型名 | 青色（cyan） | `$o.model.display_name` | 当前使用的模型名称，如 `deepseek-v4-pro` |
| 目录名 | 灰色（dim） | `$o.workspace.current_dir` | 当前工作目录的最后一级文件夹名 |
| Git 分支 + 改动数 | 黄色（yellow） | `git branch --show-current` + `git status --porcelain` | 当前 Git 分支名，如果有未提交改动则显示 `+N` |
| 上下文百分比 | 绿/黄/红 | `$o.context_window` | 上下文窗口使用率。绿色 < 50%，黄色 50%~80%，红色 ≥ 80% |
| Token 数量 | 灰色（dim） | `$o.context_window.total_input_tokens + total_output_tokens` | 当前会话累计消耗的 token 总数，超过 1000 时显示为 `k` 单位 |

## (五) 注意事项

- **PowerShell 5.1 不支持 `e` 转义符**，必须使用 `$([char]27)` 或 `[char]27` 来生成 ANSI 转义序列
- **`statusLine` 是 Claude Code 的功能，不是 opencode 的功能**。opencode 不支持 `statusLine` 配置
- **命令路径必须使用绝对路径**，`%USERPROFILE%` 或 `$env:USERPROFILE%` 不会被 `cmd.exe` 展开
- DeepSeek 等第三方 API 可能不返回 `context_window.used_percentage` 字段，脚本中需要通过 `$o.context_window.context_window_size` 是否存在来判断
- 每次新会话 token 计数会重置
