# 首页工具卡片与工具弹窗 实施计划

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 在首页新增一张「工具」卡片（首位，容纳 4 个工具图标），点击图标在原生 `<dialog>` 弹窗内用 iframe 打开工具，首个工具为汽油百分比计算器。

**Architecture:** 数据驱动——`data/tools.yaml` 存清单，`layouts/shortcodes/tools.html` 遍历渲染卡片，`layouts/shortcodes/tools-dialog.html` 渲染页面级弹窗 DOM（两者必须分开：弹窗若留在卡片短代码里会变成 feature-grid 的网格子元素，打乱主题的 `:nth-child` 补位规则），`assets/js/tools-modal.js` 负责开合交互。工具以 iframe 隔离加载（其 CSS 含无作用域的 `*` 重置，内联会摧毁全站样式），部署在站点自身 `static/` 下所以同源无跨域限制。图标复用主题的 `partials/icon.html` 机制（站点 `assets/` 优先于主题）。

**Tech Stack:** Hugo 0.166.0（extended）、Blowfish 3.7.0（vendored）、原生 HTML `<dialog>`、原生 JS（无框架、无构建步骤）

**Spec:** `docs/superpowers/specs/2026-09-18-homepage-tools-card-design.md`

## Global Constraints

- **不修改主题文件**（`themes/blowfish/` 下任何文件）——主题是 vendored，改动会在升级时冲突
- **不修改 `layouts/shortcodes/feature.html`**——它是 5 张专题卡的渲染器，与本次需求无关
- 图标 SVG 必须用 `fill="currentColor"` 或 `stroke="currentColor"`（主题 `.icon svg { width:1em; height:1em }` 靠字号定尺寸、靠 `currentColor` 继承颜色）
- 站点图标文件名须避开主题已有的 127 个图标名（如 `code` `cloud` `search` `list` `edit` `download`）
- `static/` 下所有文件都会被发布——不要在 `static/tools/` 放 README、.bat、.gitignore
- 颜色变量写法为 `rgb(var(--color-<name>))`，变量定义在 `themes/blowfish/assets/css/schemes/blowfish.css`（逗号分隔三元组，如 `--color-primary-500: 59, 130, 246`）
- 构建命令统一用 `hugo --noBuildLock`——本地常驻 `hugo server` 会持有构建锁
- 每次提交前必须 `hugo --noBuildLock` 构建通过

---

### Task 1: 工具图标 SVG

**Files:**
- Create: `assets/icons/fuel.svg`

**Interfaces:**
- Consumes: 无
- Produces: 图标名 `fuel`，供 Task 2 的短代码通过 `{{ partial "icon.html" .icon }}` 使用

- [ ] **Step 1: 确认主题无同名图标**

Run: `ls themes/blowfish/assets/icons/ | grep -x 'fuel.svg'`
Expected: 无输出（退出码 1）——若输出则该名已占用，需换名

- [ ] **Step 2: 创建图标**

写入 `assets/icons/fuel.svg`（24×24 viewBox、线框风格、`stroke="currentColor"`，与主题图标的 `currentColor` 约定一致）：

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round">
  <rect x="3" y="3" width="10" height="18" rx="2"/>
  <path d="M3 9h10"/>
  <path d="M5.5 6h2"/>
  <path d="M13 10h2a2 2 0 0 1 2 2v4a2 2 0 0 0 4 0V9.5L18.5 7"/>
