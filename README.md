# CeDOS
Simple x86 operating system written in assembly and C.

## Software Requirements
* GNU make
* Cross-Compiler + binutils: gcc build for freestanding target i386-elf. If a prefix is needed, set the corresponding variable in the makefile.
* Doxygen: If you want to build the documentation

## Make commands
### Debug build
```make debug```

### Release build
```make build```

### Clear output directories
```make clear```

### Build documentation:
```make docs```