# Advanced C++ Library

## Table of Contents

## Data Structures
- Pool
- Data Buffer
- Field
- Registry
## Design Paterns
- Memento
- Observer
- Singleton
- State Machine
- Factory
## IOStream
- Thread Safe IOStream
- Logger
- Loader
## Threading
- Thread Safe Queue
- Thread
- WorkerPool
- Persistent Worker
## Network
- Message
- Client
- Server
## Mathematics
- IVector 2
- IVector 3
- Random 2D Coordinate Generator
- Perlin Noise 2D
## Clock
- Timer
- Chronometer

## Build
```sh
make
```

## Test
```sh
# Run all the test with binary output hidden
make test
# Run all the test with binary output shown
make test VERBOSE=1
# Run a specific test 
make run test_name
# Run Server and Client in separate instances
make run server
make run client
```