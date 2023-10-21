# Chapter 6

## Description

In this chapter we add functions definitions.

## Example input file

    // This is a sample program

    fn f(x) <- x+1
    fn g(y) <- y*2

    a <- f(1)
    b <- g(3)
    (a+2)/(b*4)
    

## How to build

    make

## How to run

    ./compiler [input file] [output file]
    ./[output file]
    echo $?
