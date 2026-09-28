# AGENTS.md — 在本仓库工作时的约定

给在此仓库工作的 AI 代理（以及新加入的人）的操作约定。
项目本身是什么、怎么本地跑、目录结构，见 [README.md](README.md)；
主题来源与升级，见 [THEME.md](THEME.md)。

---

## 分支与发布

| 分支 | 角色 |
| --- | --- |
| `main` | **只放发布版本**。每次发布一个 squash 提交，历史即发布日志 |
| `develop` | 日常开发，保留完整提交历史 |

### 铁律

- **只在 `develop` 上提交，永不直接改 `main`。** 紧急修复也一样：在 `develop` 上改，
  或从 `main` 开 hotfix 分支、修完合回 `develop`。
  原因：squash 后 `main` 与 `develop` 的提交**内容相同但 SHA 不同**，git 认为它们无关；
  在 `main` 上直接提交会让下次 squash 冲突。
- **第 6 步不能省。** `merge --squash` 不会在 `develop` 留下「已合并」记录，
  必须把 `main` 同步回 `develop`，否则下次 squash 会把旧改动重新当成新改动而冲突
  （已实测复现：`CONFLICT (content)`，因为找不到真正的共同祖先）。

### 发布流程

```bash
# 1. 切到 main 并同步远端
git checkout main
git pull origin main

# 2. 把 develop 的全部改动压缩成一个提交
git merge --squash develop

# 3. 提交（写清这次发布了什么）
git commit -m "Release: 合并 develop 功能至 v1.1.0"

# 4. 打附注 tag（-a 而非轻量 tag：含作者/日期/说明，push --follow-tags 能带上）
git tag -a v1.1.0 -m "v1.1.0"

# 5. 推送 main 与 tag（CI 只在 push main 时触发部署）
git push origin main
git push origin v1.1.0

# 6. ★ 关键：把 main 同步回 develop，否则下次发布会冲突
git checkout develop
git merge main
git push origin develop
```

**本站没有版本号文件**——Hugo 构建、页面渲染都不读版本号，所以流程里没有「发布前改版本号」这一步。
（`static/tools/fuel-calculator/VERSION` 是**汽油计算器自己的版本**，与站点发布版本无关，别混用。）

**tag 格式**：`vX.Y.Z`，附注 tag。首个是 `v1.0.0`。

## Markdown 文档格式规则

项目内所有 `.md` 文件的标题必须遵循以下编号格式和排版规则（定义见 `content/markdown-notes/markdown文件格式规则.md`）：

### 编号格式

| 级别 | 编号格式 | Markdown 层级 | 示例 |
| --- | --- | --- | --- |
| 文档标题 | frontmatter `title` | H1 (`#`) | `# 文档标题` |
| 一级标题/章节标题 | `一、` `二、` `三、` | H1 (`#`) | `# 一、常见命名法` |
| 二级标题/子章节 | `(一)` `(二)` `(三)` | H2 (`##`) | `## (一) 常见命名法` |
| 三级标题 | `1.` `2.` `3.` | H3 (`###`) | `### 1.1 匈牙利命名法` |
| 四级标题 | `(1)` `(2)` `(3)` | H4 (`####`) | `#### (1) 变量命名` |
| 五级标题 | `①` `②` `③` | H5 (`#####`) | `#####` |

### 排版规则

1. **父子标题间空一行** — 父标题与子标题之间必须有一个空行
2. **禁止跨级** — H1 后可以是 H1（下一章节）或 H2（子章节），H2 后可以是 H2 或 H3，不可跳级
3. **标题上下各一个空行** — 标题上方和下方必须各有一个空行，禁止紧贴内容或其他标题

文档结构惯例：`# 文档标题`（H1）或 `# 一、标题`（H1）下全部子标题为 `## (一)`（H2），同级章节为 `# 二、`（H1）。

## 文章 Frontmatter 与封面

每篇专题文章必须有 YAML frontmatter（`---` 包裹），必填字段：

| 字段 | 格式 | 说明 |
| --- | --- | --- |
| `title` | `Markdown{NN} : {英文描述}` 或中文标题 | 文章标题 |
| `weight` | 正整数 | 专题内排序 |
| `description` | 简短描述 | 页描述 / SEO 摘要 |
| `date` | `YYYY-MM-DD` | 文章日期 |
| `tags` | `["{分类名}"]` | 与所在专题目录同名 |
| `featureimage` | `covers/{文件名}.svg` | 封面 SVG（见下） |

### 封面 SVG

- 位于 `assets/covers/`，文件名与文章文件名一致
  （如 `markdown-notes01-about-markdown.md` → `markdown-notes01-about-markdown.svg`）
- **例外——同一系列可共用一张封面**：专题内文章数量很多时（教程类，几十篇），
  允许全系列共用一张通用封面，`featureimage` 统一指向它
  （如 68 章的 `covers/typescript-tutorial.svg`），不必一章一张
- 统一规格：1500×1000 SVG，深色渐变底 + 顶部光晕
  （背景、光晕、装饰线三色取自同一色相，各专题独立配色）
