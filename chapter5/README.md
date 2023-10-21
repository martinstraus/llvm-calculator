# Chapter 5

## Description

In this chapter we make the grammar more complex, introducing:
* Grouping expressions with parenthesis.
* Variables.
* Very, very simple symbols table.
* Processing multiple lines.
* Comments.
* The last line is the output.

For simplicity, you have to end the last statement with a newline. 

## Example input file

    // This is a sample program

    a <- 1
    b <- 3
    (a+2)/(b*4) // The last statement is the return of the program. End with a newline!
    

## How to build

    make

## How to run

    ./compiler [input file] [output file]
    ./[output file]
    echo $?
