# libftpp

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