</svg>
```

- [ ] **Step 3: 验证站点资源覆盖主题资源**

Run: `hugo --noBuildLock --quiet && grep -o 'icons/fuel' public/index.html | head -1`
Expected: 此刻无输出（图标尚未被任何模板引用）——本步只验证资源可被 Hugo 识别，不报错即可

- [ ] **Step 4: 提交**

```bash
git add assets/icons/fuel.svg
git commit -m "feat: add a fuel-pump icon for the tools card"
```

---

### Task 2: 工具清单与卡片短代码

**Files:**
- Create: `data/tools.yaml`
- Create: `layouts/shortcodes/tools.html`
- Create: `layouts/shortcodes/tools-dialog.html`
- Modify: `assets/css/custom.css`（追加到文件末尾）

**Interfaces:**
- Consumes: 图标名 `fuel`（Task 1）；`--color-primary-*`、`--color-neutral-*` 变量（主题已有）
- Produces: 短代码 `{{< tools >}}`（工具卡）与 `{{< tools-dialog >}}`（弹窗，须与 `{{< /feature-grid >}}` 平级调用）；DOM 结构 `.tools-card` / `.tools-grid` / `a.tool-tile[data-tool-url]` 由 `tools.html` 输出，`#tool-dialog` / `#tool-frame` / `#tool-close` 由 `tools-dialog.html` 输出——Task 3 的 JS 依赖这些选择器

- [ ] **Step 1: 创建工具清单**

写入 `data/tools.yaml`：

```yaml
# 首页「工具」卡片的清单。加新工具只需三步：
#   1. 把工具文件放到 static/tools/<name>/（自包含的 index.html）
#   2. 把图标放到 assets/icons/<icon>.svg（须含 fill/stroke="currentColor"）
#   3. 在下面追加一条
# 图标的尺寸由字号控制（主题 .icon svg { width:1em; height:1em }），
# 改大小请调 CSS 里的 font-size，不要改 SVG 的宽高属性。
#
# ⚠ url 不要写前导斜杠。本站部署在 /WaiMinutes/ 子路径下，而 Hugo 的 relURL
# 对以 "/" 开头的输入会把子路径丢掉——实测 `"/tools/x/" | relURL` 得到
# "/tools/x/"（错，线上 404），而 `"tools/x/" | relURL` 得到
# "/WaiMinutes/tools/x/"（对）。absURL 有同样的问题。
# 短代码里用 `{{ .url | relURL }}` 转换后是绝对路径，因此在任何页面调用都对。

- name: 油价计算
  icon: fuel
  url: tools/fuel-calculator/
  label: 打开汽油百分比计算器
```

- [ ] **Step 2: 创建卡片短代码**

写入 `layouts/shortcodes/tools.html`：

```html
{{/* 首页「工具」卡片。
     与 layouts/shortcodes/feature.html（专题卡）是两个独立的短代码，互不影响：
     那个管 5 张专题卡，这个只管工具。

     ⚠ 工具必须用 iframe 加载，不能内联进来。工具的 CSS 里有**无作用域**的
     `* { margin:0; padding:0; box-sizing:border-box }` 和
     `body { display:flex; background:#f0f4f5 }`——内联会打掉全站 Tailwind 的
     所有间距，并在暗色模式下把整页背景覆盖成浅色。iframe 是隔离它的唯一手段
     （工具部署在本站 static/ 下，同源，无跨域限制）。

     图块用 <a href> 而不是 <button>：JS 未加载时点击仍能跳转到工具页（渐进增强），
     且保留中键新标签、复制链接、被搜索引擎抓取这些原生链接行为。

     弹窗 DOM 由 layouts/shortcodes/tools-dialog.html 输出而非放进全局挂载点——
     它只在首页需要，放挂载点会让全站每页都多出这段无用标记。弹窗之所以**不能**
     放在本短代码里，是因为本短代码被调用在 feature-grid 内部，会成为一个网格
     子元素而打乱主题的 :nth-child 补位规则——详见 tools-dialog.html 的注释。 */}}

{{ $tools := hugo.Data.tools }}

