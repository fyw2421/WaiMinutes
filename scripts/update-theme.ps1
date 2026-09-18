<#
.SYNOPSIS
  把 themes/blowfish 更新到上游 Blowfish 的最新版本（或指定版本）。

.DESCRIPTION
  主题是整目录提交进本仓库的（不是 submodule，也没有 go.mod），所以更新就是
  「换掉那 500 多个文件」——但直接覆盖有两个坑，这个脚本负责绕开：

  1) 上游的 release 不带 tag。v3.7.0 这个 release 的 tag_name 是 GitHub 自动生成的
     "untagged-<hash>"，git 里并不存在该 tag（真实 tag 停在 v3.6.0），所以
     `git clone --branch v3.7.0` 会失败。脚本的做法：release 若带正常 tag 就按 tag 取，
     若是 untagged 就用 release 的发布时间在 main 上定位「当时那次提交」，按 commit
     固定版本——不会把随后新推的提交一起卷进来。
  2) 上游仓库里有 exampleSite/（88M）和 images/（18M）两个目录，当初 vendor 时就没有
     要，替换时必须再次剔除，否则仓库会平白多出 100M。

  另外本仓库对主题有几处覆盖/依赖：layouts/shortcodes/feature.html、
  layouts/partials/favicons.html，以及主题 head.html 对
  layouts/partials/extend-head-uncached.html 挂载点的调用（首页工具弹窗的 JS
  由它注入）。它们不会随主题更新而变化，但可能悄悄失效，所以替换后会做针对性
  自检（见 -Apply 之后的输出）。

.PARAMETER Ref
  指定要取的版本：commit SHA、tag 或分支名。留空 = 自动解析上游最新 release。

.PARAMETER Apply
  真正执行替换。不加这个开关时只做预览：下载、比对、报告要改多少文件，不动工作区。

.PARAMETER KeepTemp
  保留临时下载目录，便于人工翻看上游文件。

.EXAMPLE
  # 1) 先预览：上游是否有新版本、会改动多少文件
  pwsh -File scripts/update-theme.ps1

  # 2) 确认无误后再替换，并自动构建自检
  pwsh -File scripts/update-theme.ps1 -Apply

  # 想固定到某个具体版本
  pwsh -File scripts/update-theme.ps1 -Ref v3.6.0

.NOTES
  替换后若要回滚：git checkout HEAD -- themes/blowfish
  （主题文件从未被本地修改过，所以整目录还原即可。）
#>
[CmdletBinding()]
param(
  [string]$Ref = '',
  [switch]$Apply,
  [switch]$KeepTemp
)

$ErrorActionPreference = 'Stop'

$repo = Split-Path -Parent $PSScriptRoot
$themeDir = Join-Path $repo 'themes/blowfish'
$siteConfig = Join-Path $repo 'hugo.toml'
$api = 'https://api.github.com/repos/nunocoracao/blowfish'
$headers = @{ 'User-Agent' = 'WaiMinutes-theme-updater' }
$stripDirs = @('exampleSite', 'images')   # 当初 vendor 时就没要这两个目录

function Write-Step($text) { Write-Host "`n== $text" -ForegroundColor Cyan }
function Write-Warn2($text) { Write-Host "   ! $text" -ForegroundColor Yellow }
function Write-Ok($text) { Write-Host "   $text" -ForegroundColor Green }


# --- 1. 解析目标版本 -------------------------------------------------------------

Write-Step '解析上游版本'

