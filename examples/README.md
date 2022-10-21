# osc-sdk-c example

## example 0

call osc_read_load_balancers and osc_read_images with outscale ownner as filter,
printf the result.
it's in C89

## example 1

call osc_read_images with *Arch* image_names as filter, and pretty print the result using json-c.
it's in GNU C 99

## exanple 2

```
Usage: ./example2 VM_NAME_FILTER [ACTION] 
```

small program do some action on all Vms that match a vm tag name filter.
It can either, list all `vm-id`, print full json info, stop or start the VMs matching hte filter.

## LICENSE

exemples are dual-license under BSD-3 and Beer-ware
