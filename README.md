<p>You run the keygenme in the release folder, and the objective of a keygenme is that you reverse engineer it by setting breakpoints and/or disassembling it with a debugger such as OllyDbg in order to find out how it validates a password when given a username.</p>

I found the algorithm the program uses to generate a password given a username, and I basically just created a nice little GUI in C++ Win32 to run that algorithm for you.

<img src="https://d26dzxoao6i3hh.cloudfront.net/items/3A2B1r3X1u0f1U280s1C/keygen.PNG">
