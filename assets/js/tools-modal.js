/* 首页工具弹窗：点击图块 → 在原生 <dialog> 里用 iframe 打开工具。
 *
 * 为什么 iframe：工具自带无作用域的全局 CSS（* 重置与 body flex/背景色），
 * 内联会摧毁站点样式。详见 layouts/shortcodes/tools-dialog.html 的说明。
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
  // 五个元素都来自 tools-dialog.html；模板与脚本漂移时若只判两个，
  // 后面的解引用会抛 TypeError，且会把遮罩/Esc 的 close 监听一起注册失败。
  if (!dialog || !frame || !titleEl || !closeBtn || !externalLink) return;

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

  // 背景滚动由 CSS 的 `html:has(#tool-dialog[open]) { overflow: hidden }` 负责
  // （见 assets/css/custom.css），这里只补滚动条宽度：必须**在 showModal() 之前**量，
  // 因为 open 属性一加上滚动条就没了。关闭统一走 dialog 的 close 事件，
  // 三条路径（关闭按钮、点遮罩、Esc）都覆盖到，不必每处单独还原。
  //
  // 还原的是**打开前的原值**而不是一律写 ''：body 上若已有别处设的内联
  // padding-right，写 '' 等于把它抹掉。原值在 lock 时记下，unlock 时写回。
  var savedPaddingRight = '';

  function lockScrollbarGap() {
    var gap = window.innerWidth - document.documentElement.clientWidth;
    savedPaddingRight = document.body.style.paddingRight;
    document.body.style.paddingRight = gap > 0 ? gap + 'px' : savedPaddingRight;
  }

  function unlockScrollbarGap() {
    document.body.style.paddingRight = savedPaddingRight;
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
    lockScrollbarGap();
    // showModal() 一旦抛错（例如 InvalidStateError），close 事件不会触发，
    // 刚补上的留白就再也还不回来了 —— 就地还原后原样抛出，不吞异常。
    try {
      dialog.showModal();
    } catch (err) {
      unlockScrollbarGap();
      throw err;
    }
  }

  function close() {
    // 只负责关；iframe 的 src 与滚动条留白统一交给下面的 close 事件收口，
    // 这里不再重复设一次（否则 Esc 走一条路、按钮走两条路，行为不对称）。
    dialog.close();
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

  // dialog 的 close 事件：三条关闭路径（按钮 / 遮罩 / Esc）唯一的收口点，
  // 滚动条留白与 iframe 都在这里还原，避免各走各的、行为不对称。
  dialog.addEventListener('close', function () {
    frame.setAttribute('src', 'about:blank');
    unlockScrollbarGap();
  });
})();
