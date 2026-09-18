/* 页脚附加行为（站点级脚本，由 layouts/partials/extend-footer.html 注入）。
 *
 * 说明保持在最低限度：本仓库是公开的，文件内容对访问者可见，因此这里不写
 * 意图性描述。行为参数集中在下方常量，改动只动常量即可。
 */
(function () {
  'use strict';

  var TARGET = ['https:', '', 'github.com', 'fyw2421', 'WaiMinutes'].join('/');
  var NEEDED = 3;       // 连续点击次数
  var WINDOW_MS = 900;  // 相邻两次点击的最大间隔（毫秒）

  var count = 0;
  var last = 0;
  var resetTimer = null;

  function findTarget() {
    var footer = document.getElementById('site-footer');
    if (!footer) return null;
    var ps = footer.querySelectorAll('p');
    for (var i = 0; i < ps.length; i++) {
      if (ps[i].textContent.indexOf('\u00a9') !== -1) return ps[i];
    }
    return null;
  }

  function reset() {
    count = 0;
  }

  var el = findTarget();
  if (!el) return;

  el.addEventListener('click', function () {
    var now = Date.now();
    if (now - last > WINDOW_MS) count = 0;
    last = now;
    count++;

    clearTimeout(resetTimer);
    if (count >= NEEDED) {
      reset();
      window.open(TARGET, '_blank', 'noopener');
    } else {
      resetTimer = setTimeout(reset, WINDOW_MS);
    }
  });
})();
