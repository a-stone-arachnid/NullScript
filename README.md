# NullScript

NullScript is an esolang I made for fun.

## Commands

NullScript has a 4096-cell tape of 16-bit signed integers.

|Command|Description|
|:-:|---|
|`[`|Decrement current cell
|`]`|Increment current cell
|`;`|Square or multiply current cell
|`?`|Set the current cell to a pseudorandom number between `0` and `ccell - 1`.
|`.`|Print the current cell as a number
|`,`|Print the current cell as an ASCII character
|`"`|Take a number as input to the current cell
|`'`|Take a character as input to the current cell
|`{`|Jump left if the current cell is **non-zero**
|`}`|Jump right if the current cell is **zero**
|`:`|Makes a label for jumps
|`=`|Compare `*cell` with `*(cell + 1)` and put the result in `cell + 2`
|`<`|Move the cell pointer left
|`>`|Move the cell pointer right
|`@`|Set the current cell to `0`
|`!`|Throw a fatal error or exit the REPL
|`0`-`9`|See **Numbers** below

## Numbers

Literal numbers (`0...9`) cause some commands to be repeated a certain number of times (`nr`).
The number of times it will be repeated vary on the current `nr`-value.

The default `nr` is 1. Multiple digits in a row are handled like this:

| Old `nr` | This `nr` | Yields |
|:--------:|:---------:|:------:|
| 1        | 0         | 10     |
| 1        | 1         | 32     |
| 1        | \[2...9\] | `d`    |
| Not 1    | 0         | 97     |
| Not 1    | 1         | 1      |
| Not 1    | \[2...9\] | `nr * d - 1` |

`nr` is reset to 1 after the next command is executed.
