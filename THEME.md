# 主题来源记录

本站使用的 [Blowfish](https://github.com/nunocoracao/blowfish) 主题以 **vendored** 方式引入：
整目录提交进本仓库（`themes/blowfish/`），**不是** git submodule，也没有用 Hugo Modules。

| 项目 | 值 |
| --- | --- |
| 上游仓库 | https://github.com/nunocoracao/blowfish |
| 版本 | v3.7.0 |
| 提交 | `295dae334547f6737a939a5d49f2807d6c0b73b8` |
| 上游发布日期 | 2026-09-13 |
| 引入日期 | 2026-09-17（提交 `ea3c25b`） |
| 主题文件数 | 518（`git ls-files` 计数；另有上游带来的 `.vscode/`、`.watchfire/` 被本仓库 `.gitignore` 忽略） |
| 本地改动 | 无（主题文件从未被修改，`git log --diff-filter=M -- themes/blowfish` 为空） |
| 更新方式 | `powershell -File scripts/update-theme.ps1`（`-Apply` 才实际替换） |

## 与上游的差异

只有删除，没有修改：剔除了上游的 `exampleSite/`（约 88 MB，主题演示站）与 `images/`
（约 18 MB）。其余文件与上游 `main` 在该提交上的内容逐字节一致。
## 版本号说明

上游 v3.7.0 这个 release **没有对应的 git tag**——它的 `tag_name` 是 GitHub 自动生成的
`untagged-<hash>`，git 里真实存在的 tag 停在 v3.6.0。所以 `git clone --branch v3.7.0`
会失败；版本一律按 **commit** 固定。这也是上表同时记录版本号与 commit 的原因，
两者缺一不可。

## 为什么是 vendored，而不是 submodule

选这个方式不是因为省事，而是 submodule 在本站的部署链路里是净损失：

- **CI 需要联网克隆上游。** 用 submodule 后 `.github/workflows/gh-pages.yml` 的
  `actions/checkout` 必须加 `submodules: recursive`，否则 `themes/blowfish` 在 CI 里是
  空目录、构建直接失败。而现在的 workflow 注释写着「已随仓库提交，无需再联网克隆」，
  vendored 让构建不依赖 GitHub 与上游仓库的可用性。
- **体积从 9 MB 变成 100 MB。** submodule 会把上游整仓拉下来，包括那两个我们并不需要、
  且无法在 submodule 里剔除的目录（`exampleSite/` + `images/` ≈ 106 MB）。vendored 只
  保留实际要用的 518 个文件。
- **上游没有可用 tag**（见上），所以 submodule 同样要手工按 commit 固定，省不掉这一步。
- **主题从未被本地修改**，所以 submodule 最主要的收益（清晰区分「上游代码」与「本地改动」）
  在这里用不上——目录本身就是纯净的上游副本。

代价是来源不显式，靠这份文件补上。

## 为什么不用 Hugo Modules

迁移计划里列过这个备选（`[module] imports`），未采用：它要求构建环境具备 Go 与模块代理
可用，而 vendored 让仓库自包含、`git clone` 后即可构建。本站主题固定不追新，也没有
多主题组合需求，模块化的收益不足以抵消这份额外依赖。

## 更新时注意

主题更新后，以下几处**不会报错但可能静默失效**，`update-theme.ps1` 会自动检查：

1. `layouts/partials/favicons.html` 依赖 `themes/blowfish/layouts/partials/head.html`
   里那句 `templates.Exists "partials/favicons.html"`；该调用点消失则图标与 manifest
   全站失效。
2. `layouts/shortcodes/feature.html` 是本仓库对主题同名短代码的覆盖，需与原版同步。
3. 主题的**根级**配置键（`[outputs]`、`[pagination]`、`[markup.highlight]` 等）写在主题
   `config/_default/` 里对本站无效——Hugo 只合并其中的 `[params]`，根级键必须手工写进
   站点 `hugo.toml`。
4. 主题在 `config.toml` 声明的 `[module.hugoVersion]` 兼容区间（当前 `>= 0.163.0,
   <= 0.166.0`）上移时，本机 Hugo 与 CI 里 pin 的版本都要跟着升。

回滚：`git checkout HEAD -- themes/blowfish`
