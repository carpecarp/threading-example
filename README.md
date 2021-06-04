# threading-example

This is a multi-threaded program that checks a list of numbers for primality. The first version of this program is ***not*** thread-safe. Compliling and running it may reveal multiple problems: numbers checked more than once, -1 (a signalling value) checked for primality, 0 (another signalling value) checked for primality, or the program may fail to exit.

The 2nd version uses a Posix thread library mutex and condition variable to induce proper thread behavior.

A candidate who is versed in writing thread safe code should be able to understand the code and insert the necessary lines to make the program thread safe given 30 minutes time.

The thread unsafe code may appear to work if there is only one or two cores. With 4 cores available, problems will certainly manifest.

## The Changes for Thread Safety

https://github.com/carpecarp/threading-example/commit/71448156c2761bb869792ba2c1b13e3f0d0d499c#diff-e297099dfdee8bae8d86a465021850ae06b2bbadf1546424ab22918bf00a0f5d

## Running the unsafe code
```
bcarp@Users-MacBook threading-example % ./a.out
./a.out
15147 is not prime
15147 is not prime
22002 is not prime
22002 is not prime
16071 is not prime
16071 is not prime
11039 is not prime
0 is not prime
37701 is not prime
37701 is not prime
37701 is not prime
46619 is prime
46619 is prime
46619 is prime
51771 is not prime
51771 is not prime
0 is not prime
27011 is prime
27011 is prime
33041 is not prime
33041 is not prime
61017 is not prime
61017 is not prime
61017 is not prime
37023 is not prime
10671 is not prime
38223 is not prime
38223 is not prime
38223 is not prime
45005 is not prime
45005 is not prime
45005 is not prime
12511 is prime
12511 is prime
63001 is not prime
63001 is not prime
63001 is not prime
49613 is prime
49613 is prime
49613 is prime
31471 is not prime
31471 is not prime
20191 is not prime
20191 is not prime
20191 is not prime
62619 is not prime
62619 is not prime
62619 is not prime
17009 is not prime
17009 is not prime
17009 is not prime
31441 is not prime
31441 is not prime
27911 is not prime
0 is not prime
52127 is prime
52127 is prime
-1 is prime
-1 is prime
  ^c
  ```
 ``` 
## Running the safe code
bcarp@Users-MacBook threading-example % ./a.out
./a.out
15147 is not prime
22002 is not prime
16071 is not prime
11039 is not prime
46619 is prime
37701 is not prime
51771 is not prime
27011 is prime
61017 is not prime
37023 is not prime
33041 is not prime
38223 is not prime
10671 is not prime
45005 is not prime
63001 is not prime
12511 is prime
49613 is prime
31471 is not prime
20191 is not prime
62619 is not prime
31441 is not prime
17009 is not prime
27911 is not prime
52127 is prime
bcarp@Users-MacBook threading-example %
```
## Possible solutions
A clever candidate might point out that changing the number of threads to 1 would make the program nominally thread safe. (See line 108) That would not be the point.
