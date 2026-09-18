# -*- coding: utf-8 -*-
"""生成站点图标：static/favicon.svg、favicon.ico 与各尺寸 PNG。

图形语言取自站内既有的两处：封面 SVG 的深色底 + 顶部光晕，以及首页主标题那条
日落渐变（#cfe08e → #e8c48e → #e8a39d → #e89cc0，断点 0/41%/68%/100%）。
字形是「汇点成线」的具象：五个顶点是五个专题色，连成的那条线是 W（Wai），也收束
成站点主标题的走势。顶点不画成圆点，原因见 DOT_R。

为什么用脚本而不是直接画图：下面的常量是唯一的几何定义，SVG 与位图由同一次
定义产出。手写 SVG + 另画一套位图会随时间漂移（改了配色只改一处）。

圆角分两种，取决于谁来裁：
  · 自带圆角（favicon.svg / .ico / 16 / 32）——浏览器标签页里没有别的裁切，圆角
    是图形的一部分；
  · 不留圆角（apple-touch-icon / android-chrome）——iOS 与 Android 会对图标套
    自己的遮罩，图里再画一层圆角会在四角露出背景，出现"双重圆角"。
两种都由同一份几何渲染，只差一个 corners 开关。

用法：python scripts/make-icons.py
依赖：Pillow（python -m pip install pillow）。本脚本同时重写 static/site.webmanifest，
      站点名与主题色都取自下方常量，改这里即可，别单独改那个 json。
"""
import json
import os

from PIL import Image, ImageChops, ImageDraw

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "static")

SITE_NAME = "Wai Minutes"

VIEW = 512                     # 设计画布边长，同时作为 SVG 的 viewBox
CORNER_RADIUS = 112            # 自带圆角档的圆角半径
GLYPH = [(110, 172), (198, 348), (256, 204), (314, 348), (402, 172)]
STROKE = 60                    # 折线粗细
# 顶点不再单独画圆点。五个顶点仍是下面这套调色板的取色位置，但把"点"画出来会在
# 中间那个峰上鼓成一个球，512 下像融化的 W，16/32 下直接把峰糊没。实测对比过
# 加点的五个版本，不加点在 32px 与 16px 下是唯一还认得出 W 的。（要试点状装饰，
# 把这里改成 30 上下即可，代码路径仍然保留。）
DOT_R = 0
TILE_STOPS = [(0.0, "#16244a"), (0.55, "#0d1730"), (1.0, "#060a14")]
GLOW = ("#3882dc", 0.30)       # 顶部光晕（与首页页面背景同色同强度）
PALETTE = [(0.00, "#cfe08e"), (0.41, "#e8c48e"), (0.68, "#e8a39d"), (1.00, "#e89cc0")]
THEME_COLOR = "#16244a"        # 供 manifest 用：取底色顶端色

# 遮罩安全区：Android 的 maskable 只保证中间 80% 直径的圆完整可见。
# 超出就会被裁，所以这里断言一遍，改坐标时能立刻发现。
SAFE_RADIUS = VIEW * 0.8 / 2


def hex_rgb(h):
    h = h.lstrip("#")
    return tuple(int(h[i:i + 2], 16) for i in (0, 2, 4))


def lerp(a, b, t):
    return tuple(round(x + (y - x) * t) for x, y in zip(a, b))


def ramp(stops, t):
    """在 [(offset, '#rrggbb')] 上按 t（0..1）取色。"""
    if t <= stops[0][0]:
        return hex_rgb(stops[0][1])
    for (o1, c1), (o2, c2) in zip(stops, stops[1:]):
        if t <= o2:
            return lerp(hex_rgb(c1), hex_rgb(c2), (t - o1) / (o2 - o1))
    return hex_rgb(stops[-1][1])


def to_hex(c):
    return "#{:02x}{:02x}{:02x}".format(*c)


def glyph_t(x):
    """字形横向位置 → 渐进步进（沿字形包围盒铺开，与首页主标题同比例）。"""
    x0, x1 = GLYPH[0][0], GLYPH[-1][0]
    return min(1.0, max(0.0, (x - x0) / (x1 - x0)))


