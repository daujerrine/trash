---
title: Mathematics, Imperative Languages and Haskell
author: Anamitra Ghorui
toc: true
geometry:
 - top=0.8in
 - left=0.5in
 - right=0.5in
 - bottom=1in
papersize: a5
---

\pagebreak

TODO Rename "variable" to "identifier" and "definition" with "binding"

# 1. Introduction

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
Prelude> 

```

`Prelude`, here, is a Haskell module. It is loaded by default into the
interpreter and contains several core functions. Any new modules you
load will appear prefixed against the prompt.

For the purposes of this document, we will not show the prompt as
"`Prelude>`", but as "`>`". See Section 8.3 on how to actually remove it
from your interpreter.

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

# 2. Functions in Haskell (Part 1)

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

# 3. "Control Structures"

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

    > isItGreat x = if x == 5 then "great" else if x == 7 then "also great" else "that's okay too"
    > putStrLn(isItGreat 7)
    also great

```

We will get to loops later.

# 4. Lists (Part 1)

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



# 5. Where are the loops?

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

# 6. Haskell is not C

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

# 7. Why Do This?

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

# 8. Few More Interpreter Facilities

## 8.1 Running Code From Files

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

## 8.2 Multiline Statements

The examples following this section will require you to enter in
multiline statements. You can start a multiline statement by typing in
`:{`, and ending with `:}` in the interpreter:

```
    > :{
    .. a x y = 
    ..     if x > y then 1
    ..     else 0
    .. :}
    > a 2 3
    0
```

The above expression can be also written without the `:{` and `:}`
commands if executed from a file. In that case, it would appear like
this:
```
a x y = 
    if x > y then 1
    else 0
```

Whitespace matters in Haskell, and indentation denotes a program block.

## 8.3 Convenience Settings for the Interpreter

You can specify certain options in the interpreter. All of these options
are prefixed with a colon (`:`), like the `:{` and `:}` mentioned
before.

Here are a few that will make it a bit easier to use the interpreter

* `:set +m`: Set multiline mode. If Haskell detects that a statement
  will require multiple lines to complete, Haskell wil automatically
  enter the multiline mode. This does not work in all cases, as you
  will see in the following sections.

* `:set prompt "> "`: This will change your prompt from `Prelude> ` to
  `> `.

* <code>:set prompt-cont ".. "</code>: This will change the multiline
  block prompt from `Prelude| ` to  <code>.. </code>

You can make it so that all of these options are enabled in startup by
typing these in to GHCI's startup script. Depending on your system,
this file may be named `.ghci` or `ghci.conf`. Their locations cab
be found [here][ghci-startup].

# 9. Functions in Haskell (Part 2) 

## 9.1 Operators are also functions

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

## 9.2 What about Operator Precedence?

There are certain operator/function tokens that are given a
[predefined precedence][haskell-opprec] by Haskell. These include
operations like ``+, -, *, /, `mod` `` etc. Other than this, since there
are no differences between operators and functions aside from operators
being in infix order by default, custom operators have the same precedence
as normal functions.

## 9.3 Pattern Matching

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

## 9.4 More Ways to Define Functions

### 9.4.1 Definitions using Pattern Matching

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
    .. fac 0 = 1
    .. fac 1 = 1
    .. fac n = n * fac (n - 1)
    .. :}
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
_Note the use of `:{` and `:}` commands over here. These define a multiline
block and is required for defining such a function written in multiple
lines. Otherwise, the function definition is always overwritten each
time we attempt to add a new pattern. This problem is avoided when
executing from a file instead._

Unlike in conventional imperative languages, it completely avoids using
an `if-else` block.

A more complex example is as follows:

```
Example 9.3:

Pattern with a 2 element list, and matching a list that has the second
element as 1:

    > :{
    .. f [a, 1] = a + 999
    .. f [a, b] = a + b
    .. :}
    > f [1000, 1]
    1999
    > f [1000, 2]
    1002

Pattern with a 2 element list and a variable:

    > :{
    .. f [a, 1] 1 = a + 999 + 111
    .. f [a, b] c = a + b + c
    .. :}
    > f [1000, 1] 1
    2110
    > f [1, 2] 3
    6
```
### 9.4.2 `case` Syntax

The `case` syntax allows you to match a given expression against a
set of patterns:

```
Example 9.4

On a single line:

    > x = 5
    > case x of 5 -> 7; 7 -> 9; 9 -> 11
    7

In multiline mode:

    > x = 5
    > case x of
    ..     5 -> 7
    ..     7 -> 9
    ..     9 -> 11
    .. 
    7
```
_Note how we no longer use `:{`, `:}` for a mutiline statement. this is
due to the `:set +m` option mentioned in Section 8.3._

