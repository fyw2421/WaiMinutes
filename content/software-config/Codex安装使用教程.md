---
title: "Codex 安装使用教程"
weight: 3
description: "Codex 的安装、环境变量配置与常用命令"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/Codex安装使用教程.svg"
---

# 一、安装

安装 Codex：

```bash
npm install -g @openai/codex
codex --version
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

**方式二：直接修改配置文件** — 如需手动调整，可编辑 `C:\Users\ShenLan-Wai\.codex\config.toml`，按上述配置写入即可。

无论使用哪种方式，`ANTHROPIC_AUTH_TOKEN` 都通过环境变量提供，不落盘到配置文件中。

### 1. CC-Switch 配置（接入 DeepSeek）

#### (1) 安装 CC-Switch

CC-Switch 是一个配置管理工具，用于切换 Claude Code、Codex、OpenCode 等 AI 工具的供应商配置。

**下载地址：** <https://github.com/anthropics/cc-switch/releases>

**安装步骤：**

1.  下载最新版本的安装包
2.  运行安装程序
3.  安装完成后，CC-Switch 会出现在系统托盘中

#### (2) 说明

新版 Codex CLI 已原生支持第三方 OpenAI 兼容接口，且 DeepSeek API 已原生支持 Responses API，因此接入 DeepSeek **不再需要本地路由**进行协议转换。CC-Switch 仅用于管理供应商配置与切换，无需开启路由。

#### (3) 打开 CC-Switch 设置

1.  工具栏 → Codex
2.  点击 **添加供应商**

| 配置项      | 值                               |
| -------- | ------------------------------- |
| 官网链接     | <https://platform.deepseek.com> |
| 供应商名称    | DeepSeek（随便填）                   |
| API Key  | `{env:"ANTHROPIC_AUTH_TOKEN"}`       |
| API 请求地址 | <https://api.deepseek.com>      |
| API 格式   | Responses API（原生）               |
| 高级选项     | Responses(原生) |
| 模型映射     | 点击获取模型列表                        |

> **注意：** 无需进入"设置 → 路由"开启路由总开关，也不要勾选"需要本地路由映射"。

#### (4) 配置 config.toml

```toml
model_provider = "custom"
model = "deepseek-v4-flash"
api_key = "{env:ANTHROPIC_AUTH_TOKEN}"
model_reasoning_effort = "high"
disable_response_storage = true
model_catalog_json = "cc-switch-model-catalog.json"

[model_providers]
[model_providers.custom]
name = "deepseek"
base_url = "https://api.deepseek.com"
wire_api = "responses"
requires_openai_auth = true

[projects]
[projects."e:\\projects\\devenv4miniprogram.pc\\frgsys.wechat.dev"]
trust_level = "trusted"

[windows]
sandbox = "elevated"
```

**说明：** `api_key` 使用 `dummy` 占位，实际密钥通过环境变量 `ANTHROPIC_AUTH_TOKEN` 提供，避免明文落盘。

#### (5) 配置文件目录

配置文件路径：`C:\Users\ShenLan-Wai\.codex\config.toml`

CC-Switch 会自动将配置写入此文件，无需手动修改。

### 2. 直接修改配置文件

如需手动配置，可直接编辑 `C:\Users\ShenLan-Wai\.codex\config.toml`，将上述配置写入文件。`ANTHROPIC_AUTH_TOKEN` 仍需通过环境变量提供，不要写入该文件。

# 三、使用

安装完成后，在终端输入 `codex` 即可启动 Codex。

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

## (二) 在 Codex 中使用

```bash
# 先用 cdp 进入项目目录
cdp dev

# 然后启动 Codex
codex
```

# 五、配置文件目录汇总

| 工具               | 配置文件路径                                    |
| ---------------- | ----------------------------------------- |
| Codex            | `C:\Users\ShenLan-Wai\.codex\config.toml` |
| cdp (CMD)        | `F:\ProgramFilesNoMove\WezTerm\cdp.bat`   |
| cdp (PowerShell) | `$PROFILE`                                |

# 六、常用命令

```bash
codex                    # 启动 Codex
codex --version          # 查看版本
codex update             # 更新 Codex 到最新版本
cdp dev                  # 快速进入 dev 项目目录
cdp classes              # 快速进入 classes 项目目录
```
