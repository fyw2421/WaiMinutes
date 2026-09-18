---
title: "Markdown04 : Formula"
weight: 5
description: "MathJax Greek-Letters Mathematical-Operators"
date: 2018-05-02
tags: ["markdown"]
featureimage: "covers/markdown-notes04-formula-mathjax.svg"
---
　　MathJax是一款运行在浏览器中的开源的数学符号渲染引擎,使用MathJax可以方便的在浏览器中显示数学公式,不需要使用图片

　　通过使用MathJax,我们可以让Markdown解析LaTeX数学表达式,通常情况下,我们需要引入MathJax插件才可能工作.

```
<script type="text/javascript" src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS_HTML"></script>
```

## 一、Formula

### 1.1 Inline Formula

* **Syntax**:

  ```markdown
  $...$
  ```

* **Sample**:

   ` $s = \pi r^2$ `

### 1.2 Displayed Formula

* **Syntax**:

  ```markdown
  $$
  ...
  $$
  ```

* **Sample**:

  ```markdown
  $$
  s=\pi r^2
  $$
  ```

## 二、Greek Letters List

请参见下表

名称      | 大写 | Tex       | 小写  | Tex
:---:     | :---:|:---:      |:---:  |:---:
`alpha`   | `A`  | `A`       | `α`   | `\alpha`
`beta`    | `B`  | `B`       | `β`   | `\beta`
`gamma`   | `Γ`  | `\Gamma`  | `γ`   | `\gamma`
`delta`   | `Δ`  | `\Delta`  | `δ`   | `\delta`
`epsilon` | `E`  | `E`       | `ϵ`   | `\epsilon`
`zeta`    | `Z`  | `Z`       | `ζ`   | `\zeta`
`eta`     | `H`  | `H`       | `η`   | `\eta`
`theta`   | `Θ`  | `\Theta`  | `θ`   | `\theta`
`iota`    | `I`  | `I`       | `ι`   | `\iota`
`kappa`   | `K`  | `K`       | `κ`   | `\kappa`
`lambda`  | `Λ`  | `\Lambda` | `λ`   | `\lambda`
`mu`      | `M`  | `M`       | `μ`   | `\mu`
`nu`      | `N`  | `N`       | `ν`   | `\nu`
`xi`      | `Ξ`  | `\Xi`     | `ξ`   | `\xi`
`omicron` | `O`  | `O`       | `ο`   | `\omicron`
`pi`      | `Π`  | `\Pi`     | `π`   | `\pi`
`rho`     | `P`  | `P`       | `ρ`   | `\rho`
`sigma`   | `Σ`  | `\Sigma`  | `σ`   | `\sigma`
`tau`     | `T`  | `T`       | `τ`   | `\tau`
`upsilon` | `Υ`  | `\Upsilon`| `υ`   | `\upsilon`
`phi`     | `Φ`  | `\Phi`    | `ϕ`   | `\phi`
`chi`     | `X`  | `X`       | `χ`   | `\chi`
`psi`     | `Ψ`  | `\Psi`    | `ψ`   | `\psi`
`omega`   | `Ω`  | `\Omega`  | `ω`   | `\omega`

## 三、Mathematical Operators List

