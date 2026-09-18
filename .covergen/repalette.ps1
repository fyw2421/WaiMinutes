param([ValidateSet('warm','cool')][string]$Mode = 'warm')
$ErrorActionPreference = 'Stop'
$root = 'F:\Privacy\WaiMinutes'
$dir = Join-Path $root 'assets\covers'

# 每个专题：冷色方案 -> 暖色方案（c1 背景上、c2 背景下、glow 顶光、accent 强调）
$map = @{
    'cpp-notes'       = @{ cool = @('#12345c','#071726','#4b8ed6','#9cc0ea'); warm = @('#4a3416','#241a0b','#d8a24a','#e8c48e') }
    'designpattern'   = @{ cool = @('#2a1f45','#150f26','#8a6cc8','#b9a4e6'); warm = @('#46161f','#230b10','#c8564f','#e8a39d') }
    'datastructures'  = @{ cool = @('#103a30','#071d18','#4fae8a','#93d8bd'); warm = @('#33401a','#191f0d','#9dbf4a','#cfe08e') }
    'git-cheat'       = @{ cool = @('#43260f','#22130a','#d08a4a','#e8b98a'); warm = @('#43260f','#22130a','#d08a4a','#e8b98a') }
    'markdown'        = @{ cool = @('#0e3640','#07191f','#4fb0bd','#96d3db'); warm = @('#4a1c30','#240e18','#d078a8','#e89cc0') }
}
function Get-Group([string]$name) {
    if ($name -like 'cpp-notes*')      { return 'cpp-notes' }
    if ($name -like 'designpattern*')  { return 'designpattern' }
    if ($name -like 'datastructures*') { return 'datastructures' }
    if ($name -like 'git-*')           { return 'git-cheat' }
    if ($name -like 'markdown*')       { return 'markdown' }
    return $null
}

$utf8 = New-Object System.Text.UTF8Encoding($false)
$changed = 0
foreach ($f in Get-ChildItem -LiteralPath $dir -Filter '*.svg' -File) {
    $g = Get-Group $f.BaseName
    if (-not $g) { Write-Warning "未归类: $($f.Name)"; continue }
    $from = $map[$g][$(if ($Mode -eq 'warm') { 'cool' } else { 'warm' })]
    $to   = $map[$g][$Mode]
    $t = [System.IO.File]::ReadAllText($f.FullName)
    for ($i = 0; $i -lt 4; $i++) {
        # 同时处理原色与已大写形式（SVG 里都是小写十六进制）
        $t = $t.Replace($from[$i], $to[$i]).Replace($from[$i].ToUpper(), $to[$i].ToUpper())
    }
    [System.IO.File]::WriteAllText($f.FullName, $t, $utf8)
    $changed++
}
Write-Host "已切换为 $Mode 方案，处理 $changed 个封面"
$map.Keys | Sort-Object | ForEach-Object { "$($_.PadRight(16)) $($map[$_][$Mode] -join '  ')" }
