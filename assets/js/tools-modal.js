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