<article class="topic-card tools-card group overflow-hidden rounded-2xl border border-neutral-200/80 bg-white/60 shadow-sm backdrop-blur-xl transition duration-200 hover:-translate-y-1 hover:border-primary-300 hover:shadow-xl hover:shadow-primary-500/10 dark:border-neutral-800 dark:bg-neutral-900/50 dark:hover:border-primary-700">
  {{/* 色带：结构与专题卡一致（三列网格让标题居中、数量靠右），
       但颜色写死不从封面提取——工具没有封面，且青色是专题卡未占用的色相。
       色值取自工具自身 UI 的主色 #0d9488，形成呼应。 */}}
  <div class="topic-band tools-band" style="background-image:linear-gradient(180deg, #0f3f3c 0%, #0b2b29 100%)">
    <h2 class="topic-band__name">工具</h2>
    <span class="topic-count">{{ len (or $tools slice) }}<span class="sr-only"> 个</span></span>
  </div>

  <div class="topic-card__body tools-card__body p-6">
    {{ if $tools }}
      <div class="tools-grid">
        {{ range $tools }}
          {{/* relURL 是必需的：它把清单里的相对路径（tools/fuel-calculator/）
               转成带子路径的绝对路径（/WaiMinutes/tools/fuel-calculator/）。
               清单里的 url 不能带前导斜杠，否则 relURL 会丢掉子路径——详见
               data/tools.yaml 的注释。 */}}
          {{/* 不加 aria-label：可见文字（.name，如「油价计算」）本身就是正确的链接名。
               若设一个不包含可见文字的 aria-label（如清单里的 .label「打开汽油百分比
               计算器」），语音控制用户按可见文字就无法激活该链接，违反 WCAG 2.5.3
               Label in Name（A 级）。 */}}
          <a class="tool-tile" href="{{ .url | relURL }}" data-tool-url="{{ .url | relURL }}">
            <span class="tool-tile__icon">{{ partial "icon.html" .icon }}</span>
            <span class="tool-tile__label">{{ .name }}</span>
          </a>
        {{ end }}
      </div>
    {{ else }}
      <p class="tools-empty">工具待添加</p>
    {{ end }}
  </div>
</article>
```

- [ ] **Step 3: 创建弹窗短代码**

写入 `layouts/shortcodes/tools-dialog.html`：

```html
{{/* 首页工具弹窗。与 layouts/shortcodes/tools.html（工具卡片）是两个独立的短代码。

     ⚠ 弹窗必须与 feature-grid 平级调用（在 {{< /feature-grid >}} 之后），
     既不能放进 feature-grid 内部，也不能嵌在工具卡片里：

     1) 不能放进 feature-grid：它会成为网格的一个直接子元素，把子元素数量从
        6 变成 7。主题的补位规则是
        `.grid-balance-3 > :last-child:nth-child(3n+1) { grid-column: 1/-1 }`，
        7 = 3×2+1 命中该规则，末张专题卡被拉成整行（实测 768px vs 其他 245px）——
        这是对既有 5 张专题卡的可见回归。`:nth-child()` 会把 display:none 的元素
        也算进去，所以**无法**用 CSS 把弹窗藏起来规避，只能在 DOM 上移出去。

     2) 也不要嵌在工具卡片里：卡片本身位于 feature-grid 内部，把弹窗留在其中只会
        让它深埋在网格 DOM 里；弹窗是页面级模态，正确位置是与 feature-grid 平级。

     弹窗用原生 <dialog>：Esc 关闭、焦点陷阱、::backdrop 都是浏览器内置的，
     showModal() 还会把它渲染到 top-layer，不受任何 z-index 堆叠上下文影响。
     JS 见 assets/js/tools-modal.js（由 extend-head-uncached.html 注入）。 */}}
<dialog id="tool-dialog" class="tool-dialog" aria-label="工具">
  <div class="tool-dialog__bar">
    <span class="tool-dialog__title" id="tool-dialog-title"></span>
    <div class="tool-dialog__actions">
      <a class="tool-dialog__open" id="tool-open-external" href="#" target="_blank" rel="noopener noreferrer">
        新标签打开
      </a>
      <button type="button" class="tool-dialog__close" id="tool-close" aria-label="关闭">
        <span aria-hidden="true">&times;</span>
      </button>
    </div>
  </div>
  <div class="tool-dialog__body">
    <iframe id="tool-frame" class="tool-frame" title="工具" loading="lazy"></iframe>
  </div>
