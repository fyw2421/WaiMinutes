---
title: "OMP 安装使用教程"
weight: 10
description: "oh-my-pi 的安装、配置文件位置、常用命令与 pi-freeflow 插件"
date: 2026-11-17
tags: ["software-config"]
featureimage: "covers/omp安装使用教程.svg"
---

# 一、安装

oh-my-pi（命令 `omp`）是一个终端里的 AI 编程 Agent，支持 API Key、OAuth 订阅登录和本地 Provider。

官网：<https://omp.sh>

## (一) 三种安装方式对比

| 方式 | 命令 | 启动速度 | 依赖 |
| --- | --- | --- | --- |
| 二进制（推荐） | `irm https://omp.sh/install.ps1 \| iex` | 约 30 ms | 无 |
| npm | `npm install -g @oh-my-pi/pi-coding-agent` | 约 1.8 s | npm 与 Node.js |
| Bun | `bun install -g @oh-my-pi/pi-coding-agent` | 约 1.8 s | Bun |

二进制方式自带运行时，启动最快，并且能用差量补丁自更新（`omp update`）；npm 与 Bun 方式每次启动都要解析一个约 18 MB 的 JS bundle，所以慢了一个数量级。

## (二) 二进制安装（推荐）

### 1. 卸载旧版本（如有）

之前用 npm 或 Bun 装过同一个包时，先卸掉，避免 PATH 上的旧入口抢占新安装：

```powershell
npm uninstall -g @oh-my-pi/pi-coding-agent
bun uninstall -g @oh-my-pi/pi-coding-agent
```

### 2. 执行安装脚本

安装目录默认是 `%LOCALAPPDATA%\omp`，用 `PI_INSTALL_DIR` 环境变量改到别的盘：

```powershell
$env:PI_INSTALL_DIR = "F:\ProgramFilesNoMove\omp"
irm https://omp.sh/install.ps1 | iex
```

脚本会校验下载文件的 sha256，校验通过后才替换 `omp.exe`；随后把安装目录追加到**用户级 PATH**，并检查 Git Bash 并把它的路径写进 `~/.omp/agent/settings.json` 的 `shellPath`。

### 3. 重启终端并验证

PATH 只对新开的窗口生效：

```powershell
omp --version
```

## (三) npm 安装

npm 方式需要 Node.js，没装的话先到 <https://nodejs.org> 下载安装。

### 1. 安装

```powershell
npm install -g @oh-my-pi/pi-coding-agent
npm install -g bun   # bun 环境
```

### 2. 重启终端

```powershell
omp --version
```

## (四) Bun 安装

### 1. 安装 Bun

已经有 npm 时，用 npm 装最省事：

```powershell
npm install -g bun
```

没有 npm 也可以走 PowerShell 脚本或 winget：

```powershell
irm https://bun.sh/install.ps1 | iex   # PowerShell 脚本
winget install OvenShed.Bun            # winget
```

也可以直接从 <https://bun.sh> 下载安装包。验证：

```powershell
bun --version
```

### 2. 安装 omp

```powershell
bun install -g @oh-my-pi/pi-coding-agent
```

### 3. 重启终端

```powershell
omp --version
```

# 二、更新

```powershell
omp update            # 检查并安装当前通道的更新
omp update --check    # 只检查，不安装
omp update --plugins  # 顺带更新已安装的插件
```

npm 与 Bun 安装没有自更新通道，用包管理器升级：

```powershell
npm update -g @oh-my-pi/pi-coding-agent
bun update -g @oh-my-pi/pi-coding-agent
```

npm 与 Git 安装的插件没有通用的升级命令，重新 install 指定版本或 ref 即可（marketplace 插件才用 `omp plugin upgrade`）。

# 三、配置文件位置

