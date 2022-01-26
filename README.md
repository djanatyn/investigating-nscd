# investigating-nscd

Investigating `glibc` translation of `getaddrinfo` syscalls with `nscd`.

> The answer is that local processes don’t know to connect to `/var/run/nscd/socket`. Or rather, some do, and some don’t. The processes that do know about `/var/run/nscd/socket` are those linked against `glibc` and using `getaddrinfo` from that library.

- [jameshfisher.com "Don’t use nscd"](https://jameshfisher.com/2018/02/05/dont-use-nscd/)

# usage

## watching `nscd` daemon syscalls
``` sh
; sudo strace -Cfp $(pgrep -of nscd)

```

## stracing compiled `getaddrinfo`
```sh
; ls *.c | entr -sc 'cc dns.c && strace -Cf ./a.out 2>&1 | tee strace.log && echo'
```

## checking for linked `glibc` `getaddrinfo` symbols
``` sh
; nm $(ldd a.out | rg 'libc.so.6 => (.+) \(.+\)$' -r '$1' -o) \
    | grep getaddrinfo
```
```
00000000000e3100 T getaddrinfo
00000000000e3100 t __GI_getaddrinfo
```

## translated syscalls during execution

``` sh
socket(AF_UNIX, SOCK_STREAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0) = 5
connect(5, {sa_family=AF_UNIX, sun_path="/var/run/nscd/socket"}, 110) = 0
sendto(5, "\2\0\0\0\r\0\0\0\6\0\0\0hosts\0", 18, MSG_NOSIGNAL, NULL, 0) = 18
poll([{fd=5, events=POLLIN|POLLERR|POLLHUP}], 1, 5000) = 1 ([{fd=5, revents=POLLIN}])
recvmsg(5, {msg_name=NULL, msg_namelen=0, msg_iov=[{iov_base="hosts\0", iov_len=6}, {iov_base="\310O\3\0\0\0\0\0", iov_len=8}], msg_iovlen=2, msg_control=[{cmsg_len=20, cmsg_level=SOL_SOCKET, cmsg_type=SCM_RIGHTS, cmsg_data=[6]}], msg_controllen=20, msg_flags=MSG_CMSG_CLOEXEC}, MSG_CMSG_CLOEXEC) = 14
mmap(NULL, 217032, PROT_READ, MAP_SHARED, 6, 0) = 0x7fec44f8b000
```