def check_safe_zone():
    cx = cy = VIEW / 2
    worst = 0.0
    for x, y in GLYPH:
        for dx, dy in ((-1, -1), (1, -1), (-1, 1), (1, 1), (0, 0)):
            px = x + dx * DOT_R * 0.707
            py = y + dy * DOT_R * 0.707
            worst = max(worst, ((px - cx) ** 2 + (py - cy) ** 2) ** 0.5)
    assert worst <= SAFE_RADIUS, f"字形超出遮罩安全区: {worst:.1f} > {SAFE_RADIUS:.1f}"
    return worst


def render(size, corners=True, ss=4):
    """按目标尺寸原生渲染（超采样 ss 倍后降采样），而不是从大图缩。"""
    w = size * ss
    k = w / VIEW

    # 底色：纵向渐变。先画一根 1px 宽的颜色列，再横向拉成整幅。
    col = Image.new("RGB", (1, w))
    col.putdata([ramp(TILE_STOPS, y / (w - 1)) for y in range(w)])
    tile = col.resize((w, w), Image.BILINEAR)

    # 顶部光晕：径向渐变。Pillow 的 radial_gradient 是中心黑、边缘白，
    # 取反后中心最亮，正好当光晕的 alpha。
    glow_r = int(w * 0.85)
    glow = ImageChops.invert(Image.radial_gradient("L")).resize((glow_r * 2, glow_r * 2), Image.BILINEAR)
    alpha = glow.point(lambda v: int(v * GLOW[1]))
    tile.paste(Image.new("RGB", (glow_r * 2, glow_r * 2), hex_rgb(GLOW[0])),
               (int(w / 2) - glow_r, -glow_r), alpha)

    if corners:
        mask = Image.new("L", (w, w), 0)
        ImageDraw.Draw(mask).rounded_rectangle([0, 0, w - 1, w - 1], radius=CORNER_RADIUS * k, fill=255)
        out = Image.new("RGBA", (w, w), (0, 0, 0, 0))
        out.paste(tile, (0, 0), mask)
    else:
        out = tile.convert("RGBA")

    # 折线：先整条画进一张 L 掩膜（joint="curve" 让转折处也圆滑），
    # 再让横向渐变透过掩膜贴上去——这样描边是一条连续的线，不会在转折处
    # 留下分段拼接的缺口。
    pts = [(x * k, y * k) for x, y in GLYPH]
    stroke_w = max(1, round(STROKE * k))
    smask = Image.new("L", (w, w), 0)
    sd = ImageDraw.Draw(smask)
    sd.line(pts, fill=255, width=stroke_w, joint="curve")
    for (x, y) in (pts[0], pts[-1]):          # 圆头端点
        r = stroke_w / 2
        sd.ellipse([x - r, y - r, x + r, y + r], fill=255)

    grad = Image.new("RGB", (w, 1))
    grad.putdata([ramp(PALETTE, glyph_t(x / k)) for x in range(w)])
    out.paste(grad.resize((w, w), Image.BILINEAR), (0, 0), smask)

    # 顶点圆点：颜色直接取该点所在位置的渐变色（DOT_R = 0 时不画）
    if DOT_R:
        dd = ImageDraw.Draw(out)
        r = DOT_R * k
        for x, y in pts:
            c = ramp(PALETTE, glyph_t(x / k))
            dd.ellipse([x - r, y - r, x + r, y + r], fill=c + (255,))
    return out


