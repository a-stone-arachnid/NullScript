# NullScript

NullScript is an esolang that I made. It contains basic commands and mostly resembles a combination of Deadfish~ and brainf***.

Check it out in the `v1/` folder!

## Documentation

NullScript has a 256-cell tape of integers, a limited-use 127-char string zone (`strmem`), an 8-item LIFO parameter queue (a.k.a. `PQ`) and a cell pointer that is always one of the cells (like in brainf***).

NullScript uses the Mac OS Roman charset.

|Char|Function|
|:---:|:---|
|`]`|Increments the current cell|
|`[`|Decrement the current cell|
|`;`|Square the current cell|
|`.`|Print the current cell as a number|
|`,`|Print the current cell as a character|
|`~`|Reset the current cell to 0|
|`&`|Get an integer from input|
|`^`|Get a character from input|
|`<`|Move the cell pointer left|
|`>`|Move the cell pointer right|
|`{`|If the current cell is 0, skip past the next `}`|
|`}`|Go back to the previous `{`|
|`*`|Push the current cell to the `PQ`|
|`'`|Move the top value from the `PQ` into the current cell|
|`+`|Sets the current cell to the sum of the top two numbers on the `PQ`|
|`-`|Sets the current cell to the difference of the top two numbers on the `PQ`|
|`*`|Sets the current cell to the product of the top two numbers on the `PQ`|
|`/`|Sets the current cell to the quotient of the top two numbers on the `PQ`, rounded down|
|`"`|Sets the current cell to the remainder of the quotient of the top two numbers on the `PQ`|
|`?`|Generates a random number, between 0 and the current cell|
|`$`|Store a string from input in `strmem`|
|`#`|Print the string from `strmem`|
|`@`|Take a number from `PQ`, and put `strmem`'s char at that position into the accumulator|
|`` ` ``||
|`!`|Throw an error (implementation-dependant)|

* When squaring a cell pushes it over the limit of the cell's size, the cell should go to 0.
* When printing a cell as a character and the cell isn't between 0 and 127, the behavior is undefined.
* When moving the cell pointer would put it below cell 0 or above cell 255 (on zero-based indexing), the cell pointer wraps around to the other side of the tape
* When getting a value from an empty `PQ`, take input instead.

NullScript files should use the `.ns` file extension.