</dialog>
```

- [ ] **Step 4: 追加样式**

追加到 `assets/css/custom.css` **末尾**：

```css
/* ---------- 首页：工具卡片与工具弹窗 ---------- */

/* 网格固定两列（2 个工具并排）。格子不需要 3 列——卡片内宽仅 197px，
   三列每格只剩 55px，中文标签放不下。 */
.tools-grid {
  display: grid;
  grid-template-columns: repeat(2, 1fr);
  gap: 10px;
}

/* 数量为奇数时让最后一个铺满整行，避免尾行右侧空一格。
   这条正是主题 grid-balance 的补位写法（见 compiled/main.css 的
   `.grid-balance > :last-child:nth-child(odd)`），沿用同一惯例。
   效果：1 个铺满 / 2 个并排 / 3 个为 2+1（末行铺满）/ 4 个为 2×2。 */
.tools-grid > :last-child:nth-child(odd) {
  grid-column: 1 / -1;
}

.tool-tile {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  gap: 6px;
  padding: 12px 6px;
  border: 1px solid rgb(var(--color-neutral-200));
  border-radius: var(--radius-lg);
  background-color: rgb(var(--color-neutral-50));
  text-decoration: none;
  transition: border-color 0.15s, background-color 0.15s, transform 0.15s;
}

.tool-tile:hover {
  border-color: rgb(var(--color-primary-400));
  transform: translateY(-2px);
}

.tool-tile:focus-visible {
  outline: 2px solid rgb(var(--color-primary-500));
  outline-offset: 2px;
}

/* 图标尺寸由字号决定：主题的 .icon svg 是 width:1em/height:1em。 */
.tool-tile__icon {
  font-size: 28px;
  line-height: 1;
  color: rgb(var(--color-primary-600));
}

.tool-tile__label {
  font-size: 12px;
  color: rgb(var(--color-neutral-600));
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
  max-width: 100%;
}

.dark .tool-tile {
  background-color: rgb(255 255 255 / 0.04);
  border-color: rgb(var(--color-neutral-700));
}

.dark .tool-tile__icon {
  color: rgb(var(--color-primary-400));
}

.dark .tool-tile__label {
  color: rgb(var(--color-neutral-400));
}

.tools-empty {
  margin: 0;
  font-size: 13px;
  color: rgb(var(--color-neutral-500));
}

/* 弹窗容器。宽高都给足：工具卡片原生 440×722px，输入后长到 782px，
   常见笔记本视口的 90vh 能容纳；窄屏下用 vw 铺满，避免 iframe 内部再产生
   一层横向滚动。 */
.tool-dialog {
  width: min(96vw, 560px);
  height: min(92vh, 860px);
  max-width: none;
  max-height: none;
  padding: 0;
  border: none;
  border-radius: var(--radius-lg);
  overflow: hidden;
  background-color: transparent;
}

.tool-dialog::backdrop {
  background-color: rgb(0 0 0 / 0.55);
}

.tool-dialog__bar {
  display: flex;
  align-items: center;
  justify-content: space-between;
  gap: 12px;
  padding: 10px 14px;
  background-color: rgb(var(--color-neutral-50));
  border-bottom: 1px solid rgb(var(--color-neutral-200));
}

.tool-dialog__title {
  font-size: 14px;
  font-weight: 600;
  color: rgb(var(--color-neutral-700));
}

.tool-dialog__actions {
  display: flex;
  align-items: center;
  gap: 10px;
}

.tool-dialog__open {
  font-size: 12px;
  color: rgb(var(--color-primary-600));
  text-decoration: none;
}

.tool-dialog__open:hover {
  text-decoration: underline;
}

