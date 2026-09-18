# ⚠ 历史脚本，不要重跑。
#
# 它把 30 篇 Jekyll 文章按「扁平路径」写成 content/<section>/<name>.md，
# 但其中 19 篇现在已经是 leaf bundle（content/cpp-notes/cpp-notes03-datatype/index.md …）。
# 重跑会在 bundle 旁边生成重复页面，并覆盖已恢复的 front matter——它只产出旧的 Jekyll 键，
# 连 layout: post 那一行改写（type:
#   layout: 'post'）都不是主题能消费的 front matter。
# 保留仅供查阅当初的字段映射与目录归属。

# 硬拦截。上面那段注释拦不住重跑：它是普通注释，实测删掉警告行脚本就照样执行；
# 而且 PowerShell 对未知命令默认只报红不终止，连退出码都还是 0，没法用退出码兜底。
# 确实需要重跑时，先注释掉下面这一行——这份摩擦是故意留的。
throw "历史脚本，仅作记录。重跑会覆盖 19 篇 leaf bundle 的 front matter 并生成重复页面。"

# ⚠ 目录已在 2026-09-18 从 WaiMinutes.Blowfish.hugo 改名为 WaiMinutes，
# 所以下面的 $src 与 $dstRoot 现在同名——但 $src 指向的旧 Jekyll 仓库早已不在。
# 本脚本已 throw 拦截，路径仅供查阅当初的字段映射。
$src = "F:\Privacy\WaiMinutes\_posts"
$dstRoot = "F:\Privacy\WaiMinutes\content"

if (-not (Test-Path $dstRoot)) { New-Item -ItemType Directory -Path $dstRoot -Force | Out-Null }

# Mapping files to destination directories
$map = @{
    "cpp-notes01-namerules-precomplile.md" = "cpp-notes"
    "cpp-notes02-iostatements.md" = "cpp-notes"
    "cpp-notes03-datatype.md" = "cpp-notes"
    "cpp-notes04-class-function.md" = "cpp-notes"
    "cpp-notes05-inheritance.md" = "cpp-notes"
    "cpp-notes06-polymorphism.md" = "cpp-notes"
    "cpp-notes07-template.md" = "cpp-notes"
    "cpp-notes08-exception.md" = "cpp-notes"
    "cpp-notes09-stl.md" = "cpp-notes"
    "cpp-notes10-smartpointers.md" = "cpp-notes"
    "cpp-notes11-cpp11-newfeature01.md" = "cpp-notes"
    "cpp-notes12-cpp11-newfeature02.md" = "cpp-notes"
    "datastructures01-summarize.md" = "data-structures"
    "designpattern01-principles-types.md" = "design-pattern"
    "designpattern02-simplefactory-factorymethod-abstractfactory.md" = "design-pattern"
    "designpattern03-builder-prototype-singleton.md" = "design-pattern"
    "designpattern04-adapter-facade-bridge.md" = "design-pattern"
    "designpattern05-composite-decorator-flyweight-proxy.md" = "design-pattern"
    "designpattern06-memento-mediator-observer.md" = "design-pattern"
    "designpattern07-state-visitor-interpret.md" = "design-pattern"
    "designpattern08-iterator-strategy-command.md" = "design-pattern"
    "designpattern09-templatemethod-chainofresponsibility.md" = "design-pattern"
    "git-cheat-sheets01-config-setup.md" = "git-notes"
    "git-cheat-sheets02-commit-reset.md" = "git-notes"
    "git-cheat-sheets03-branch-merge.md" = "git-notes"
    "markdown-custom-writing-format.md" = "markdown-notes"
    "markdown-notes01-about-markdown.md" = "markdown-notes"
    "markdown-notes02-classical-syntax.md" = "markdown-notes"
    "markdown-notes03-markdown-extra.md" = "markdown-notes"
    "markdown-notes04-formula-mathjax.md" = "markdown-notes"
}

Get-ChildItem $src -Filter "*.md" | ForEach-Object {
    $name = $_.Name
    if (-not $map.ContainsKey($name)) {
        Write-Host "SKIP: $name"
        return
    }
    
    $destDir = Join-Path $dstRoot $map[$name]
    if (-not (Test-Path $destDir)) { New-Item -ItemType Directory -Path $destDir -Force | Out-Null }
    
    $content = Get-Content $_.FullName -Raw -Encoding UTF8
    
    # Remove aliases block if present
    $content = $content -replace '(?ms)^aliases:\s*\n(\s*-\s*[^\n]+\n)*\n?', ''
    
    # Convert front matter
    $content = $content -replace 'layout:\s*post', "type: `n  layout: 'post'"
    $content = $content -replace 'catalog:\s*true', "toc: true"
    $content = $content -replace 'header-img:\s*"([^"]+)"', "cover:`n  image: `"$1`""
    $content = $content -replace 'author:\s*"([^"]+)"', "authors: [`"$1`"]"
    
    $outFile = Join-Path $destDir $name
    Set-Content -Path $outFile -Value $content -Encoding UTF8
    Write-Host "Migrated: $name -> $($map[$name])/$name"
}
