# 首页工具卡片与工具弹窗 设计文档

**日期:** 2026-09-18
**状态:** 待评审
**相关:** 本文档是实施计划 `docs/superpowers/plans/2026-09-18-homepage-tools-card.md` 的依据

## 背景

站点当前首页（`content/_index.md`，landing 布局）用主题的 `feature-grid` 列出 5 张专题卡：C++ 笔记、Git 速查、Markdown、数据结构、设计模式。

另有一个独立的小工具「汽油百分比计算器」（单文件 HTML，约 50KB，自包含，无本地依赖），目前放在仓库之外，只能单独打开。希望把它集成到站点里，并从首页进入。

需求原文：「我想在首页第一个 card 放工具，这个是第一个。我希望一个 card 可以容纳四个工具的图标。」

因此本设计要解决两件事：**首页新增一张「工具」卡片**（容纳 4 个工具），以及**工具的打开方式**（弹窗而非页面跳转）。

## 目标

1. 首页第一张卡片为「工具」，内含 4 个工具图标位（当前只有 1 个工具，其余待添加）
2. 点击图标在页内弹窗打开工具，不跳转、不离开当前页面
3. 后续添加新工具时，改动量应尽可能小（只加文件 + 改一处清单）

## 非目标

- 不改动现有的 5 张专题卡及其短代码（`layouts/shortcodes/feature.html`）
- 不引入前端框架或构建步骤（站点无 npm 流程，主题 Tailwind 是预编译提交的）
- 不做工具的多语言版本（站点只有中文）
- 不改造工具自身的功能逻辑（只做一处窄屏样式修复）

## 实测约束

以下数值均由浏览器实测得出，是设计的事实基础：

| 约束 | 实测值 | 来源 |
|------|--------|------|
| 首页内容容器 | `max-w-3xl` = 768px | `themes/blowfish/layouts/partials/home/landing.html` |
| 卡片实际宽度 | 245px（桌面 1440px 视口）| 浏览器实测 |
| 现有卡片数 | 5 张，3 列网格呈 3+2，尾行空一格 | 浏览器实测 |
| `grid-balance-3` 触发条件 | `:last-child:nth-child(3n+1)` | `themes/blowfish/assets/css/compiled/main.css:4765` |
| 卡片内可用宽度 | 245 − 48(p-6) = 197px | 计算 |
| 2×2 图标格 | 桌面每格 94px；移动端（卡片 302px）每格 121px | 浏览器实测 |
| 4 图标单行所需 | 40×4 + 8×3 = 184px（可放入 197px，但每列标签仅 43px，中文放不下）| 计算 |
| 工具横向溢出条件 | 内容最小宽度 335px；`clientWidth`（视口减滚动条）小于它时溢出，差值即两者之差 | 浏览器实测 + 计算 |
| 工具内容高度 | 卡片 722px；输入后 782px | 浏览器实测 |

**关键推论：** 当前 5 张卡时 `grid-balance-3` 不触发（最后一张是第 5 个，5 ≠ 3n+1）；加入工具卡成为 6 张后，正好 2×3 排满，规则同样不触发。**因此新增卡片不需要处理跨列补位。**

## 关键决策

### 决策 1：工具必须用 iframe 加载，不能内联

工具的 CSS 含无作用域的全局重置：

```css
* { margin: 0; padding: 0; box-sizing: border-box; }
body { display: flex; justify-content: center; background: #f0f4f5; }
```

若内联进首页，`*` 会打掉全站 Tailwind 的所有间距，`body` 的 flex 与浅色背景会覆盖站点主题（暗色模式下整页变白）。

**结论：iframe 是唯一可行方案，不是"更干净的选项"。** 同源加载（工具部署在站点自身的 `static/`）所以无跨域限制。

### 决策 2：弹窗用原生 `<dialog>`

宿主站点（hux.pro 式的做法）用的是自定义 `div[role=dialog]` + 手写遮罩。本站改用原生 `<dialog>`：

- Esc 关闭、焦点陷阱、`::backdrop` 均为浏览器内置
- `showModal()` 渲染到 top-layer，不受 z-index 堆叠上下文影响
- 代码量约为自定义实现的 1/3，无障碍默认合格

需自行补一条：点击遮罩关闭（原生不提供）。

### 决策 3：图标复用主题的 icon 机制

主题 `partials/icon.html` 的实现是 `resources.Get (print "icons/" . ".svg")`，而 Hugo 的资源查找是**站点 `assets/` 优先于主题**（已实测验证：同名文件时渲染出站点版本）。

因此站点放 `assets/icons/<name>.svg` 即可用 `{{ partial "icon.html" "<name>" }}` 调用，**无需修改主题文件**。需注意主题的 `.icon svg { width:1em; height:1em }` 意味着图标尺寸由字号控制。站点图标名须避开主题已有的 127 个（如 `code`、`cloud`、`search`）。

### 决策 4：按实际数量渲染格子，不留空占位

4 个工具位是容量上限而非固定布局。按实际数量渲染（1/2/3/4 个），未使用的格子不显示——空占位看起来像未完工。

格子自适应复用主题 `grid-balance` 的补位写法 `:last-child:nth-child(odd) { grid-column: 1/-1 }`，效果：1 个铺满、2 个并排、3 个为 2+1（末行铺满）、4 个为 2×2。

