# Contributing

The code style that will be used in this project is inspired by [PEP 8][].
Obviously, as a C project, parts of PEP 8 will be irrelevant.

As a UNSW CSE project, the [CSE style guide][] is to be kept in mind.
You may also wish to consult @rwaldron's [idiomatic.js][].

The following is a clarification of the style to be used where PEP 8,
the CSE style guide, and idiomatic.js are lenient.

## Indentation

Please see [PEP 8][]'s indentation section.

A soft tab of 4 spaces is to be used to indent code.
<!--
3-space indents are poison.
The CSE style guide should die in a fire for even suggesting 3 spaces.
-->

```viml
set softtabstop=4 shiftwidth=4 expandtab
```

## Whitespace in expressions and statements

Consider [PEP 8][] law for this section.
See also [idiomatic.js][] section 2.D for what this should look like in a C-like language.

* Keywords (such as `if` and `while`) *must* have a space after them.

## Blocks

Please see the [CSE style guide][] section 4.  This is to be considered law.

## Comments

Please see [idiomatic.js][] section 9.  Consider this law.
One may also wish to consider [PEP 8][] on comments.


[PEP 8]: http://legacy.python.org/dev/peps/pep-0008/
[CSE style guide]: https://wiki.cse.unsw.edu.au/info/CoreCourses/StyleGuide
[idiomatic.js]: https://github.com/rwaldron/idiomatic.js
