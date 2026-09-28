---
title: "OpenCode 安装使用教程"
weight: 2
description: "OpenCode 的安装、桌面端与 CLI 配置"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/OpenCode安装使用教程.svg"
---

# 一、安装

安装 OpenCode：

**方式一：npm 安装 CLI**

```bash
npm i -g opencode-ai
```

**方式二：桌面端下载**

1.  下载地址：<https://github.com/anomalyco/opencode/releases>
2.  下载 `opencode-windows-x64.zip`
3.  解压缩到指定目录（如 `F:\ProgramFilesNoMove\OpenCode`）

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

**方式二：直接修改配置文件** — 如需手动调整，可编辑 `C:\Users\ShenLan-Wai\.config\opencode\opencode.json`，按上述配置 JSON 写入即可。

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

*   工具栏选择 **OpenCode**
*   点击 **添加新供应商**

#### (3) 供应商配置

| 配置项      | 值                               |
| -------- | ------------------------------- |
| 官网链接     | <https://platform.deepseek.com> |
| 接口格式     | OpenAI Compatible               |
| API Key  | `{env:"ANTHROPIC_AUTH_TOKEN"}`       |
| Base URL | <https://api.deepseek.com/v1>   |
| 额外选项     | setCacheKey : true              |
| 模型配置     | 点击获取模型列表                        |

#### (4) 配置 JSON

```json
{
  "$schema": "https://opencode.ai/config.json",
  "provider": {
    "deepseek": {
      "models": {
        "deepseek-v4-flash": {
          "name": "DeepSeek V4 Flash"
        },
        "deepseek-v4-pro": {
          "name": "DeepSeek V4 Pro"
        }
      },
      "npm": "@ai-sdk/openai-compatible",
      "options": {
        "apiKey": "{env:ANTHROPIC_AUTH_TOKEN}",
        "baseURL": "https://api.deepseek.com/v1",
        "setCacheKey": true
      }
    }
  }
}
```

**说明：** `apiKey` 使用 `{env:ANTHROPIC_AUTH_TOKEN}` 引用环境变量，避免密钥明文落盘。

#### (5) 配置文件目录

配置文件路径：`C:\Users\ShenLan-Wai\.config\opencode\opencode.json`

CC-Switch 会自动将配置写入此文件，无需手动修改。

> **注意**：jsonc（JSON with Comments）允许在配置文件中写注释（//），标准 .json 则不行。两者功能完全相同，选 .jsonc 只是方便加注释说明。

### 2. 直接修改配置文件

如需手动配置，可直接编辑 `C:\Users\ShenLan-Wai\.config\opencode\opencode.json`，将上述配置 JSON 写入文件。`ANTHROPIC_AUTH_TOKEN` 仍需通过环境变量提供，不要写入该文件。

# 三、使用

安装完成后，在终端输入 `opencode` 即可启动 OpenCode。

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

## (二) 在 OpenCode 中使用

```bash
# 先用 cdp 进入项目目录
cdp dev

# 然后启动 OpenCode
opencode
```

# 五、配置文件目录汇总

| 工具               | 配置文件路径                                                 |
| ---------------- | ------------------------------------------------------ |
| OpenCode         | `C:\Users\ShenLan-Wai\.config\opencode\opencode.json` |
| cdp (CMD)        | `F:\ProgramFilesNoMove\WezTerm\cdp.bat`                |
| cdp (PowerShell) | `$PROFILE`                                             |

# 六、常用命令

```bash
opencode                   # 启动 OpenCode
opencode upgrade           # 升级 OpenCode 到最新版本
/sessions         # 查看会话列表
cdp dev                    # 快速进入 dev 项目目录
cdp classes                # 快速进入 classes 项目目录
cdp forest                 # 快速进入 forest 项目目录
cdp ynote                  # 快速进入 ynote 目录
cdp tmp                    # 快速进入 tmp 目录
cdp apifox                 # 快速进入 apifox 目录
```
