.. toctree::
   :maxdepth: 2

Linecrypt specifications (LC 1.1)
=================================
Every program has an input and a multistack assigned to it. A multistack is like a group of stacks, where each one has
a value assigned to it (Like a dictionary).
Versions of LC before 2.0 will always have their program linked to an input (Normally a premade string).

All programs start with an empty stack assigned to the value 0.

Specifiers
----------
Specifiers represent blocks of commands that will be run once an event happens (Normally related to input).

Start specifiers
^^^^^^^^^^^^^^^^
Syntax: `s/ ... /`

Start specifiers get run at the start of the linked input, even if there is no input.

Character specifiers
^^^^^^^^^^^^^^^^^^^^
Syntax: `c/ ... /`

Character specifiers get run for every character in the input.

Word specifiers
^^^^^^^^^^^^^^^
Syntax: `w/ ... /`

Word specifiers get run for every word (space and EOF) in the input.

.. warning::
    Word specifiers might be removed on LC 2.0 due to their specificness.

End specifiers
^^^^^^^^^^^^^^
Syntax: `e/ ... /`

End specifiers get run at the end of the linked input, even if there is no input.

Conditionals
------------
Conditionals represent blocks of code that will only be run if a condition is met, like in any other programming
language.

Equal Conditional
^^^^^^^^^^^^^^^^^
Syntax: `?/ ... /`

Content inside equal conditionals will only be run if the last stack value is 0. Will not delete last stack value on
comparison.

.. warning::
    Equal conditionals will be replaced in LC 2.0 by normal conditionals, that will take a boolean value from the stack.

Greater Conditional
^^^^^^^^^^^^^^^^^^^
Syntax: `?>/ ... /`

Content inside greater conditionals will only be run if the last stack value is greater than 0. Will not delete last
stack value on comparison.

.. warning::
    Greater conditionals will be replaced in LC 2.0 by normal conditionals, that will take a boolean value from the stack.

Variables
---------
.. warning::
    "Variables" do NOT act like normal programming variables. That is, you CANNOT modify them; They only push values to the stack. They will be probably be renamed in LC 2.0.

"Variables" are named tokens that push certain values to the stack. You currently *cannot* create new variables, as they
are defined by the interpreter (LCC in this case).

.. csv-table:: All variables in LCC and LC
    :header: "Name", "Implementation-defined", "Value Type", "Returns", "Notes"

    "`#CHAR#`", "No", "string", "The current character being read from the input stream. If there is no input or it is
    used in `s//`, the result is implementation-defined.", "#CHAR# will not advance the input stream, only the character
    specifier will."
    "`#WORD#`", "No", "string", "The concatenation of every character read until now from the latest space.", "#WORD#
    will be probably replaced or removed in later releases of LC due to its specificness. It is not properly implemented in pylc."
    "`#TEXT#`", "No", "string", "Returns the concatenation of every character read until now.", "#TEXT# will be empty if
    run from `s//` or full if run from `e//`."
    "`#INPUT_LENGTH#`", "No", "integer", "Returns the length of the input string as a number.", "#INPUT_LENGTH# is constant and will not change while executing a program."
    "`#STACK_LENGTH#`", "No", "integer", "Returns the length of the current stack as a number.", ""
    "`#VERSION#`", "No", "string", "Returns the linecrypt specifications version.", "Added in LC 1.1."
    "`#__LCC__#`", "Yes", "string", "Returns the version of the LCC compiler.", "Specific to LCC."