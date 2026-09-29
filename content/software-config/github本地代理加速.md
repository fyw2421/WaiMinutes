---
title: "GitHub 本地代理加速"
weight: 5
description: "DevSideCar 代理软件的安装与 GitHub 加速配置"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/github本地代理加速.svg"
---

# 一、DevSideCar 代理软件

## (一) 下载地址

- **GitHub Releases**：<https://github.com/docmirror/dev-sidecar/releases>
- 选择最新版本，下载 `dev-sidecar-windows-amd64.zip`
  - **说明**：`amd64` 即 Intel/AMD x64 架构，适用于绝大多数 Windows 电脑。如果你使用的是 ARM 架构（如 Surface Pro X），请下载对应版本。

## (二) 安装步骤

1.  解压到指定目录，如 `F:\ProgramFilesNoMove\DevSideCar`
2.  运行 `dev-sidecar.exe`
3.  首次运行时会请求防火墙权限，点击「允许访问」

## (三) 基本配置

| 配置项     | 值                                    |
| ------- | ------------------------------------ |
| 加速模式    | 默认模式 / 安全模式（可选）            |
| 本地代理端口 | `31181`（默认）                        |
| 自动启动    | 建议开启，避免每次手动启动             |
| 系统代理    | 可不开启，按需选择                     |

**模式说明：**

- **默认模式**：直接使用代理加速，访问速度最快，但会明文转发 HTTPS 流量用于加速，适合个人开发环境。
- **安全模式**：不开启 HTTPS 解密，仅加速特定域名，隐私性更好，但部分加速功能受限。

**证书安装（安全模式无需安装）：**

默认模式下，DevSideCar 会拦截 HTTPS 流量进行加速，需要安装其生成的 CA 证书：

1.  打开 DevSideCar，进入 **设置** → **证书管理**
2.  点击 **导出证书**，保存为 `dev-sidecar-ca.crt`
3.  安装证书到系统：
    - 双击 `dev-sidecar-ca.crt`
    - 选择「本地计算机」→「安装证书」
    - 选择「将所有的证书都放入下列存储」→「受信任的根证书颁发机构」
4.  安装完成后重启 DevSideCar

> **注意**：安全模式下不需要安装证书，因为 DevSideCar 不会解密 HTTPS 流量。但部分 GitHub 加速功能可能失效。

## (四) 功能说明

DevSideCar 主要功能包括：

- **GitHub 加速**：自动重定向 GitHub 相关域名到镜像或加速地址
- **npm 加速**：支持 npm 源加速
- **Host 管理**：支持批量管理 Hosts 配置
- **系统代理**：可选择性开启，开启后自动配置系统代理，无需手动设置浏览器/终端代理；如不需要可关闭

## (五) 注意事项

- DevSideCar 运行时会占用本地 `31181` 端口，确保该端口未被其他程序占用
- 若使用 VPN 或其他代理软件，可能需要调整端口避免冲突
- 关闭 DevSideCar 后，系统代理会自动恢复，无需手动修改
- **系统代理可根据需要选择是否开启**，仅使用加速功能时可不开启系统代理
- 使用默认模式时需安装 CA 证书，安全模式则不需要

# 二、GitHub PAT（Personal Access Token）

## (一) 什么是 PAT

PAT（Personal Access Token）是 GitHub 提供的个人访问令牌，用于替代密码进行 API 调用、Git 操作等。

## (二) 注册/创建 PAT

1.  登录 GitHub，进入 **Settings** → **Developer settings** → **Personal access tokens** → **Tokens (classic)**
2.  点击 **Generate new token** → **Generate new token (classic)**
3.  配置令牌：
    - **Note**：填写令牌用途说明，如 `ynote-sync`
    - **Expiration**：选择过期时间，建议 90 天
    - **Scopes**：按需勾选权限，常用权限：
      - `repo` — 访问仓库
      - `workflow` — 修改 CI/CD 工作流
      - `write:packages` — 发布包
4.  点击 **Generate token**
5. **重要**：复制生成的 token，只显示一次，请妥善保存

## (三) 使用 PAT

**Git 操作：**

```bash
### 首次配置：设置 Git 凭据助手，避免每次都输入账号密码
git config --global credential.helper manager-core
```

配置后，执行 Git 操作时第一次输入用户名和 PAT，系统会记住凭据，后续操作无需重复输入。

**查看已保存的凭据：**

**方式一：图形界面**

1.  打开「控制面板」→「用户账户」→「凭据管理器」
2.  选择「Windows 凭据」
3.  找到 `git:https://github.com` 条目，即可查看已保存的用户名和密码
4.  如需修改或删除凭据，点击对应条目进行编辑或删除

**方式二：命令行**

```bash
### 查看所有 Git 相关凭据
cmdkey /list | findstr "git:"

### 查看 GitHub 凭据详情
cmdkey /list | findstr "github"
```

```bash
git clone https://github.com/username/repo.git
Username: your-username
Password: ghp_xxxxxxxxxxxx（粘贴 PAT）
```

**GitHub CLI：**

```bash
gh auth login
### 选择 GitHub.com
### 选择 HTTPS
### 粘贴 PAT
```

## (四) 注意事项

- PAT 相当于密码，切勿提交到代码仓库或公开分享
- 建议定期更换 PAT，设置合理过期时间
- 如不再需要，及时在 GitHub Settings 中删除对应令牌
