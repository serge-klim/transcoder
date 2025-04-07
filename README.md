# transcoder
[![compilation](https://github.com/serge-klim/transcoder/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/serge-klim/transcoder/actions/workflows/cmake-multi-platform.yml)
[![CodeQL](https://github.com/serge-klim/transcoder/actions/workflows/codeql.yml/badge.svg?branch=main)](https://github.com/serge-klim/transcoder/actions/workflows/codeql.yml)
[![codecov.io](https://codecov.io/gh/serge-klim/transcoder/branch/main/graph/badge.svg)](https://codecov.io/gh/serge-klim/transcoder)

### Motivation 

Simplify encoding/decoding tuples like C++ structures. It is might be useful for handling data coming from the wire.Such data usually needs to be packed/unpacked into platform specific object layout. 
As well as byte-ordered due to [endianness](https://en.wikipedia.org/wiki/Endianness) differences.  
Once data is represented as a tuple or has [Boost.describe](https://www.boost.org/doc/libs/1_87_0/libs/describe/doc/html/describe.html) metadata available, it can be used as simply as this: 

```
auto encoded_buffer = tc::encode(message);
auto begin = encoded_buffer.data();
auto decoded_message = tc::decode<Message>(begin, begin+encoded_buffer.size());
```
here for example, implementation of [Nasdaq TotalView-ITCH 5.0](https://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf) protocol using this library.

described structures: [https://github.com/serge-klim/md_prsr/tree/main/nasdaq/itch_v5.0](https://github.com/serge-klim/md_prsr/tree/main/md_prsr/nasdaq/itch_v5.0)
and usage example: https://github.com/serge-klim/md_prsr/blob/main/tools/nasdaq_itch_v5.0/message_dump.cpp#L19

library avalible as [vcpkg](https://vcpkg.io) package transcoder via this [vcpkg registry](https://github.com/serge-klim/pkgs):

[vcpkg-configuration.json](https://github.com/serge-klim/md_prsr/blob/main/) example:
```
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/microsoft/vcpkg",
    "baseline": "80d54ff62d528339c626a6fbc3489a7f25956ade"
  },
  "registries": [
    {
      "kind": "git",
      "repository": "https://github.com/serge-klim/pkgs",
      "baseline": "a069078be0f78b0fc763473a46d911b7ed85ec91",
      "packages": [ "transcoder" ]
    }
  ]
}
```