- 布局元素：编号标签（`MARKDOWN · NN` 或 `{TOPIC} · NN`）、大标题（82px）、副标题（32px）、装饰线、卡片区（圆角矩形 + 序号 + 中英文标题）、底部说明 + `Wai Minutes` 水印
- 编号规则中已有的专题文章按序号编号（`01`、`02`...）；参考性文档用描述性标题，序号延后
- 仅 `scripts/make-icons.py` 生成的 favicon 系列是脚本产出；封面 SVG 需手动编写或按此规范生成

## 行尾与编码

全仓库统一 **UTF-8 无 BOM + LF**（行尾单字节 `0x0A`）：

- **新建文件一律 LF + 无 BOM**——`.md`、`.svg`、`.html`、`.css`、`.yaml`、`.h`、`.cpp` 等全部适用
- **编辑已有文件只改内容，保留它原来的行尾。** 本仓库 HEAD 里 CRLF 与 LF 是混用的
  （`assets/design-pattern/*.h|*.cpp` 与部分 content 文件为 CRLF，其余为 LF），
  顺手把整文件行尾换掉会让 `git diff` 变成整文件级、**无法审阅**
- **禁止 UTF-8 BOM**（`EF BB BF`）。HEAD 里 BOM 数为 0；BOM 会让 `grep`/`diff`/linter
  以为整文件被改过，Hugo 输出 HTML 时还得额外剥掉
- **例外**：Windows 原生 `.bat` / `.cmd` 必须用 CRLF（本仓库目前没有这两类文件）
- **例外**：`assets/design-pattern/` 与 `assets/datastructure/` 下部分 `.h` / `.cpp` 是 **GBK 编码**
  （含中文注释）。不要用 UTF-8 解码工具重写它们——会把内容写坏；
  需要改行尾时只做字节级替换（`0D 0A` → `0A`）
- 提交前自查：带 `--ignore-cr-at-eol` 与不带该参数的 `git diff --numstat` 数字应接近，
  差值就是纯行尾噪音（2026-09-28 对齐后实测差值 = 2）

## 构建

- **不要修改 `themes/blowfish/`。** 主题整目录提交进本仓库（vendored），改动会在升级主题时冲突。
  所有定制放 `layouts/`、`assets/`、`data/`，见下。
- 本地开着 `hugo server` 时，另开终端构建会**卡在构建锁上**（一直等、不报错）。
  一律用：
  ```bash
  hugo --noBuildLock
  ```
- 提交前**必须**构建通过，且输出**零警告**。
- CI（`.github/workflows/gh-pages.yml`）用 `hugo --minify` 构建；本地可照此预演。

## 站点定制的落点

| 想改什么 | 改哪里 |
| --- | --- |
| 首页专题卡 / 工具卡片 / 工具弹窗 | `layouts/shortcodes/` |
| 下载类链接行为 | `layouts/_default/_markup/render-link.html` |
| favicon、PWA manifest | `layouts/partials/favicons.html` |
| 首页注入脚本 | `layouts/partials/extend-head-uncached.html` |
| 任何样式 | `assets/css/custom.css`（主题的 Tailwind 是预编译提交的，站点新类只能写这里） |
| 工具清单 | `data/tools.yaml` |
| 站点图标 | 改 `scripts/make-icons.py` 后重跑，**不要手改 `static/` 下的 PNG** |

这些文件都是从主题原版复制后修改的，**主题升级时需跟着同步**——`scripts/update-theme.ps1`
的自检清单会逐项提醒。

## 工具与附件

- 工具本体放 `static/tools/<name>/`。**`static/` 下所有文件都会原样发布**，
  所以只放 `index.html` 与它的 `VERSION`，不要放 README、`.bat`、`.gitignore`。
- 加新工具：放文件 → 放图标到 `assets/icons/` → 在 `data/tools.yaml` 追加一条。
  **`url` 不要写前导斜杠**（Hugo 的 `relURL` 对以 `/` 开头的输入会丢掉 `/WaiMinutes` 子路径，线上 404）。

## 脚本

| 脚本 | 用途 |
| --- | --- |
| `scripts/make-icons.py` | 生成站点图标（需 Python + Pillow） |
| `scripts/update-theme.ps1` | 更新 vendored 主题（先预览，`-Apply` 才替换） |
| `scripts/re-migrate-posts.ps1` | ⚠️ **历史脚本，任何情况下都不要重跑**（内含 `throw` 硬拦截）。它按扁平路径生成文章，而其中 19 篇现在是 leaf bundle，重跑会生成重复页面并覆盖 front matter |

## 用户决策要求

所有修复需用户确认后实施。**不得擅自执行任何修改**，直到用户明确指令。

## 不要提交

- `public/`、`resources/_gen/`、`.hugo_build.lock`、`.superpowers/` —— 本仓库 `.gitignore` 已覆盖
- `.claude/settings.local.json` —— 靠**全局** gitignore（`~/.config/git/ignore`）忽略，
  不是本仓库的规则。所以别把 `.claude/` 下的东西加进来，也别在 `.gitignore` 里重复加规则
- 未经用户明确指示，**不要执行 `git commit` / `git push`**：改动做完后停在工作区，由用户决定
