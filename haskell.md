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
> -- [Function (mathematics). Wikipedia, the Free Encyclopedia][wiki-function]. Retrieved
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

We will get to how functions like `putStrLn` work later.

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

## 4. Lists (Part 1)

You can declare lists in Haskell like this:

```
Example 4.1

    > a = [1, 2, 3, 4]
    > a
    [1,2,3,4]
```

List Indexing works like this:

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
solutions to our problems. **Haskell is built around efficient execution
of such recursive functions and is the main method to solve problems in
it**.

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

3. **Allow you to use constructs from discrete mathematics:** 
   There are a lot of parallels between discrete mathematics and
   constructs present in Haskell, and allow you to use those structures
   in that manner.

4. **Reduce the general complexity of a program:**
    This is in part due to all of the above mentioned statements.

## 8. Few More Interpreter Facilities

### Running Code From Files

You can type all of the above examples into a file and run them in the
interpreter. Try creating a file with the following code:

```
    formula a b = (a * b) / (a + b)

```

And saving it with a filename, say, `code.hs`. (Haskell sourcecode 
uses the `.hs` extension.)

Now, open a terminal and supply the file to the interpreter. You will
be greeted with the following:

```
    $ ghci formula.hs
    GHCi, version 8.6.5: http://www.haskell.org/ghc/  :? for help
    Loaded GHCi configuration from /home/turpentine/.ghci
    [1 of 1] Compiling Main             ( a.hs, interpreted )
    Ok, one module loaded.
    >
```

Now try using the function:

```
    > formula 2 3
    1.2
```

We will eventually discuss _Compiled Haskell_. However, it will come in
a later section and for now this will be enough to execute the examples
present in the following sections.

### Multiline Statements in the Interpreter

The examples following this section will require you to enter in
multiline statements. You can start a multiline statement by typing in
`:{`, and ending with `:}`:

```
    > :{
    Prelude| a x y = 
    Prelude|     if x > y then 1
    Prelude|     else 0
    Prelude| :}
    > a 2 3
    0
```

## 9. Haskell Functions (Part 2) 

### 9.1 Operators are also functions

All functions that have these following characters [[1]][haskell-lexical]
allow you to supply 2 arguments to the function in _infix_ order:

```
 ! # $ % & * + . / < = > ? @ \ ^ | - ~
```

```
Example 9.1

Let's make an operator that finds the hypotenuse of a right angled
triangle:

    > a %% b = sqrt (a ^ 2 + b ^ 2)
    > 3 %% 4
    5.0

"sqrt", here, is a library function and already available to you.
```

We can use an operator in prefix operator by simply adding parantheses
to the token:

```
Example 9.2

    > (%%) 3 4
    5.0
```

You can also define operators in this prefix manner:

```
Example 9.3

    > (%%) a b = sqrt (a ^ 2 + b ^ 2)
    > 3 %% 4
    5.0

```

TODO acknowledge this is simply pattern matching

You can add as many characters as you want to the operator:

```
Example 9.4

    > (%%%%%%%%%%%%%%) a b = sqrt (a ^ 2 + b ^ 2)
    > 3 %%%%%%%%%%%%%% 4
    5.0
```

Conversely, we can turn any other function into a prefix operator by
enclosing its token in tildes (`` ` ``):

```
Example 9.5

    > hypotenuse a b = sqrt (a ^ 2 + b ^ 2)
    > 3 `hypotenuse` 4
    5.0
```

### 9.2 What about Operator Precedence?

There are certain operator/function tokens that are given a
[predefined precedence][haskell-opprec] by Haskell. These include
operations like ``+, -, *, /, `mod` `` etc. Other than this, since there
are no differences between operators and functions aside from operators
being in infix order by default, custom operators have the same precedence
as normal functions.

### 9.3 Pattern Matching

Each function definition in Haskell is a "pattern". 
Definition of a function with arguments makes you define a pattern. 
Haskell will look for these series of tokens and arguments with
appropriate type and _[greedily][wiki-greedy]_ match the pattern
specified by the function. By "Greedy," we mean that it will look for
the earliest, and closest match while performing the pattern matching.
Mirroring the short-sighted nature of most greedy people looking for 
immediate satiation.

We utilise this concept in Haskell to match about any pattern consisting
of any expression, and not just restricted to simple variables. We can
even use constants in patterns. We will discuss this in the next section.

TODO Phrase this better maybe.

```
Example 9.1:

Matching a list of 2 elements:
    > f [a, b] = a + b
    > f [2, 3]
    5
    
Matching a list of 2 elements and another variable:
    > f [a, b] c = a + b + c
    > f [1, 2] 3
    6
```

### 9.4 More Ways to Define Functions

#### 9.4.1 Definitions using Pattern Matching

Aside from using the `if` block, we can also use pattern matching to
define a function, and what should the function return at a particular
value. This allows for a more natural-language or mathematical language
manner of defining a function: 

```
Example 9.2:

Mathematical definition of the factorial function:
    Factorial(0) = 1
    Factorial(1) = 1
    Factorial(N) = N * Factorial(N - 1)
    
The factorial function in Haskell using pattern matching:

    > :{
    Prelude| fac 0 = 1
    Prelude| fac 1 = 1
    Prelude| fac n = n * fac (n - 1)
    Prelude| :}
    > fac 5
    120

Factorial in a C-like Language:

int fac(int n) {
    if (n == 0)
        return 1;
    else if (n == 1)
        return 1;
    else
        return n * fac(n - 1)
}

```
_(Note the use of `:{` and `:}` commands over here. These define a multiline
block and is required for defining such a function written in multiple
lines. Otherwise, the function definition is always overwritten each
time we attempt to add a new pattern.) This problem is avoided when
executing from a file instead._

Unlike in conventional imperative languages, it completely avoids using
an `if-else` block.

A more complex example is as follows:

```
Example 9.3:

Pattern with a 2 element list, and matching a list that has the second
element as 1:

    > :{
    Prelude| f [a, 1] = a + 999
    Prelude| f [a, b] = a + b
    Prelude| :}
    > f [1000, 1]
    1999
    > f [1000, 2]
    1002

Pattern with a 2 element list and a variable:

    > :{
    Prelude| f [a, 1] 1 = a + 999 + 111
    Prelude| f [a, b] c = a + b + c
    Prelude| :}
    > f [1000, 1] 1
    2110
    > f [1, 2] 3
    6
```

#### 9.4.2 


------------------------------------------------------------------------

[ghc]: https://www.haskell.org/ghc/
[wiki-function]: https://en.wikipedia.org/wiki/Function_(mathematics)
[haskell-lexical]: https://www.haskell.org/onlinereport/lexemes.html
[haskell-opprec]: https://www.haskell.org/onlinereport/decls.html#fixity
[wiki-greedy]: https://en.wikipedia.org/wiki/Greedy_algorithm