.tool-dialog__close {
  display: flex;
  align-items: center;
  justify-content: center;
  width: 28px;
  height: 28px;
  padding: 0;
  border: none;
  border-radius: 50%;
  background-color: transparent;
  font-size: 20px;
  line-height: 1;
  color: rgb(var(--color-neutral-600));
  cursor: pointer;
}

.tool-dialog__close:hover {
  background-color: rgb(var(--color-neutral-200));
}

/* 弹窗内部固定浅色：工具自身是浅色 UI，跟随站点暗色会出现
   浅色工具嵌在深色边框里的割裂感。这是有意为之，不是遗漏。 */
.tool-dialog__bar {
  color-scheme: light;
}

.tool-dialog__body {
  height: calc(100% - 49px); /* 减去标题栏高度，让 iframe 恰好填满剩余空间 */
  background-color: #f0f4f5;  /* 与工具自身背景一致，加载中不会闪白/闪黑 */
}

.tool-frame {
  display: block;
  width: 100%;
  height: 100%;
  border: 0;
}
```

- [ ] **Step 5: 在首页插入短代码**

修改 `content/_index.md`：
- 在 `{{< feature-grid >}}` 之后、第一个 `{{< feature ... >}}` **之前**插入 `{{< tools >}}`
- 在 `{{< /feature-grid >}}` **之后**插入 `{{< tools-dialog >}}`（必须平级，不能放进网格内）

修改后该文件应为（其余内容不动）：

```markdown
---
title: "积微知远"
---

{{< feature-grid >}}
{{< tools >}}
{{< feature title="C++ 笔记" url="cpp-notes/" label="查看 C++ 笔记" >}}
命名规范、编译预处理<br>类与继承、多态<br>模板、STL<br>智能指针<br> C++11 新特性
{{< /feature >}}
...（其余 4 张专题卡保持不变）
{{< /feature-grid >}}
{{< tools-dialog >}}
```

- [ ] **Step 6: 构建并检查渲染结果**

Run:
```bash
hugo --noBuildLock --quiet && \
grep -c 'tools-card' public/index.html && \
grep -c 'tool-tile' public/index.html && \
grep -c 'tool-dialog' public/index.html
```
Expected: 三行数字均 ≥ 1（分别约 1、1、1）

- [ ] **Step 7: 目视验证卡片**

启动 `hugo server --bind 127.0.0.1 --port 1313`，浏览器打开 `http://localhost:1313/WaiMinutes/`。
Expected:
- 首页共 6 张卡，呈 2 行 3 列
- 第一张卡标题为「工具」，右侧数字为 `1`，色带为青色
- 卡内有一个图块：油价图标 + 「油价计算」文字
- 现有 5 张专题卡外观无变化

- [ ] **Step 8: 提交**

```bash
git add data/tools.yaml layouts/shortcodes/tools.html layouts/shortcodes/tools-dialog.html assets/css/custom.css content/_index.md
git commit -m "feat: add the homepage tools card with a data-driven tool list"
```

---

### Task 3: 弹窗交互脚本

**Files:**
- Create: `assets/js/tools-modal.js`
- Create: `layouts/partials/extend-head-uncached.html`

**Interfaces:**
- Consumes: Task 2 产出的 DOM——`#tool-dialog`、`#tool-frame`、`#tool-close`、`#tool-dialog-title`、`#tool-open-external`、`.tool-tile[data-tool-url]`
- Produces: 无（终端功能）

- [ ] **Step 1: 创建脚本**

写入 `assets/js/tools-modal.js`：