def svg():
    stops = "\n".join(
        f'      <stop offset="{o:g}" stop-color="{c}"/>' for o, c in PALETTE
    )
    tile_stops = "\n".join(
        f'      <stop offset="{o:g}" stop-color="{c}"/>' for o, c in TILE_STOPS
    )
    pts = " ".join(f"{x},{y}" for x, y in GLYPH)
    dots = (
        "\n".join(
            f'  <circle cx="{x}" cy="{y}" r="{DOT_R}" fill="{to_hex(ramp(PALETTE, glyph_t(x)))}"/>'
            for x, y in GLYPH
        )
        if DOT_R
        else ""
    )
    return f"""<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {VIEW} {VIEW}" width="{VIEW}" height="{VIEW}" role="img" aria-label="{SITE_NAME}">
  <!-- 由 scripts/make-icons.py 生成，勿手改：改几何/配色请改该脚本后重跑。 -->
  <defs>
    <linearGradient id="tile" x1="0" y1="0" x2="0" y2="1">
{tile_stops}
    </linearGradient>
    <radialGradient id="glow" cx="0.5" cy="0" r="0.85">
      <stop offset="0" stop-color="{GLOW[0]}" stop-opacity="{GLOW[1]:g}"/>
      <stop offset="1" stop-color="{GLOW[0]}" stop-opacity="0"/>
    </radialGradient>
    <linearGradient id="line" gradientUnits="userSpaceOnUse" x1="{GLYPH[0][0]}" y1="0" x2="{GLYPH[-1][0]}" y2="0">
{stops}
    </linearGradient>
  </defs>
  <rect width="{VIEW}" height="{VIEW}" rx="{CORNER_RADIUS}" fill="url(#tile)"/>
  <rect width="{VIEW}" height="{VIEW}" rx="{CORNER_RADIUS}" fill="url(#glow)"/>
  <polyline points="{pts}" fill="none" stroke="url(#line)" stroke-width="{STROKE}"
            stroke-linecap="round" stroke-linejoin="round"/>{dots}
</svg>
"""


def main():
    worst = check_safe_zone()
    print(f"字形最外缘距中心 {worst:.1f}px（安全区 {SAFE_RADIUS:.1f}px）")

    pngs = {
        "favicon-16x16.png": (16, True),
        "favicon-32x32.png": (32, True),
        "apple-touch-icon.png": (180, False),        # iOS 自会套遮罩
        "android-chrome-192x192.png": (192, False),  # Android 自会套遮罩
        "android-chrome-512x512.png": (512, False),
    }
    for name, (size, corners) in pngs.items():
        img = render(size, corners=corners)
        path = os.path.join(OUT, name)
        img.save(path)
        print(f"  {name:28} {size}x{size}  {os.path.getsize(path):>7} B")

    # .ico 里逐尺寸原生渲染（各档独立绘制，不是把 512 缩下去）
    frames = [render(s, corners=True) for s in (16, 32, 48)]
    ico = os.path.join(OUT, "favicon.ico")
    frames[-1].save(ico, format="ICO", append_images=frames[:-1], sizes=[(16, 16), (32, 32), (48, 48)])
    print(f"  {'favicon.ico':28} {len(frames)} 帧  {os.path.getsize(ico):>7} B")

    with open(os.path.join(OUT, "favicon.svg"), "w", encoding="utf-8", newline="\n") as f:
        f.write(svg())
    print(f"  {'favicon.svg':28} 矢量    {os.path.getsize(os.path.join(OUT, 'favicon.svg')):>7} B")

    # manifest：src 用相对路径。相对路径按 manifest 自身位置解析，站点部署在
    # /WaiMinutes/ 子路径下也能命中；写死 "/..." 会跑到域名根上去（主题原版就是
    # 这个毛病，四个图标路径全 404）。
    manifest = {
        "name": SITE_NAME,
        "short_name": SITE_NAME,
        "icons": [
            {"src": "android-chrome-192x192.png", "sizes": "192x192", "type": "image/png", "purpose": "any maskable"},
            {"src": "android-chrome-512x512.png", "sizes": "512x512", "type": "image/png", "purpose": "any maskable"},
        ],
        "theme_color": THEME_COLOR,
        "background_color": THEME_COLOR,
        "display": "standalone",
    }
    with open(os.path.join(OUT, "site.webmanifest"), "w", encoding="utf-8", newline="\n") as f:
        json.dump(manifest, f, ensure_ascii=False, indent=2)
        f.write("\n")
    print(f"  {'site.webmanifest':28}         {os.path.getsize(os.path.join(OUT, 'site.webmanifest')):>7} B")


if __name__ == "__main__":
    main()
