<p align='center'>
   <img id="banner" src="./graphics/logo.png" style="width: 65%; align: center;"/>
   <br/>
   <i>An opensource page rank library.</i>
   <br/><br/>
</p>

# Amber 

Amber is lightweight,fast and open source page rank library that can be used not only for web search but also for other purposes as well.

# Documentation

Read the documentation [here](https://github.com/SaptakBhoumik/amber/blob/master/docs/README.MD)

# Examples

Examples are available in the [example folder](https://github.com/SaptakBhoumik/amber/tree/master/example)

## Features

- Easy to use
- Fast
- Highly modular
- Occupies less memory

## Future plans

- [ ] Add support for other languages
- [ ] Improve the search algorithm
- [ ] Make it faster

## Implementation Language

- C++20

## Requirements to build it
 
 - [Clang](https://clang.llvm.org/)
 - [Python3](https://www.python.org/downloads/)

## Build it

- Clone the repo

    ``git clone https://github.com/SaptakBhoumik/amber.git``

- Go to the folder

    ``cd amber``

- Build it

    ``python3 make.py release`` 

- Install it

    ``python3 make.py install``

- Compile an example
    ``clang++ example/pagerank.cpp  -lamber -pthread std=c++20``    

## Have questions?

Cool, you can contact me via mail.
<br> Email: saptakbhoumik@gmail.com

## Want to contribute?

Great, go ahead and make the changes you want, then submit a new pull request

Take a look at the [open issues](https://github.com/SaptakBhoumik/amber/issues) to find a mission that resonates with you.

Please check [CONTRIBUTING.md](https://github.com/SaptakBhoumik/amber/blob/master/CONTRIBUTING.md) to learn how you can contribute.

## License

The Amber library is licensed under the [Mozilla Public License](https://github.com/SaptakBhoumik/amber/blob/master/LICENSE), which is attached in this repository