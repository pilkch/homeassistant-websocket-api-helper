## Rationale


## Curl WebSocket Support

> The WebSocket API was introduced as experimental in 7.86.0 and is still experimental today.

> It is only built-in if explicitly opted in at build time. We discourage use of the WebSocket API in production because of its experimental state. We might change API, ABI and behavior before this "goes live". 


## Usage

**NOTE: There is a slight issue, in podman the shared volumes show up as owned by root in the container, so there is something going on with the uid/gid mapping, but files can be written so I guess it is it doesn't matter?**

```bash
$ (cd libcurl && ./build.sh)
$ cmake .
$ make
```
