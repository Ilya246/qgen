# What?
qgen is a small utility to generate nonsense strings akin to what you'd find in Mindustry (game) b104.9, the April 1st build, with code adapted directly from there.

## Compiling
Linux: `g++ qgen.cpp -o qgen`
Windows: `g++ qgen.cpp -o qgen.exe`

## Usage
Run it and do `./qgen -h`. This should print the following help notice:
```
Usage: ./qgen <amount> <length> [-h] [-c] [-s] [-f <file>] [-o <file>]
Arguments:
        -h
                Print this help notice.
        -c
                Make -o add to the default quotes as opposed to overwriting them.
        -s
                Force output to console even if output file is provided.
        -f <file>
                Output to file instead of console.
        -o <file>
                Overwrite the source quotes with quotes from file, separated by newlines.
```

### Example usage:
Generate 5 quotes of length 3
```
$ ./qgen 5 3
mind stop make sense to 
it's finally this is one cannot create 
verify stop it's too late. 
you think it's finally happening never make another 
one will every single moment in
$
```
