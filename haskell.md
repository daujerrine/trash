Mathematics, Imperative Languages and Haskell
=============================================
_Anamitra Ghorui_

## 1. Introduction

_I will assume that you are proificient in at least one C or C-Like language, such as Python, Java, C++, Javascript etc._

Haskell is a programming language.

The current de-facto compiler for Haskell is the Glasgow Haskell
Compiler (GHC).
[You can find it here.][ghc]

You can start an interpreter on a terminal using the `ghci` command. You
will be greeted with a prompt like this:

```

$ ghci
GHCi, version 8.6.5: http://www.haskell.org/ghc/  :? for help
Loaded GHCi configuration from /home/turpentine/.ghci
> 

```
Here are a few basic commands. Try typing them in:

```
Example 1.1

Comments:

    > -- You can put comments like this.

"Hello World" Demonstration:
    
    > putStrLn("Hello") -- "Put String Line"
    Hello

Variables:
    
    > x = 5
    > x
    5

Operators:
    
    > x = 5
    > x + 5
    10 

Convert things to strings:
    
    > show(5)
    "5"
    > putStrLn(show(5))
    5

Concatenation:

    > putStrLn("The number is: " ++ show(5))
    The number is: 5
```

Unless if it is to establish precedence order, parantheses for arguments
are not necessary:

```
Example 1.2

    > putStrLn "Hello"
    Hello
    > show 5
    "5"
```

```
Example 1.3

    > putStrLn "Hello " ++ show 5

    <interactive>:3:1: error:
        • Couldn't match expected type ‘[Char]’ with actual type ‘IO ()’
        • In the first argument of ‘(++)’, namely ‘putStrLn "Hello"’
          In the expression: putStrLn "Hello" ++ show 5
          In an equation for ‘it’: it = putStrLn "Hello" ++ show 5
```

The error message says that there are invalid arguments to the operator
`++`, because `++` sees the following precedence:

```
(putStrLn "Hello ") ++ (show 5).
```

```
Example 1.4
    
    > putStrLn "Hello " ++ (show 5)

    <interactive>:4:1: error:
        • Couldn't match expected type ‘[Char]’ with actual type ‘IO ()’
        • In the first argument of ‘(++)’, namely ‘putStrLn "Hello"’
          In the expression: putStrLn "Hello" ++ (show 5)
          In an equation for ‘it’: it = putStrLn "Hello" ++ (show 5)
```

Still invalid arguments to the operator `++` with the same precedence.


```
Example 1.5

    > putStrLn ("Hello " ++ (show 5))
    Hello 5

```
Correct arguments and precedence.


```
Example 1.6

    > putStrLn ("Hello " ++ show 5)
    Hello 5

```
Also correct arguments and precedence.

For now, we will ignore the detailed description of the
error messages such as the above.

## 2. Functions in Haskell (Part 1)

In Haskell, _basic_ functions behave as if they were
_mathematical expressions_. That is. Functions simply contain a logical
formula. The result of the formula after we substitute any variables
present and evaluate the formula is what is returned. 

```
Example 2.1

Mathematical/Natural Language Expression:
    
    Let f(x) = x + 5.
    Hence, f(9) = 14.
    
    Let f(x, y) = x + y + 5.
    Hence, f(9, 1) = 15.
    
Haskell:
    
    > f x = x + 5
    > f 9
    14
    > f x y = x + y + 5
    > f 9 1
    15
    
Equivalent expression in Python:
    >>> f = lambda x: x + 5
    >>> f(9)
    14
    >>> f = lambda x, y: x + y + 5
    >>> f(9, 1)
    15
```

_All_ of these above expressions satisfy the mathematical definition of
a function:

> In mathematics, a function is a binary relation between two sets that
> associates every element of the first set to exactly one element of
> the second set.
> 
> -- Function (mathematics). Wikipedia, the Free Encyclopedia. Retrieved
> 23rd December 2020

We will refer to these two sets: the one which is the input set, and
the output set to which the function _maps_ each value from the 
input set to, as the _Domain_ and the _Codomain_ respectively.

```
                         f(x) = x + 5
                            
        Domain                                  Codomain
         ...                                      ...
          1  ----------------------------------->  6
          2  ----------------------------------->  7
          3  ----------------------------------->  8
          4  ----------------------------------->  9
          5  ----------------------------------->  10
         ...                                      ...

```

TODO:
1. Operators are functions
2. Function declaration methods
3. Type system

## 3. "Control Structures"

There is only one "control structure" in Haskell: the `if` statement.

```
Example 3.1

    > x = 5
    > if x == 5 then "great" else "that's okay too"
    "great"
    > x = 6
    > if x == 5 then "great" else "that's okay too"
    "that's okay too"

You may notice that the if statement is returning a string.
```

You can use these in functions as well:

```
Example 3.2

    > isItGreat x = if x == 5 then "great" else "that's okay too" 
    > isItGreat 8
    "that's okay too"
    > putStrLn(isItGreat 8)
    that's okay too
    > putStrLn(isItGreat 5)
    great

```

You can make a nested if-else statement like this by using the `else if`
notation:

```
Example 3.3

Haskell's interpreter allows you to break a line into more readable
chunks by hitting Enter/Return after each segment.

    > isItGreat x = if x == 5 then "great"
    Prelude| else if x == 7 then "also great"
    Prelude| else "that's okay too"
    > putStrLn(isItGreat 7)
    also great

```

We will get to loops later.

## 4. Arrays/Lists

You can declare arrays in Haskell like this:

