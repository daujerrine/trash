const std = @import("std");
const expect = @import("std").testing.expect;
const print = std.debug.print;

const outer_scope = 111;

// Functions

fn addFive(x: u32) u32 {
    return x + 5;
}

// Recursion

fn fibonacci(n: u16) u16 {
    if (n == 0 or n == 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
// Error Sets
// Can be returned instead of actual return value
const MathError = error{DivisionByZero};

// Merging Error Sets
const MergedError = MathError || error{IntegerOverflow};

fn divide(n: u32, m: u32) MathError!u32 {
    if (m == 0) {
        return MathError.DivisionByZero;
    } else {
        return n / m;
    }
}

fn fail() error{NoReasonAtAll}!void {
    return error.NoReasonAtAll;
}

// Try and Errdefer
// try == x catch |err| return err
// errdefer only runs if error is caught
fn tryFail() error{NoReasonAtAll}!bool {
    errdefer print("Errdefer Triggered\n", .{});
    defer print("This defer is still called.\n", .{});

    try fail();
    return true;
}

pub fn main() void {

    // Shadowing is not possible.
    //     const outer_scope = 222;

    // Formatted Strings
    //
    // x and X: output numeric value in hexadecimal notation
    // s:
    //  * for pointer-to-many and C pointers of u8, print as a C-string using
    //    zero-termination
    //  * for slices of u8, print the entire slice as a string without
    //    zero-termination
    // e: output floating point value in scientific notation
    // d: output numeric value in decimal notation
    // b: output integer value in binary notation
    // o: output integer value in octal notation
    // c: output integer as an ASCII character. Integer type must have 8 bits at
    //    max.
    // u: output integer as an UTF-8 sequence. Integer type must have 21 bits at
    //    max.
    // *: output the address of the value instead of the value itself.
    // any: output a value of any type using its default format

    print("Hello, {s}!\n", .{"World"});

    // Variables.
    // (const|var) identifier[: type] = value

    // Explicit typing
    const c: i32 = 5; // Constant value int32_t
    var v: u32 = 50; // Variable value uint32_t

    // Inferred typing
    const infrdc = @as(i32, 5); // Explicit type casting ("as")
    const infrdc2 = 500;
    var infrdv = 1 + c; // Inferred var from existing typed var

    // Note how we havent used any direct literals on typing over here on vars.
    // This is because literals such as numbers will have the type comptime_int
    // comptime_float etc. These are independent of the final machine
    // representation of the given value. Such comptime values are converted
    // into runtime storage types based on context of their usage.
    //
    // We are allowed to use comptime types on const values since these are
    // not going to change in value throughout their lifetimes and hence can be
    // substituted on read when we require to, and then convert it to a runtime
    // storage unit.

    // Variables must be assigned a value. To pick a garbage
    // value or undefined value use undefined.
    const undefc: i32 = undefined;
    var undefv: u32 = undefined;

    // Arrays
    const arr = [5]u8{ 'a', 'b', 'c', 'd', 'e' }; // Explicit
    const arr2 = [_]u8{ 'a', 'b', 'c', 'd', 'e', 'f', 'g' }; // Inferred
    const arr2len = arr2.len; // Get length

    print("array value: {c}\n", .{arr[0]});

    // Placeholder variable/Ignoreable variable
    // Does not return errors on shadowing

    _ = 5;
    _ = 6;

    {
        _ = 1;
        _ = 2;
    }

    // Control Structures

    // If

    if (1 > 0) {
        print("1 is greater than 0\n", .{});
    } else {
        print("You happen to assume a different set of " ++
            "mathematical axioms.\n", .{});
    }

    // While
    // While is Zig's traditional for (;;) loop.
    // You cannot declare iterative variables within the while statement.
    // while (condition) [: (continue_expression)] { ... }

    var i: u8 = 0;

    while (i < 10) : (i += 1) {
        print("{c}", .{'A' + i});
    }

    print("\n", .{});

    // For
    // For is exclusiely used for traversing iterables.
    // for (iterable_var) |value [, index]| { ... }

    for (arr2) |ch, index| {
        print("{c} :: {}\n", .{ ch, index });
    }

    // Defer
    // Defer defers a set of operations in First-In-Last-Out (Stack) order.

    {
        defer print("Value of v is {}.\n", .{v});
        defer v += 1;
        defer print("Last statement in defered group. Value of v is {}.\n", .{v});

        print("Non-defered statement. Value of v is {}\n", .{v});
    }

    // Error handling
    // We pass values instead of exceptions for handling errors.

    var quotient = divide(1, 0);
    print("Dividing 1 by 0 we get: {}\n", .{quotient});

    // Catch statement
    // We can pass values using catch
    var quotient2 = divide(1, 0) catch 0;
    print("Dividing 1 by 0 again we get: {}\n", .{quotient2});

    // We can run code blocks using catch
    // If the function was void there would be no assignment here

    _ = divide(1, 0) catch |err| {
        print("Received {} on divide(1, 0)\n", .{err});
    };

    // Errdefer and try
    // See Above

    var failval = tryFail();

    // Switches

    var q: i32 = 5;

    // As statements:

    switch (q) {
        1...4 => {
            print("Value between 1 and 4\n", .{});
        },

        5, 10 => {
            print("Value either 5 or 10\n", .{});
        },

        else => {
            print("Value not handled by switch", .{});
        },
    }

    // As expression passed to var

    var r: i32 = switch (q) {
        1...4 => 2,
        5, 10 => 4,
        else => 6,
    };

    print("r value = {}\n", .{r});

    // Unreachable and Illegal behaviour
    // Used to mark places that a correct program will never go into, hence
    // illegal behaviour.

    if (r == 4) {
        print("value of r is correct.\n", .{});
    } else unreachable;
}

// Tests
test "will :not fail" {
    expect(true);
}

// test "will fail" {
//     expect(false);
// }
