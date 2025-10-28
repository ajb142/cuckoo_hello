# Cuckoo Hello
This is a simple hello world project to demonstrate that cross compiling works and this can be ran on a Nest Thermostat

# Build instruction

## Download the toolchain. 
If you have used my other repo to gain root access then you probably have this kicking around. If not it can be downloaded from here:

wget http://files.chumby.com/toolchain/arm-2008q3-72-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2

Extract the file and set up path. 
```
tar xjvf arm-2008q3-72-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
rm arm-2008q3-72-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
PATH=$PATH:`pwd`/arm-2008q3/bin
```

Note: path will need setting every time unless you modify your baschrc file.

## build
run the following:
```
arm-none-linux-gnueabi-gcc -o hello main.c
```

## upload
SSH to the Nest and start a simple server to receive the file:
```
nc -l -p 51234 > hello
```

On the build host. send the hello exe via netcat:
```
cat ./hello | nc -w1 NEST_IP_ADDR 51234
```

Back on the nest, alter permissions:
```
chmod +x ./hello
```

Now you can run with
```
./hello
```

# Future work
I may turn this into a demonstrator for the various IO the nest has.