if ($Ref) {
  $targetRef = $Ref
  $versionLabel = "手动指定 $Ref"
  Write-Ok "使用指定版本：$Ref"
} else {
  $release = Invoke-RestMethod -Uri "$api/releases/latest" -Headers $headers
  $versionLabel = $release.name
  if ($release.tag_name -and $release.tag_name -notlike 'untagged-*') {
    # 上游将来若改用规范 tag，直接按 tag 取即可
    $targetRef = $release.tag_name
    Write-Ok "最新 release：$($release.name)（tag $($release.tag_name)，$($release.published_at)）"
  } else {
    # untagged release：按发布时间在 main 上定位当时那次提交
    $until = [uri]::EscapeDataString($release.published_at)
    $commits = Invoke-RestMethod -Uri "$api/commits?sha=main&until=$until&per_page=1" -Headers $headers
    if (-not $commits) { throw "无法在 main 上定位 $($release.published_at) 之前的提交" }
    $targetRef = $commits[0].sha
    Write-Ok "最新 release：$($release.name)（$($release.published_at)）"
    Write-Ok "  该 release 未打 tag，按发布时间固定到 commit $($targetRef.Substring(0,9))"
  }
}

$currentVersion = '未知'
$pkg = Join-Path $themeDir 'package.json'
if (Test-Path $pkg) {
  $currentVersion = (Get-Content $pkg -Raw | ConvertFrom-Json).version
}
Write-Host "   当前仓库内版本：$currentVersion" -ForegroundColor Gray


# --- 2. 下载并解压到临时目录 -----------------------------------------------------

# 解压必须用 Windows 自带的 bsdtar（System32\tar.exe）。从 Git Bash 之类环境里调用
# 时 PATH 上的 tar 往往是 GNU tar，它会把 "C:\..." 里的冒号当成远程主机名，
# 报 "Cannot connect to C: resolve failed"。按绝对路径点名可以避开这个歧义。
$tarExe = Join-Path $env:SystemRoot 'System32\tar.exe'
if (-not (Test-Path $tarExe)) { $tarExe = 'tar' }

# 下载缓存：100MB 一次，预览与 -Apply 是两次运行，缓存下来免得下两遍。
# 想强制重新下载就删掉这个目录。
$cacheDir = Join-Path $env:TEMP 'blowfish-theme-cache'
New-Item -ItemType Directory -Path $cacheDir -Force | Out-Null
$cacheFile = Join-Path $cacheDir (($targetRef -replace '[^0-9A-Za-z._-]', '_') + '.tar.gz')

$tmp = Join-Path $env:TEMP ("blowfish-update-" + (Get-Date -Format 'yyyyMMdd-HHmmss'))
New-Item -ItemType Directory -Path $tmp -Force | Out-Null
$tarFile = Join-Path $tmp 'src.tar.gz'

if (Test-Path $cacheFile) {
  Write-Step "复用已下载的 $targetRef"
  Copy-Item $cacheFile $tarFile
  Write-Ok "缓存：$cacheFile"
} else {
  Write-Step "下载 $targetRef"
  $url = "$api/tarball/$targetRef"
  Invoke-WebRequest -Uri $url -OutFile $cacheFile -Headers $headers
  Copy-Item $cacheFile $tarFile
  $sizeMb = [math]::Round((Get-Item $cacheFile).Length / 1MB, 1)
  Write-Ok "已下载 $sizeMb MB（已缓存）"
}

Push-Location $tmp
try {
  & $tarExe -xzf $tarFile
  if ($LASTEXITCODE -ne 0) { throw "解压失败：$tarExe" }
} finally {
  Pop-Location
}

$newDir = (Get-ChildItem $tmp -Directory | Select-Object -First 1).FullName
if (-not $newDir) { throw "解压后没找到目录" }

foreach ($d in $stripDirs) {
  $p = Join-Path $newDir $d
  if (Test-Path $p) { Remove-Item -Recurse -Force $p }
  Write-Ok "已剔除 $d/（vendor 时就不要的目录）"
}
Write-Host "   上游内容：$newDir" -ForegroundColor Gray


# --- 3. 与当前主题比对 -----------------------------------------------------------

Write-Step '比对差异'
$nameStatus = & git -C $repo diff --no-index --name-status -- $newDir $themeDir 2>$null
$changed = @($nameStatus | Where-Object { $_ -match '\S' })

