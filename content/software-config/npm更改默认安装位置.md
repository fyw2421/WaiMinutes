---
title: "npm 更改默认安装位置"
weight: 7
description: "将 npm 全局路径从 C 盘迁移到其他盘"
date: 2026-09-18
tags: ["software-config"]
featureimage: "covers/npm更改默认安装位置.svg"
---

将 npm 全局路径从 C 盘迁移到其他盘，解决 C 盘占用问题。

# 一、查看方式

## (一) 查看当前路径

```bash
npm config get prefix
npm config get cache
npm root -g
```

默认路径：

| 配置     | 默认路径                               |
| ------ | ---------------------------------- |
| prefix | `C:\Users\用户名\AppData\Roaming\npm` |
| cache  | `C:\Users\用户名\.npm`                |

## (二) 查看已安装的全局包

```bash
npm ls -g --depth=0
```

# 二、修改/迁移

## (一) 创建新目录

```cmd
md "F:\ProgramFilesNoMove\npm"
md "F:\ProgramFilesNoMove\npm-cache"
```

## (二) 修改 npm 配置

```bash
npm config set prefix "F:\ProgramFilesNoMove\npm"
npm config set cache "F:\ProgramFilesNoMove\npm-cache"
```

## (三) 迁移旧文件

1.  进入旧 npm 目录：`C:\Users\用户名\AppData\Roaming\npm`
2.  复制 `node_modules`、`.bin` 到 `F:\ProgramFilesNoMove\npm`
3.  复制 `C:\Users\用户名\.npm` 全部内容到 `F:\ProgramFilesNoMove\npm-cache`

## (四) 配置环境变量

此电脑 → 右键属性 → 高级系统设置 → 环境变量

1.  **用户变量 - Path** 新增：`F:\ProgramFilesNoMove\npm`
2.  **新增系统变量**：`NODE_PATH` → `F:\ProgramFilesNoMove\npm\node_modules`

# 三、验证

## (一) 验证路径变更

新开终端执行：

```bash
npm config get prefix
npm config get cache
```

输出应为新的 F 盘路径。

## (二) 验证全局包

```bash
npm ls -g --depth=0
```

能列出所有全局包即迁移成功。

> **注意：** 本地项目 `node_modules` 跟随项目文件夹，与全局路径无关，无需迁移。
