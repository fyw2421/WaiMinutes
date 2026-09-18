# Wai Minutes

个人技术笔记站点：C++、数据结构、设计模式、Git 与 Markdown 的学习笔记。

用 [Hugo](https://gohugo.io/) + [Blowfish](https://github.com/nunocoracao/blowfish) 主题构建，托管在 GitHub Pages。

线上地址：<https://fyw2421.github.io/WaiMinutes/>

## 本地开发

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

## 构建与部署

推送到 `main` 分支即触发部署，`.github/workflows/gh-pages.yml` 会：

1. `actions/checkout` 拉取仓库（主题已随仓库提交，无需联网克隆）
2. 安装 Hugo 0.166.0 extended
3. `hugo --minify` 构建到 `public/`
4. 通过 `actions/deploy-pages@v4` 发布

**仓库设置里需要一次性配置**：Settings → Pages → Build and deployment →
**Source 选「GitHub Actions」**（不是 "Deploy from a branch"）。

## 目录结构

```
content/            30 篇文章 + 各专题的目录页（19 篇是 leaf bundle，自带配图）
assets/
  css/custom.css    站点全部自定义样式（背景、卡片、主题覆盖都在这里）
  covers/           每个专题的封面 SVG
  icons/            站点自有的图标（覆盖主题同名文件）
  js/               站点自有的脚本
  design-pattern/   文章里可下载的源码（.h / .cpp）
data/tools.yaml     首页「工具」卡片的清单
layouts/            对主题模板的覆盖与扩展（见下方说明）
static/             原样发布到站点的文件（图标、工具本体、附件）
scripts/            开发脚本（见下方说明）
themes/blowfish/    Blowfish 主题，已整目录提交进本仓库
```

### layouts/ 里都是对主题的覆盖或扩展

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

## 脚本

| 脚本 | 用途 |
| --- | --- |
| `scripts/update-theme.ps1` | 更新 vendored 主题（先预览，`-Apply` 才替换）。主题来源与升级细节见 [THEME.md](THEME.md) |
| `scripts/make-icons.py` | 生成站点图标（`static/` 下的 favicon 系列与 manifest）。**图标是产物，改样式请改这个脚本再重跑，不要手改 PNG** |
| `scripts/re-migrate-posts.ps1` | ⚠️ **历史脚本，不要重跑。** 它把 30 篇 Jekyll 文章按扁平路径生成，而其中 19 篇现在是 leaf bundle，重跑会生成重复页面并覆盖已恢复的 front matter。保留仅供查阅当初的字段映射 |

另有一个 `.covergen/repalette.ps1`，用于批量切换封面 SVG 的冷暖配色。

## 主题

Blowfish **3.7.0**，以 vendored 方式引入（整目录提交，非 submodule）。
来源 commit、与上游的差异、为什么不用 submodule、升级注意事项，见 [THEME.md](THEME.md)。

## 依赖

- Hugo extended 0.163.0 – 0.166.0（CI 使用 0.166.0）
- `scripts/make-icons.py` 需要 Python 与 Pillow（仅在重新生成图标时需要）