if ($changed.Count -eq 0) {
  Write-Ok "已是最新，工作区无需改动。"
  if (-not $KeepTemp) { Remove-Item -Recurse -Force $tmp }
  return
}

# 统计各状态数量
$stats = @{ A = 0; M = 0; D = 0 }
foreach ($line in $changed) {
  $code = ($line -split "\s+")[0]
  if ($code.StartsWith('A')) { $stats.A++ }
  elseif ($code.StartsWith('D')) { $stats.D++ }
  else { $stats.M++ }
}
Write-Host "   共 $($changed.Count) 个文件不同：新增 $($stats.A) / 修改 $($stats.M) / 删除 $($stats.D)" -ForegroundColor White

Write-Host "`n   变动最大的 15 个文件（按改动行数）：" -ForegroundColor Gray
& git -C $repo diff --no-index --stat -- $newDir $themeDir 2>$null |
  Select-Object -SkipLast 1 | Select-Object -Last 15 | ForEach-Object { Write-Host "     $_" }

Write-Host "`n   想看完整 diff：" -ForegroundColor Gray
Write-Host "     git diff --no-index -- `"$newDir`" `"$themeDir`"" -ForegroundColor Gray


# --- 4. 预览模式到此为止 ---------------------------------------------------------

if (-not $Apply) {
  Write-Step '预览结束（未改动任何文件）'
  Write-Host "   确认上面这份清单后，执行替换：" -ForegroundColor White
  Write-Host "     pwsh -File scripts/update-theme.ps1 -Apply" -ForegroundColor White
  if (-not $KeepTemp) { Remove-Item -Recurse -Force $tmp }
  return
}


# --- 5. 替换 ---------------------------------------------------------------------

Write-Step '替换主题目录'

# 主题目录必须先干净：若有本地改动，替换会直接丢掉它们（这些改动不会进 diff，
# 因为比对的另一端是上游，不是本地基线）。宁可直接停下让人处理。
$dirty = & git -C $repo status --porcelain -- themes/blowfish
if ($dirty) {
  throw "themes/blowfish 有未提交的改动，先处理再更新：`n$($dirty | Out-String)"
}
Write-Ok '主题目录干净，可以安全替换'

Remove-Item -Recurse -Force $themeDir
Copy-Item -Recurse $newDir $themeDir
Write-Ok '已替换'


# --- 6. 自检 ---------------------------------------------------------------------

if (-not $KeepTemp) { Remove-Item -Recurse -Force $tmp }

Write-Step '构建自检'
$buildDir = Join-Path $env:TEMP 'blowfish-update-build'
if (Test-Path $buildDir) { Remove-Item -Recurse -Force $buildDir }
# --noBuildLock：本地常开着 `hugo server`，它持有构建锁，否则这里会一直等
& hugo --noBuildLock --destination $buildDir --logLevel warn
if ($LASTEXITCODE -ne 0) {
  Write-Warn2 '构建失败。回滚：git checkout HEAD -- themes/blowfish'
  throw '构建未通过'
}
Write-Ok '构建通过'
Remove-Item -Recurse -Force $buildDir

Write-Step '需要人工确认的几处'
Write-Host @"
   本节针对本仓库对主题的几处覆盖与扩展挂载点，以及"主题根级配置不参与合并"——主题更新后它们最容易静默失效（Hugo 不会为此报错）。

   1) favicon 扩展点是否还在
"@ -ForegroundColor Gray

$headTpl = Join-Path $themeDir 'layouts/partials/head.html'
if ((Get-Content $headTpl -Raw) -match 'templates\.Exists\s+"partials/favicons\.html"') {
  Write-Ok '     仍在 head.html 中调用 partials/favicons.html'
} else {
  Write-Warn2 'head.html 里那个 templates.Exists 调用点消失了！layouts/partials/favicons.html 将不再生效，'
  Write-Warn2 '图标/ manifest 会全站消失。需要按新的 head.html 结构重新挂载。'
}