```javascript
/* 首页工具弹窗：点击图块 → 在原生 <dialog> 里用 iframe 打开工具。
 *
 * 为什么 iframe：工具自带无作用域的全局 CSS（* 重置与 body flex/背景色），
 * 内联会摧毁站点样式。详见 layouts/shortcodes/tools.html 的说明。
 *
 * 为什么用原生 <dialog> 而不是自定义 div：Esc 关闭、焦点陷阱、::backdrop、
 * top-layer 渲染（不受 z-index 影响）都是浏览器内置的，代码量约为手写实现的
 * 三分之一，且无障碍默认合格。需要自己补的只有「点遮罩关闭」一条。
 *
 * 图块是 <a href>：脚本加载失败时点击仍会正常跳转到工具页（渐进增强），
 * 所以这里必须 preventDefault 才能阻止跳转。
 */
(function () {
  'use strict';

  var dialog = document.getElementById('tool-dialog');
  var frame = document.getElementById('tool-frame');
  var titleEl = document.getElementById('tool-dialog-title');
  var closeBtn = document.getElementById('tool-close');
  var externalLink = document.getElementById('tool-open-external');
  if (!dialog || !frame) return; // 不是首页，直接退出

  // <dialog> 在旧浏览器（Safari < 15.4）不存在 showModal，退回直接跳转，
  // 而不是抛错让整个脚本失效。
  if (typeof dialog.showModal !== 'function') {
    document.querySelectorAll('.tool-tile').forEach(function (tile) {
      tile.addEventListener('click', function () {
        window.location.href = tile.getAttribute('href');
      });
    });
    return;
  }

  function open(tile) {
    var url = tile.getAttribute('data-tool-url') || tile.getAttribute('href');
    // 标题取自图块的可见文字（.tool-tile__label），不用 aria-label：
    // 图块刻意不设 aria-label（可见文字已正确，设了会违反 WCAG 2.5.3）。
    var labelEl = tile.querySelector('.tool-tile__label');
    var label = labelEl ? labelEl.textContent.trim() : '';
    frame.setAttribute('title', label || '工具');
    frame.setAttribute('src', url);
    titleEl.textContent = label;
    externalLink.setAttribute('href', url);
    dialog.showModal();
  }

  function close() {
    dialog.close();
    // 置空以停止工具内的定时器与网络请求（汽油工具会轮询油价接口）。
    frame.setAttribute('src', 'about:blank');
  }

  document.querySelectorAll('.tool-tile').forEach(function (tile) {
    tile.addEventListener('click', function (e) {
      // 让中键/修饰键点击保持原生行为（新标签打开）。
      if (e.metaKey || e.ctrlKey || e.shiftKey || e.altKey || e.button !== 0) return;
      e.preventDefault();
      open(tile);
    });
  });

  closeBtn.addEventListener('click', close);

  // 点遮罩关闭：原生 dialog 只在点击 backdrop 时把 target 设为 dialog 本身，
  // 点内容区时 target 是内部元素，据此区分。
  dialog.addEventListener('click', function (e) {
    if (e.target === dialog) close();
  });

  // Esc 触发的是原生 close 事件，这里做同样的清理。
  dialog.addEventListener('close', function () {
    frame.setAttribute('src', 'about:blank');
  });
})();
```

- [ ] **Step 2: 创建注入 partial**

写入 `layouts/partials/extend-head-uncached.html`：

```html
{{/* Blowfish 的头部扩展点。主题 head.html:238 调用它时传入的是**当前页**
     （对比 extend-head.html 传入 .Site，无法判断页面），所以能用 .IsHome。

     为什么用 -uncached 版本而不是 extend-head.html：需要 .IsHome 来判断首页，
     而 extend-head.html 拿到的是 .Site。代价是每页执行一次判断，可忽略。

     JS 用 resources 管线处理（minify + 指纹），与主题自身的做法一致：
     指纹让文件可被永久缓存，content 变了 URL 就变，不会出现浏览器拿旧脚本。 */}}

{{ if .IsHome }}
  {{ $js := resources.Get "js/tools-modal.js" | resources.Minify | resources.Fingerprint (site.Params.fingerprintAlgorithm | default "sha512") }}
  <script defer type="text/javascript" src="{{ $js.RelPermalink }}" integrity="{{ $js.Data.Integrity }}"></script>
{{ end }}
```