You can use this syntax to perform pattern matching in a function:

```
Example 9.5:

    > fac n = case n of
    ..     0 -> 1;
    ..     1 -> 1;
    ..     otherwise -> n * fac (n - 1);
    .. 
    > fac 5
    120
```

Note the use of the `otherwise` keyword over here. `otherwise` contains
the default case that will match any case that has not been already
specified in the case block.

Case blocks are simply syntactic sugar and analogous to normal
function pattern matching. Hence, we can rewrite the first definition in
Example 9.3 as follows:

```
Example 9.6:

    > f x = case x of
    ..     [a, 1] -> a + 999
    ..     [a, b] -> a + b
    .. 
```

Notice how we have utilised `a`, `b` as placeholder variables here.

### 9.4.3 Guard Syntax

Guard syntax allows you to define a function by checking against a
set of conditions rather than equating patterns. This is a shorter
method to write `if-else` statements present in imperative languages.

Within an interpreter you will have to use an explicit multiline block
to define the function on multiple lines, unlike the previous one.
Each conditional block is prefixed with an '`|`'

```
Example 9.7

This function finds the max of two numbers:

    > :{
    .. f a b
    ..     | a > b = a
    ..     | otherwise = b
    .. 
    .. :}
    > f 2 3
    3
```

You can also write the function in one line, albeit being less readable:

```
Example 9.8:

    > f a b | a > b = a | otherwise = b
    > f 3 2
    3
```

The `otherwise` keyword here was used in the same manner as in the
previous case block method.

A more complex example is as follows:

```
Example 9.9

This is a 3 way comparator. It returns greater than, less than or equal
to depending on the two numbers. This is the "spaceship" operator you
may find in many languages like PHP and C++:
    
    > :{
    .. a <=> b
    ..     | a > b = GT
    ..     | a == b = EQ
    ..     | a < b  = LT
    .. 
    .. :}
    > 2 <=> 2
    EQ
    > 2 <=> 3
    LT
    > 4 <=> 3
    GT

```

`GT`, `EQ`, `LT` are predefined symbols. We will look into how to define
our own such symbols later.

### 9.4.4 `let .. in ..` Syntax

`let` syntax allows you to define a set of local variables in a block,
and use those variables in the final expression of the function.

```
Example 9.10:

Inline method:

    > hypotenuse x y = let sqrx = x^2; sqry = y^2; in sqrt (sqrx + sqry)
    > hypotenuse 3 4
    5.0

Non-Inline method:

    > :{
    .. hypotenuse x y = let
    ..     sqrx = x ^ 2
    ..     sqry = y ^ 2
    ..     in sqrt (sqrx + sqry)
    .. :}
    > hypotenuse 3 4
    5.0
```

### 9.4.5 `where` Syntax

`where` is similar to `let`, but in this case we supply local variables
after they are defined.


```
Example 9.11

Inline method:

> a = x + y where x = 4; y = 3;
.. 
> a
7

Non-Inline method:

> :{
.. a = x + y
..     where x = 4
..           y = 3
.. :}
> a
7

```

It can be used alongwith guards and other statements as well:

```
Example 9.12:

This is the 3 way comparator but we replace the numbers with their
negative counterparts:

> :{
.. a <=> b
..     | p > q = GT
..     | p < q = LT
..     | p == q = EQ
..     where p = -a
..           q = -b
.. :}
> 2 <=> 3
GT
> 3 <=> 2
LT

```

## 9.5 Currying

In Haskell, a function with multiple arguments can be turned into
another function by supplying a partial number of arguments to it.
This is called [Currying][wiki-currying]. If the original function had
`n` arguments, and we have supplied `k` arguments starting from the
left of the argument list, then the new function will have `n - k`
arguments.

For example, we have a function with 2 arguments over here and we
supply one of them. In this case below, the parameter `x` of the
function has been bound and a new function is made, with a single
parameter `y`. This is then bound to a new identifier named `g`:

```
Example 9.13:

    > f x y = x + y
    > g = f 2
    > g 3
    5
```
Here is a far larger example:

```
Example 9.14:

    > f u v w x y z = u + v + w + x + y + z
    > g = f 1
    > h = g 2
    > i = h 3
    > j = i 4
    > k = l 5
    > k = j 5
    > l = k 6
    > l
    21
    
    > m = f 1 2 3 4 5
    > m 6
    21
```

This is useful for, let's say, creating a function to divide all the
elements in a list without having to create an explicit function to
do so. We will look into how to do this later.

## 9.6 Function Composition

