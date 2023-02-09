# NX-Solver
Numerical Expression Solver (NXS - A Scripting Language) {Not yet the official name}
Author: Everett Gaius

NXS is an interpreter written in C++ that evaluates mathematical expressions and solves them to produce numerical results (and graphs + tables) on the fly. The interpreter takes mathematical expressions as input to parse and process the expression, perform the necessary calculations, and return the result. The interpreter can handle a variety of mathematical operations and functions, making it a versatile tool for solving mathematical problems. 

At this point, NXS is at its infancy stage. I'm currently looking for co-maintainers who can help me with the following:

- Documentation
- Testing

If interested, contact me thru PM.

Some things it can do right now:

```
    ' 3D Distance Computation
    '
    ' The distance in a 3D plane
    ' is the Square Root of:
    '
    '   l ^ 2 + w ^ 2 + h ^2
    '

    l = 100;
    w = 200;
    h = 300;
    d = $sqrt(l * l + w * w + h * h);
    $prt(d);

```

```
    ' Double for loop in NXS
    '

    s = 1;
    e = 4;

    $for(   $lt(s, e),
            $(  a = 1,
                b = 4,

                $for(
                    $lt(a, b),
                    $(  $prt(s * a),
                        a = a + 1)
                ),
                s = s + 1)
    );

```

```
    $prtcls();
    $loc(1, 1);

    '
    ' Draw background
    '


    i = 1;
    max = 31;

    $for(   $lte(i, max),
            $(  
                j = 1,
                $for(   $lte(j, max * 2),
                        $(
                            $prtc(97 + $rand() % 26),
                            j = j + 1
                        )
                    ),

                $prtc(10),
                i = i + 1)
    );


    '
    ' Draw heart
    '

    pi = 0.0;
    red = 31;

    $for(   $lt(pi, $pi() * 2),
            $(  
                abcos = $abs($cos(pi)),
                r = 2.0 - 2.0 * $sin(pi) + $sin(pi) * $sqrt(abcos) / ($sin(pi) + 1.4),
                $loc(31 + r * 5 * $cos(pi), 10 - r * 5 * $sin(pi)),
                $prtcol(red),
                $prtc(65 + $rand() % 26),
                pi = pi + 0.025)
    );


    $loc(1, 32);
    $prtdef();
```