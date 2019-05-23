# NullScript

NullScript is an esolang that I made. It contains basic commands and mostly resembles a combination of Deadfish~ and brainf***.

Check it out in the `v1/` folder!

## Documentation

NullScript has a 256-cell tape of integers and a cell pointer that is on one of the cells.

|Char|Function|
|:---:|:---|
|`]`|Increment the current cell|
|`[`|Decrement the current cell|
|`;`|Square the current cell|
|`.`|Print the current cell as a number|
|`,`|Print the current cell as a character|
|`~`|Reset the current cell to 0|
|`&`|Get an integer from input|
|`<`|Move the cell pointer left|
|`>`|Move the cell pointer right|

* When squaring a cell pushes it over the limit of the cell's size, the cell should go to 0.
* When printing a cell as a character and the cell isn't between 0 and 127, the behavior is undefined.
* When moving the cell pointer would put it below cell 0 or above cell 255 (on zero-based indexing), the cell pointer wraps around to the other side of the tape

NullScript files should use the `.ns` file extension.