Write-Host @"

   2) extend-head-uncached.html 挂载点是否还在
"@ -ForegroundColor Gray

# 首页工具弹窗的 JS 由站点侧 layouts/partials/extend-head-uncached.html 注入
# （靠 .IsHome 判断首页）。但注入的前提是主题 head.html 仍调用这个 partial；
# 站点模板里那处调用被 templates.Exists 保护，挂载点消失不会报错，只会静默失效。
if ((Get-Content $headTpl -Raw) -match 'templates\.Exists\s+"partials/extend-head-uncached\.html"') {
  Write-Ok '     仍在 head.html 中调用 partials/extend-head-uncached.html'
} else {
  Write-Warn2 'head.html 里那个 templates.Exists 调用点消失了！首页工具弹窗的 JS 将不再注入，'
  Write-Warn2 '点击图块只会退回普通跳转（无弹窗）。需要按新的 head.html 结构重新挂载。'
}

Write-Host @"

   3) feature.html 覆盖是否仍然贴合原版
"@ -ForegroundColor Gray
& git -C $repo diff --no-index --stat -- (Join-Path $themeDir 'layouts/shortcodes/feature.html') (Join-Path $repo 'layouts/shortcodes/feature.html') 2>$null
Write-Host '     上面是站点覆盖与新版原版的差异行数；若原版改了接口（参数名、读取的 Params），需同步这份覆盖。' -ForegroundColor Gray

Write-Host @"

   4) render-link.html 覆盖是否仍然贴合原版
"@ -ForegroundColor Gray

# 站点覆盖了主题的 markdown 链接钩子，唯一目的是给下载类链接加 download 属性；
# 其余逻辑（相对路径解析、外链 target）是照抄主题原版的。原版若改动，这份要同步——
# 否则会悄悄回退成旧行为（比如主题新增的链接处理不会生效）。
& git -C $repo diff --no-index --stat -- (Join-Path $themeDir 'layouts/_default/_markup/render-link.html') (Join-Path $repo 'layouts/_default/_markup/render-link.html') 2>$null
Write-Host '     上面是站点覆盖与新版原版的差异行数；请确认差异仍只限于 download 判定那部分。' -ForegroundColor Gray

Write-Host @"

   5) 主题声明的 Hugo 版本区间（本机与 CI 都必须落在区间内）
"@ -ForegroundColor Gray

# 主题在自己的 config.toml 里用 [module.hugoVersion] 声明兼容区间（这是少数几个
# "写在主题里也照样生效"的键，由模块系统直接读取）。更新后这个区间可能上移，
# 届时本机 Hugo 与 .github/workflows/gh-pages.yml 里 pin 的版本都要跟着升，
# 否则构建会告警、CI 可能直接失败。
$themeModuleCfg = Join-Path $themeDir 'config.toml'
if (Test-Path $themeModuleCfg) {
  $cfgText = Get-Content $themeModuleCfg -Raw
  $minM = [regex]::Match($cfgText, 'min\s*=\s*"([\d.]+)"')
  $maxM = [regex]::Match($cfgText, 'max\s*=\s*"([\d.]+)"')
  $extM = [regex]::Match($cfgText, 'extended\s*=\s*(\w+)')

  $localHugo = (& hugo version) -replace '^hugo v([\d.]+).*', '$1'
  $wf = Join-Path $repo '.github/workflows/gh-pages.yml'
  $ciHugo = ''
  if (Test-Path $wf) {
    $ciM = [regex]::Match((Get-Content $wf -Raw), "hugo-version:\s*'?([\d.]+)'?")
    if ($ciM.Success) { $ciHugo = $ciM.Groups[1].Value }
  }

  if ($minM.Success) { Write-Host "     主题要求 Hugo >= $($minM.Groups[1].Value)" -ForegroundColor Gray }
  if ($maxM.Success) { Write-Host "     主题要求 Hugo <= $($maxM.Groups[1].Value)" -ForegroundColor Gray }
  if ($extM.Success) { Write-Host "     extended 构建：$($extM.Groups[1].Value)" -ForegroundColor Gray }

  foreach ($pair in @(@{ Name = '本机'; Ver = $localHugo }, @{ Name = 'CI'; Ver = $ciHugo })) {
    if (-not $pair.Ver) { continue }
    $v = [version]$pair.Ver
    $tooLow = $minM.Success -and ($v -lt [version]$minM.Groups[1].Value)
    $tooHigh = $maxM.Success -and ($v -gt [version]$maxM.Groups[1].Value)
    if ($tooLow) {
      Write-Warn2 "$($pair.Name) Hugo $($pair.Ver) 低于主题要求 —— 需要升级 Hugo"
    } elseif ($tooHigh) {
      Write-Warn2 "$($pair.Name) Hugo $($pair.Ver) 高于主题声明的上限 —— 通常仍可构建（Hugo 只告警），但要么降 Hugo、要么等主题放宽；CI 若报错就按此调整"
    } else {
      Write-Ok "     $($pair.Name) Hugo $($pair.Ver) 在区间内"
    }
  }
}

