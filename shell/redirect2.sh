#! /bin/bash

## Redirect across exec: "echo hello, world" shall write to file log.r2
exec >log.r2
setsid echo hello, world
