# Proper Scientific Calculator

## Introduction

This is an attempt to create a better, well documented, well designed open source mathematical expression parser and a scientific calculator.

There are very nice projects out there, we like to mention them below;

LIST.

But unfortunately they're not comprehensive enough neither that is their motivation.

This math engine (we will call it **euler** from now on) we implemented in this project is capable of doing basic arithmetic to very complex matrices and calculus operations. But since it is planned to put it in a microcontroller its not as powerful as wolfram's products or GNU Octave or Matlab etc. This project aims to make the most powerful math parser that can be fitted in a main stream microcontroller, such as STM32 MCUs.

**Euler** is designed to be standalone, portable, simple but yet powerful. It can run on anything that has an FPU (a version without FPU coming soon).

**Euler** uses **re2c** lexer and **lemon** parser generator for parsing, and implements some of *GNU Scientific Library* code. It uses **C** language and as long as it lasts it will continue to use **C** (nerd alert!)

### Currently Supported Platforms

- Linux
- STM32F746NG

But one can easily implement their own architecture. For more details please refer to the **Wiki Page**.

## Getting Started on Linux

To see the capabilities, _euler_ can be run on most of the linux distributions with a simple make command.

First clone the repo to your machine.

```bash
$git clone https://github.com/arifbalik/proper_calculator.git
```

Then enter the directory and call make as follows;

```bash
$cd proper_calculator
$make clean && make ARCH=linux
```

After this command, _euler_ welcome message will show up and program will wait for an input.

Syntax of _euler_ and more on building and implementing a custom project can be found on the Wiki Page.

# Contributors

- Arif Balik