Write-Host @"

   6) 主题的根级配置键（不会合入站点，必须按需手工搬到 hugo.toml）
"@ -ForegroundColor Gray

# 主题根级键（来自 config/_default/hugo.toml 与 markup.toml）——这些写在主题里
# 对本站一律无效，Hugo 只把 config/_default/ 里的 [params] 合并进来。
$rootKeys = @(
  @{ Key = 'outputs';          Why = '搜索索引 public/index.json 依赖 home 的 JSON 输出' },
  @{ Key = 'pagination';       Why = '每页条数 pagerSize' },
  @{ Key = 'markup.highlight'; Why = '代码块明暗两套 CSS 依赖 noClasses = false' },
  @{ Key = 'enableRobotsTXT';  Why = '生成 robots.txt' },
  @{ Key = 'imaging';          Why = '图片处理默认参数' },
  @{ Key = 'related';          Why = '相关文章（主题 related.html 会读）' }
)
$themeCfgText = ''
foreach ($f in @('hugo.toml', 'markup.toml')) {
  $p = Join-Path $themeDir "config/_default/$f"
  if (Test-Path $p) { $themeCfgText += (Get-Content $p -Raw) }
}
$siteCfgText = Get-Content $siteConfig -Raw

foreach ($item in $rootKeys) {
  $pattern = switch ($item.Key) {
    'outputs' { '\[outputs\]' }
    'pagination' { '\[pagination\]' }
    'markup.highlight' { '\[markup\.highlight\]' }
    'enableRobotsTXT' { 'enableRobotsTXT' }
    'imaging' { '\[imaging\]' }
    'related' { '\[related\]' }
  }
  $inTheme = $themeCfgText -match $pattern
  $inSite = $siteCfgText -match $pattern
  if ($inTheme -and -not $inSite) {
    Write-Warn2 "$($item.Key) —— 主题有、本站没有（$($item.Why)）"
  }
}

Write-Host @"

   7) 别忘了
      · .github/workflows/gh-pages.yml 里那句注释写着主题版本号，手工更新一下。
      · 检查 diff：git diff --stat themes/blowfish
      · 发布前目视核对 assets/css/custom.css 依赖的主题类名是否还在
        （bg-white/85、.main-menu、#single_header、min-w-0/max-w-fit 等；
         类名被改时 Hugo 不报错，只是样式失效）。
      · 回滚：git checkout HEAD -- themes/blowfish
"@ -ForegroundColor Gray

Write-Step '完成'
Write-Host "   主题已更新到 $versionLabel"