### 决策 5：数据驱动，清单与实现分离

工具清单放 `data/tools.yaml`，短代码遍历渲染。新增工具只需：放文件到 `static/tools/<name>/`、放图标到 `assets/icons/`、在 yaml 追加一条。**不需要改模板。**

### 决策 6：色带取青色系

5 张专题卡的色带色相依次为橄榄金、绯红、绿、琥珀、品红。青色是唯一未被占用的色相，且与工具自身 UI 的主色 `#0d9488` 同源，形成呼应。

### 决策 7：图块用 `<a href>` 而非 `<button>`（渐进增强）

图块是链接（指向工具的真实 URL），JS 拦截点击改为开弹窗。

- JS 未加载或执行失败时，点击仍然可用——直接跳转到工具页
- 保留原生链接语义：可中键新标签打开、可复制链接、可被搜索引擎抓取
- 成本为零：仅多一个 `href` 属性

代价：JS 正常时需 `preventDefault()`，且弹窗关闭后焦点的返回依赖 `<dialog>` 的原生行为。

## 文件结构

| 文件 | 职责 |
|------|------|
| `assets/icons/fuel.svg` | 油价工具图标（手绘，`fill="currentColor"`，24×24 viewBox）|
| `data/tools.yaml` | 工具清单：name / icon / url / label |
| `layouts/shortcodes/tools.html` | 渲染工具卡（含数据读取与格子布局）|
| `layouts/shortcodes/tools-dialog.html` | 渲染工具弹窗 DOM（**新建**）|
| `assets/js/tools-modal.js` | 弹窗交互：打开、关闭、Esc、点遮罩 |
| `assets/css/custom.css` | 追加：卡片样式、格子、图块、弹窗容器（**修改现有文件**）|
| `layouts/partials/extend-head-uncached.html` | 注入 JS（**新建，主题预留的挂载点**）|
| `content/_index.md` | 在 `feature-grid` 内插入 `{{< tools >}}`，并在 `{{< /feature-grid >}}` 之后插入 `{{< tools-dialog >}}`（**修改**）|
| `static/tools/fuel-calculator/index.html` | 工具本体（从现有目录复制）|
| `static/tools/fuel-calculator/VERSION` | 版本号（工具按同级相对路径读取）|

### 文件结构说明

- **弹窗 DOM 由短代码输出**，而非放进挂载点：弹窗只在首页需要，放挂载点会给全站每页增加无用字节。
- **JS 走 `extend-head-uncached.html`（不是 `extend-head.html`）**：主题 `head.html:233` 调 `extend-head.html` 时传入的是 `.Site`，没有页面上下文，无法用 `.IsHome` 判断首页；而 `head.html:238` 调 `extend-head-uncached.html` 时传入 `.`（当前页），才能做首页判断。命名里的 "uncached" 指不按 Site 缓存，代价是每页执行一次判断，可忽略。
- **CSS 追加到 `assets/css/custom.css`**：该文件已被主题 head.html 自动 bundle，样式集中在一处便于排查；首页专用的样式放这里与现有 `.topic-card` 系列做法一致。
- 工具目录**只复制 `index.html` 与 `VERSION`**：工具的 `README.md`、`update-version.bat`、`.gitignore` 不应进 `static/`（`static/` 下所有文件都会被发布到站点，等于上传无用文件）。

## 交互流程

```
用户点击图块（a.tool-tile，href 指向工具真实 URL）
  → JS 捕获 click，preventDefault() 阻止跳转
  → 读取 data-tool-url，设置 iframe.src
  → dialog.showModal()
  → 工具在 iframe 内独立运行（自己的 CSS、自己的网络请求）

JS 不可用时：
  → 不拦截，浏览器按 href 直接跳转到工具页（功能不丢）

关闭途径（三种）：
  → 点击关闭按钮
  → 按 Esc（原生）
  → 点击遮罩（自行监听：e.target === dialog 时关闭）

关闭时把 iframe.src 置为 about:blank，停止工具内的定时与网络请求。
```

## 验证方式

1. `hugo` 构建通过、无警告
2. 首页 6 张卡呈 2×3，工具卡在首位、色带为青色
3. 点击图标弹窗打开，工具加载出真实油价数据
4. Esc、关闭按钮、点遮罩三种方式均可关闭
5. 键盘 Tab 可聚焦图块，Enter 打开；弹窗打开后焦点不逃出
6. 移动端 390px / 360px 宽度下无横向滚动条
7. 暗色模式下卡片与弹窗（浅色）观感正常
8. 现有 5 张专题卡的外观与链接未受影响

## 风险

| 风险 | 影响 | 对策 |
|------|------|------|
| 工具横向溢出（内容最小宽度 335px > `clientWidth`） | 窄视口弹窗内必然出现 | 工具内 `@media (max-width:400px)` 加 `flex-wrap`（本计划包含）|
| 短代码与 `feature.html` 混淆 | 误改专题卡 | 新建独立 `tools.html`，不触碰 `feature.html` |
| 主题升级后挂载点失效 | JS/CSS 不加载 | `update-theme.ps1` 已有自检项（检查 extend 挂载点）|
| 暗色模式下弹窗为浅色 | 视觉突兀 | 接受：工具是独立应用，浅色是其设计（已在评审中确认）|

## 未决事项

无。色带颜色、图标风格、格子数量策略均已在评审中确定。
