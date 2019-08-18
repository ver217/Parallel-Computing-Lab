# 说明
输入数据由若干文件组成，每个文件描述一个Akari问题的初始状态，编写程序读入此文件，根据初始状态求解。有的有一个解，有的有多个解，我们保证有解。

文件由若干行组成，第一行为两个整数 n，m，代表棋盘的行数和列数。之后的 n 行每行有 m 个整数表示棋盘的每个格子的状态，若它为 -2，则表示是白格子，若它为 -1，则表示是没有数字的黑格子，若它为 0-4，则表示是数字 0-4 的黑格子。若你想把灯泡放在白色格子上面，则需要将 -2 改为 5，因为 5 表示有灯泡的格子。

你需要在右侧代码编辑框给出的函数中编写你的代码，函数的参数为我们给出的light up矩阵，你需要在该函数中返回相同大小的结果矩阵。对于有多个解的light up，你可以返回其中的任意一组解，我们将对你返回的矩阵进行检查，若结果正确，提示The answer is right!，否则提示其它。

<-2 || >6表示被点亮

# 输入
可以在[Light Up](https://www.puzzle-light-up.com/)上找到Puzzle

``` javascript
function transfer() {
    const dom = document.querySelector('.board-back');
    let divs = dom.children;
    let row = Math.sqrt(divs.length - 1);
    let cnt = row;
    let total = `${row} ${row}`;
    let line = '';
    for (let i = 1; i < divs.length; i++) {
        if (divs[i].classList.contains('wall'))
            line += '-1 ';
        else if (divs[i].classList.contains('cell-off'))
            line += '-2 ';
        else if (divs[i].classList.contains('light-up-task-cell'))
            line += divs[i].innerText + ' ';
        if (--cnt == 0) {
            total += `\n${line}`;
            line = '';
            cnt = row;
        }
    }
    return total;
}
```