运算符        | 说明    | 运算符案例    | 案例实现
:---:         |:---:    |:---:|:---:
`+`           | 加      | $ x + y $     | `$ x + y $ `
`- `          | 减      | $ x - y $     | `$ x - y $ `
`\times`      | 乘      | $ x \times y $| `$ x \times y $ `
`\cdot`       | 乘      | $ x \cdot y $ | `$ x \cdot y $`
`\ast`        | 乘      | $ x \ast y $  | `$ x \ast y $`
`\div`        | 除      | $ x \div y $  | `$ x \div y $`
`\frac`       | 分数    | $ \frac{x}{y} $| `$ \frac{x}{y} $`
`^`           | 上标    | $ x ^ y $     | `$ x ^ y $`
`_`           | 下标    | $ x _ y $     | `$ x _ y $`
`\sqrt`       | 开二次方| $ \sqrt x $   | `$ \sqrt x $`
`\sqrt`       | 开方    | $ \sqrt[x]{y^4+3y-1} $| `$ \sqrt[x]{y^4+3y-1} $`
`\pm`         | 加减    | $ x \pm y $   | `$ x \pm y $`
`\mp`         | 减加    | $ x \mp y $   | `$ x \mp y $`
`=`           | 等于    | $ x = y $     | `$ x = y $`
`\leq`        | 小于等于| $ x \leq y $  | `$ x \leq y $`
`\geq`        | 大于等于| $ x \geq y $  | `$ x \geq y $`
`\ngeq`       | 不大于等于| $ x \ngeq y $| `$ x \ngeq y $`
`\neq`        | 不等于  | $ x \neq y $   |    `$ x \neq y $`
`\approx`     | 约等于  | $ x \approx y $| `$ x \approx y $`
`\equiv`      | 恒等于  | $ x \equiv y $ | `$ x \equiv y $`
`\bigodot`    | 定义运算符| $ x \bigodot y=x+y^2 $ | ` $ x \bigodot y=x+y^2 $`
`\bigotimes`  | 定义运算符|$ x \bigotimes y=x+y^2 $|` $ x \bigotimes y=x+y^2 $`
`\in`         | 属于    | $ x \in y $    | `$ x \in y $`
`\notin`      | 不属于  | $ x \notin y $ | `$ x \notin y $`
`\subset`     | 子集    | $ x \subset y $| `$ x \subset y $`
`\not\subset` | 非子集  | $ x \not\subset y $| `$ x \not\subset y $`
`\subseteq`   | 子集    | $ x \subseteq y $  | `$ x \subseteq y $`
`\supset`     | 超集    | $ x \supset y $    | `$ x \supset y $`
`\supseteq`   | 超集    | $ x \supseteq y $  | `$ x \supseteq y $`
`\cup`        | 并      | $ x \cup y $       | `$ x \cup y $`
`\cap`        | 交      | $ x \cap y $       | `$ x \cap y $`
`\log`        | 对数    | $ \log(x) $        | `$ \log(x) $`
`\overline`   | 平均数  | $ \overline{x} $   |`$ \overline{x} $`
`\overline`   | 连线符号| $ \overline{a+b+c+d} $  | `$ \overline{a+b+c+d} $`
`\underline`  | 下划线  | $ \underline{a+b+c+d} $ | `$ \underline{a+b+c+d} $`
`\overbrace`  | 上大括号| $ \overbrace{a+d}^{2.0} $ | `$\overbrace{a+d}^{2.0}$`
`\underbrace` | 下大括号| $ \underbrace{a+d}_3 $    | ` $\underbrace{a+d}_3$`
`\partial`    | 部分    | $ \frac{\partial x}{\partial y} $ | `$ \frac{\partial x}{\partial y} $`
`\lim`        | 极限    | $ \lim_{x\to\infty} $ |    `$ \lim_{x\to\infty} $`
`\displaystyle`| 块公式格式 | limx→∞    | `$ \displaystyle \lim_{x\to\infty} $`
`\sum`        | 求和    | $ \sum_1^n $  | ` $ \sum_1^n $`
`\infty`      | 极限    | $ \sum_{i=0}^\infty i^2 $ | `$ \sum_{i=0}^\infty i^2 $`
`\int`        | 积分    | $ \int_0^1 x^2 {\rm d}x $ | `$ \int_0^1 x^2 {\rm d}x $`
`\ldots`      | 底端对齐的省略号| $ 1,2,\ldots,n $  |`$ 1,2,\ldots,n $`
`\cdots`      | 中线对齐的省略号| $ x_1^2 + x_2^2 + \cdots + x_n^2 $ | `$ x_1^2 + x_2^2 + \cdots + x_n^2 $`
`\uparrow`    | 上箭头  | $ \uparrow $    | `$ \uparrow $`
`\Uparrow`    | 上箭头  | $ \Uparrow $    | `$ \Uparrow $`

