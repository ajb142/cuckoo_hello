# Cuckoo Hello
This is a simple hello world project to demonstrate 
 - Cross compiling works and this can be ran on a Nest Thermostat
 - Input devices cann be read
 - Screen canbe controlled

# Build instruction
## Build
run the following:
```
mkdir build && cd build
cmake ..
cmake --build .
cd ..
```

## Upload
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
## Run
Stop the nest client so we can control hardware:
```
/etc/init.d/nestlabs stop
```

Now you can run with
```
./hello
```