配置与数据都在用户目录下的 `.omp`，默认路径 `C:\Users\<用户名>\.omp\agent\`：

| 路径 | 内容 |
| --- | --- |
| `config.yml` | 全局设置（模型角色、主题、审批模式等） |
| `settings.json` | 界面相关设置，安装脚本写入的 `shellPath` 也在这里 |
| `sessions/` | 会话记录（JSONL） |
| `plugins/node_modules/` | 已安装的 npm 插件 |
| `profiles/<名称>/agent/` | 命名配置，独立的设置、认证与会话 |

项目级配置放在仓库根目录的 `.omp/config.yml`，只对该目录生效（omp 不会向上级目录查找）。

用命令查看与修改：

```powershell
omp config path                 # 打印当前生效的 agent 目录
omp config list                 # 列出全部设置与当前值
omp config get theme.dark       # 读取单项
omp config set theme.dark catppuccin-macchiato
```

# 四、第一次使用

在项目目录里启动：

```powershell
cd path\to\your-project
omp
```

首次启动会打开设置向导：先在 **Sign in** 标签选择 Provider 完成浏览器登录（或用环境变量里的 API Key，按 `Esc` 跳过），再选默认模型，最后调整终端外观。之后的启动直接进入输入框。

常用交互：

| 操作 | 按键/命令 |
| --- | --- |
| 发送提示词 | `Enter` |
| 换行 | `Alt`+`Enter`（Windows Terminal） |
| 查看工具输出/完整 diff | `Ctrl`+`O` |
| 选择模型 | `/model` 或 `Alt`+`M` |
| 切换思考等级 | `Shift`+`Tab` |
| 中断当前回合 | `Esc` |
| 退出 | `/exit` 或 `Ctrl`+`D` |

# 五、常用命令

## (一) 启动与会话

| 命令 | 说明 |
| --- | --- |
| `omp` | 在当前目录启动交互式会话 |
| `omp "提示词"` | 带提示词启动 |
| `omp -p "提示词"` | 非交互模式，输出结果后退出 |
| `git diff \| omp -p "审查这段 diff"` | 从标准输入读取内容 |
| `omp -c` | 继续当前目录最近一次会话 |
| `omp -r` | 打开会话选择器 |
| `omp -r <id>` | 按 ID 前缀恢复指定会话 |
| `omp --fork <id>` | 复制一个会话后继续 |
| `omp --from-claude` | 导入 Claude Code 的会话 |
| `omp --from-codex` | 导入 Codex 的会话 |
| `omp --no-session` | 本次不保存会话 |
| `omp --session-dir <路径>` | 指定会话存储目录 |

## (二) 模型

| 命令 | 说明 |
| --- | --- |
| `omp models` | 列出当前可用的模型 |
| `omp models <provider>` | 列出某个 Provider 的模型 |
| `omp models find <文本>` | 搜索模型 |
| `omp --model <模型>` | 指定模型，支持 `provider/model` 与 `:effort` 后缀 |
| `omp --thinking <级别>` | `off`、`low`、`medium`、`high`、`max` 等 |

## (三) 插件

| 命令 | 说明 |
| --- | --- |
| `omp plugin list` | 列出已安装插件 |
| `omp plugin install <包或路径>` | 安装 npm / Git / 本地目录插件 |
| `omp plugin uninstall <名称>` | 卸载插件 |
| `omp plugin link <路径>` | 把本地目录链接为插件（开发用） |
| `omp plugin enable/disable <名称>` | 启用/禁用，不删除文件 |
| `omp plugin doctor` | 检查插件安装状态 |
| `/reload-plugins` | 在会话中重载插件 |

## (四) 其他

| 命令 | 说明 |
| --- | --- |
| `omp config` | 读取/修改配置（见第三节） |
| `omp stats` | 查看用量统计 |
| `omp usage` | 查看各 Provider 的用量上限 |
| `omp commit` | 生成提交信息并按需拆分提交 |
| `omp git` | 交互式 Git 界面 |
| `omp share <session>` | 加密分享会话 |
| `omp completions zsh` | 生成 Shell 补全脚本 |
| `omp gc` | 清理存储垃圾 |

# 六、插件：pi-freeflow

## (一) 介绍

`pi-freeflow` 给 OMP 增加一个 `freeflow` Provider，提供 32 个免费模型（部分支持 100 万上下文），并自带中继池与调用日志。作者 trefeon，MIT 协议。

它只是扩展插件，不是独立 CLI：只能由 OMP 或 Pi 加载，OpenCode、Cursor 等工具用不了。

## (二) 安装

```powershell
omp plugin install pi-freeflow
```

安装后新开一个会话即可加载；会话内执行：

```text
/reload-plugins
```

## (三) 使用

交互模式：

```text
omp
/model → freeflow → 选模型 → 选思考等级
```

单次对话：

```powershell
omp -p --model freeflow/deepseek-v4.1-flash-free "build me a SaaS"
omp -p --model freeflow/beam-free:max "solve this bug"
```

模型 ID 支持完整 ID、短别名和 `:effort` 后缀（`:minimal` 到 `:xhigh`，部分模型支持 `:max`）。

常用模型：

| 模型 | 上下文 | 说明 |
| --- | --- | --- |
| `deepseek-v4.1-flash-free` | 1M | 通用，支持图片输入 |
| `beam-free` | 1M | 长上下文 |
| `kilo-auto/free` | 256K | 通用 |
| `openrouter/free` | 200K | 通用，支持图片输入 |

完整列表用 `omp models freeflow` 查看。

## (四) 注意

- 免费额度的限流按出口 IP 计算，共享 IP 打满时需要在插件里加中继（`/freeflow add <url> <label>`）。
- 插件日志在 `~/.omp/agent/pi-freeflow.log`，会话内的 `/freeflow logs` 也能看。工具调用审计日志在 `~/.omp/agent/toolcall-audit.jsonl`。

# 七、常见问题

## (一) 安装后提示找不到 omp

PATH 只对新开的终端窗口生效。重新打开窗口，或临时刷新当前会话：

```powershell
$env:Path = [Environment]::GetEnvironmentVariable("Path", "User") + ";" + [Environment]::GetEnvironmentVariable("Path", "Machine")
```

## (二) 想改安装目录

用 `PI_INSTALL_DIR` 指定后再跑一次安装脚本：

```powershell
$env:PI_INSTALL_DIR = "F:\ProgramFilesNoMove\omp"
irm https://omp.sh/install.ps1 | iex
```

## (三) npm 方式启动慢

每次启动要解析约 18 MB 的 JS bundle，约 1.8 秒。追求启动速度就改用二进制方式。

## (四) omp 命令指向了旧的安装位置

安装目录是**追加**到 PATH 的，如果更早的 `bun install -g`、Scoop 等入口排在前面，输入 `omp` 仍然会命中旧的那份。安装脚本检测到这种情况会打印 `[WARN] 'omp' still resolves to ...`：

```powershell
Get-Command omp          # 看实际命中的路径
```

把旧入口从 PATH 移除，或把新目录调到它前面。