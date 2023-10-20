# Chapter 5

## Description

In this chapter we make the grammar more complex, introducing:
* Grouping expressions with parenthesis.
* Variables.
* Very, very simple symbols table.
* Processing multiple lines.
* Comments.
* The last line is the output.

## Example input file

    a <- 1
    b <- 3
    (a+2)/(b*4)

## How to build

    make

## How to run

    ./compiler [input file] [output file]
