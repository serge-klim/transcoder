# transcoder
[![compilation](https://github.com/serge-klim/transcoder/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/serge-klim/transcoder/actions/workflows/cmake-multi-platform.yml)
[![CodeQL](https://github.com/serge-klim/transcoder/actions/workflows/codeql.yml/badge.svg?branch=main)](https://github.com/serge-klim/transcoder/actions/workflows/codeql.yml)
[![codecov.io](https://codecov.io/gh/serge-klim/transcoder/branch/main/graph/badge.svg)](https://codecov.io/gh/serge-klim/transcoder)

### Motivation 

Simplify encoding/decoding tuples like C++ structures. It is might be useful for handling data coming from the wire. Such data usually needs to be packed/unpacked into platform specific object layout. 
As well as byte-ordered due to [endianness](https://en.wikipedia.org/wiki/Endianness) differences.  
Once data is represented as a C++ tuple or has [Boost.describe](https://www.boost.org/doc/libs/1_87_0/libs/describe/doc/html/describe.html) metadata available, it can be used as simply as this: 

```
auto encoded_buffer = tc::encode(message);
auto begin = encoded_buffer.data();
auto decoded_message = tc::decode<Message>(begin, begin+encoded_buffer.size());
```
here for example, implementation of [Nasdaq TotalView-ITCH 5.0](https://www.nasdaqtrader.com/content/technicalsupport/specifications/dataproducts/NQTVITCHSpecification.pdf) protocol using this library.

described structures: [https://github.com/serge-klim/md_prsr/tree/main/nasdaq/itch_v5.0](https://github.com/serge-klim/md_prsr/tree/main/md_prsr/nasdaq/itch_v5.0)  
and usage example: https://github.com/serge-klim/md_prsr/blob/main/tools/nasdaq_itch_v5.0/message_dump.cpp#L19

how is it better than coded/generated parcer?
* testing is simplier, once transformation works only data layout needs to be tested.  
* different transformation can be run on for eg:  
  - [parquet](https://github.com/serge-klim/pqx) ([usage e.g](https://github.com/serge-klim/md_prsr/blob/main/tools/nasdaq_itch_v5.0/parquet_writer.cpp#L55))
  - [hdf5](https://github.com/serge-klim/h5x) ([usage e.g](https://github.com/serge-klim/md_prsr/blob/main/tools/nasdaq_itch_v5.0/hdf5_writer.cpp#L60))
  - TODO: [odbc addapter](https://github.com/serge-klim/odbcx) (same idea, but for now only supports [Boost.Fusion](http://www.boost.org/doc/libs/1_68_0/libs/fusion/doc/html/))


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
      "baseline": "ebfbf2ff2491e3728374b7e8524f755b110e3a4c",
      "packages": [ "transcoder" ]
    }
  ]
}
```

