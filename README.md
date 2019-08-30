# Proper Scientific Calculator

[![Build status](https://ci.appveyor.com/api/projects/status/cmixc0p8csoy08n6?svg=true)](https://ci.appveyor.com/project/arifbalik/proper-calculator) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/d20996c73bc24168b8e67f459447e375)](https://www.codacy.com/app/arifbalik/proper_calculator?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=arifbalik/proper_calculator&amp;utm_campaign=Badge_Grade) [![](https://tokei.rs/b1/github/arifbalik/proper_calculator?category=code)](https://github.com/arifbalik/proper_calculator).

## Introduction
This is an attempt to create a better, well documented, well designed open source mathematical expression parser and a scientific calculator.

There are very nice projects out there, but what we're trying to do here is to create a embedded framework of libraries to be used in embedded systems where size is important.

This math engine (we will call it **euler** from now on) we implemented in this project is capable of doing basic arithmetic to very complex matrices and calculus operations. But since it is planned to put it in a microcontroller its not as powerful as wolfram's products or GNU Octave or Matlab etc. This project aims to make the most powerful math parser that can be fitted in a main stream microcontroller, such as STM32 MCUs.

**Euler** is designed to be standalone, portable, simple but yet powerful. It can run on anything that has an FPU (a version without FPU coming soon).

**Euler** uses **re2c** lexer and **lemon** parser generator for parsing, and implements some of *GNU Scientific Library* code. It uses **C** language and as long as it lasts it will continue to use **C** (nerd alert!)

### Currently Supported Platforms

-   Linux
-   STM32F746NG

But one can easily implement their own architecture. For more details please refer to the **Wiki Page**.

## Getting Started on Linux

To see the capabilities, *euler* can be run on most of the linux distributions with a simple make command.

First clone the repo to your machine.

```bash
  git clone https://github.com/arifbalik/proper_calculator.git
```

Then enter the directory and call make as follows;

```bash
  cd proper_calculator
  make clean && make ARCH=linux
```

After this command, *euler* welcome message will show up and program will wait for an input.

Syntax of *euler* and more on building and implementing a custom project can be found on the Wiki Page.

## Contributors

-   Arif Balik
