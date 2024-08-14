# Scream

Makes your kernel scream (optionally into the void).

```console
cat /dev/scream
```

## Build and install

```console
make
sudo ./scream_load
```

To uninstall, you may delete the device node and unload the module:

```console
sudo rm -f /dev/scream
sudo rmmod scream
```