Function composition in Haskell is exactly the same as mathematical
function composion. If there are 2 functions, `f(x)` and `g(x)`, then
the composed function `k(x) = f(x) . g(x)` is equivalent to  `f(g(x))`.

The syntax uses the `.` operator and is used as follows:

```
    > f x = x - 3
    > g x = 3 - x
    > k = f . g
    > k 0
    0
    > k = g . f
    > k 0
    6
```

This is useful for chaining the functionalities of multiple functions
together.

# 10. Haskell's Type System

Haskell is a [statically-typed][wiki-static] language. Types are
determined right when a variable is compiled and retains that type for
the rest of its runtime. This ensures that all type errors that will
ever occur are detected at compile time.

GHCI is actually an _incremental bytecode compiler_ as opposed to an
actual interpreter. Thus whenever you declare a function, type checking
is done as soon as you pass the definition to the interpreter, and
any errors present are returned to you.

This is in contrast to what happens in a dynamically typed language
such as Python. Functions forwarded to the interpreter are only checked
for syntax errors, and then _type checking is deferred until the
function is actually called_.

The following example illustrates this difference:

```
Example 10.1:

Python Code:
    
    >>> def half_add_str(x):
    ...     return x / 2 + "a";
    ... 
    >>> half_add_str(3)
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
      File "<stdin>", line 2, in half_add_str
    TypeError: unsupported operand type(s) for +: 'float' and 'str'


Haskell Code:

    > halfAddStr x = x / 2 + "a"

    <interactive>:8:16: error:
        • No instance for (Fractional [Char]) arising from a use of ‘/’
        • In the first argument of ‘(+)’, namely ‘x / 2’
          In the expression: x / 2 + "a"
          In an equation for ‘halfAddStr’: halfAddStr x = x / 2 + "a"

    <interactive>:8:16: error:
        • No instance for (Num [Char]) arising from a use of ‘+’
        • In the expression: x / 2 + "a"
          In an equation for ‘halfAddStr’: halfAddStr x = x / 2 + "a"

```

Note how the Haskell interpreter immediately notices the type error and
in the Python interpreter the type checking only happens when we call
the function.


Up till now, we have not once explicitly define the type of a given
identifier and were depending upon Haskells inferred types to assign a
type to the identifier. Now we will learn how to define the type of an
identifier and how does the type syntax work in Haskell.

## 10.1 Explicit Declation of the Type of an Identifier

All explicit type assignments are done using the `::` operator.
Definition of the identifier must also be done on the same block as
the type definition.

```
Example 10.2:

Defintion on the same line:

    > val :: Int; val = 5
    > val
    5

Definition in a block:

    > :{
    .. val :: Int
    .. val = 5
    .. :}
    > val
    5
```

"`Int`", here, is the integer type. For a full list of standard types,
see [here][haskell-types].

The example below illustrates how types are being enforced when we
attempt to assign a floating point number to an integer:

```
Example 10.3:
    > :{
    .. val :: Int
    .. val = 5.123
    .. :}

    <interactive>:18:7: error:
        • No instance for (Fractional Int) arising from the literal ‘5.123’
        • In the expression: 5.123
          In an equation for ‘val’: val = 5.123
```

In the interpreter, you can check the type of an identifier by using the
`:t` command.

```
    > :t val
    val :: Int
```

## 10.2 Defining Function Types

```
    > :t f
    f :: Num a => a -> a -> a -> a -> a -> a -> a
    > :t g
    g :: Num a => a -> a -> a -> a -> a -> a
    > :t h
    h :: Num a => a -> a -> a -> a -> a
    > :t i
    i :: Num a => a -> a -> a -> a
    > :t j
    j :: Num a => a -> a -> a
    > :t k
    k :: Num a => a -> a
    > :t l
    l :: Num a => a

```

# 11. Lists (Part 2)

## 11.1 "Cons" and Concatenation




------------------------------------------------------------------------

[ghc]: https://www.haskell.org/ghc/
[wiki-function]: https://en.wikipedia.org/wiki/Function_(mathematics)
[wiki-currying]: https://en.wikipedia.org/wiki/Currying
[ghci-startup]: https://downloads.haskell.org/ghc/latest/docs/html/users_guide/ghci.html#the-ghci-files
[haskell-lexical]: https://www.haskell.org/onlinereport/lexemes.html
[haskell-opprec]: https://www.haskell.org/onlinereport/decls.html#fixity
[haskell-types]: https://www.haskell.org/onlinereport/haskell2010/haskellch6.html
[wiki-greedy]: https://en.wikipedia.org/wiki/Greedy_algorithm