- [ ] **Step 3: 构建并确认脚本只在首页注入**

Run:
```bash
hugo --noBuildLock --quiet && \
echo "首页: $(grep -c 'tools-modal' public/index.html)" && \
echo "文章页: $(grep -c 'tools-modal' public/cpp-notes/cpp-notes01-namerules-precomplile/index.html)" && \
echo "专题页: $(grep -c 'tools-modal' public/cpp-notes/index.html)"
```
Expected: 首页为 1（或 2，若路径中出现两次），文章页与专题页均为 **0**

- [ ] **Step 4: 浏览器验证交互**

在 `http://localhost:1313/WaiMinutes/` 上：
1. 点「油价计算」图块
   Expected: 弹窗打开，标题栏显示「打开汽油百分比计算器」，iframe 内加载出工具（可见「汽油百分比计算」标题与油价数据）
2. 按 Esc
   Expected: 弹窗关闭
3. 再次打开，点击标题栏的 ×
   Expected: 弹窗关闭
4. 再次打开，点击弹窗外的遮罩区域
   Expected: 弹窗关闭
5. 再次打开后，检查 `document.getElementById('tool-frame').getAttribute('src')`，关闭后再查一次
   Expected: 关闭后为 `about:blank`

- [ ] **Step 5: 提交**

```bash
git add assets/js/tools-modal.js layouts/partials/extend-head-uncached.html
git commit -m "feat: open tools in a native dialog with an iframe"
```

---

### Task 4: 部署工具本体并修复窄屏溢出

**Files:**
- Create: `static/tools/fuel-calculator/index.html`
- Create: `static/tools/fuel-calculator/VERSION`
- Modify: `static/tools/fuel-calculator/index.html`（窄屏修复）

**Interfaces:**
- Consumes: Task 2 的 `data/tools.yaml` 中 `url: tools/fuel-calculator/`
- Produces: 可访问的工具页 `/WaiMinutes/tools/fuel-calculator/`

- [ ] **Step 1: 复制工具文件**

源目录为 `C:\Users\ShenLan-Wai\Desktop\tmp\fuel-calculator\`。**只复制两个文件**：

```bash
mkdir -p static/tools/fuel-calculator
cp "C:/Users/ShenLan-Wai/Desktop/tmp/fuel-calculator/index.html" static/tools/fuel-calculator/
cp "C:/Users/ShenLan-Wai/Desktop/tmp/fuel-calculator/VERSION" static/tools/fuel-calculator/
```

不要复制 `README.md` / `update-version.bat` / `.gitignore`——`static/` 下所有文件都会被发布到站点。

- [ ] **Step 2: 确认 VERSION 与 index.html 同级**

Run: `ls static/tools/fuel-calculator/`
Expected: 只有 `index.html` 与 `VERSION` 两项

工具的 `loadVersion()` 按 `new URL('VERSION', window.location.href)` 读取，即**同级路径**；不同级会导致页脚显示回退值 `V1.0.0`。

- [ ] **Step 3: 修复窄屏横向溢出**

在 `static/tools/fuel-calculator/index.html` 中找到 `@media (max-width: 400px)` 块，在其中**追加**如下规则（该块现在只处理了模式栏、输入区与价格表，漏掉了头部的两个胶囊）：

```css
            /* 头部两个胶囊（油箱容量、日期）撑出了内容的最小宽度：实测为 335px。
               可用宽度（clientWidth，即视口减去滚动条）小于它时出现横向溢出，差值即
               两者之差——所以是否溢出与溢出多少都随滚动条宽度而变，不必记具体档位。
               允许换行并收缩文字宽度即可消除。 */
            .header { flex-wrap: wrap; gap: 8px; }
            .header-actions { flex-wrap: wrap; }
            .logo-text { overflow: hidden; text-overflow: ellipsis; }
