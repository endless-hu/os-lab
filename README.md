# os-lab

This is an OS lab **archive** for *Operating Systems*, 2022 Summer at *College of Electrical Engineering, Zhejiang University*, instructed by Prof. Baochun Li at *the University of Toronto*.
It contains the code of all four labs and the final course project.

Course Website: [https://oscourse.org](https://oscourse.org/)

## Structure

```
.
├── Documentation.zip
├── LICENSE
├── Lab1Handout.pdf
├── README.md
├── lab1-docker.tar.gz
├── lab2
│   ├── DesiredOutput2.pdf
│   ├── Lab-2-Handout.pdf
│   ├── Main.c
│   ├── Synch.c
│   └── Synch.h
├── lab2-docker.tar.gz
├── lab3
│   ├── Lab-3-Handout.pdf
│   ├── Task1.c
│   ├── Task1.h
│   ├── Task2.c
│   ├── Task2.h
│   ├── Task3.c
│   └── Task3.h
├── lab3-docker.tar.gz
├── lab4
│   ├── Kernel.c
│   ├── Kernel.h
│   └── Lab-4-Handout.pdf
├── lab4-docker.tar.gz
├── project
│   ├── CourseProjectHandout.pdf
│   ├── Kernel.c
│   ├── Kernel.h
│   └── project-sample-output.txt
└── project-docker.tar.gz

4 directories, 28 files
```

## Some Guidance

The labs were done in docker images. To see how to use `docker` to run `Blitz`, please refer to 
[Lab1Handout.pdf](./Lab1Handout.pdf).

To see the effect of my code, please copy these files into the corresponding docker container and follow the lab instructions to build and run `Blitz`.

## Overview

### Lab 1

A starter lab that help you get familiar with `Blitz` and `Docker`. There is no coding.

### Lab 2

Implement the class `Mutex` and the Producer-Comsumer Solution.

### Lab 3

1. Implement the Dining Philosopher’s Solution using a `Monitor`.
2. Solving the Sleeping Barber Problem using `Semaphores` and `Mutex` Locks.
3. Solving the Gaming Parlor Problem using a `Monitor`.

### Lab 4

1. Implement the `Init`, `GetANewThread`, and `FreeThread` methods of the `ThreadManager` class.
2. Implement the class `ProcessManager` so that it can allocate new processes and recycle old ones. A `ProcessControlBlock` should be assigned to each process.
3. Implement the class `FrameManager` so that it can allocate physical pages to a virtual address and recycle all pages from a virtual address by utilizing the class `BitMap`.

All of managers must be thread-safe.

### Course Project

Implement the user-level thread and some system calls.
