## Minimum Latency Problem

#### My implementation of the Minimum Latency Problem for the Kit-LOG process.
- [] Benchmark results

#### Usage:

#### Setup the meson build:

Assuming that the `build/` is the release build directory:

```
meson setup build --buildtype=release
```

For debugging, setup a `build_debug/` directory:

```
meson setup build_debug --buildtype=debug
```


#### Compile the build:

```
meson compile -C <build_directory>
```

Run the executable:

```
./<build_directory>/mlp ./data/instances/<instance>
```