```
Example 4.1

    > a = [1, 2, 3, 4]
    > a
    [1,2,3,4]
```

Array Indexing works like this:

```
Example 4.2

    > a !! 1
    > 2
```

There are a few inbuilt functions that will come in handy later:

```
Example 4.3

The following function returns the First element, or head of the array:

    > head a
    1

This will return the rest of the array, excluding the array:

    > tail a
    [2,3,4]
    > tail (tail a)
    [3, 4]
```

* In the second expression, we use brackets to separate the arguments
  and establish the order of evaluation. (tail (tail a)) is also a
  valid and equivalent statement
* You may notice that the above two allow us to operate on arrays like
  linked lists.

Let's tail a few more times:

```
    > tail (tail (tail (tail a)))
    []
```

We have reached the end of the list, which was of length 4. Hence ot
returns an empty list. Let's keep this property in mind.

Putting in so many brackets is a bit tiresome. We can abbreviate such
expressions using the "$" operator:

```
    > tail $ tail $ tail $ tail a
    []
```

The "$" operator says that everything to its right is inside a new layer
of parantheses.

This one returns the length, as expected:
```
Example 4.4

    > length a
    4
```

## 5. Where are the loops?

There are no looping control structures in Haskell. You instead perform
most computation using _recursive functions_.

```
Example 5.1

Summing numbers in a C-Like Language. We simply add numbers to keep
track of the array index here:
    
    int x[] = {1, 2, 3, 4, 5};
    int sum = 0;
    for (int i = 0; i < lengthOf(x); i++) {
        sum += x[i];
    }

Summing numbers in Python. Instead of using a numerical index, we use
an iterator instead (although it is still possible to use a numerical
index here, the intent here is to illustrate the use of iterators
in a conventional language):
    
    x = [1, 2, 3, 4, 5]
    sum = 0
    for i in x:
        sum += i

Now, Summing numbers in Haskell:
    
    > sumNumbers x = if x == [] then 0
         else head x + sum (tail x)
    > sumNumbers [1, 2, 3, 4]
    10
```

This is an implementation of a recursive function. Let's see what's
happening here:
 
```   
    if x == [] then 0
```

We had previously seen that if we reach the end of a list, the tail
tail function returns an empty list (since the end of the list is tailed
by nothing). Hence, we say that once we reach the end of the list, we
check whether x is an empty list or not. If it is, We say that the sum
of the list is 0, which is true. This is our _Base Case_ for the
recursion.

```
    else head x + sum (tail x)
```

Else, we say that the sum is the first item of the list, plus the sum
of the rest of the list.

If we were to write this in a C-Like Language, it would look like this:

```  
    int sumNumbers(int x[]) {
        if (isEmpty(x)) {
            return 0;
        } else {
            return x[0] + sumNumbers(tail(x));
        }
    }
```
Such a function would otherwise cause a Stack Overflow with very large
input in other languages. However, this does not happen in Haskell.

## 6. Haskell is not C

By now it might be obvious to you that this is something radically
different, way different than what you might have expected if you have
never previously heard about _Functional Programming Languages_ of which
haskell is one.

Languages such as C, C++, Java, Python all are called
_Imperative Languages_. Here, we clearly specify the steps to follow
in a specified order, which change the program's _current state_ (such
as, by changing the value of a variable) to achieve a solution. In 
short, _time matters most of the time_.

Functional Programming languages, however, come under
_Declarative Programming Languages_. In declarative languages, we
ask for a solution, but _not_ how to achieve that solution. SQL and Make Files
are examples of Declarative Programming languages which are _not_
Functional Programming Languages.

In Functional Programming languages, we compose our programs using
functions, however, there is little to no explicit definition of the
order in which statements shall execute. We also avoid changing values
of variables with respect to time as much as possible, that is,
changing or _mutating_ the state. Hence,

_We attempt to minimise how much time matters._

Recursion is used as a tool to define an _implicit order_ in the
solutions to our problems, ans hence a. **Haskell is built around efficient execution of such recursive functions and is the main method to solve problems in it**.

Have a look at Example 5.1 again and compare the Non-Haskell
versions to the Haskell ones, and see:
1. How many variables are used in the program.
2. How many times a variable is mutated by the program in both versions.
3. How and how many times that the mutation is percievable with respect
   to the current scope.

## 7. Why Do This?

While a proper introduction of haskell's features have not yet been
given, here are some of the advantages of having no mutation of state
and no explicit execution order:

1. **Allow you to focus of the algorithm instead of its underlying implementation:**
   In languages like C, you have to implement algorithms such as binary
   search by explicitly defining certain variables in order to keep track
   of the sub array, and change their state in each step to find the
   solution. Thus you have to modify the original algorithm quite a bit
   to get an efficient, practical program.
   In Haskell, however, you merely focus on the theoretical solution and
   you leave it to the compiler to convert it into an effecient program
   in most cases.

2. **Allow you to write better concurrent programs:**
   The fact that there is no mutation of state in functional programming
   means that all values in a concurrent variable will be fixed. Usually
   we instead implement an event-based message queue, where a concurrent
   process waits for a value on the queue, and continues only when a value
   is obtained. TODO

3. **Allow you to use constructs from discrete mathematics:** There are
   a lot of parallels between discrete mathematics and constructs present
   in Haskell, and allow you to use those structures in that manner.

4. **Reduce the general complexity of a program:** This is in part due
   to all of the above mentioned statements.


## 8. Haskell Functions (Part 2) 



------------------------------------------------------------------------

[ghc]: https://www.haskell.org/ghc/