## 四、上标和下标

` ^ `表示上标,` _ `表示下标.如果上下标的内容多于一个字符,要用`{}`把这些内容括起来当成一个整体.上下标是可以嵌套的,也可以同时使用.

* Syntax : `$ x^{y^z} = ( 1 + e^x )^{-2xy^w} $`

* Output : $ x^{y^z} = ( 1 + e^x )^{-2xy^w} $

另外,如果要在左右两边都有上下标,可以用`\sideset`命令.

Sample|Output
:---|:---:
` $ \sideset {^2}{ ^3 }{(1+x)}  $ `   | $ \sideset {^2}{ ^3 }{(1+x)} $
` $ \sideset{^1_2}{^3_4}\bigotimes $ `| $ \sideset{^1_2}{^3_4}\bigotimes $

## 五、分数表示

```markdown
* $ \frac {分子} {分母} $
* $ {分子} \over {分母} $
```

Sample|Output
:---:|:---:
`$ \frac {c+d} {a+b}$`    | $ \frac {c+d} {a+b}$
`$ {c+d} \over {a+b}$ `    | $ {c+d} \over {a+b}$

*注意对于`\frac`的方法,如果分子分母都是单个数,那么大括号`{}`可以省略,如`$\frac12$`表示1/2.*

## 六、各种括号

　　`()`、`[]`和`|`可以直接表示自己,而`{}`本来用于分组,因此需要用`{}`来表示自身,也可以使用`\lbrace` 和`\rbrace`来表示,其它括号见下面那个表.

　　注意原始符号并不会随着公式大小缩放.有时候我们想要括号和分隔符显示的大点,比如上面例子中希望括号能把整个分数都包住,那么可以用\left和\right标记,实现自适应调整.

Sample|Output
:---:|:---:
`$\left {c+d} \over {a+b} \right$`| $ \left( {c+d} \over {a+b} \right) $
`$ $ ( {c+d} \over {a+b} ) $`     | $ ( {c+d} \over {a+b} ) $

符号名称    | LaTex代码         | 例子                              | 产生的效果
:---:       |   :---            | :---                              | :---:
小括号      | ( 和 )            | \left(x\right)                    | $ \left(\frac12\right) $
中括号      | [ 和 ]            | \left[\frac12\right]              | $ \left[\frac12\right]$
大括号      | { 和 }            | \left{\frac12\right}              | $ \left\{\frac12\right\} $
尖括号      | \langle 和 \rangle| \left\langle\frac12\right\rangle  | $ \left\langle\frac12\right\rangle $
向上取整    | \lceil 和 \rceil  | \left\lceil\frac12\right\rceil    | $\left\lceil\frac12\right\rceil$
向下取整    | \lfloor 和 \rfloor| \left\lfloor\frac12\right\rfloor  | $\left\lfloor\frac12\right\rfloor$

**注意**:

　　*\left和\right标记必须是成对出现的,但有时候我们只用到其中一个,比如只用一个`|`当作分割线,这时候可以通过.来表示空的那一方,即用\left.表达左边空的情况,用\right.表达右边空的情况.*

**Sample**:

```markdown
$\left. \frac{du}{dx} \right| _{x=0}$
```

**Output**:

$ \left. \frac{du}{dx} \right\| _{x=0} $

## 七、参考

* [**常用数学符号表**](http://www.mohu.org/info/symbols/symbols.htm)

* [**MathJax数学符号手册**](http://meta.math.stackexchange.com/questions/5020/mathjax-basic-tutorial-and-quick-reference)

* [**MathJax与LaTex公式简介**](http://mlworks.cn/posts/introduction-to-mathjax-and-latex-expression/)

* [**小敏纸的博文**](http://blog.csdn.net/lanxuezaipiao/article/details/44341645)

* [**FedFun**](http://blog.csdn.net/whqet/article/details/44277965)



