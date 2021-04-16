# NullScript

NullScript is an esolang I made for fun.

## Commands

NullScript has a 512-cell tape of 16-bit signed integers.

|Command|Description|
|:-:|---|
|`[`|Decrement current cell
|`]`|Increment current cell
|`;`|Square current cell
|`?`|Set the current cell to a pseudorandom number between `0` and `ccell`.
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
