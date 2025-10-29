# Cuckoo Hello
This is a simple hello world project to demonstrate that cross compiling works and this can be ran on a Nest Thermostat

# Build instruction
## build
run the following:
```
mkdir build && cd build
cmake ..
cmake --build .
cd ..
```

## upload
SSH to the Nest and start a simple server to receive the file:
```
nc -l -p 51234 > ./hello
```

On the build host. send the hello exe via netcat:
```
cat ./build/output/hello | nc -w1 NEST_IP_ADDR 51234
```

Back on the nest, alter permissions:
```
chmod +x ./hello
```

Now you can run with
```
./hello
```
# Issues building
The toolchain was compiled for 32 bit systems. If you are trying to compile on 64bit systems, the following guide may help:
https://devnodes.in/blog/linux/arm-no-such-a-file-or-directory/


# Future work
I may turn this into a demonstrator for the various IO the nest has.
