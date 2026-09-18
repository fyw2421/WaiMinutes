# Wai Minutes

个人技术笔记站点：C++、数据结构、设计模式、Git 与 Markdown 的学习笔记。

用 [Hugo](https://gohugo.io/) + [Blowfish](https://github.com/nunocoracao/blowfish) 主题构建，托管在 GitHub Pages。

线上地址：<https://fyw2421.github.io/WaiMinutes/>

# 一、站点概览

- **首页**：5 张专题卡（CSS 网格）+ 工具卡（油价计算），工具通过弹窗 iframe 打开
- **专题页**：每专题文章列表，带封面 SVG，支持按标题排序
- **全部文章**（/articles/）：卡片铺满页面宽度
- **搜索**：前端模糊搜索（Cmd/Ctrl+K 或 `/` 键触发），`index.json` 由 Hugo 构建时生成
- **暗色模式**：自动跟随系统，`html` 上手动覆盖深蓝极光背景
- **404 页面**：主题内置，随机名言引用

# 二、目录结构

```
content/            30 篇文章 + 各专题的目录页（19 篇是 leaf bundle，自带配图）
assets/
  css/custom.css    站点全部自定义样式（背景、卡片、主题覆盖都在这里）
  covers/           30 篇专题文章的封面 SVG（与文章同名）
  icons/            站点自有的图标（覆盖主题同名文件）
  js/               站点自有的脚本（tools-modal.js、footer-extra.js）
  design-pattern/   设计模式专题的可下载 C++ 源码（56 个 .h/.cpp 文件）
  git-notes/        Git 专题附加文件（.gitignore 模板）
data/tools.yaml     首页「工具」卡片的清单（加新工具改这个）
layouts/            对主题模板的覆盖与扩展（见下方说明）
static/             原样发布到站点的文件（工具本体、图标、附件）
scripts/            开发脚本（见下方说明）
themes/blowfish/    Blowfish 主题，已整目录提交进本仓库（不要修改）
```

## (一) 首页布局

首页由 `content/_index.md` 组合三个短代码构建：

```
{{< feature-grid >}}  → 专题卡（feature.html）+ 工具卡（tools.html）
{{< tools-dialog >}}  → 工具弹窗（必须在此之后、与 feature-grid 平级）
```

- **专题卡**（`shortcodes/feature.html`）：从主题原版复制后加主题色覆盖，主题升级时需同步
- **工具卡**（`shortcodes/tools.html`）：从 `data/tools.yaml` 读取清单，渲染图块网格
- **工具弹窗**（`shortcodes/tools-dialog.html`）：用原生 `<dialog>` + iframe 打开工具页面。
  **不能**放进 feature-grid 内部（会打乱 `:nth-child` 补位规则，使末张卡片拉成整行），
  也**不能**嵌在工具卡片里。
- **iframe**：工具 CSS 含无作用域全局重置（`* { margin:0 }`、`body { flex }`），
  内联会摧毁全站样式，iframe 是唯一隔离手段。弹窗加载失败时 JS 会退回直接跳转（渐进增强）。

## (二) layouts/ 覆盖说明

主题文件在 `themes/blowfish/` 下**不要直接修改**——它整目录提交进本仓库，改动会在升级主题时冲突。
所有站点级定制都放 `layouts/`：

| 文件 | 作用 |
| --- | --- |
| `_default/_markup/render-link.html` | 给下载类链接（`.h` / `.cpp` / `.zip` 等）加 `download` 属性 |
| `partials/favicons.html` | favicon 与 PWA manifest 的链接 |
| `partials/extend-head-uncached.html` | 首页工具弹窗的 JS 注入 |
| `shortcodes/feature.html` | 首页专题卡（色带 + 描述） |
| `shortcodes/tools.html` | 首页「工具」卡片 |
| `shortcodes/tools-dialog.html` | 工具弹窗（**必须与 feature-grid 平级**，理由见文件内注释） |

这几个文件都是从主题原版复制后改的，**主题升级时需要跟着同步**——`scripts/update-theme.ps1`
的自检清单会逐项提醒。

# 三、内容组织

5 个专题目录（`content/` 下），各专题的文章按 `weight` 排序（`[params.list] orderByWeight = true`）：

| 专题 | 文章数 | 文章目录 |
| --- | --- | --- |
| C++ 笔记 | 12 篇 | `cpp-notes/` |
| 数据结构 | 1 篇 | `data-structures/` |
| 设计模式 | 9 篇 | `design-pattern/` |
| Git 速查 | 3 篇 | `git-notes/` |
| Markdown | 5 篇 | `markdown-notes/` |

## (一) 标题编号规则

文章内标题遵循 `markdown-notes/markdown标题编号规则.md` 定义的国家标准格式：

| 级别 | 编号格式 | Markdown 层级 | 示例 |
| --- | --- | --- | --- |
| 一级标题 | `一、` `二、` `三、` | H1 (`#`) | `# 一、常见命名法` |
| 二级标题 | `(一)` `(二)` `(三)` | H2 (`##`) | `## (一) 常见命名法` |
| 三级标题 | `1.` `2.` `3.` | H3 (`###`) | `### 1.1 匈牙利命名法` |
| 四级标题 | `(1)` `(2)` `(3)` | H4 (`####`) | `#### (1) 变量命名` |
| 五级标题 | `①` `②` `③` | H5 (`#####`) | `#####` |

当前文章的 markdown 层级偏高一档（实际用 H2/H3 渲染 H1/H2 编号），修正 pending。

## (二) 其他

- 每篇专题文章带一张封面 SVG，放在 `assets/covers/`，与文章同名
- 19 篇是 **leaf bundle**（目录自带 `index.md`/`_index.md` 和封面），其余是单文件
- 新增文章时：建目录或文件 + 放封面 SVG + 在专题 `_index.md` 的描述中添加条目（≤4 行，每行 ≤7 个汉字，对应窄屏两列卡片的 `min-height` 约束）

# 四、本地开发

需要 **Hugo 0.163.0 – 0.166.0 的 extended 版本**（主题在 `themes/blowfish/config.toml` 里
用 `[module.hugoVersion]` 声明了这个区间；超出范围构建会告警）。

```bash
hugo server --bind 127.0.0.1 --port 1313
```

然后打开 <http://localhost:1313/WaiMinutes/>——**必须带 `/WaiMinutes/` 这段路径**，
因为 `baseURL` 含子路径，直接开 `localhost:1313` 会 404。

> ⚠️ 本地同时开着 `hugo server` 时，另开一个终端跑构建会**卡在构建锁上**（一直等，不报错）。
> 加 `--noBuildLock` 可以跳过：
>
> ```bash
> hugo --noBuildLock
> ```

# 五、构建与部署

推送到 `main` 分支即触发部署，`.github/workflows/gh-pages.yml` 会：

1. `actions/checkout` 拉取仓库（主题已随仓库提交，无需联网克隆）
2. 安装 Hugo 0.166.0 extended
3. `hugo --minify` 构建到 `public/`
4. 通过 `actions/deploy-pages@v4` 发布

**仓库设置里需要一次性配置**：Settings → Pages → Build and deployment →
**Source 选「GitHub Actions」**（不是 "Deploy from a branch"）。

# 六、脚本

| 脚本 | 用途 |
| --- | --- |
| `scripts/update-theme.ps1` | 更新 vendored 主题（先预览，`-Apply` 才替换）。主题来源与升级细节见 [THEME.md](THEME.md) |
| `scripts/make-icons.py` | 生成站点图标（`static/` 下的 favicon 系列与 manifest）。**图标是产物，改样式请改这个脚本再重跑，不要手改 PNG**。需 Python + Pillow |
| `scripts/re-migrate-posts.ps1` | ⚠️ **历史脚本，不要重跑。** 它把 30 篇 Jekyll 文章按扁平路径生成，而其中 19 篇现在是 leaf bundle，重跑会生成重复页面并覆盖已恢复的 front matter。保留仅供查阅当初的字段映射 |

# 七、主题

Blowfish **3.7.0**，以 vendored 方式引入（整目录提交，非 submodule）。
来源 commit、与上游的差异、为什么不用 submodule、升级注意事项见 [THEME.md](THEME.md)。

# 八、分支与发布

分支与发布流程见 [AGENTS.md](AGENTS.md)。核心规则：

- **只在 `develop` 上提交**，永不直接改 `main`
- 发布时在 `main` 上 `git merge --squash develop`，然后同步回 `develop`

# 九、依赖

- Hugo extended 0.163.0 – 0.166.0（CI 使用 0.166.0）
- `scripts/make-icons.py` 需要 Python 与 Pillow（仅在重新生成图标时需要）