```

- [ ] **Step 4: 验证工具可访问且无溢出**

Run:
```bash
hugo --noBuildLock --quiet
ls public/tools/fuel-calculator/
curl -s -o /dev/null -w "%{http_code}\n" http://localhost:1313/WaiMinutes/tools/fuel-calculator/
curl -s -o /dev/null -w "%{http_code}\n" http://localhost:1313/WaiMinutes/tools/fuel-calculator/VERSION
```
Expected: 目录含 `index.html` 与 `VERSION`；两个 curl 均为 `200`

然后浏览器打开 `http://localhost:1313/WaiMinutes/tools/fuel-calculator/`，把视口调到 320px 宽：
Expected: 无横向滚动条（`document.documentElement.scrollWidth <= clientWidth`）

- [ ] **Step 5: 提交**

```bash
git add static/tools/fuel-calculator/
git commit -m "feat: ship the fuel calculator as a site tool

Fixes a horizontal overflow: the header's tank and date pills pushed the
content past the available width, and the existing 400px media query only
covered the mode bar, inputs and price table."
```

---

### Task 5: 端到端验收

**Files:**
- 无（纯验证）

**Interfaces:**
- Consumes: Task 1–4 的全部产出
- Produces: 验收结论

- [ ] **Step 1: 全量构建**

Run: `hugo --noBuildLock`
Expected: 退出码 0，无 WARN/ERROR

- [ ] **Step 2: 首页桌面端检查（1440×900）**

- 6 张卡呈 2×3，工具卡在首位、色带青色、数字为 `1`
- 工具卡内图块为 1 个且**铺满整行**（因为数量为奇数）
- 5 张专题卡外观与链接未变

- [ ] **Step 3: 弹窗全流程**

- 点击图块 → 弹窗内工具加载出真实油价（今日价格与下轮预测可见）
- 弹窗内可正常操作工具（切换输入模式、输入金额得到百分比）
- 三种关闭方式（×、Esc、点遮罩）均有效

- [ ] **Step 4: 键盘可访问性**

Run: 在首页按 Tab 直到聚焦到工具图块，按 Enter
Expected: 弹窗打开；`document.activeElement` 位于 dialog 内部（焦点陷阱生效）；Esc 关闭后焦点回到触发图块

- [ ] **Step 5: 移动端检查（390px 与 360px）**

- 首页无横向滚动条
- 工具卡图块并排可读
- 弹窗铺满宽度，工具内部无横向滚动条

- [ ] **Step 6: 暗色模式检查**

切换到暗色：卡片与专题卡观感一致；弹窗内部仍为浅色（符合预期，工具自身是浅色 UI）

- [ ] **Step 7: 渐进增强检查**

在浏览器禁用 JS 后刷新首页：
Expected: 点击图块仍能跳转到 `/tools/fuel-calculator/`（因为图块是 `<a href>`）

- [ ] **Step 8: 工作区确认**

Run: `git status --short`
Expected: 干净（所有改动已提交）

---

## 附：后续添加第 2–4 个工具的步骤

（不属于本计划范围，仅作说明，与 `data/tools.yaml` 的注释一致）

1. 工具文件放 `static/tools/<name>/index.html`
2. 图标放 `assets/icons/<icon>.svg`（`currentColor`，名字不与主题冲突）
3. 在 `data/tools.yaml` 追加：

```yaml
- name: 工具名
  icon: <icon>
  url: tools/<name>/
  label: 打开<工具名>
```

⚠ `url` **不能带前导斜杠**。本站部署在 `/WaiMinutes/` 子路径下，Hugo 的 `relURL`
对以 `/` 开头的输入会丢掉子路径：`"/tools/x/" | relURL` 得到 `"/tools/x/"`（错，
线上 404），而 `"tools/x/" | relURL` 得到 `"/WaiMinutes/tools/x/"`（对）。

无需改模板或 CSS——格子布局会自动适应数量（2 个并排、3 个末行铺满、4 个 2×2）。
